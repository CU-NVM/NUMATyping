
#include "RecursiveNumaTyper.h"
#include "../utils/utils.h"
#include <clang/AST/Decl.h>
#include <clang/AST/Expr.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/Rewrite/Core/Rewriter.h>
#include <string>
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/Tooling.h"
#include <sstream>
#include <string>
#include <unordered_map>
#include <map>

std::string allocator_funcs = R"(using allocator_type = NumaAllocator<T,NodeID>;// Alloc<T, NodeID>;
using pointer_alloc_type =NumaAllocator<T*,NodeID>; //Alloc<T*, NodeID>;
public:
       
    numa(T t) : T(t) {}
    inline operator T() {return *this;}
    inline operator T&(){return *this;}
	inline T operator-> (){
		static_assert(std::is_pointer<T>::value,"-> operator is only valid for pointer types");
		return this;
	}

    static void* operator new(std::size_t count)
    {
        allocator_type alloc;
        return alloc.allocate(count);
    }
 
    static void* operator new[](std::size_t count)
    {
        //todo: disable placement new for numa.
        allocator_type alloc;
        return alloc.allocate(count);
    }
	
    numa& operator[](std::size_t index) {
        static_assert(std::is_pointer<T>::value,"[] operator is only valid for pointer types");
        return this[index];
    }

    // Const version of the [] operator
    const T& operator[](std::size_t index) const {
        static_assert(std::is_pointer<T>::value,"[] operator is only valid for pointer types");
        return this[index];
    }

    int node_id = NodeID;
    constexpr operator int() const { return NodeID; }
    )";
std::string utils::getNumaAllocatorCode(std::string classDecl, std::string nodeID){
 return R"(public: 
    static void* operator new(std::size_t sz){
        std::cout<<"new operator called"<<std::endl;
		 void* p = numa_alloc_onnode(sz * sizeof()"+classDecl+R"(), )"+ nodeID +R"();
        if (p == nullptr) {
            throw std::bad_alloc();
        }
        return p;
    }

    static void* operator new[](std::size_t sz){
		std::cout<<"new operator called"<<std::endl;
		 void* p = numa_alloc_onnode(sz * sizeof()"+classDecl+R"(), )"+ nodeID +R"();
        if (p == nullptr) {
            throw std::bad_alloc();
        }
        return p;
    }

    static void operator delete(void* ptr){
		std::cout<<"delete operator called"<<std::endl;
		numa_free(ptr, 1 * sizeof()"+classDecl+R"());
    }

    static void operator delete[](void* ptr){
		std::cout<<"delete operator called"<<std::endl;
		numa_free(ptr, 1 * sizeof()"+classDecl+R"());
    }
)";
}

RecursiveNumaTyper::RecursiveNumaTyper(clang::ASTContext &context, clang::Rewriter &rewriter)
    : Transformer(context, rewriter)
{}
using namespace clang;
void RecursiveNumaTyper::start()
{
    using namespace clang::ast_matchers;
    MatchFinder functionFinder;
    auto functionDeclMatcher = functionDecl().bind("functionDecl");
    functionFinder.addMatcher(functionDeclMatcher, this);
    functionFinder.matchAST(context);
}



std::string RecursiveNumaTyper::getNumaConstructorSignature(clang::CXXConstructorDecl* constructor) {
    std::string ConstructorName = constructor->getParent()->getNameAsString();    

    FunctionDecl* constructorDefinition= constructor->getDefinition();
    // Initialize an empty string to build the signature
    std::string ConstructorSignature = "numa (";
    
    // Get the number of parameters
    unsigned ParamCount = constructorDefinition->getNumParams();
    
    // Traverse through parameters
    for (unsigned i = 0; i < ParamCount; ++i) {
        ParmVarDecl *Param = constructorDefinition->getParamDecl(i);
        
        // Get the type of the parameter and print it
        std::string ParamType;
        llvm::raw_string_ostream OS(ParamType);
        Param->getType().print(OS, constructorDefinition->getASTContext().getPrintingPolicy());
        
        // Append parameter type to the signature
        ConstructorSignature += OS.str();
        
        // If parameter has a name, append it
        if (!Param->getName().empty()) {
            ConstructorSignature += " " + Param->getNameAsString();
        }
        
        // Add comma between parameters, except after the last one
        if (i < ParamCount - 1) {
            ConstructorSignature += ", ";
        }
    }
    // Close the parameter list in the signature
    ConstructorSignature += ")";

    return ConstructorSignature;
}


