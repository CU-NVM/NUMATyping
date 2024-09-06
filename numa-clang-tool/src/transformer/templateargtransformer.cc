
#include "templateargtransformer.h"
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


TemplateArgTransformer::TemplateArgTransformer(clang::ASTContext &context, clang::Rewriter &rewriter)
    : Transformer(context, rewriter)
{}
using namespace clang;
void TemplateArgTransformer::start()
{
    using namespace clang::ast_matchers;

    MatchFinder functionFinder;
    MatchFinder classFinder;
    // MatchFinder variableFinder;
    // MatchFinder classFinder;
    // MatchFinder methodDeclFinder;      
    // MatchFinder templateSpcializationFinder;
    // MatchFinder templateFinder;

    // auto callExprMatcher = callExpr().bind("callExpr");
    // auto varDeclMatcher = varDecl().bind("varDecl");
    // auto classMatcher = cxxRecordDecl().bind("class");
    // auto templateSpecializationMatcher = classTemplateSpecializationDecl().bind("templateSpecialization");
    // auto templateMatcher = classTemplateDecl().bind("template");
    // auto CXXNewExprMatcher = cxxNewExpr().bind("newExpr");
    // auto MethodDeclMatcher = cxxMethodDecl().bind("methodDecl");
    auto functionDeclMatcher = functionDecl().bind("functionDecl");


    //functionFinder.addMatcher(callExprMatcher, this);
    // variableFinder.addMatcher(varDeclMatcher, this);
    // templateFinder.addMatcher(templateMatcher, this);
    // templateSpcializationFinder.addMatcher(templateSpecializationMatcher, this);
    // classFinder.addMatcher(classMatcher, this);
    // classFinder.addMatcher(CXXNewExprMatcher, this);
    // methodDeclFinder.addMatcher(MethodDeclMatcher, this);
    functionFinder.addMatcher(functionDeclMatcher, this);

    functionFinder.matchAST(context);
    // printNumaDeclTable();
    // clearNumaDeclTable();
    // methodDeclFinder.matchAST(context);
    // classFinder.matchAST(context);
    // variableFinder.matchAST(context);
    // templateFinder.matchAST(context);
    // templateSpcializationFinder.matchAST(context);

}

std::string TemplateArgTransformer::replaceNewType(const std::string& str, std::string N) {
    std::string output =str;
    std::string::size_type pos = 0;
    while ((pos = output.find("new", pos)) != std::string::npos) {
        //split the word after new before the '[' or the '(' character
        std::string word = output.substr(pos + 4, output.find_first_of("([", pos + 4) - (pos + 4));
        std::string replacement = "numa<" + word + ", " + N +">";
        //replace the word after new with replacement
        output.replace(pos + 4, word.size(), replacement);
        pos += replacement.size();

    }
    return output;
}
std::string TemplateArgTransformer::replaceConstructWithInits(const std::string& input) {
    // Find the position of '('
    size_t pos = input.find('(');
    if (pos == std::string::npos) {
        // '(' not found, return the input string as is
        return input;
    }

    // Extract the substring before '('
    std::string prefix = input.substr(0, pos);

    // Construct the replacement string
    std::string replacement = "numa";

    // Find the position of ')' after '('
    size_t endPos = input.find(')', pos);
    if (endPos == std::string::npos) {
        // ')' not found, return the input string as is
        return input;
    }
    //Extract what comes after ':'
    size_t colonPos = input.find(':', endPos);
    //get as string
    std::string colon = input.substr(colonPos+1);
    //look for '(' in colon
    size_t paramStart = colon.find('(');
    //get the substring after '('
    std::string param = colon.substr(paramStart+1);
    std::string suffix = input.substr(endPos + 1);
    return replacement + "()" + ":" + replacement + "(" + param;
}

