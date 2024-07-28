/*!
_ _
__ _(_) |_ _ _ ___ _ _
\ \ / |  _| '_/ -_) ' \
/_\_\_|\__|_| \___|_||_|
* @date 28.07.2024
*/

#include <xitren/comm/observer.hpp>
#include <xitren/parsers/errors/locator.hpp>

#include <fmt/core.h>

#include <stdexcept>

namespace xitren::parsers::errors {

using broken_pipe    = std::runtime_error;
using unexpected_tag = std::system_error;

}    // namespace xitren::parsers::errors
