/*!
_ _
__ _(_) |_ _ _ ___ _ _
\ \ / |  _| '_/ -_) ' \
/_\_\_|\__|_| \___|_||_|
* @date 24.07.2024
*/
#pragma once
#include <nlohmann/json.hpp>
#include <xitren/comm/observer.hpp>
#include <xitren/parsers/errors/exceptions.hpp>
#include <xitren/problem.hpp>

#include <fmt/core.h>

#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <string>

namespace xitren::parsers::errors {

class parser : public comm::observer<std::list<std::string>>, public comm::observable<nlohmann::json> {
    using data_type = std::list<std::string>;
    using mode      = void (parser::*)(std::string const&);

    struct parser_step {
        parser_step()  = delete;
        ~parser_step() = default;
        parser_step&
        operator=(parser_step const& other)
            = delete;
        parser_step&
        operator=(parser_step const&& other)
            = delete;
        parser_step(parser_step const& val) = default;
        parser_step(parser_step&& val)      = default;

        parser_step(std::string_view const json, std::string_view const code) : json_tag{json}, code_tag{code} {}

        const std::string_view json_tag;
        const std::string_view code_tag;
        std::string            value;
    };
    using steps_vault = std::array<parser_step, 10>;

public:
    parser() = default;
    ~parser() override { out_ << pool_; }
    parser&
    operator=(parser const& other)
        = delete;
    parser&
    operator=(parser const&& other)
        = delete;
    parser(parser const& val) = delete;
    parser(parser&& val)      = delete;

    void
    data(void const* /*src*/, data_type const& nd) final
    {
        using namespace std::string_literals;
        for (auto const& line : nd) {
            (this->*mode_)(line);
        }
        if ((steps_iter_ + 1) != steps_.end()) {
            throw unexpected_tag(errno, std::system_category(), "Unexpected end!");
        }
        mode_ = &parser::find_first;
        nlohmann::json error;
        for (auto& item : steps_) {
            error[item.json_tag] = item.value;
        }
        notify_observers(error);
        for (auto& item : steps_) {
            item.value = ""s;
        }
    }

private:
    nlohmann::json pool_{};
    std::ofstream  out_;
    mode           mode_{&parser::find_first};

    // clang-format off
    steps_vault steps_{{
                        {problem::short_key, " @short "},
                        {problem::unique_tag_key, " @unique_tag "},
                        {problem::module_key, " @module "},
                        {problem::report_key, " @report "},
                        {problem::because_key, " @because "},
                        {problem::long_key, " @long "},
                        {problem::documented_key, " @doc_at "},
                        {problem::solution1_key, " @solution1 "},
                        {problem::solution2_key, " @solution2 "},
                        {problem::solution3_key, " @solution3 "}
                       }};
    // clang-format on

    const std::string     another_tag_{"@"};
    const std::string     line_tag_{" * "};
    const std::string     end_tag_{" */"};
    steps_vault::iterator steps_iter_ = nullptr;

    void
    find_first(std::string const& line)
    {
        auto*       begin = steps_.begin();
        auto const& tag   = begin->code_tag;
        if (auto iter = std::string::npos; (iter = line.find(tag)) != std::string::npos) {
            mode_        = &parser::fill;
            steps_iter_  = begin;
            begin->value = line.substr(iter + tag.size());
            return;
        }
        if (line.find(another_tag_) != std::string::npos) {
            throw unexpected_tag(errno, std::system_category(), "Unexpected tag " + another_tag_);
        }
    }

    void
    fill(std::string const& line)
    {
        auto*       next = steps_iter_ + 1;
        auto const& tag  = next->code_tag;
        if (next != steps_.end()) {
            if (auto iter = std::string::npos; (iter = line.find(tag)) != std::string::npos) {
                next->value = line.substr(iter + tag.size());
                steps_iter_++;
                return;
            }
        }
        if (line.find(another_tag_) != std::string::npos) {
            throw unexpected_tag(errno, std::system_category(), "Unexpected tag " + another_tag_);
        }
        if (auto iter = std::string::npos; (iter = line.find(line_tag_)) != std::string::npos) {
            auto pure = line.substr(iter + line_tag_.size());
            steps_iter_->value += " " + pure;
            return;
        }
        if (auto iter = std::string::npos; (iter = line.find(end_tag_)) != std::string::npos) {
            return;
        }
        throw unexpected_tag(errno, std::system_category(), "Not found " + line_tag_);
    }
};

}    // namespace xitren::parsers::errors