std::string utils::getMemberInitString(std::map<std::string, std::string>& initMemberlist) {
    std::string result = ": ";
    for (auto it = initMemberlist.begin(); it != initMemberlist.end(); ++it) {
    result += it->first + "(" + it->second + ")";
    
    // Add a comma and space unless it's the last element
        if (std::next(it) != initMemberlist.end()) {
            result += ", ";
        }
    }
    return result;
}

std::string utils::getDelegatingInitString(CXXConstructorDecl* constructor) {
     for (const auto *Init : constructor->inits()) {
        if (Init->isDelegatingInitializer()) {
                    // Cast the initializer expression to CXXConstructExpr to get the arguments
            if (const auto *ConstructExpr = dyn_cast<CXXConstructExpr>(Init->getInit())) {
                // Get the number of arguments
                unsigned ArgCount = ConstructExpr->getNumArgs();
                
                // Start building the delegated constructor signature
                std::string DelegatedSignature = ": numa(";
                
                // Extract arguments and append to the signature
                for (unsigned i = 0; i < ArgCount; ++i) {
                    const Expr *ArgExpr = ConstructExpr->getArg(i);

                    std::string ArgStr;
                    llvm::raw_string_ostream ArgOS(ArgStr);
                    ArgExpr->printPretty(ArgOS, nullptr, constructor->getASTContext().getPrintingPolicy());
                    
                    DelegatedSignature += ArgOS.str();
                    if (i < ArgCount - 1) {
                        DelegatedSignature += ", ";
                    }
                }
                DelegatedSignature += ")";
                return DelegatedSignature;
            }
        }
    }
    return " ";
}

std::string RecursiveNumaTyper::getNumaMethodSignature(CXXMethodDecl* method){
    FunctionDecl *methodDefinition = method->getDefinition();
    std::string ReturnTypeStr;
    llvm::raw_string_ostream ReturnTypeOS(ReturnTypeStr);
    methodDefinition->getReturnType().print(ReturnTypeOS, methodDefinition->getASTContext().getPrintingPolicy());

    // Get method name
    std::string MethodName = methodDefinition->getNameAsString();

    // Get parameters
    std::string ParamsStr = "(";
    for (unsigned i = 0; i < methodDefinition->getNumParams(); ++i) {
        ParmVarDecl *Param = methodDefinition->getParamDecl(i);
        std::string ParamTypeStr;
        llvm::raw_string_ostream ParamTypeOS(ParamTypeStr);
        Param->getType().print(ParamTypeOS, methodDefinition->getASTContext().getPrintingPolicy());

        ParamsStr += ParamTypeOS.str();
        if (!Param->getName().empty()) {
            ParamsStr += " " + Param->getNameAsString();
        }

        if (i < method->getNumParams() - 1) {
            ParamsStr += ", ";
        }
    }
    ParamsStr += ")";

    // Build full method signature
    std::string MethodSignature = "virtual "+ReturnTypeOS.str() + " " + MethodName + ParamsStr;
    return MethodSignature;
}


