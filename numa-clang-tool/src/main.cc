/**
 * @file main.cc
 * @brief Main entry point for the NUMA-aware C++ transformation tool
 * @author Kidus Workneh
 * 
 * This tool provides automated NUMA-aware code transformations using Clang's
 * LibTooling infrastructure. It supports multiple transformation passes for
 * optimizing C++ code for NUMA architectures.
 */

#include <llvm/Support/CommandLine.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang-c/Index.h>
#include <iostream>
#include "actions/recurseFrontendAction.h"
#include "actions/castFrontendAction.h"
#include "utils/utils.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Basic/DiagnosticOptions.h"
#include "clang/Basic/FileManager.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Basic/TargetOptions.h"
#include "clang/CodeGen/ModuleBuilder.h"
#include "clang/Parse/ParseAST.h"
#include "llvm/TargetParser/Host.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/InitLLVM.h"
#include "llvm/Support/raw_ostream.h"
#include <string>
#include <iostream>
#include <sstream>
#include <string>

#include "clang/AST/AST.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Basic/SourceManager.h"

using namespace std;
using namespace llvm;
using namespace clang;
using namespace clang::tooling;
using namespace llvm::cl;

// ============================================================================
// Command Line Options Setup
// ============================================================================

/** @brief Custom category for tool-specific command line options */
static cl::OptionCategory MyToolCategory("my-tool options");

/** @brief Standard help message for compilation database options */
static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);

/** @brief Additional help text for this specific tool */
static cl::extrahelp MoreHelp("\nMore help text...\n");

/**
 * @brief Debug utility to print compilation commands
 * @param Commands Vector of compile commands to display
 */
void print(const std::vector<CompileCommand> &Commands) {
  if (Commands.empty()) {
    return;
  }
  for (auto opt : Commands[0].CommandLine) {
    llvm::errs() << "\t" << opt << "\n";
  }
}

// ============================================================================
// Frontend Action Factory
// ============================================================================

/**
 * @brief Creates appropriate frontend action factory based on pass name
 * @param Name String identifier for the transformation pass
 * @return Unique pointer to the corresponding frontend action factory
 */
static std::unique_ptr<FrontendActionFactory>
makeFactoryForPass(llvm::StringRef Name) {
  return llvm::StringSwitch<std::unique_ptr<FrontendActionFactory>>(Name)
    .Case("recurse", newFrontendActionFactory<RecurseFrontendAction>())  // NUMA class specialization pass
    .Case("cast",    newFrontendActionFactory<CastFrontendAction>())     // Type casting transformation pass
    // .Case("yourpass", newFrontendActionFactory<YourPassAction>())     // Template for additional passes
    .Default(nullptr);
}

// ============================================================================
// Main Function
// ============================================================================

/**
 * @brief Main entry point for the NUMA transformation tool
 * @param argc Number of command line arguments
 * @param argv Array of command line argument strings
 * @return Exit code (0 for success, non-zero for failure)
 */
int main(int argc, const char **argv) {

    static cl::OptionCategory ToolCat("my-clang-tool options");

    // ============================================================================
    // Command Line Interface Options
    // ============================================================================

    /** @brief Required option to specify which transformation pass to run */
    static cl::opt<std::string> PassName(
    "pass",
    cl::desc("Which pass to run (e.g., recurse, cast)"),
    cl::value_desc("name"),
    cl::Required,
    cl::cat(ToolCat));

    /** @brief Optional list of input files to process */
    static cl::list<std::string> InputFilesOpt(
    "input",
    cl::desc("Comma-separated or repeatable list of input files"),
    cl::ZeroOrMore,
    cl::CommaSeparated,
    cl::value_desc("file1.cpp,file2.cpp,..."),
    cl::cat(ToolCat));

    /** @brief Alternative positional file arguments */
    static cl::list<std::string> PositionalFiles(
    cl::Positional,
    cl::desc("<source files>"),
    cl::ZeroOrMore,
    cl::cat(ToolCat));

    // Parse command line arguments with custom category filter
    auto ExpectedParser = CommonOptionsParser::create(argc, argv, ToolCat);
    if (!ExpectedParser) {
        errs() << toString(ExpectedParser.takeError()) << "\n";
        return 1;
    }
    CommonOptionsParser &OptionsParser = ExpectedParser.get();

    // ============================================================================
    // Source File Collection
    // ============================================================================

    std::vector<std::string> Files;
    
    // Prioritize explicit --input flag over positional arguments
    if (!InputFilesOpt.empty()) {
        Files.insert(Files.end(), InputFilesOpt.begin(), InputFilesOpt.end());
    } else {
        // Fallback to positional files or compile_commands.json entries
        auto Pos = OptionsParser.getSourcePathList();
        Files.insert(Files.end(), Pos.begin(), Pos.end());
    }

    // Handle case where no files are specified
    if (Files.empty()) {
        llvm::errs() << "warning: no --input or positional file; "
                            "using dummy file to satisfy parser.\n";
        // Attempt to use files from compilation database
        auto DBFiles = OptionsParser.getSourcePathList();
        if (!DBFiles.empty())
        Files.push_back(DBFiles.front());
        else
        Files.push_back("dummy.cpp"); // Fallback dummy file
    }

    // ============================================================================
    // Pass Selection and Execution
    // ============================================================================

    // Create frontend action factory for the specified pass
    auto Factory = makeFactoryForPass(PassName);
    if (!Factory) {
        errs() << "error: unknown pass '" << PassName << "'. Available passes:\n"
            << "  - recurse\n"
            << "  - cast\n";
        return 1;
    }

    // Execute the transformation tool with selected pass
    ClangTool Tool(OptionsParser.getCompilations(), Files);
    return Tool.run(Factory.get());
}
