/** @file `plugins.c`
 *  @brief This file implements the gbox clang plugins for gbclang and gbclang++
 *         preprocessing macros.
 */

#include "gbox/macros/plugins.hpp"

#include <clang/AST/ASTContext.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Basic/TokenKinds.h>
#include <clang/Lex/LiteralSupport.h>
#include <clang/Lex/Token.h>
#include <llvm/ADT/StringRef.h>

#include <vector>

#include "gbox/macros/parser.hpp"
#include "gbox/macros/tokens.hpp"
#include "llvm/Support/raw_ostream.h"

using namespace gbox;

// TODO: Demonstrate full s2s translation in-memory using the system described prior.
TokenStream proc_macro_executor(TokenStream &input) { return TokenStream(); }

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

    clang::FileID file = sm.getFileID(src_begin);
    unsigned start = sm.getFileOffset(src_begin);
    unsigned end = sm.getFileOffset(src_end);

    // Lexing stage

    // TODO: SIMPLIFY
    const size_t length = end - start + 1;
    std::string slice = sm.getBufferData(file).substr(start, length).str();
    std::replace(slice.begin(), slice.end(), '\n', '\0');
    auto owned_buffer = llvm::MemoryBuffer::getMemBufferCopy(slice, "");
    auto buffer = owned_buffer->getMemBufferRef();

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

    gbox::Parser parser = gbox::Parser(
        gbox::ClangCtx{tokens, sm, res, res.Context->getDiagnostics()},
        gbox::FileInfo{file, buffer, length}
    );

    TokenStream stream;
    if (!parser.parse(stream)) {
        llvm::outs() << "Failed to parse tokens into TokenStream\n";
        return;
    }

    // Invoke TokenStream proc macro plugins

    // FIXME: Exactly what's on the tin
    // for (size_t idx = 0; idx < annotations.size(); idx += 1) {
    //     if (annotations[idx] == "executor") {
    //         proc_macro_executor(stream);
    //     }
    // }

    // Match against each token, to process them iqndividually as strings

    llvm::outs() << "Parsing complete! Token Tree: \n";
    // TODO: As one string, not a vector of string.
    std::string code = stream.toString();
    llvm::outs() << code;
}

void ProcMacroConsumer::HandleTranslationUnit(clang::ASTContext &ctx) {
    HandleFuncDecl handler;
    clang::ast_matchers::MatchFinder finder;

    // Match against function definitions
    finder.addMatcher(clang::ast_matchers::functionDecl().bind("funcDecl"), &handler);
    finder.matchAST(ctx);
}

static clang::FrontendPluginRegistry::Add<ProcMacroPlugin> X(
    "gbox-macros-plugin", "Procedural macro scheme for C/C++"
);