void TemplateArgTransformer::recursive_introspective_typer(clang::FieldDecl *field, std::string N, clang::SourceLocation endLoc,clang::ClassTemplateSpecializationDecl* varClassTemplateSpecializationDecl){

    clang::CXXRecordDecl *fieldAsCXXRecordDecl;
    if(field->getType()->isPointerType()){
        fieldAsCXXRecordDecl = field->getType()->getPointeeType()->getAsCXXRecordDecl();
    }
    else{
        fieldAsCXXRecordDecl = field->getType()->getAsCXXRecordDecl();
    }

    for(auto fields: fieldAsCXXRecordDecl->fields() )
    {
        //introspect the field and if the field is not fundamental, call itself again
        if(!fields->getType()->isBuiltinType()){
            recursive_introspective_typer(fields, N, endLoc, varClassTemplateSpecializationDecl);
        }
        
    }
    // if the introspection doesnt find any user defined class, continue building the numa template specialization
    // NOTE: you cant call the fundamental introspective typer here because that function uses the global variable numaed_class, which is different than the 
    // introspected field class.        You can try and work around this.       todo
    rewriter.InsertTextAfter(rewriteLoc, "template< typename T, int NodeID>\n"); //The variable N holds the real node ID number
    rewriter.InsertTextAfter(rewriteLoc, "class numa< T, NodeID , NumaAllocator, typename std::enable_if<std::is_same<T, "+fieldAsCXXRecordDecl->getNameAsString()+">::value>::type>"+ "{\n");
    rewriter.InsertTextAfter(rewriteLoc, allocator_funcs);

    for(auto field : fieldAsCXXRecordDecl->fields())
    {
        rewriter.InsertTextAfter(rewriteLoc, "   numa<" + field->getType().getAsString() + ", " + N + "> " + field->getNameAsString() + ";\n");
    }

    rewriter.InsertTextAfter(rewriteLoc, "public:\n");


    //get all constructors out of numaed class
    for(auto constructor : fieldAsCXXRecordDecl->ctors())
    {
        if(constructor){
            if(constructor->getNumCtorInitializers() > 0){
            llvm::outs() << "Constructor has an initializer list\n";
            llvm::outs() << "Construcor has " << constructor->getNumCtorInitializers() << " initializers\n";
            std::string constructor_name = constructor->getNameAsString();
            //constructor->dump();
            //get the entire line of the constructor as text
            SourceRange ConstructorRange = constructor->getSourceRange();
            const SourceManager &SM = constructor->getASTContext().getSourceManager();
            llvm::StringRef ConstructorText = Lexer::getSourceText(CharSourceRange::getTokenRange(ConstructorRange), SM, constructor->getASTContext().getLangOpts());
            llvm::outs() << "Constructor Text:\n" << ConstructorText << "\n";
            std::string line = (std::string)ConstructorText;
            //llvm::outs()<<replaceConstructWithInits(line)<<"\n"; 
            rewriter.InsertTextAfter(rewriteLoc, replaceConstructWithInits(line));
            rewriter.InsertTextAfter(rewriteLoc, "\n");
            }
            else{
                if(constructor->isUserProvided()){   
                    rewriter.InsertTextAfter(rewriteLoc, "numa(");
                    //if the constructor has no parameters, we just close the constructor
                    if (constructor->parameters().size() == 0)
                    {
                        rewriter.InsertTextAfter(rewriteLoc, ");\n");
                    }
                    else{
                        //rewrite the paramenters of the constructor
                        for(auto param : constructor->parameters())
                        {
                            rewriter.InsertTextAfter(rewriteLoc, param->getType().getAsString() + " " + param->getNameAsString());
                            //avoid the last comma
                            if(param != constructor->parameters().back())
                            {
                                rewriter.InsertTextAfter(rewriteLoc, ", ");
                            }
                        }

                        rewriter.InsertTextAfter(rewriteLoc, ")");
                        
                        if(constructor->hasBody()){
                        llvm::outs() << "constructor has a body\n" ;
                        constructor->dump();
                        //if the constructor has a body, before we rewrite the body, we have to replace the new expression with new numa<T,N>
                        SourceRange BodyRange = constructor->getBody()->getSourceRange();
                        const SourceManager &SM = constructor->getASTContext().getSourceManager();
                        llvm::StringRef BodyText = Lexer::getSourceText(CharSourceRange::getTokenRange(BodyRange), SM, constructor->getASTContext().getLangOpts());
                        llvm::outs() << "Constructor Body:\n" << BodyText << "\n";
                        //Pass it through a function that searches for 'new' in the body and replaces 'new''s return type with numa<T,N>
                        //std::string numaedBody = replaceNewType(std::string(BodyText),N);
                        rewriter.InsertTextAfter(rewriteLoc, BodyText);
                        rewriter.InsertTextAfter(rewriteLoc, "\n");
                        }
                    }
                }
            }
        }
    }
    
    //get all methods from numaed class
    for(auto method: fieldAsCXXRecordDecl->methods()){
        //skip if numaed_class method is class name
        if(method){
            
            if(method->getNameAsString() == fieldAsCXXRecordDecl->getNameAsString())
            {
                continue;
            }
            else{
                //copy the entire method
                if(method->hasBody()){
                    //if the method has a body, before we rewrite the body, we have to replace the new expression with new numa<T,N>
                    SourceRange MethodRange = method->getSourceRange();
                    const SourceManager &SM = method->getASTContext().getSourceManager();
                    llvm::StringRef MethodText = Lexer::getSourceText(CharSourceRange::getTokenRange(MethodRange), SM, method->getASTContext().getLangOpts());
                    //Pass it through a function that searches for 'new' in the body and replaces 'new''s return type with numa<T,N>
                    // /std::string numaedBody = replaceNewType(std::string(MethodText),N);
                    rewriter.InsertTextAfter(rewriteLoc, MethodText);
                    rewriter.InsertTextAfter(rewriteLoc, "\n");
                }
            }
        }
    }

    //Then we copy the helper methods from numa (possibly everything)   todo
    auto all_decls = varClassTemplateSpecializationDecl->decls();
    for(auto decl : all_decls)
    {
        
        llvm::outs() << "The decl is: " << decl->getDeclKindName() << "\n";
        //for now we are obly copying the methods
        if((std::string)decl->getDeclKindName() == "CXXMethod")
        {
            llvm::outs() << "The method name is: " << decl->getDeclKindName() << "\n";

            // decl->dump();

            SourceRange MethodRange = decl->getSourceRange();
            const SourceManager &SM = decl->getASTContext().getSourceManager();
            llvm::StringRef MethodText = Lexer::getSourceText(CharSourceRange::getTokenRange(MethodRange), SM, decl->getASTContext().getLangOpts());

            // Print the method body as a string.
            llvm::outs() << "Method Body:\n" << MethodText << "\n";

            //insert MethodText 
            
            rewriter.InsertTextAfter(rewriteLoc, MethodText);
            rewriter.InsertTextAfter(rewriteLoc , "\n");

        }
    }
    rewriter.InsertTextAfter(endLoc , "};\n");
}




