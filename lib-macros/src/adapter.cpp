/**
 * @brief This file implements the adapter module for clang
 */

#include "gbox/macros/adapter.hpp"

#include <string>

#include "clang/Basic/Diagnostic.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/CompilerInvocation.h"

using namespace gbox::adapter;
using namespace gbox::result;

Action::Action(clang::DiagnosticsEngine &dengine)
    : dengine_(dengine), action_(gbox::ProcMacroAction()) {}

gbox::adapter::Result<std::string> Action::execute(std::vector<const char *> args) {
    auto invocation = std::shared_ptr<clang::CompilerInvocation>();
    if (!clang::CompilerInvocation::CreateFromArgs(*invocation, args, this->dengine_)) {
        return Err(ErrorKind::InvalidArgs);
    }

    auto instance = clang::CompilerInstance(invocation);
    instance.setDiagnostics(&this->dengine_);

    if (!instance.ExecuteAction(this->action_) || this->dengine_.hasErrorOccurred()) {
        return Err(ErrorKind::Action);
    }

    return Ok(this->action_.getRewritten());
}