void RecursiveNumaTyper::extractNumaDecls(clang::Stmt* fnBody, ASTContext *Context){
    //fnBody->dump();

    utils::CompoundStmtVisitor CompoundStmtVisitor(Context);
    utils::DeclStmtVisitor DeclStmtVisitor(Context);
    utils::CXXNewExprVisitor CXXNewExprVisitor(Context);
    utils::VarDeclVisitor VarDeclVisitor(Context);
    utils::NewExprInBinaryOperatorVisitor NewExprInBinaryOperatorVisitor(Context);

    if(NewExprInBinaryOperatorVisitor.TraverseStmt(fnBody)){
        llvm::outs() << "Found binary operator\n";
        for(auto newExpr: NewExprInBinaryOperatorVisitor.getBinaryOperators()){
            if(newExpr){
                numaDeclTable.push_back(newExpr);
            }
        }
    }
    if(DeclStmtVisitor.TraverseStmt(fnBody)){
        for(auto declStmt : DeclStmtVisitor.getDeclStmts()){
            if(CXXNewExprVisitor.TraverseStmt(declStmt)){
                //add the vardecl name, the type and the newExpr to the numaTable
                for(auto newExpr : CXXNewExprVisitor.getCXXNewExprs()){
                    if(newExpr){
                        auto newType = newExpr->getType().getAsString();
                        for (auto decl : declStmt->decls()){
                            if(newType.substr(0,4).compare("numa") == 0){
                                if(VarDecl *varDecl = dyn_cast<VarDecl>(decl)){
                                    llvm::outs() << "Gonna add variable "<< varDecl->getNameAsString() << " to the numa table\n";
                                    numaDeclTable.push_back(newExpr);
                                }
                            }
                        }
                    }
                CXXNewExprVisitor.clearCXXNewExprs();
                }
            }
             
            // findNumaExpr(compoundStmt, result.Context); 
        }
    }
    DeclStmtVisitor.clearDeclStmts();
}


void RecursiveNumaTyper::addAllSpecializations(clang::ASTContext* Context){
    clang::TranslationUnitDecl *TU = Context->getTranslationUnitDecl();
    for (const auto *Decl : TU->decls()) {
        if (const auto *SpecDecl = clang::dyn_cast<clang::ClassTemplateSpecializationDecl>(Decl)) {
            const auto *TemplateDecl = SpecDecl->getSpecializedTemplate();
            //check if its numa
            if(TemplateDecl->getNameAsString().compare("numa") == 0){
                // llvm::outs()<< "We found a numa specialization here it is\n";
                // SpecDecl->dump();
                clang::CXXRecordDecl* FoundType;
                llvm::APSInt FoundInt;
                //get arguments : You have to go through this because in the entire context, it might not be the case that the first parameter is always a type and the second is always an integral
                for (const auto &Arg : SpecDecl->getTemplateArgs().asArray()) {
                    if (Arg.getKind() == clang::TemplateArgument::ArgKind::Type) {
                        if(const RecordType* RT = Arg.getAsType()->getAs<RecordType>()){
                            if (CXXRecordDecl *CXXRD = dyn_cast<CXXRecordDecl>(RT->getDecl())) {
                                
                                FoundType= CXXRD;
                            }
                        }
                    }
                    if (Arg.getKind() == clang::TemplateArgument::ArgKind::Integral) {
                        
                        FoundInt = Arg.getAsIntegral();

                        llvm::outs() << "The type is " << FoundType->getNameAsString() << "\n";
                        llvm::outs() << "The integral is " << Arg.getAsIntegral() << "\n";
                        
                        if(!NumaSpeclExists(FoundType, FoundInt.getExtValue())){
                            llvm::outs() << "Adding " << FoundType->getNameAsString() << " and " << FoundInt << " to the specialized classes\n";
                            RecursiveNumaTyper::specializedClasses.insert({FoundType, FoundInt.getExtValue()});
                        }
                    }
                }
            }
        }
    }   
}

bool RecursiveNumaTyper::NumaSpeclExists(const clang::CXXRecordDecl* FirstTempArg, int64_t SecondTempArg){
    auto it = specializedClasses.find(FirstTempArg);
    if (it != specializedClasses.end() && it->second == SecondTempArg) {
        return true;
    } else {
        return false;
    }
}

