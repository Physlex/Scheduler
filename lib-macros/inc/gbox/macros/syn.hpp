#ifndef GBOX_MACROS_SYN_HPP_
#define GBOX_MACROS_SYN_HPP_

/** @file `syn.hpp`
 *  @brief This file implements the syntax parsers and token definitions
 *         for the gbox proc macros implementation.
 */

#include "gbox/macros/syn/parse.hpp"
#include "gbox/macros/syn/tokens.hpp"

namespace gbox::syn {
using FileInfo = parse::FileInfo;
using Parser = parse::Parser;
using TokenStream = tokens::TokenStream;
using Span = tokens::Span;
}  // namespace gbox::syn

#endif  // GBOX_MACROS_SYN_HPP_
