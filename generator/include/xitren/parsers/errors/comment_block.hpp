/*!
_ _
__ _(_) |_ _ _ ___ _ _
\ \ / |  _| '_/ -_) ' \
/_\_\_|\__|_| \___|_||_|
* @date 28.07.2024
*/
#pragma once

#include <xitren/comm/observer.hpp>
#include <xitren/parsers/errors/locator.hpp>

#include <fmt/core.h>

#include <list>
#include <stdexcept>

namespace xitren::parsers::errors {

class comment_block : public std::list<std::string> {
public:
    auto&
    line() const
    {
        return line_;
    }

    auto&
    filename() const
    {
        return filename_;
    }

    void
    line(std::size_t const& value)
    {
        line_ = value;
    }

    void
    filename(std::string_view const& value)
    {
        filename_ = value;
    }

private:
    std::size_t line_;
    std::string filename_;
};

}    // namespace xitren::parsers::errors