void RecursiveNumaTyper::makeVirtual(const CXXRecordDecl* classDecl){
    for(auto method : classDecl->methods()){
        if(method->isUserProvided()){
            //check if it is not a constructor
            if(method->isVirtual()){
                continue;
            }
            if(method->getNameAsString() != classDecl->getNameAsString()){
                method->setVirtualAsWritten(true);
                // print reconstructed function
                //method->dump();
            }
            // Insert the virtual keyword if method is not a constructor
            if(method->getNameAsString() != classDecl->getNameAsString()){
                rewriteLocation = method->getBeginLoc();
                rewriter.InsertTextBefore(rewriteLocation, "virtual ");
                //getFIle ID 
                fileIDs.push_back(rewriter.getSourceMgr().getFileID(rewriteLocation));     
            }

        }
    }
}




void RecursiveNumaTyper::specializeClass(clang::ASTContext* Context, const clang::CXXRecordDecl* FirstTempArg, int64_t SecondTempArg){
    // if( FirstTempArg->isB){
    //     return;
    // }
    if(NumaSpeclExists(FirstTempArg, SecondTempArg)){
        return;
    }

    llvm::outs() << "About to specialize "<< FirstTempArg->getNameAsString() << " as numa\n";

    //check if specialization exists
    //insert to specialized classes
    RecursiveNumaTyper::specializedClasses.insert({FirstTempArg, SecondTempArg});

    constructSpecialization(Context, FirstTempArg, SecondTempArg);

}

void RecursiveNumaTyper::constructSpecialization(clang::ASTContext* Context,const clang::CXXRecordDecl* classDecl, int64_t nodeID){
    makeVirtual(classDecl);
    
    rewriteLocation = classDecl->getEndLoc();
    SourceLocation semiLoc = Lexer::findLocationAfterToken(
                rewriteLocation, tok::semi, rewriter.getSourceMgr(), rewriter.getLangOpts(), 
                /*SkipTrailingWhitespaceAndNewLine=*/true);
    
    std::vector<FieldDecl*> publicFields;
    std::vector<FieldDecl*> privateFields;
    std::vector<CXXMethodDecl*> publicMethods;
    std::vector<CXXMethodDecl*> privateMethods;

    for(auto field : classDecl->fields()){
        if(field->getAccess() == AS_public){
            publicFields.push_back(field);
        }
        else if(field->getAccess() == AS_private){
            privateFields.push_back(field);
        }
    }

    for(auto method : classDecl->methods()){
        if(method->getAccess() == AS_public){
            publicMethods.push_back(method);
        }
        else if(method->getAccess() == AS_private){
            privateMethods.push_back(method);
        }
    }

    rewriter.InsertTextAfter(semiLoc, "\ntemplate<>\n"
                                            "class numa<"+classDecl->getNameAsString()+"," + std::to_string(nodeID)+">{\n");
    rewriter.InsertTextAfter(semiLoc, utils::getNumaAllocatorCode(classDecl->getNameAsString(), std::to_string(nodeID)));

    numaPublicMembers(Context, semiLoc, publicFields, publicMethods, nodeID);
    numaPrivateMembers(Context, semiLoc, privateFields, privateMethods, nodeID);
    rewriter.InsertTextAfter(semiLoc, "};\n");  

    fileIDs.push_back(rewriter.getSourceMgr().getFileID(rewriteLocation));

}

