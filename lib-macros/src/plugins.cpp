/** @file `plugins.c`
 *  @brief This file implements the gbox clang plugins for gbclang and gbclang++
 *         preprocessing macros.
 */

#include "clang/AST/ASTConsumer.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"

using namespace clang;
using namespace clang::attr;
using namespace clang::ast_matchers;
using namespace llvm;

/**
 *  @brief TODO: DOCS
 */
class AnnotatedCallbackHandler : public MatchFinder::MatchCallback {
  public:
    //! @brief TODO: DOCS
    void run(const MatchFinder::MatchResult &Result) override {
        const FunctionDecl *FD = Result.Nodes.getNodeAs<FunctionDecl>("funcDecl");
        if (FD) {
            if (FD->hasBody()) {
                llvm::outs() << "Function: " << FD->getNameAsString() << " has attributes:\n";
                for (auto attr : FD->getAttrs()) {
                    // Check for specific attribute types, e.g., AnnotateAttr
                    if (const AnnotateAttr *Annot = dyn_cast<AnnotateAttr>(attr)) {
                        llvm::outs() << "  Annotate attribute: " << Annot->getAnnotation() << "\n";
                    } else {
                        // For general attributes, you can print their kind or spelling
                        llvm::outs() << "  Attribute kind ID: " << attr->getKind() << "\n";
                        // Or print a pretty string representation
                        std::string SS;
                        llvm::raw_string_ostream S(SS);
                        PrintingPolicy Policy(FD->getLangOpts());
                        attr->printPretty(S, Policy);
                        llvm::outs() << "  Attribute string: " << S.str() << "\n";
                    }
                }
            }
        }
    }
};

/**
 *  @brief TODO: DOCS
 */
class ProcMacroConsumer : public ASTConsumer {
  public:
    ProcMacroConsumer(std::string FileName) : FileName(std::move(FileName)) {}

    void HandleTranslationUnit(ASTContext &Context) override {
        AnnotatedCallbackHandler Handler;
        MatchFinder Finder;

        // Match any function declaration
        Finder.addMatcher(
            functionDecl().bind("funcDecl"),
            &Handler
        );

        Finder.matchAST(Context);
    }

  private:
    std::string FileName;  
};

class ProcMacroPlugin : public PluginASTAction {
  protected:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(
        CompilerInstance &CI, llvm::StringRef InFile
    ) override {
        return std::make_unique<ProcMacroConsumer>(InFile.str());
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

static FrontendPluginRegistry::Add<ProcMacroPlugin>
    X("gbox-macros-plugin", "Procedural macro scheme for C/C++");