void TemplateArgTransformer::fundamental_introspective_typer(std::string className, std::string N, clang::SourceLocation endLoc, clang::ClassTemplateSpecializationDecl* varClassTemplateSpecializationDecl){

    llvm::outs() << "Here is numaed_class " << numaed_class->getNameAsString() << "\n";
    //rewriter.buffer_begin()->second.write(llvm::outs());
    //Write your header of your new numa template specialization
    rewriter.InsertTextAfter(rewriteLoc, "class "+ className+";\n");
    llvm::outs() << "HEREE\n";
    //rewriter.getEditBuffer(rewriter.getSourceMgr().getMainFileID()).write(llvm::outs());
    rewriter.InsertTextAfter(rewriteLoc, "template<typename T, int NodeID>\n");
    rewriter.InsertTextAfter(rewriteLoc, "class numa<T , NodeID, NumaAllocator, typename std::enable_if<std::is_same<T, "+className+">::value>::type>"+ "{\n");

    rewriter.InsertTextAfter(rewriteLoc, allocator_funcs);
    //iterate through the field of the class and start typing the fields of the class
    for(auto field : numaed_class->fields())
    {
        rewriter.InsertTextAfter(rewriteLoc, "   numa<" + field->getType().getAsString() + ", " + integerarg + "> " + field->getNameAsString() + ";\n");
    }

    rewriter.InsertTextAfter(rewriteLoc, "public:\n");

    //get all constructors out of numaed class
    for(auto constructor : numaed_class->ctors())
    {
        if(constructor){

            //check if the constructor has initilizer list
            if(constructor->getNumCtorInitializers() > 0){
                llvm::outs() << "Constructor has an initializer list\n";
                llvm::outs() << "Construcor has " << constructor->getNumCtorInitializers() << " initializers\n";
                std::string constructor_name = constructor->getNameAsString();
                //constructor->dump();
                //get the entire line of the constructor as text
                SourceRange ConstructorRange = constructor->getSourceRange();
                const SourceManager &SM = constructor->getASTContext().getSourceManager();
                llvm::StringRef ConstructorText = Lexer::getSourceText(CharSourceRange::getTokenRange(ConstructorRange), SM, constructor->getASTContext().getLangOpts());
                llvm::outs() << "Constructor Text:\n" << ConstructorText << "\n";
                std::string line = (std::string)ConstructorText;
                //llvm::outs()<<replaceConstructWithInits(line)<<"\n"; 
                rewriter.InsertTextAfter(rewriteLoc, replaceConstructWithInits(line));
                rewriter.InsertTextAfter(rewriteLoc, "\n");
            }

            else{
                if(constructor->isUserProvided()){   
                    rewriter.InsertTextAfter(rewriteLoc, "numa(");

                    //if the constructor has no parameters, we just close the constructor
                    if (constructor->parameters().size() == 0)
                    {
                        rewriter.InsertTextAfter(rewriteLoc, ");\n");
                    }
                    //rewrite the paramenters of the constructor
                    else{
                        for(auto param : constructor->parameters())
                        {
                            rewriter.InsertTextAfter(rewriteLoc, param->getType().getAsString() + " " + param->getNameAsString());
                            //avoid the last comma
                            if(param != constructor->parameters().back())
                            {
                                rewriter.InsertTextAfter(rewriteLoc, ", ");
                            }
                        }
                        //after rewriting the parameters, we close the constructor
                        rewriter.InsertTextAfter(rewriteLoc, ")");

                        //if the constructor has a body, before we rewrite the body, we have to replace the new expression with new numa<T,N>
                        if(constructor->hasBody()){
                            llvm::outs() << "constructor has a body\n" ;
                            constructor->dump();
                            SourceRange BodyRange = constructor->getBody()->getSourceRange();
                            const SourceManager &SM = constructor->getASTContext().getSourceManager();
                            llvm::StringRef BodyText = Lexer::getSourceText(CharSourceRange::getTokenRange(BodyRange), SM, constructor->getASTContext().getLangOpts());
                            llvm::outs() << "Constructor Body:\n" << BodyText << "\n";
                            //Pass it through a function that searches for 'new' in the body and replaces 'new''s return type with numa<T,N>
                            //std::string numaedBody = replaceNewType(std::string(BodyText), N);
                            //Then we replace the body 
                            rewriter.InsertTextAfter(rewriteLoc, BodyText);
                            rewriter.InsertTextAfter(rewriteLoc, "\n");
                            }
                    }
                }
            }
        }
    }
    

    //get all methods from numaed class
    //for methods we don't have to worry about the method name or return type and parameters; because those are on the stack we don't numa anything
    for(auto method: numaed_class->methods()){
        //skip if numaed_class method is class name
        if(method){
            if(method->getNameAsString() == numaed_class->getNameAsString())
            {
                continue;
            }
            else{
                //if the method has a body, before we rewrite the body, we have to replace the new expression with new numa<T,N>
                if(method->hasBody()){
                    SourceRange MethodRange = method->getSourceRange();
                    const SourceManager &SM = method->getASTContext().getSourceManager();
                    llvm::StringRef MethodText = Lexer::getSourceText(CharSourceRange::getTokenRange(MethodRange), SM, method->getASTContext().getLangOpts());
                    //Pass it through a function that searches for 'new' in the body and replaces 'new''s return type with numa<T,N>
                    //std::string numaedBody = replaceNewType(std::string(MethodText),N);
                    rewriter.InsertTextAfter(rewriteLoc, MethodText);
                    rewriter.InsertTextAfter(rewriteLoc, "\n");
                }
            }
        }
    }

    

    //Then we copy the helper methods from numa (possibly everything)   todo
    auto all_decls = varClassTemplateSpecializationDecl->decls(); 
    for(auto decl : all_decls)
    {
        //for now we are obly copying the methods
        if((std::string)decl->getDeclKindName() == "CXXMethod")
        {
            llvm::outs() << "The method name is: " << decl->getDeclKindName() << "\n";
            SourceRange MethodRange = decl->getSourceRange();
            const SourceManager &SM = decl->getASTContext().getSourceManager();
            llvm::StringRef MethodText = Lexer::getSourceText(CharSourceRange::getTokenRange(MethodRange), SM, decl->getASTContext().getLangOpts());
            // Print the method body as a string.
            llvm::outs() << "Method Body:\n" << MethodText << "\n";
            rewriter.InsertTextAfter(rewriteLoc, MethodText);
            rewriter.InsertTextAfter(rewriteLoc , "\n");

        }
    }
}

