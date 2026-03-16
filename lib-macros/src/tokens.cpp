/**
 * TODO: DOCS
 */

#include "gbox/macros/tokens.hpp"
#include <memory>

using namespace gbox;

TokenTree::TokenTree(Literal value) : value_(std::move(value)), kind_(Kind::Literal) {}
TokenTree::TokenTree(Ident value)   : value_(std::move(value)), kind_(Kind::Ident)   {}
TokenTree::TokenTree(Punc value)    : value_(std::move(value)), kind_(Kind::Punc)    {}
TokenTree::TokenTree(std::unique_ptr<Group> value) : value_(std::move(value)), kind_(Kind::Group) {}

TokenTree::Value::~Value() {
}

TokenTree::Value::Value(Literal value) : literal(std::move(value)) {}
TokenTree::Value::Value(Ident value) : ident(std::move(value)) {}
TokenTree::Value::Value(Punc value) : punc(std::move(value)) {}
TokenTree::Value::Value(std::unique_ptr<Group> value)  : group(std::move(value)) {}

TokenTree::~TokenTree() {
    switch (this->kind_) {
        case TokenTree::Kind::Group: this->value_.group.~unique_ptr<Group>(); break;
        case TokenTree::Kind::Ident: this->value_.ident.~Ident(); break;
        case TokenTree::Kind::Punc: this->value_.punc.~Punc(); break;
        case TokenTree::Kind::Literal: this->value_.literal.~Literal(); break;
    }
}
