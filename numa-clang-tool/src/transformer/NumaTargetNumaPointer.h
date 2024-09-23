#ifndef NUMATARGETNUMAPOINTER_HPP
#define NUMATARGETNUMAPOINTER_HPP

#include "transformer.h"

// #include "RecursiveNumaTyper.h"
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
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/Analysis/BasicAliasAnalysis.h"
#include "clang/CodeGen/CodeGenAction.h"
// #include "clang/lib/CodeGen/CodeGenModule.h"
#include "clang/Frontend/FrontendActions.h"

#include <set>
#include <map>

// namespace clang
// {
//     class ASTContext;
//     class raw_ostream;
//     class Rewriter;
// }
using namespace clang;


class GetModule: public CodeGenAction{

   std::unique_ptr<llvm::Module> M = takeModule();
};

class AliasAnalysisPass: public llvm::PassInfoMixin<AliasAnalysisPass>{
    public:
        llvm::PreservedAnalyses run(llvm::Function &F, llvm::FunctionAnalysisManager &FAM){
            llvm::outs() << "Running Alias Analysis Pass\n";
        }
};






class NumaTargetNumaPointer : public Transformer
{
    private: 
    
    public:
        explicit NumaTargetNumaPointer(clang::ASTContext &context, clang::Rewriter &rewriter);

        virtual void start() override;
        virtual void print(clang::raw_ostream &stream) override;
        virtual void run(const clang::ast_matchers::MatchFinder::MatchResult &result);

        
};





#endif