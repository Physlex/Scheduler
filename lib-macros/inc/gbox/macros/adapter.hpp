#ifndef GBOX_MACROS_ADAPTER_HPP_
#define GBOX_MACROS_ADAPTER_HPP_

/**
 * @file adapter.hpp
 * @brief This module implements the adaptor for the clang plugin macro action
 */

#include "gbox/macros/plugins.hpp"

namespace gbox {
namespace adapter {

class Action {
  public:
    /// @brief Constructor for the action
    Action();

    /// @brief Default deconstructor for the compiler action
    ~Action() = default;

    /**
     * @brief Execute the action for the current file
     */
    bool execute(std::vector<std::string> args);

  private:
    gbox::ProcMacroAction action;
};

}  // namespace adapter
}  // namespace gbox

#endif  // GBOX_MACROS_ADAPTER_HPP_
