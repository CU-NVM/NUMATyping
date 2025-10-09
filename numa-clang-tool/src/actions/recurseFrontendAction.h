#ifndef RECURSEFRONTENDACTION_HPP
#define RECURSEFRONTENDACTION_HPP

#include <llvm/ADT/StringRef.h>
#include <clang/Frontend/FrontendActions.h>
#include <clang/AST/ASTContext.h>
#include <clang/AST/ASTConsumer.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Rewrite/Core/Rewriter.h>
#include <llvm/Support/raw_ostream.h>
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Basic/Diagnostic.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/raw_ostream.h"
#include "../consumer/recurseConsumer.h"
#include <memory>
#include <vector>
#include <string>
using namespace clang;
namespace clang
{
    class CompilerInstance;
}

class RecurseFrontendAction : public clang::ASTFrontendAction {
public:
  virtual std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &compiler, llvm::StringRef inFile) override;
  //void EndSourceFileAction() override;
private:
  clang::Rewriter TheRewriter;

};



#endif