void RecursiveNumaTyper::numaPublicMembers(clang::ASTContext* Context, clang::SourceLocation& rewriteLocation, std::vector<FieldDecl*> publicFields, std::vector<CXXMethodDecl*> publicMethods, int64_t nodeID){
    rewriter.InsertTextAfter(rewriteLocation, "public:\n");
     for(auto fields :publicFields){
        
        //QualType fieldType = QualType(classDecl->getTypeForDecl(),0);

        /*Case where the field is a built in type but not a pointer */
        if(fields->getType()->isBuiltinType()){
         
            rewriter.InsertTextAfter(rewriteLocation, "numa<"+fields->getType().getAsString()+","+std::to_string(nodeID)+"> "+ fields->getNameAsString()+";\n" );
        }

        /*Case where the field is a built in type and a pointer*/
        else if(fields->getType()->isPointerType() && fields->getType()->getPointeeType()->isBuiltinType()){
               
                rewriter.InsertTextAfter(rewriteLocation, "numa<"+fields->getType()->getPointeeType().getAsString() +"*,"+std::to_string(nodeID)+"> "+ fields->getNameAsString()+";\n" );
            
        }

        /*Case where the field is not a built in type but is a pointer*/
        else if(fields->getType()->isPointerType() && !fields->getType()->getPointeeType()->isBuiltinType()){
            
            rewriter.InsertTextAfter(rewriteLocation, "numa<"+fields->getType()->getPointeeType().getAsString() +"*,"+std::to_string(nodeID)+"> "+ fields->getNameAsString()+";\n" );
        
            makeVirtual(fields->getType()->getPointeeCXXRecordDecl());
            if(NumaSpeclExists(fields->getType()->getPointeeCXXRecordDecl() , nodeID)){
                continue;
            }else{

            //llvm::outs() << "About to specialize "<< fields->getNameAsString() << " as numa\n";

            //insert to specialized classes
            //RecursiveNumaTyper::specializedClasses.insert({QualType(fields->getType()->getPointeeCXXRecordDecl()->getTypeForDecl(),0) , nodeID});

            //constructSpecialization(Context, fields->getType()->getPointeeType()->getAsCXXRecordDecl(), nodeID);
            
            }
        }
        /*Case where the field is not a built in type and not a pointer*/
        else if (!fields->getType()->isBuiltinType() && !fields->getType()->isPointerType()){
            
            rewriter.InsertTextAfter(rewriteLocation, "numa<"+fields->getType().getAsString() +","+std::to_string(nodeID)+"> "+ fields->getNameAsString()+";\n" );
            if(NumaSpeclExists(fields->getType()->getAsCXXRecordDecl(), nodeID)){
                continue;
            }else{
                //insert to specialized classes
                RecursiveNumaTyper::specializedClasses.insert({fields->getType()->getAsCXXRecordDecl(), nodeID});
                constructSpecialization(Context, fields->getType()->getAsCXXRecordDecl(), nodeID);
            }
        }
        else{
        }
         
    }   

    for(auto method : publicMethods){
            //check if constructor   
        if (auto Ctor = dyn_cast<CXXConstructorDecl>(method)){   
            if(Ctor->isUserProvided()){
                numaConstructors(Ctor, rewriteLocation, nodeID);
            }
        }
        else if (auto Dtor = dyn_cast<CXXDestructorDecl>(method)){
            if(Dtor->isUserProvided()){
                numaDestructors(Dtor, rewriteLocation, nodeID);
            }
        }
        else{
            if(method->getDefinition()){
                if(method->isUserProvided()){
                    numaMethods(method,rewriteLocation,nodeID);
                }
            }
        }
    }
}