void TemplateArgTransformer::myTemplateTransformer(clang::VarDecl* varDecl){
    auto varType = varDecl->getType();   
        //get the type of the variable as string
    auto varTypeAsString = varType.getAsString();

    //get the first 4 characters of the string to check if it is numa
    if(varTypeAsString.substr(0,4).compare("numa") == 0){
        //check if the numa variable decleration is a numa type pointer   :    numa<T,N>* var;
        //llvm::outs() << "Is pointer: " << isptr << "\n";
        if (varDecl->getType()->isPointerType()){
            //check if the decleration is initialized     :    numa<T,N>* var = &some_address
            if(varDecl->hasInit()){
                //varDecl->getInit()->dump();
                
                //check to see if it is initialized with an address of heap memory with 'new' expression     :   numa<T,N>* var = new T(); 
                //remember the stack is per thread so we have no control of the node of the stack. Thats why we only focus on heap memory 

                std::string isNew = varDecl->getInit()->getStmtClassName();
                if (isNew.compare("CXXNewExpr") == 0){
                    isNew = "new";
                    llvm::outs() << varDecl->getNameAsString() << " is a numa type. It is a pointer and it is initialized to a 'new' heap object.\n"; 
                    //get the pointee type as a CXXRecordDecl       :     getting the numa<T,N> out of numa<T,N>* var = new T();
                    auto varCXXRecordDecl = varDecl->getType().getTypePtr()->getPointeeType()->getAsCXXRecordDecl();

                    //CXXRecordDecls can be casted as ClassTemplateDecl or ClassTemplateSpecializationDecl      :       todo: check if this is safe
                    //This is to iterate through the template arguments to get the nodeID and the type separateley      :       getting [T,N] out of numa<T, N>
                    auto varClassTemplateSpecializationDecl = dyn_cast<ClassTemplateSpecializationDecl>(varCXXRecordDecl);

                    for(auto &arg : varClassTemplateSpecializationDecl->getTemplateArgs().asArray()){
                        // arg.getKind() returns an enum of TemplateArgument::ArgKind
                        //get the Node ID       :       getting N out of [T,N]
                        if(arg.getKind() == TemplateArgument::ArgKind::Integral)
                            {   
                                integerarg = std::to_string(arg.getAsIntegral().getExtValue());
                                llvm::outs() << "The node number for "<< varDecl->getNameAsString() <<" is : " <<integerarg << "\n";
                            }
                    }

                    //The need for iterating again is because we want to extract the node number first
                    for(auto &arg : varClassTemplateSpecializationDecl->getTemplateArgs().asArray()){
                        //get the type of the variable       :       getting T out of [T,N]
                        if(arg.getKind() == TemplateArgument::ArgKind::Type){
                            //check if the type is a builtin type       :   check to see if T is not a int, float, double, char, etc

                            if (arg.getAsType()->isBuiltinType())
                            {
                                //if it is a builtin type, we break. There is nothing to do here. On to the next var decl.
                                break;
                            }

                            //get the type as a CXXRecordDecl to start introspecting the fields of the class    :       getting T as class T.       
                            auto argCXXRecordDecl = arg.getAsType()->getAsCXXRecordDecl();
                            if(argCXXRecordDecl){
                                numaed_class = argCXXRecordDecl;                //numaed class is a global variable that is used to introspect the class
                            }
                            className = argCXXRecordDecl->getNameAsString();

                            //we will start rewriting our numa class right above the already declared class. We get the location to rewrite.
                            llvm::outs()<< "HERE\n";
                            rewriteLoc= argCXXRecordDecl->getBeginLoc();
                            
                            rewriteLoc.dump(varDecl->getASTContext().getSourceManager());
                            llvm::outs()<<"AND HERE\n";
                            llvm::outs() << "The location for its template class name is  " << rewriteLoc.printToString(varDecl->getASTContext().getSourceManager()) << "\n";
                            //iterate through the fields of the class
                            for(auto field : argCXXRecordDecl->fields())
                            {      
                                //Check if the field is a pointer
                                auto isFieldPtr = field->getType()->isPointerType();

                                if (isFieldPtr){
                                    //llvm::outs() << "The field is a pointer \n";
                                    auto isFieldFundamentalPtr = field->getType()->getPointeeType()->isFundamentalType();
                                    
                                    //if the field is a fundamental pointer type, we will not call the recursive typer for user declared types.
                                    //however, we havent decided if there will be a difference between numa<int*,N> and numa<int,N>*.  TODO
                                    if (isFieldFundamentalPtr){
                                        //llvm::outs() << "The field is a pointer to a fundamental type " << "\n";
                                        //skip for now, we are not sure what we want to do with this case.
                                    }
                                    else if(!field->getType()->getPointeeType()->isFundamentalType()){
                                        llvm::outs()<< "The field member variable is a pointer but not a fundamental type \n";
                                        llvm::outs() <<"The field member variable's type is "<< field->getType().getAsString() << ". About to recursively introspect.\n";

                                        //if the field is a pointer to a user defined class we have to call our recursive introspective typer
                                        recursive_introspective_typer(field, integerarg, rewriteLoc, varClassTemplateSpecializationDecl);   
                                    }
                                }
                                // if field is not a pointer and also if it is a user defined class, we again need to call our recursive introspective typer
                                else if (!field->getType()->isFundamentalType()){
                                    recursive_introspective_typer(field, integerarg, rewriteLoc,varClassTemplateSpecializationDecl);
                                }
                                else {
                                }

                                //Otherwise, i.e if field is a pure fundamental type, we will insert it to the field info table
                                //Field info table is a map with the variable declaration name as a key and another map that has the type as a key and a boolean
                                //value that tells whether the type is a pointer or not.
                                field_info_table.insert({field->getNameAsString(), {{field->getType().getAsString(), field->getType()->isPointerType()}}});

                                //print out filed_info_table in a pretty way
                                for(auto &field : field_info_table){
                                    llvm::outs() << field.first << " : ";
                                    for(auto &type : field.second){
                                        llvm::outs() << type.first << " : " << type.second << "\n";
                                    }
                                }
                            }

                        //Now you have all your fundamental types in the field_info_table you can call fundamental_introsective_typer    
                        fundamental_introspective_typer(className, integerarg, rewriteLoc, varClassTemplateSpecializationDecl); 
                        
                        //once you are done, you can clear the field_info_table for the next numa<T,N> var declaration
                        field_info_table.clear();
                        }
                    }
                }
            }
            //close your constructed template with a  };
            rewriter.InsertTextAfter(rewriteLoc , "};\n");
            llvm::outs() << "HOLLAAA\n";  
            llvm::StringRef file_name = rewriter.getSourceMgr().getFilename(rewriteLoc);
            //add "out_" to the file name that is after the last /
            std::string output_file = "out_" + file_name.rsplit('/').second.str();
            llvm::outs() << "The output file is: " << output_file << "\n";
            std::error_code error_code;
            llvm::raw_fd_ostream outFile(output_file, error_code);
            rewriter.buffer_begin()->second.write(outFile); 
        }
    }           
}



void TemplateArgTransformer::findNumaExpr(CompoundStmt* compoundStmt, ASTContext *Context){
    //check if the type of the new expression starts with numa
    //newExpr->dump();
    CXXNewExprVisitor CXXNewExprVisitor(Context);
    if(CXXNewExprVisitor.TraverseStmt(compoundStmt)){
        for(auto newExpr : CXXNewExprVisitor.getCompoundStmts()){
            if(newExpr){
                auto newType = newExpr->getType().getAsString();
                if(newType.substr(0,4).compare("numa") == 0){
                    newExpr->dump();
                }
            }
        }
    }     
}
    
void TemplateArgTransformer::startRecursiveTyping(std::map<clang::VarDecl*, const clang::CXXNewExpr*> numaDeclTable, clang::ASTContext *Context){

}

