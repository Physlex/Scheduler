#ifndef GBOX_MACROS_PROC_MACRO_HPP_
#define GBOX_MACROS_PROC_MACRO_HPP_

/**
 * TODO: DOCS
 */
#include <stdint.h>

/**
 * TODO: DOCS
 */
class TokenStream {
  public:
    TokenStream() = default;
};

/**
 * TODO: DOCS
 */
enum LitKind {
    
};

/**
 * TODO: DOCS
 */
class Span {
  public:
    Span() = default;
    Span(uint32_t start, uint32_t end) : start_(start), end_(end) {};

  private:
    uint32_t start_;
    uint32_t end_;
};

/**
 * TODO: DOCS
 */
class Literal {
  public:
    Literal() = default;

  private:
    const char *txt;
    LitKind kind;
    Span span;
};

/**
 * TODO: DOCS
 */
class Ident {
  public:
    Ident() = default;

  private:
    const char *txt;
    bool is_raw;
    Span span;
};

#endif  // GBOX_MACROS_PROC_MACRO_HPP_
