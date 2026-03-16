/** @file `plugins.c`
 *  @brief This file implements the gbox clang plugins for gbclang and gbclang++
 *         preprocessing macros.
 */

#include "gbox/macros/plugins.hpp"

#include <clang/AST/ASTContext.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Basic/TokenKinds.h>
#include <clang/Lex/Token.h>
#include <llvm/ADT/StringRef.h>

#include <vector>

#include "gbox/macros/tokens.hpp"

using namespace gbox;

// TODO: Demonstrate full s2s translation in-memory using the system described prior.
TokenStream proc_macro_executor(TokenStream input) {
    llvm::outs() << "invoked executor proc macro!";
    return input;
}

// Mini-recursive descent parser implementation
namespace parse {

static void literals(
    TokenStream &stream, clang::Token tok, llvm::StringRef slice, Span span
) {
    // TODO: Make some of these non-string types (crazy, right?)
    stream.emplace_back(Literal(slice.data(), span, Literal::Kind::String));
}

static void identifiers(
    TokenStream &stream, clang::Token tok, llvm::StringRef slice, Span span
) {
    stream.emplace_back(Ident(slice.data(), span, tok.is(clang::tok::raw_identifier)));
}

}  // namespace parse

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

    unsigned start = sm.getFileOffset(src_begin);
    unsigned end = sm.getFileOffset(src_end);

    clang::FileID file = sm.getFileID(src_begin);
    llvm::StringRef buffer = sm.getBufferData(file);
    llvm::StringRef func_src_text = buffer.substr(start, end - start + 1);

    llvm::MemoryBufferRef func_src_buff =
        llvm::MemoryBufferRef(func_src_text, "<proc_macro_attribute function slice>");

    // Construct our token stream from the clang tokenstream

    TokenStream stream;

    clang::Lexer lexer =
        clang::Lexer(file, func_src_buff, sm, res.Context->getLangOpts());

    // Collect a set of all tokens in one pass
    clang::Token tok;
    while (!lexer.LexFromRawLexer(tok)) {
        const uint32_t tok_start = sm.getFileOffset(tok.getLocation());
        const uint32_t tok_length = tok.getLength();
        const char *text_start = func_src_buff.getBufferStart();

        const auto span = Span(tok_start, tok_length);
        const auto slice = llvm::StringRef(text_start + span.start(), span.length());
        if (tok.isAnyIdentifier()) {
            parse::identifiers(stream, tok, slice, span);
        } else if (tok.isLiteral()) {
            parse::literals(stream, tok, slice, span);
        }
    }

    for (auto annotation : annotations) {
        if (annotation == "executor") {
            proc_macro_executor(std::move(stream));
        }
    }
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
