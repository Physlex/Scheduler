#ifndef GBOX_MACROS_ADAPTER_HPP_
#define GBOX_MACROS_ADAPTER_HPP_

/**
 * @file adapter.hpp
 * @brief This module implements the adaptor for the clang plugin macro action
 */

#include "clang/Basic/Diagnostic.h"
#include "gbox/macros/plugins.hpp"
#include "gbox/macros/result.hpp"

namespace gbox::adapter {

/**
 *  @brief This enumerator defines the error kinds avaliable for a given action
 */
enum class ErrorKind { InvalidArgs, Action };

/// Adaptor result alias. Uses the gbox::adaptor::ErrorKind as it's errorfull value.
template <typename T>
using Result = gbox::result::Result<T, ErrorKind>;

/// This class implements the action adaptor type
///
/// The action adaptor converts a minimal slice of clang input arguments into
/// the related clang frontend action framework.
class Action {
  public:
    /// @brief Constructor for the action
    Action(clang::DiagnosticsEngine &dengine);

    /// @brief Default deconstructor for the compiler action
    ~Action() = default;

    /**
     *  @brief Provided the given arguments, execute the action.
     */
    Result<std::string> execute(std::vector<const char *> args);

  private:
    gbox::ProcMacroAction action_;
    clang::DiagnosticsEngine &dengine_;
};

}  // namespace gbox::adapter

#endif  // GBOX_MACROS_ADAPTER_HPP_
