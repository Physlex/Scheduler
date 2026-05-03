/** @file `plugins.c`
 *  @brief This file implements the gbox clang plugins for gbclang and gbclang++
 *         preprocessing macros.
 */

#include "gbox/proc/action/plugins.hpp"

#include <clang/AST/ASTContext.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Basic/TokenKinds.h>
#include <clang/Lex/LiteralSupport.h>
#include <clang/Lex/Token.h>
#include <llvm/ADT/StringRef.h>

#include <vector>

#include "clang/Basic/SourceLocation.h"
#include "clang/Frontend/CompilerInstance.h"
#include "gbox/proc/syn/parse.hpp"
#include "gbox/proc/syn/tokens.hpp"
#include "llvm/ADT/RewriteBuffer.h"
#include "llvm/Support/raw_ostream.h"

using namespace gbox::plugins;
using namespace gbox;

// TODO: Demonstrate full s2s translation in-memory using the system described prior.
tokens::TokenStream proc_macro_executor(tokens::TokenStream &input) {
    return tokens::TokenStream();
}

void HandleFuncDecl::run(const clang::ast_matchers::MatchFinder::MatchResult &res) {
    const clang::FunctionDecl *func_decl =
        res.Nodes.getNodeAs<clang::FunctionDecl>("funcDecl");
    if (!func_decl) {
        return;
    }

    const clang::FunctionDecl *fdef = func_decl->getDefinition();
    if (!fdef) {
        return;
    }

    std::vector<llvm::StringRef> annotations;

    for (auto attr : fdef->getAttrs()) {
        const auto *annotated_attr = dyn_cast<clang::AnnotateAttr>(attr);
        if (annotated_attr) {
            annotations.push_back(annotated_attr->getAnnotation());
        }
    }

    // Skip functions that have no annotations
    if (annotations.size() == 0) {
        return;
    }

    clang::ASTContext *ctx = res.Context;
    clang::SourceManager &sm = ctx->getSourceManager();

    clang::SourceRange src_range = fdef->getSourceRange();
    clang::SourceLocation src_begin = src_range.getBegin();
    clang::SourceLocation src_end = src_range.getEnd();

    auto file = sm.getFileID(src_begin);
    unsigned start = sm.getFileOffset(src_begin);
    unsigned end = sm.getFileOffset(src_end);

    // Lexing stage

    // TODO: SIMPLIFY
    const size_t length = end - start + 1;
    std::string slice = sm.getBufferData(file).substr(start, length).str();
    auto buffer = llvm::MemoryBufferRef(slice, "<scratch>");

    llvm::outs() << "Text:\n" << slice << "\n";

    clang::Token tok;
    std::vector<clang::Token> tokens;
    auto lexer = clang::Lexer(file, buffer, sm, res.Context->getLangOpts());
    while (!lexer.LexFromRawLexer(tok)) {
        tokens.push_back(tok);
    }

    if (!tok.is(clang::tok::eof)) {
        tokens.push_back(tok);
    }

    // Parsing stage

    parse::Parser parser = parse::Parser(
        parse::ClangCtx{tokens, sm, res, res.Context->getDiagnostics()},
        parse::FileInfo{file, buffer, length}
    );

    tokens::TokenStream stream;
    if (!parser.parse(stream)) {
        llvm::outs() << "Failed to parse tokens into tokens::TokenStream\n";
        return;
    }

    // Invoke tokens::TokenStream proc macro plugins

    // FIXME: Exactly what's on the tin
    // for (size_t idx = 0; idx < annotations.size(); idx += 1) {
    //     if (annotations[idx] == "executor") {
    //         proc_macro_executor(stream);
    //     }
    // }

    // Match against each token, and generate C++ code from them

    std::string code = stream.toString();
    std::string test_code =
        "static inline int32_t hello_msg(void *args) { printf(\"TEST: It Worked!\"); }";
    const clang::SourceRange range = fdef->getSourceRange();
    this->rewriter_.ReplaceText(range, test_code);
}

void ProcMacroConsumer::HandleTranslationUnit(clang::ASTContext &ctx) {
    HandleFuncDecl handler(this->rewriter_);
    clang::ast_matchers::MatchFinder finder;

    // Match against function definitions
    finder.addMatcher(clang::ast_matchers::functionDecl().bind("funcDecl"), &handler);
    finder.matchAST(ctx);
}

bool ProcMacroAction::BeginSourceFileAction(clang::CompilerInstance &ci) {
    const clang::FrontendOptions opts = ci.getFrontendOpts();
    if (opts.ProgramAction == clang::frontend::EmitObj) {
        return true;
    }

    return false;
}

void ProcMacroAction::EndSourceFileAction() {
    const llvm::RewriteBuffer *buf = rewriter_.getRewriteBufferFor(
        getCompilerInstance().getSourceManager().getMainFileID()
    );

    if (buf) {
        this->rewritten_ = std::string(buf->begin(), buf->end());
    }
}

std::string ProcMacroAction::getRewritten() const { return this->rewritten_; }
