#ifndef GBOX_MACROS_PROC_MACRO_TOKENS_HPP_
#define GBOX_MACROS_PROC_MACRO_TOKENS_HPP_

// TODO: DOCS

#include <stdint.h>

#include <memory>
#include <string>
#include <vector>

namespace gbox {

class TokenTree;
class Group;

/// TODO: DOCS
class TokenStream : public std::vector<TokenTree> {
  public:
    inline std::string toString() const {
        uint32_t cursor = 0;
        return this->toString(cursor);
    }

  private:
    std::string toString(uint32_t &cursor) const;
};

// TODO: DOCS
class Span {
  public:
    Span(uint32_t start, uint32_t length) : start_(start), length_(length) {};

    inline uint32_t start() const { return this->start_; }

    inline uint32_t length() const { return this->length_; }

  private:
    uint32_t start_;
    uint32_t length_;
};

// TODO: DOCS
template <typename Symbol>
class TokenData {
  public:
    TokenData(Symbol sym, Span span) : sym_(std::move(sym)), span_(span) {}

    const inline Symbol &symbol() const { return this->sym_; }

    inline Span span() const { return this->span_; }

  private:
    Symbol sym_;
    Span span_;
};

// TODO: DOCS
class Literal {
  public:
    // This enum defines the kinds / forms a literal can take
    enum class Kind {
        // A standard integral type like 0xFF, 32, 0, ...
        Integer,
        // A floating point type such as 0.3f, 0.1, 10e, ...
        Float,
        // Any character type. Includes utf and ascii, as well as wide.
        Char,
        // Any text type
        String,
        // Binary data
        Binary,
    };

    Literal(std::string sym, Span span, Literal::Kind kind)
        : data_(sym, span), kind_(kind) {}

    inline std::string symbol() const { return this->data_.symbol(); }

    inline Span span() const { return this->data_.span(); }

  private:
    TokenData<std::string> data_;
    Literal::Kind kind_;
};

// TODO: DOCS
class Ident {
  public:
    Ident(std::string symbol, Span span, bool is_raw)
        : data_(symbol, span), is_raw_(is_raw) {}

    inline std::string symbol() const { return this->data_.symbol(); }

    inline Span span() const { return this->data_.span(); }

  private:
    TokenData<std::string> data_;
    bool is_raw_;
};

// TODO: DOCS
class Punc {
  public:
    Punc(std::string sym, Span span, bool is_join)
        : data_(sym, span), is_join_(is_join) {}

    inline std::string symbol() const { return this->data_.symbol(); }

    inline Span span() const { return this->data_.span(); }

  private:
    TokenData<std::string> data_;
    bool is_join_;
};

// FIXME: This should probably go into a shared utilities or std-extensions library
template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};

template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

// TODO: DOCS
// NOTE: This will be the FIRST proc macro to be created for structs.
//       because _HOLY_SHIT_
class TokenTree {
  public:
    enum class Kind { Literal, Ident, Punc, Group };

    union Value {
        Value() {}
        Value(Literal l);
        Value(Ident i);
        Value(Punc p);
        Value(std::unique_ptr<Group> g);

        ~Value();

        Value(const Value &) = delete;
        Value &operator=(const Value &) = delete;
        Value(Value &&) {}
        Value &operator=(Value &&) { return *this; }

        Literal literal;
        Ident ident;
        Punc punc;
        std::unique_ptr<Group> group;
    };

  public:
    /// @brief Construct token tree from a literal
    TokenTree(Literal value);

    /// @brief Construct token tree from an identifier
    TokenTree(Ident value);

    /// @brief Construct token tree from punctuation
    TokenTree(Punc value);

    /// @brief Construct token tree from a group
    TokenTree(std::unique_ptr<Group> value);

    /// @brief Specialized destructor for tagged union type
    ~TokenTree();

    // These are the deleted copy constructors that make this move-only
    TokenTree(const TokenTree &) = delete;
    TokenTree &operator=(const TokenTree &) = delete;
    TokenTree &operator=(TokenTree &&) = default;
    TokenTree(TokenTree &&other) noexcept : kind_(other.kind_) {
        switch (kind_) {
            case Kind::Literal:
                new (&value_.literal) Literal(std::move(other.value_.literal));
                break;
            case Kind::Ident:
                new (&value_.ident) Ident(std::move(other.value_.ident));
                break;
            case Kind::Punc:
                new (&value_.punc) Punc(std::move(other.value_.punc));
                break;
            case Kind::Group:
                new (&value_.group) std::unique_ptr<Group>(std::move(other.value_.group));
                break;
        }
    }

    inline TokenTree::Kind kind() { return this->kind_; }

    /// TODO: DOCS
    template <typename Fn>
    auto match(Fn &&f) const {
        switch (this->kind_) {
            case Kind::Punc:
                return f(this->value_.punc);
            case Kind::Ident:
                return f(this->value_.ident);
            case Kind::Literal:
                return f(this->value_.literal);
            case Kind::Group:
                return f(this->value_.group);
        }
    }

  private:
    TokenTree::Value value_;
    TokenTree::Kind kind_;
};

// TODO: DOCS
class Group {
  public:
    // TODO: DOCS
    enum class Delimiter {
        /// A token such as one of "()"
        Parenthesis,
        /// A token such as one of "{}"
        Brace,
        /// A token such as one of "[]"
        Square,
    };

    Group(TokenStream stream, Span span, Delimiter delim)
        : data_(std::move(stream), span), delim_(delim) {}

    inline const TokenStream &tree() const { return this->data_.symbol(); }

    inline const Span span() const { return this->data_.span(); }

    inline const Delimiter kind() const { return this->delim_; }

  private:
    TokenData<TokenStream> data_;
    Delimiter delim_;
};

}  // namespace gbox

#endif  // GBOX_MACROS_PROC_MACRO_TOKENS_HPP_
