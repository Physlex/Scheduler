/** @file `plugins.c`
 *  @brief This file implements the gbox clang plugins for gbclang and gbclang++
 *         preprocessing macros.
 */

#include "clang/AST/ASTConsumer.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"

using namespace clang;
using namespace clang::attr;
using namespace llvm;

using namespace clang;

class HelloConsumer : public ASTConsumer {
  std::string FileName;
public:
  explicit HelloConsumer(std::string FileName) : FileName(std::move(FileName)) {}

  void HandleTranslationUnit(ASTContext &Context) override {
    llvm::outs() << "Hello, " << FileName << "\n";
  }
};

class HelloAction : public PluginASTAction {
  protected:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(
        CompilerInstance &CI, llvm::StringRef InFile
    ) override {
        return std::make_unique<HelloConsumer>(InFile.str());
    }

    bool ParseArgs(
        const CompilerInstance &CI, const std::vector<std::string> &Args
    ) override {
        return true;
    }

    ActionType getActionType() override {
        return AddAfterMainAction;
    }
};

static FrontendPluginRegistry::Add<HelloAction>
    X("hello-plugin", "Prints Hello, <file_name>");