void RecursiveNumaTyper::numaPrivateMembers(clang::ASTContext* Context, clang::SourceLocation& rewriteLocation, std::vector<FieldDecl*> privateFields, std::vector<CXXMethodDecl*> privateMethods, int64_t nodeID){
    rewriter.InsertTextAfter(rewriteLocation, "private:\n");
    for(auto fields :privateFields){
        /*Case where the field is a built in type but not a pointer */
        if(fields->getType()->isBuiltinType()){
            rewriter.InsertTextAfter(rewriteLocation, "numa<"+fields->getType().getAsString()+","+std::to_string(nodeID)+"> "+ fields->getNameAsString()+";\n" );
        }

        /*Case where the field is a built in type and a pointer*/
        else if(fields->getType()->isPointerType() && fields->getType()->getPointeeType()->isBuiltinType()){
                rewriter.InsertTextAfter(rewriteLocation, "numa<"+fields->getType()->getPointeeType().getAsString() +"*,"+std::to_string(nodeID)+"> "+ fields->getNameAsString()+";\n" );
            
        }

        /*Case where the field is not a built in type but is a pointer*/
        else if(fields->getType()->isPointerType() && !fields->getType()->getPointeeType()->isBuiltinType()){
           
            rewriter.InsertTextAfter(rewriteLocation, "numa<"+fields->getType()->getPointeeType().getAsString() +"*,"+std::to_string(nodeID)+"> "+ fields->getNameAsString()+";\n" );
            
            
            if(NumaSpeclExists(fields->getType()->getPointeeCXXRecordDecl() , nodeID)){
                continue;
            }
            else{
            }
        }
        /*Case where the field is not a built in type and not a pointer*/
        else if (!fields->getType()->isBuiltinType() && !fields->getType()->isPointerType()){
            rewriter.InsertTextAfter(rewriteLocation, "numa<"+fields->getType().getAsString() +","+std::to_string(nodeID)+"> "+ fields->getNameAsString()+";\n" );
            if(NumaSpeclExists(fields->getType()->getAsCXXRecordDecl(), nodeID)){
                continue;
            }else{

            //insert to specialized classes
            RecursiveNumaTyper::specializedClasses.insert({fields->getType()->getAsCXXRecordDecl(), nodeID});

            constructSpecialization(Context, fields->getType()->getAsCXXRecordDecl(), nodeID);
            }
        }
        else{
        }
    } 
   for(auto method : privateMethods){
            //check if constructor
        if (auto Ctor = dyn_cast<CXXConstructorDecl>(method)){
            
            if(Ctor->isUserProvided()){
                numaConstructors(Ctor, rewriteLocation, nodeID);
            }
        }
        else if (auto Dtor = dyn_cast<CXXDestructorDecl>(method)){
            if(Dtor->isUserProvided()){
                   numaDestructors(Dtor, rewriteLocation, nodeID);
            }
        }
        else{
            if(method->getDefinition()){
               if(method->isUserProvided()){
                    numaMethods(method,rewriteLocation,nodeID);
                }
            }
        }
    }
}

void RecursiveNumaTyper::numaConstructors(clang::CXXConstructorDecl* constructor, clang::SourceLocation& rewriteLocation, int64_t nodeID){
    std::map<std::string, std::string> initMembers;
    bool isDelegatingInit = false;
    bool isMemberInit = false;

    std::string initMembersString;
    //llvm::outs() << "CONSTRUCTOR SIGNATURE IS: "<< numaConstructorSignature(constructor) << "\n";
    std::string numaConstructorSignatrue = getNumaConstructorSignature(constructor);  
    if(constructor->getNumCtorInitializers() > 0){
       
        for (auto Init = constructor->init_begin(); Init != constructor->init_end(); ++Init) {
            if ((*Init)->isWritten()) {
                llvm::outs() << "  Initializes member is written\n";
                if((*Init)->isDelegatingInitializer()){
                    llvm::outs() << "  Initializes member is delegating\n";
                    isDelegatingInit = true;
                }
                if((*Init)->isMemberInitializer()){
                    llvm::outs() << "  Initializes member is member\n";
                    isMemberInit = true;
                    FieldDecl *initializedField = (*Init)->getMember();
                    if(initializedField){
                        std::string fieldName = initializedField->getNameAsString();
                        Expr *initExpr = (*Init)->getInit();
                        //get value
                        if (initExpr) {
                            std::string InitValue;
                            llvm::raw_string_ostream OS(InitValue);
                            initExpr->printPretty(OS, nullptr, constructor->getASTContext().getPrintingPolicy());
                            // Store the member name and its corresponding initialization value
                            initMembers[fieldName] = OS.str();
                        }
                    }
                    initMembersString= utils::getMemberInitString(initMembers);
                }
            }
        }
    }
    if (isMemberInit){
        numaConstructorSignatrue += initMembersString;
    }
    if(isDelegatingInit){
        numaConstructorSignatrue += utils::getDelegatingInitString(constructor);
    }
    rewriter.InsertTextAfter(rewriteLocation, numaConstructorSignatrue);
    // rewriter.InsertTextAfter(rewriteLocation, "{");
    if (constructor->hasBody()) { // Check if the method has a body
        const Stmt *ConstructorBody = constructor->getBody(); // Get the body
        std::string BodyStr;
        llvm::raw_string_ostream OS(BodyStr);
        // Pretty print the body
        ConstructorBody->printPretty(OS, nullptr, constructor->getASTContext().getPrintingPolicy());
        rewriter.InsertTextAfter(rewriteLocation, OS.str());
    }
    else{
        rewriter.InsertTextAfter(rewriteLocation, "{}\n");
    }
}
   

        


