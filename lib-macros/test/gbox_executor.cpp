/** @file `gbox_executor.cpp`
 *  @brief This file implements tests for the gbox executor attribute parser.
 */

#include <gtest/gtest.h>

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Tooling.h"

using namespace clang;
using namespace clang::tooling;
using namespace clang::attr;
using namespace llvm;

class FindNamedClassVisitor
    : public RecursiveASTVisitor<FindNamedClassVisitor> {
   public:
    bool VisitCXXRecordDecl(CXXRecordDecl* declaration) {
        // For debugging, dumping the AST nodes will show which nodes are
        // already being visited.
        declaration->dump();

        // The return value indicates whether we want the visitation to proceed.
        // Return false to stop the traversal of the AST.
        return true;
    }
};

class FindNamedClassConsumer : public clang::ASTConsumer {
   public:
    virtual void HandleTranslationUnit(clang::ASTContext& context) {
        // Traversing the translation unit decl via a RecursiveASTVisitor
        // will visit all nodes in the AST.
        visitor.TraverseDecl(context.getTranslationUnitDecl());
    }

   private:
    // A RecursiveASTVisitor implementation.
    FindNamedClassVisitor visitor;
};

class FindNamedClassAction : public clang::ASTFrontendAction {
   public:
    virtual std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
        clang::CompilerInstance& Compiler, llvm::StringRef InFile) {
        return std::make_unique<FindNamedClassConsumer>();
    }
};

TEST(runToolOnCode, CanSyntaxCheckCode) {
    runToolOnCode(
        std::make_unique<FindNamedClassAction>(), "class X {}; int main() { return 0; }"
    );
}
