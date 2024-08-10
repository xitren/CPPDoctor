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

class broken_pipe : public std::runtime_error {
public:
    using runtime_error::runtime_error;
};

class unexpected_tag : public std::system_error {
public:
    using system_error::system_error;
};

}    // namespace xitren::parsers::errors