void RecursiveNumaTyper::numaDestructors(clang::CXXDestructorDecl* destructor, clang::SourceLocation& rewriteLocation, int64_t nodeID){
    //if the constructor has no parameters, we just close the constructor

    rewriter.InsertTextAfter(rewriteLocation, "virtual ~numa(");
    if (destructor->parameters().size() == 0){
        rewriter.InsertTextAfter(rewriteLocation, ")\n");
    
        if(destructor->hasBody()){

            destructor->dump();
            SourceRange BodyRange = destructor->getBody()->getSourceRange();
            const SourceManager &SM = destructor->getASTContext().getSourceManager();
            llvm::StringRef BodyText = Lexer::getSourceText(CharSourceRange::getTokenRange(BodyRange), SM, destructor->getASTContext().getLangOpts());
            //Pass it through a function that searches for 'new' in the body and replaces 'new''s return type with numa<T,N>
            //std::string numaedBody = replaceNewType(std::string(BodyText), N);
            //Then we replace the body 
            rewriter.InsertTextAfter(rewriteLocation, BodyText);
            rewriter.InsertTextAfter(rewriteLocation, "\n");
        }
    }
    //rewrite the paramenters of the constructor
    else{
        for(auto param : destructor->getDefinition()->parameters())
        {
            //get the implementation of the constructor
            
            rewriter.InsertTextAfter(rewriteLocation, param->getType().getAsString() + " " + param->getNameAsString());
            //avoid the last comma
            if(param != destructor->getDefinition()->parameters().back())
            {
                rewriter.InsertTextAfter(rewriteLocation, ", ");
            }
        }
        //after rewriting the parameters, we close the constructor
        rewriter.InsertTextAfter(rewriteLocation, ")");

        //if the constructor has a body, before we rewrite the body, we have to replace the new expression with new numa<T,N>
        if(destructor->hasBody()){
            //destructor->dump();
            SourceRange BodyRange = destructor->getBody()->getSourceRange();
            const SourceManager &SM = destructor->getASTContext().getSourceManager();
            llvm::StringRef BodyText = Lexer::getSourceText(CharSourceRange::getTokenRange(BodyRange), SM, destructor->getASTContext().getLangOpts());
            //Pass it through a function that searches for 'new' in the body and replaces 'new''s return type with numa<T,N>
            //std::string numaedBody = replaceNewType(std::string(BodyText), N);
            //Then we replace the body 
            rewriter.InsertTextAfter(rewriteLocation, BodyText);
            rewriter.InsertTextAfter(rewriteLocation, "\n");
        }
    } 
}



