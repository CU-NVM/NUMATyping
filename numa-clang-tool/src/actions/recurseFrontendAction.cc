#include "recurseFrontendAction.h"
#include <clang/AST/ASTContext.h>
#include <clang/AST/ASTConsumer.h>
#include <clang/Frontend/CompilerInstance.h>
#include "clang/Lex/Preprocessor.h"
#include "../consumer/recurseConsumer.h"





std::unique_ptr<clang::ASTConsumer> RecurseFrontendAction::CreateASTConsumer(clang::CompilerInstance &compiler, llvm::StringRef inFile)
{
    llvm::errs() << "** Creating AST consumer from RecurseFrontendAction **" << inFile << "\n";
    TheRewriter.setSourceMgr(compiler.getSourceManager(), compiler.getLangOpts());
    return std::make_unique<RecurseConsumer>(TheRewriter, &compiler.getASTContext());
}

