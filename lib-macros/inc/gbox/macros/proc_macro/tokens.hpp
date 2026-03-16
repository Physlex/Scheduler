#ifndef GBOX_MACROS_PROC_MACRO_TOKENS_HPP_
#define GBOX_MACROS_PROC_MACRO_TOKENS_HPP_

// TODO: DOCS

#include <stdint.h>
#include <memory>
#include <variant>
#include <memory.h>

class Literal;
class Ident;
class Punc;
class Group;

/* 
 * @brief Alias for a typed union of any of any of the token types
 *        NOTE: Groups are handled seperately due to recursive definitions
 */
using TokenTree = std::variant<Literal, Ident, Punc, std::unique_ptr<Group>>;

// TODO: DOCS
class Span {
  public:
    Span(uint32_t start, uint32_t length) : start_(start), length_(length) {};

    inline uint32_t start() const {
        return this->start_;
    }

    inline uint32_t length() const {
        return this->length_;
    }

  private:
    uint32_t start_;
    uint32_t length_;
};

// TODO: DOCS
template<typename Symbol>
class TokenData {
  public:
    TokenData(Symbol sym, Span span) : sym_(std::move(sym)), span_(span) {}

  private:
    Symbol sym_;
    Span span_;
};

// TODO: DOCS
class Literal {
  public:
    /**
    * TODO: DOCS
    */
    enum class Kind {
        Int,
        Float,
        String,
        Char
    };

  Literal(const char *sym, Span span, Literal::Kind kind) : data_(sym, span), kind_(kind) {}

  private:
    TokenData<const char *> data_;
    Literal::Kind kind_;
};

// TODO: DOCS
class Ident {
  public:
    Ident(
        const char *symbol, Span span, bool is_raw
    ) : data_(symbol, span), is_raw_(is_raw){}

  private:
    TokenData<const char *> data_;
    bool is_raw_;
};

// TODO: DOCS
class Punc {
  public:
    Punc(const char *sym, Span span, bool is_join) : data_(sym, span), is_join_(is_join) {}

  private:
    TokenData<const char *> data_;
    bool is_join_;
};

// TODO: DOCS
enum class Delimiter {
    Parenthesis,
    Brace,
    Bracket,
};

// TODO: DOCS
class Group {
  public:
    Group(TokenTree tree, Span span, Delimiter delim) : data_(std::move(tree), span), delim_(delim) {}

  private:
    TokenData<TokenTree> data_;
    Delimiter delim_;
};

#endif  // GBOX_MACROS_PROC_MACRO_TOKENS_HPP_