void TemplateArgTransformer::extractNumaDecls(clang::Stmt* fnBody, ASTContext *Context){
    CompoundStmtVisitor CompoundStmtVisitor(Context);
    DeclStmtVisitor DeclStmtVisitor(Context);
    CXXNewExprVisitor CXXNewExprVisitor(Context);
    VarDeclVisitor VarDeclVisitor(Context);
    if(DeclStmtVisitor.TraverseStmt(fnBody)){
        for(auto declStmt : DeclStmtVisitor.getDeclStmts()){
            if(CXXNewExprVisitor.TraverseStmt(declStmt)){
                //add the vardecl name, the type and the newExpr to the numaTable
                for(auto newExpr : CXXNewExprVisitor.getCompoundStmts()){
                    if(newExpr){
                        auto newType = newExpr->getType().getAsString();
                        for (auto decl : declStmt->decls()){
                            if(newType.substr(0,4).compare("numa") == 0){
                                if(VarDecl *varDecl = dyn_cast<VarDecl>(decl)){
                                    llvm::outs() << "Gonna add variable "<< varDecl->getNameAsString() << " to the numa table\n";
                                    setNumaDeclTable(varDecl, newExpr);
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


bool TemplateArgTransformer::NumaDeclExists(clang::ASTContext *Context, QualType FirstTempArg, int64_t SecondTempArg){
    clang::TranslationUnitDecl *TU = Context->getTranslationUnitDecl();
    llvm::outs() << "About to check if the numa template specialization for " << FirstTempArg.getAsString() << " and " << SecondTempArg << " exists\n";
    for (const auto *Decl : TU->decls()) {
        if (const auto *SpecDecl = clang::dyn_cast<clang::ClassTemplateSpecializationDecl>(Decl)) {
            const auto *TemplateDecl = SpecDecl->getSpecializedTemplate();
            //check if its numa
            if(TemplateDecl->getNameAsString().compare("numa") == 0){
                // llvm::outs()<< "We found a numa specialization here it is\n";
                // SpecDecl->dump();
                clang::QualType FoundType;
                llvm::APSInt FoundInt;
                //get arguments : You have to go through this because in the entire context, it might not be the case that the first parameter is always a type and the second is always an integral
                for (const auto &Arg : SpecDecl->getTemplateArgs().asArray()) {
                    if (Arg.getKind() == clang::TemplateArgument::ArgKind::Type) {
                        FoundType = Arg.getAsType();
                        //llvm::outs() << "The type is " << FoundType.getAsString() << "\n";
                    }
                    if (Arg.getKind() == clang::TemplateArgument::ArgKind::Integral) {
                        //llvm::outs() << "The integral is " << Arg.getAsIntegral() << "\n";
                        FoundInt = Arg.getAsIntegral();
                    }
                }

                //if not in specialized classes ad the foundtype and found integral to the specialized classes
                auto it = specializedClasses.find(FoundType);
                if (it != specializedClasses.end() && it->second == FoundInt) {
                    
                } else {
                    llvm::outs() << "Adding " << FoundType.getAsString() << " and " << FoundInt << " to the specialized classes\n";
                   // TemplateArgTransformer.insert({FoundType, FoundInt});
                }


                // llvm::outs() << "The found type is " << FoundType.getAsString() << " and the found integral is " << FoundInt << "\n";
                // llvm::outs() << "The checked type is " << FirstTempArg.getAsString() << " and the checked integral is " << SecondTempArg << "\n";
                // if(FoundType == FirstTempArg && FoundInt == SecondTempArg){
                //     return true;    
                // }
            }
        }
    }   
    return true;     
}

void TemplateArgTransformer::addAllSpecializations(clang::ASTContext* Context){
    clang::TranslationUnitDecl *TU = Context->getTranslationUnitDecl();
    for (const auto *Decl : TU->decls()) {
        if (const auto *SpecDecl = clang::dyn_cast<clang::ClassTemplateSpecializationDecl>(Decl)) {
            const auto *TemplateDecl = SpecDecl->getSpecializedTemplate();
            //check if its numa
            if(TemplateDecl->getNameAsString().compare("numa") == 0){
                // llvm::outs()<< "We found a numa specialization here it is\n";
                // SpecDecl->dump();
                clang::QualType FoundType;
                llvm::APSInt FoundInt;
                //get arguments : You have to go through this because in the entire context, it might not be the case that the first parameter is always a type and the second is always an integral
                for (const auto &Arg : SpecDecl->getTemplateArgs().asArray()) {
                    if (Arg.getKind() == clang::TemplateArgument::ArgKind::Type) {
                        FoundType = Arg.getAsType();
                        //llvm::outs() << "The type is " << FoundType.getAsString() << "\n";
                    }
                    if (Arg.getKind() == clang::TemplateArgument::ArgKind::Integral) {
                        //llvm::outs() << "The integral is " << Arg.getAsIntegral() << "\n";
                        FoundInt = Arg.getAsIntegral();
                    }
                }
                //if not in specialized classes add the foundtype and found integral to the specialized classes
                auto it = specializedClasses.find(FoundType);
                if (it != specializedClasses.end() && it->second == FoundInt) {
                    
                } else {
                    llvm::outs() << "Adding " << FoundType.getAsString() << " and " << FoundInt << " to the specialized classes\n";
                    
                    TemplateArgTransformer::specializedClasses.insert({FoundType, FoundInt.getExtValue()});
                   // TemplateArgTransformer.insert({FoundType, FoundInt});
                }
            }
        }
    }   
}

bool TemplateArgTransformer::NumaSpeclExists(clang::QualType FirstTempArg, int64_t SecondTempArg){
    auto it = specializedClasses.find(FirstTempArg);
    if (it != specializedClasses.end() && it->second == SecondTempArg) {
        return true;
    } else {
        return false;
    }
}

void TemplateArgTransformer::makeVirtual(CXXRecordDecl * classDecl){
    for(auto method : classDecl->methods()){
        if(method->isUserProvided()){
            //check if it is not a constructor
            if(method->getNameAsString() != classDecl->getNameAsString()){
                method->setVirtualAsWritten(true);
                // print reconstructed function
                //method->dump();
            }
            // Insert the virtual keyword if method is not a constructor
            if(method->getNameAsString() != classDecl->getNameAsString()){
                rewriteLoc = method->getBeginLoc();
                rewriter.InsertTextBefore(rewriteLoc, "virtual ");
                //getFIle ID 
                fileIDs.push_back(rewriter.getSourceMgr().getFileID(rewriteLoc));     
            }

        }
    }
}




void TemplateArgTransformer::specializeClass(clang::ASTContext* Context, clang::QualType FirstTempArg, int64_t SecondTempArg){

    //check if the class is in specialized classes
    if( FirstTempArg.getCanonicalType()->isBuiltinType()){
        llvm::outs() << "The numa argument "<< FirstTempArg.getAsString() << " is canonically a built in type\n";
        return;
    }
    if(NumaSpeclExists(FirstTempArg, SecondTempArg)){
        llvm::outs() << "The numa template specialization for " << FirstTempArg.getAsString() << " and " << SecondTempArg << " already exists\n";
        return;
    }

    llvm::outs() << "About to specialize "<< FirstTempArg.getAsString() << " as numa\n";

    //insert to specialized classes
    TemplateArgTransformer::specializedClasses.insert({FirstTempArg, SecondTempArg});

    llvm::outs() << "Making the methods of "<< FirstTempArg->getAsCXXRecordDecl()->getNameAsString() << " virtual\n";

    

    constructSpecialization(Context, FirstTempArg->getAsCXXRecordDecl(), SecondTempArg);

    // bool exists = NumaDeclExists(Context, FirstTempArg, SecondTempArg);

    // if(exists || FirstTempArg.getCanonicalType()->isBuiltinType()){
    //     llvm::outs() << "The numa template "<< FirstTempArg.getAsString() << " is already specialized according as numa or is canonically a built in type\n";
    // }
    // else{
       
        
    //     constructSpecialization(Context, FirstTempArg->getAsCXXRecordDecl(), SecondTempArg);
    // }
}

void TemplateArgTransformer::constructSpecialization(clang::ASTContext* Context, clang::CXXRecordDecl* classDecl, int64_t nodeID){
    makeVirtual(classDecl);
    
    rewriteLoc = classDecl->getEndLoc();
    SourceLocation semiLoc = Lexer::findLocationAfterToken(
                rewriteLoc, tok::semi, rewriter.getSourceMgr(), rewriter.getLangOpts(), 
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

    numaPublicMembers(Context, semiLoc, publicFields, publicMethods, nodeID);
    numaPrivateMembers(Context, semiLoc, privateFields, privateMethods, nodeID);

    rewriter.InsertTextAfter(semiLoc, "};\n");  

 
    //numaFields(classDecl, nodeID);
    // numaConstructors(classDecl, nodeID);
    // numaHeapInMethods(classDecl, nodeID);
    fileIDs.push_back(rewriter.getSourceMgr().getFileID(rewriteLoc));

}

void TemplateArgTransformer::numaPublicMembers(clang::ASTContext* Context, clang::SourceLocation& rewriteLocation, std::vector<FieldDecl*> publicFields, std::vector<CXXMethodDecl*> publicMethods, int64_t nodeID){
    rewriter.InsertTextAfter(rewriteLocation, "public:\n");
     for(auto fields :publicFields){
        llvm::outs() << "field about to be checked is " << fields->getNameAsString() << "\n";
        //QualType fieldType = QualType(classDecl->getTypeForDecl(),0);

        /*Case where the field is a built in type but not a pointer */
        if(fields->getType()->isBuiltinType()){
            llvm::outs()<<"Field is a fundamental type\n";
            rewriter.InsertTextAfter(rewriteLocation, "numa<"+fields->getType().getAsString()+","+std::to_string(nodeID)+"> "+ fields->getNameAsString()+";\n" );
        }

        /*Case where the field is a built in type and a pointer*/
        else if(fields->getType()->isPointerType() && fields->getType()->getPointeeType()->isBuiltinType()){
                llvm::outs()<<"Field is a pointer\n";
                rewriter.InsertTextAfter(rewriteLocation, "numa<"+fields->getType()->getPointeeType().getAsString() +","+std::to_string(nodeID)+">* "+ fields->getNameAsString()+";\n" );
            
        }

        /*Case where the field is not a built in type but is a pointer*/
        else if(fields->getType()->isPointerType() && !fields->getType()->getPointeeType()->isBuiltinType()){
            llvm::outs()<<"Field is a pointer to a user defined class\n";
            rewriter.InsertTextAfter(rewriteLocation, "numa<"+fields->getType()->getPointeeType().getAsString() +","+std::to_string(nodeID)+">* "+ fields->getNameAsString()+";\n" );
            
            if(NumaSpeclExists(QualType(fields->getType()->getPointeeCXXRecordDecl()->getTypeForDecl(),0) , nodeID)){
                llvm::outs() << "The numa template specialization for " << fields->getNameAsString() << " and " << nodeID << " already exists\n";
                continue;
            }else{

            llvm::outs() << "About to specialize "<< fields->getNameAsString() << " as numa\n";

            //insert to specialized classes
            TemplateArgTransformer::specializedClasses.insert({QualType(fields->getType()->getPointeeCXXRecordDecl()->getTypeForDecl(),0) , nodeID});

            constructSpecialization(Context, fields->getType()->getPointeeType()->getAsCXXRecordDecl(), nodeID);
            }
        }
        /*Case where the field is not a built in type and not a pointer*/
        else if (!fields->getType()->isBuiltinType() && !fields->getType()->isPointerType()){
            llvm::outs()<<"Field is a user defined class but not a pointer\n";
            rewriter.InsertTextAfter(rewriteLocation, "numa<"+fields->getType().getAsString() +","+std::to_string(nodeID)+"> "+ fields->getNameAsString()+";\n" );
            if(NumaSpeclExists(QualType(fields->getType()->getAsCXXRecordDecl()->getTypeForDecl(),0), nodeID)){
                llvm::outs() << "The numa template specialization for " << fields->getNameAsString() << " and " << nodeID << " already exists\n";
                continue;
            }else{
                llvm::outs() << "About to specialize "<< fields->getNameAsString() << " as numa\n";
                //insert to specialized classes
                TemplateArgTransformer::specializedClasses.insert({QualType(fields->getType()->getAsCXXRecordDecl()->getTypeForDecl(),0), nodeID});
                constructSpecialization(Context, fields->getType()->getAsCXXRecordDecl(), nodeID);
            }
        }
        else{
            llvm::outs()<<"None of the above\n";
        }
         
    }   

    for(auto method : publicMethods){
            //check if constructor
        llvm::outs() << "The method name is " << method->getNameAsString() << "\n";
        if (auto Ctor = dyn_cast<CXXConstructorDecl>(method)){
            
            if(Ctor->isUserProvided()){
                llvm::outs() << "THE CONSTRUCTOR IS " << Ctor->getNameAsString() << "\n";
                numaConstructors(Ctor, rewriteLocation, nodeID);
            }
        }
        else if (auto Dtor = dyn_cast<CXXDestructorDecl>(method)){
            if(Dtor->isUserProvided()){
                llvm::outs() << "THE DESTRUCTOR IS " << Dtor->getNameAsString() << "\n";
                numaDestructors(Dtor, rewriteLocation, nodeID);
            }
        }
        else{
        }
    }
}

void TemplateArgTransformer::numaPrivateMembers(clang::ASTContext* Context, clang::SourceLocation& rewriteLocation, std::vector<FieldDecl*> privateFields, std::vector<CXXMethodDecl*> privateMethods, int64_t nodeID){
        rewriter.InsertTextAfter(rewriteLocation, "private:\n");
     for(auto fields :privateFields){
        llvm::outs() << "field about to be checked is " << fields->getNameAsString() << "\n";
        //QualType fieldType = QualType(classDecl->getTypeForDecl(),0);

        /*Case where the field is a built in type but not a pointer */
        if(fields->getType()->isBuiltinType()){
            llvm::outs()<<"Field is a fundamental type\n";
            rewriter.InsertTextAfter(rewriteLocation, "numa<"+fields->getType().getAsString()+","+std::to_string(nodeID)+"> "+ fields->getNameAsString()+";\n" );
        }

        /*Case where the field is a built in type and a pointer*/
        else if(fields->getType()->isPointerType() && fields->getType()->getPointeeType()->isBuiltinType()){
                llvm::outs()<<"Field is a pointer\n";
                rewriter.InsertTextAfter(rewriteLocation, "numa<"+fields->getType()->getPointeeType().getAsString() +","+std::to_string(nodeID)+">* "+ fields->getNameAsString()+";\n" );
            
        }

        /*Case where the field is not a built in type but is a pointer*/
        else if(fields->getType()->isPointerType() && !fields->getType()->getPointeeType()->isBuiltinType()){
            llvm::outs()<<"Field is a pointer to a user defined class\n";
            rewriter.InsertTextAfter(rewriteLocation, "numa<"+fields->getType()->getPointeeType().getAsString() +","+std::to_string(nodeID)+">* "+ fields->getNameAsString()+";\n" );
            
            if(NumaSpeclExists(QualType(fields->getType()->getPointeeCXXRecordDecl()->getTypeForDecl(),0) , nodeID)){
                llvm::outs() << "The numa template specialization for " << fields->getNameAsString() << " and " << nodeID << " already exists\n";
                continue;
            }else{

            llvm::outs() << "About to specialize "<< fields->getNameAsString() << " as numa\n";

            //insert to specialized classes
            TemplateArgTransformer::specializedClasses.insert({QualType(fields->getType()->getPointeeCXXRecordDecl()->getTypeForDecl(),0) , nodeID});

            constructSpecialization(Context, fields->getType()->getPointeeType()->getAsCXXRecordDecl(), nodeID);
            }
        }
        /*Case where the field is not a built in type and not a pointer*/
        else if (!fields->getType()->isBuiltinType() && !fields->getType()->isPointerType()){
            llvm::outs()<<"Field is a user defined class but not a pointer\n";
            rewriter.InsertTextAfter(rewriteLocation, "numa<"+fields->getType().getAsString() +","+std::to_string(nodeID)+"> "+ fields->getNameAsString()+";\n" );
            if(NumaSpeclExists(QualType(fields->getType()->getAsCXXRecordDecl()->getTypeForDecl(),0), nodeID)){
                llvm::outs() << "The numa template specialization for " << fields->getNameAsString() << " and " << nodeID << " already exists\n";
                continue;
            }else{

            llvm::outs() << "About to specialize "<< fields->getNameAsString() << " as numa\n";

            //insert to specialized classes
            TemplateArgTransformer::specializedClasses.insert({QualType(fields->getType()->getAsCXXRecordDecl()->getTypeForDecl(),0), nodeID});

            constructSpecialization(Context, fields->getType()->getAsCXXRecordDecl(), nodeID);
            }
        }
        else{
            llvm::outs()<<"None of the above\n";
        }
    } 
    for(auto method : privateMethods){
        //check if constructor
        if (auto Ctor = dyn_cast<CXXConstructorDecl>(method)){
            
            if(Ctor->isUserProvided()){
                llvm::outs() << "THE CONSTRUCTOR IS " << Ctor->getNameAsString() << "\n";
                numaConstructors(Ctor, rewriteLocation, nodeID);
            }
        }
        else if (auto Dtor = dyn_cast<CXXDestructorDecl>(method)){
            if(Dtor->isUserProvided()){
                numaDestructors(Dtor, rewriteLocation, nodeID);
            }
        }
        else{
        }
    }
}

void TemplateArgTransformer::numaConstructors(clang::CXXConstructorDecl* constructor, clang::SourceLocation& rewriteLocation, int64_t nodeID){
    bool isWritten = false;
    if(constructor->getNumCtorInitializers() > 0){
        llvm::outs() << "Constructor "<<constructor->getNameAsString()<< " has an initializer list\n";
        llvm::outs() << "The initializer list has " << constructor->getNumCtorInitializers() << " initializers\n";
        for (auto Init = constructor->init_begin(); Init != constructor->init_end(); ++Init) {
            if ((*Init)->isWritten()) {
                llvm::outs() << "  Initializes member is written\n";
                isWritten = true;
            }
        }
    }
    if(isWritten){
            std::string constructor_name = constructor->getNameAsString();
            //constructor->dump();
            //get the entire line of the constructor as text
            SourceRange ConstructorRange = constructor->getSourceRange();
            const SourceManager &SM = constructor->getASTContext().getSourceManager();
            llvm::StringRef ConstructorText = Lexer::getSourceText(CharSourceRange::getTokenRange(ConstructorRange), SM, constructor->getASTContext().getLangOpts());
            llvm::outs() << "Constructor Text:\n" << ConstructorText << "\n";
            std::string line = (std::string)ConstructorText;
            //llvm::outs()<<replaceConstructWithInits(line)<<"\n"; 
            rewriter.InsertTextAfter(rewriteLocation, replaceConstructWithInits(line));
            rewriter.InsertTextAfter(rewriteLocation, "\n");
    }
    else{
        llvm::outs() << "Constructor "<<constructor->getNameAsString()<< " has no initializer list\n";
        rewriter.InsertTextAfter(rewriteLocation, "numa(");

        //if the constructor has no parameters, we just close the constructor
        if (constructor->parameters().size() == 0){
            rewriter.InsertTextAfter(rewriteLocation, ")\n");
        
            if(constructor->hasBody()){
                llvm::outs() << "constructor has a body\n" ;
                constructor->dump();
                SourceRange BodyRange = constructor->getBody()->getSourceRange();
                const SourceManager &SM = constructor->getASTContext().getSourceManager();
                llvm::StringRef BodyText = Lexer::getSourceText(CharSourceRange::getTokenRange(BodyRange), SM, constructor->getASTContext().getLangOpts());
                llvm::outs() << "Constructor Body:\n" << BodyText << "\n";
                //Pass it through a function that searches for 'new' in the body and replaces 'new''s return type with numa<T,N>
                //std::string numaedBody = replaceNewType(std::string(BodyText), N);
                //Then we replace the body 
                rewriter.InsertTextAfter(rewriteLocation, BodyText);
                rewriter.InsertTextAfter(rewriteLocation, "\n");
            }
        }
        //rewrite the paramenters of the constructor
        else{
            for(auto param : constructor->getDefinition()->parameters())
            {
                //get the implementation of the constructor
                
                rewriter.InsertTextAfter(rewriteLocation, param->getType().getAsString() + " " + param->getNameAsString());
                llvm::outs() << "The parameter is " << param->getType().getAsString() << " and the name is " << param->getNameAsString() << "\n";
                //avoid the last comma
                if(param != constructor->getDefinition()->parameters().back())
                {
                    rewriter.InsertTextAfter(rewriteLocation, ", ");
                }
            }
            //after rewriting the parameters, we close the constructor
            rewriter.InsertTextAfter(rewriteLocation, ")");

            //if the constructor has a body, before we rewrite the body, we have to replace the new expression with new numa<T,N>
            if(constructor->hasBody()){
                llvm::outs() << "constructor has a body\n" ;
                constructor->dump();
                SourceRange BodyRange = constructor->getBody()->getSourceRange();
                const SourceManager &SM = constructor->getASTContext().getSourceManager();
                llvm::StringRef BodyText = Lexer::getSourceText(CharSourceRange::getTokenRange(BodyRange), SM, constructor->getASTContext().getLangOpts());
                llvm::outs() << "Constructor Body:\n" << BodyText << "\n";
                //Pass it through a function that searches for 'new' in the body and replaces 'new''s return type with numa<T,N>
                //std::string numaedBody = replaceNewType(std::string(BodyText), N);
                //Then we replace the body 
                rewriter.InsertTextAfter(rewriteLocation, BodyText);
                rewriter.InsertTextAfter(rewriteLocation, "\n");
            }
        } 
    }
}
   

        


void TemplateArgTransformer::numaDestructors(clang::CXXDestructorDecl* destructor, clang::SourceLocation& rewriteLocation, int64_t nodeID){
    //if the constructor has no parameters, we just close the constructor

    rewriter.InsertTextAfter(rewriteLocation, "~numa(");
    if (destructor->parameters().size() == 0){
        rewriter.InsertTextAfter(rewriteLocation, ")\n");
    
        if(destructor->hasBody()){
            llvm::outs() << "destructor has a body\n" ;
            destructor->dump();
            SourceRange BodyRange = destructor->getBody()->getSourceRange();
            const SourceManager &SM = destructor->getASTContext().getSourceManager();
            llvm::StringRef BodyText = Lexer::getSourceText(CharSourceRange::getTokenRange(BodyRange), SM, destructor->getASTContext().getLangOpts());
            llvm::outs() << "Destructor Body:\n" << BodyText << "\n";
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
            llvm::outs() << "The parameter is " << param->getType().getAsString() << " and the name is " << param->getNameAsString() << "\n";
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
            llvm::outs() << "destructor has a body\n" ;
            destructor->dump();
            SourceRange BodyRange = destructor->getBody()->getSourceRange();
            const SourceManager &SM = destructor->getASTContext().getSourceManager();
            llvm::StringRef BodyText = Lexer::getSourceText(CharSourceRange::getTokenRange(BodyRange), SM, destructor->getASTContext().getLangOpts());
            llvm::outs() << "Destructor Body:\n" << BodyText << "\n";
            //Pass it through a function that searches for 'new' in the body and replaces 'new''s return type with numa<T,N>
            //std::string numaedBody = replaceNewType(std::string(BodyText), N);
            //Then we replace the body 
            rewriter.InsertTextAfter(rewriteLocation, BodyText);
            rewriter.InsertTextAfter(rewriteLocation, "\n");
        }
    } 
}



void TemplateArgTransformer::run(const clang::ast_matchers::MatchFinder::MatchResult &result){
    if(result.SourceManager->isInSystemHeader(result.Nodes.getNodeAs<FunctionDecl>("functionDecl")->getSourceRange().getBegin()))
        return;
    if(result.SourceManager->getFilename(result.Nodes.getNodeAs<FunctionDecl>("functionDecl")->getLocation()).find("../numaLib/numatype.hpp") != std::string::npos)
        return;
    if(result.SourceManager->getFilename(result.Nodes.getNodeAs<FunctionDecl>("functionDecl")->getLocation()).empty())
        return;
    if(result.Nodes.getNodeAs<FunctionDecl>("functionDecl")->isImplicit())
        return;
    if(result.Nodes.getNodeAs<FunctionDecl>("functionDecl")->getNameAsString().empty())
        return;

    if(result.Nodes.getNodeAs<FunctionDecl>("functionDecl")->isThisDeclarationADefinition()){   
        if(!result.Nodes.getNodeAs<FunctionDecl>("functionDecl")->getBody()->children().empty()){
            auto fnBody = result.Nodes.getNodeAs<FunctionDecl>("functionDecl")->getBody();
            llvm::outs() << "Processing Function : "<< result.Nodes.getNodeAs<FunctionDecl>("functionDecl")
            ->getNameAsString() << "\n";   
            // SourceRange SrcRange = result.Nodes.getNodeAs<FunctionDecl>("functionDecl")->getSourceRange();
            // SourceManager &SM = result.Context->getSourceManager();
            // LangOptions LO = result.Context->getLangOpts();

            // // Get the source code text from the source range
            // std::string FuncText = Lexer::getSourceText(CharSourceRange::getTokenRange(SrcRange), SM, LO).str();

            // llvm::outs()  << FuncText << "\n";
            extractNumaDecls(fnBody, result.Context);
            } 
    
        }
        printNumaDeclTable(); 

        addAllSpecializations(result.Context);

        for(auto &UserNumaDecl : numaDeclTable){
            QualType FirstTempArg;
            int64_t SecondTempArg;
            auto CXXRecordNumaType = UserNumaDecl.second->getType()->getPointeeType()->getAsCXXRecordDecl();
            auto TemplateNumaType = dyn_cast<ClassTemplateSpecializationDecl>(CXXRecordNumaType);
            llvm::ArrayRef<TemplateArgument> TemplateArgs = TemplateNumaType->getTemplateArgs().asArray();
            FirstTempArg = TemplateArgs[0].getAsType();
            SecondTempArg = TemplateArgs[1].getAsIntegral().getExtValue();

            specializeClass(result.Context,FirstTempArg,SecondTempArg);
        } 
    
    return;
}


void TemplateArgTransformer::print(clang::raw_ostream &stream)
{
    for(auto &fn : functions)
        stream << fn << "(..)\n";
}

