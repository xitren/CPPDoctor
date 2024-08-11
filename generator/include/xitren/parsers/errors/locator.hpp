/*!
_ _
__ _(_) |_ _ _ ___ _ _
\ \ / |  _| '_/ -_) ' \
/_\_\_|\__|_| \___|_||_|
* @date 28.07.2024
*/
#pragma once

#include <xitren/comm/observer.hpp>
#include <xitren/parsers/errors/comment_block.hpp>

#include <fmt/core.h>

#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <string>

namespace xitren::parsers::errors {

class locator : public std::list<comment_block> {
public:
    locator()  = delete;
    ~locator() = default;
    locator&
    operator=(locator const& other)
        = delete;
    locator&
    operator=(locator const&& other)
        = delete;
    locator(locator const& val) = delete;
    locator(locator&& val)      = default;

    explicit locator(std::string const& name) : filename_{name}
    {
        std::ifstream file{name};
        if (!file) {
            throw std::system_error(errno, std::system_category(), "failed to open " + name);
        }
        std::string line;
        while (std::getline(file, line)) {
            line_++;
            (this->*mode_)(line);
        }
    }

    static auto
    locate(std::string const& name)
    {
        locator loc(name);
        return loc;
    }

private:
    comment_block current_{};
    void (locator::*mode_)(std::string const&) = &locator::find_opening;
    const std::string open_tag_{"/**"};
    const std::string middle_tag_{"*"};
    const std::string end_tag_{"*/"};
    const std::string filename_{};
    std::size_t       line_{};

    void
    find_opening(std::string const& line)
    {
        auto iter = std::string::npos;
        if ((iter = line.find(open_tag_)) != std::string::npos) {
            if (line.find(end_tag_, iter) != std::string::npos)
                return;
            mode_ = &locator::find_closing;
        }
    }

    void
    find_closing(std::string const& line)
    {
        if (line.find(end_tag_) != std::string::npos) {
            mode_ = &locator::find_opening;
            current_.line(line_);
            current_.filename(filename_);
            this->push_back(current_);
            current_.clear();
            return;
        }
        auto iter = std::string::npos;
        if ((iter = line.find(middle_tag_)) == std::string::npos) {
            throw std::system_error(errno, std::system_category(), "failed to find formatter '" + middle_tag_ + "' ");
        }
        current_.push_back(line);
    }
};

}    // namespace xitren::parsers::errors
