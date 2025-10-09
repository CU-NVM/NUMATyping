#ifndef UTILS_HPP
#define UTILS_HPP

#include <fstream>
#include <vector>
#include <string>

#include <llvm/ADT/StringRef.h>
#include <llvm/ADT/Twine.h>

#include <clang/Tooling/Tooling.h>
#include <clang/Frontend/FrontendActions.h>
#include "clang/Tooling/CompilationDatabase.h"
#include <clang/AST/Decl.h>
#include <clang/AST/Expr.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/Rewrite/Core/Rewriter.h>
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/Tooling.h"
#include <sstream>
#include <string>
#include "llvm/Support/raw_ostream.h"
#include <set>
#include <map>

using namespace clang;

namespace utils
{ 
   
    class NewExprInBinaryOperatorVisitor : public RecursiveASTVisitor<NewExprInBinaryOperatorVisitor>
    {
    public:
        explicit NewExprInBinaryOperatorVisitor(const clang::ASTContext *Context) {}
        bool VisitBinaryOperator(BinaryOperator *BO)
        {
            if (const Expr *RHS = BO->getRHS()) {
            std::string original_type;
                if(const CXXReinterpretCastExpr *ReinterpretCastExpr = dyn_cast<CXXReinterpretCastExpr>(RHS->IgnoreParenImpCasts())){
                    if(const CXXNewExpr *NewExpr = dyn_cast<CXXNewExpr>(ReinterpretCastExpr->getSubExpr()->IgnoreParenImpCasts())){
                        if ((NewExpr->getType().getAsString().substr(0, 4).compare("numa") == 0)|| (original_type.substr(0,4).compare("numa")==0))
                        {
                            const Expr *LHS = BO->getLHS();
                            QualType LHSType = LHS->getType();
                            CXXNewExprs.insert({NewExpr,LHSType});
                        }
                    }
                }


                if (const CXXNewExpr *NewExpr = dyn_cast<CXXNewExpr>(RHS->IgnoreParenImpCasts())) {

                    if ((NewExpr->getType().getAsString().substr(0, 4).compare("numa") == 0)|| (original_type.substr(0,4).compare("numa")==0))
                    {
                        const Expr *LHS = BO->getLHS();
                        QualType LHSType = LHS->getType();
                        CXXNewExprs.insert({NewExpr,LHSType});
                    }

                }
            }
            return true; 
        }
        
        std::map<const CXXNewExpr *, QualType> getBinaryOperators() { return CXXNewExprs; }
        void clearBinaryOperators() { CXXNewExprs.clear(); }
    private:
        std::map<const CXXNewExpr *, QualType> CXXNewExprs;
        // std::string original_type;
        
    };

    
    class DeclStmtVisitor : public RecursiveASTVisitor<DeclStmtVisitor>{
    public:
        explicit DeclStmtVisitor(const clang::ASTContext *Context) {}
        bool VisitDeclStmt(DeclStmt *DS) {
            DeclStmts.push_back(DS);
            return true;
        }
        std::vector<DeclStmt* > getDeclStmts() { return DeclStmts; }
        void clearDeclStmts() { DeclStmts.clear(); }
    private:
        std::vector<DeclStmt*> DeclStmts;
        
    };

    bool fileExists(const std::string &file);
   

    class CompoundStmtVisitor : public RecursiveASTVisitor<CompoundStmtVisitor>{
        public:
            explicit CompoundStmtVisitor(const clang::ASTContext *Context){}
            bool VisitCompoundStmt(CompoundStmt *CD) {
                CompoundStmts.push_back(CD);
                return true;
            }
            std::vector<CompoundStmt* > getCompoundStmts() { return CompoundStmts; }
            void clearCompoundStmts() { CompoundStmts.clear(); }
        private:
            std::vector<CompoundStmt*> CompoundStmts;
            
    };

    class CXXNewExprVisitor : public RecursiveASTVisitor<CXXNewExprVisitor>{
    public:
        explicit CXXNewExprVisitor(const clang::ASTContext *Context)  {}
        bool VisitCXXNewExpr(CXXNewExpr *NE) {
            CXXNewExprs.push_back(NE);
            SourceLocation Loc = NE->getBeginLoc();
            CXXNewExprLocs.push_back(Loc);
            return true;
        }
        std::vector<CXXNewExpr* > getCXXNewExprs() { return CXXNewExprs; }
        std::vector<SourceLocation> getCXXNewExprLocs() { return CXXNewExprLocs; }
        void clearCXXNewExprs() { CXXNewExprs.clear(); CXXNewExprLocs.clear(); }
    private:
        std::vector<CXXNewExpr*> CXXNewExprs;
        std::vector<SourceLocation> CXXNewExprLocs;
        
    };

    class AssignmentOperatorCallVisitor :public RecursiveASTVisitor<AssignmentOperatorCallVisitor>{
    public:
        explicit AssignmentOperatorCallVisitor(const clang::ASTContext *Context){}
        bool VisitCXXOperatorCallExpr(CXXOperatorCallExpr *OCE){
            // if(OCE->isLValue()){
            AssignmentOperatorCallExprs.push_back(OCE);
            SourceLocation Loc = OCE->getBeginLoc();
            AssignmentOperatorCallExprLocs.push_back(Loc);
            return true;
            // }
            // return false;
        }
        std::vector<CXXOperatorCallExpr* > getAssignmentOperatorCallExprs() { return AssignmentOperatorCallExprs; }
        std::vector<SourceLocation> getAssignmentOperatorCallExprLocs() { return AssignmentOperatorCallExprLocs; }
        void clearAssignmentOperatorCallExprs() { AssignmentOperatorCallExprs.clear(); AssignmentOperatorCallExprLocs.clear(); }
    private:
        std::vector<CXXOperatorCallExpr*> AssignmentOperatorCallExprs;
        std::vector<SourceLocation> AssignmentOperatorCallExprLocs;
        
    };

    class MemberExprVisitor : public RecursiveASTVisitor<MemberExprVisitor>{
    public:
        explicit MemberExprVisitor(const clang::ASTContext *Context){}
        bool VisitMemberExpr(MemberExpr *ME) {
            MemberExprs.push_back(ME);
            return true;
        }
        std::vector<MemberExpr* > getMemberExprs() { return MemberExprs; }
        void clearMemberExprs() { MemberExprs.clear(); }
    private:
        std::vector<MemberExpr*> MemberExprs;
        
    };


    class VarDeclVisitor : public RecursiveASTVisitor<VarDeclVisitor>
    {
    public:
        explicit VarDeclVisitor(const clang::ASTContext *Context){}
        bool VisitVarDecl(VarDecl *VD) {
            VarDecls.push_back(VD);
            return true;
        }
        std::vector<VarDecl* > getVarDecls() { return VarDecls; }
        void clearVarDecls() { VarDecls.clear(); }
    private:
        std::vector<VarDecl*> VarDecls;
        
    };


    std::string getMemberInitString(std::map<std::string, std::string>& initMemberlist); 
    std::string getDelegatingInitString(CXXConstructorDecl* Ctor);
    std::string getNumaAllocatorCode(std::string classDecl, std::string nodeID);

}

#endif