void RecursiveNumaTyper::numaMethods(clang::CXXMethodDecl* method, clang::SourceLocation& rewriteLocation, int64_t nodeID){
    std::string methodSignature = getNumaMethodSignature(method);

    rewriter.InsertTextAfter(rewriteLocation, methodSignature);
    if (method->hasBody()) { // Check if the method has a body
        const Stmt *MethodBody = method->getBody(); // Get the body
        std::string BodyStr;
        llvm::raw_string_ostream OS(BodyStr);
        // Pretty print the body
        MethodBody->printPretty(OS, nullptr, method->getASTContext().getPrintingPolicy());
        // llvm::outs() << "Method Body:\n" << OS.str() << "\n";
        rewriter.InsertTextAfter(rewriteLocation, OS.str());
    }
    else{
        rewriter.InsertTextAfter(rewriteLocation, "{}\n");
    }
}



void RecursiveNumaTyper::run(const clang::ast_matchers::MatchFinder::MatchResult &result){
    if(result.SourceManager->isInSystemHeader(result.Nodes.getNodeAs<FunctionDecl>("functionDecl")->getSourceRange().getBegin()))
        return;
    if(result.SourceManager->getFilename(result.Nodes.getNodeAs<FunctionDecl>("functionDecl")->getLocation()).find("../../numaLib/numatype.hpp") != std::string::npos)
        return;
    if(result.SourceManager->getFilename(result.Nodes.getNodeAs<FunctionDecl>("functionDecl")->getLocation()).empty())
        return;
    if(result.Nodes.getNodeAs<FunctionDecl>("functionDecl")->isImplicit())
        return;
    if(result.Nodes.getNodeAs<FunctionDecl>("functionDecl")->getNameAsString().empty())
        return;

    addAllSpecializations(result.Context);
    if(result.Nodes.getNodeAs<FunctionDecl>("functionDecl")->isThisDeclarationADefinition()){   
        if(!result.Nodes.getNodeAs<FunctionDecl>("functionDecl")->getBody()->children().empty()){
            auto fnBody = result.Nodes.getNodeAs<FunctionDecl>("functionDecl")->getBody();
            llvm::outs() << "Processing Function : "<< result.Nodes.getNodeAs<FunctionDecl>("functionDecl")
            ->getNameAsString() << "\n";   
            extractNumaDecls(fnBody, result.Context);
            } 
    
        }


        // llvm::outs()<<"------------------------------Printing NumaDeclTable----------------------------------\n";
        // llvm::outs()<<"Size of NumaDeclTable: "<<numaDeclTable.size()<<"\n";
        // for(auto it = numaDeclTable.begin(); it != numaDeclTable.end(); it++){
        //     llvm::outs()<<"VarDecl Name: " << it->first->getNameAsString() << "\n";
        //     llvm::outs()<<"VarType: " << it->first->getType().getAsString() << "\n";
        //     llvm::outs()<<"CXXNewExpr Return Type: " << it->second->getType().getAsString() << "\n";
        // }

        

        for(auto &UserNumaDecl : numaDeclTable){
            CXXRecordDecl* FirstTempArg;
            //QualType FirstTempArg;
            int64_t SecondTempArg;
            auto CXXRecordNumaType = UserNumaDecl->getType()->getPointeeType()->getAsCXXRecordDecl();
            auto TemplateNumaType = dyn_cast<ClassTemplateSpecializationDecl>(CXXRecordNumaType);
            llvm::ArrayRef<TemplateArgument> TemplateArgs = TemplateNumaType->getTemplateArgs().asArray();


            if(const RecordType* RT = TemplateArgs[0].getAsType()->getAs<RecordType>()){
                 if (CXXRecordDecl *CXXRD = dyn_cast<CXXRecordDecl>(RT->getDecl())) {
                    // llvm::outs() << "About to specialize  " << CXXRD->getNameAsString() << "\n";
                    FirstTempArg = CXXRD;
                    SecondTempArg = TemplateArgs[1].getAsIntegral().getExtValue();
                    specializeClass(result.Context,FirstTempArg,SecondTempArg);
                }
            }

            //FirstTempArg = TemplateArgs[0].getAsType();
            

            
        } 
    
    return;
}

void RecursiveNumaTyper::print(clang::raw_ostream &stream)
{

}