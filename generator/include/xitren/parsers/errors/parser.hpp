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

#include <fmt/core.h>

#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <string>

namespace xitren::parsers::errors {

struct error_desc {
    std::string short_desc;
    std::string report_as;
    std::string because;
    std::string long_desc;
    std::string documented_at;
    std::string solution1;
    std::string solution2;
    std::string solution3;
};

class parser : public error_desc, public comm::observer<std::list<std::string>> {
    using data_type = std::list<std::string>;

public:
    parser() = delete;
    ~parser() override { out_ << pool_; }
    parser&
    operator=(parser const& other)
        = delete;
    parser&
    operator=(parser const&& other)
        = delete;
    parser(parser const& val) = delete;
    parser(parser&& val)      = delete;

    explicit parser(std::string const& name)
    {
        std::ifstream in{name};
        if (in) {
            nlohmann::json data = nlohmann::json::parse(in);
            version_            = data["version"];
            version_++;
        }
        pool_["version"] = version_;
        out_             = std::ofstream{name};
        if (!out_) {
            throw std::system_error(errno, std::system_category(), "failed to open " + name);
        }
    }

    void
    data(void const* /*src*/, data_type const& nd) final
    {
        using namespace std::string_literals;
        for (auto const& line : nd) {
            (this->*mode_)(line);
        }
        mode_ = &parser::find_short;
        nlohmann::json error;
        error["type"]            = report_as;
        error["short_desc"]      = short_desc;
        error["because"]         = because;
        error["long_desc"]       = long_desc;
        error["documented_at"]   = documented_at;
        error["solution_first"]  = solution1;
        error["solution_second"] = solution2;
        error["solution_third"]  = solution3;
        pool_["problems"].push_back(error);
        solution3 = solution2 = solution1 = documented_at = long_desc = because = report_as = short_desc = ""s;
    }

private:
    int            version_{};
    nlohmann::json pool_{};
    std::ofstream  out_;
    void (parser::*mode_)(std::string const&) = &parser::find_short;

    const std::string short_tag_{"@short "};
    const std::string report_tag_{"@report "};
    const std::string because_tag_{"@because "};
    const std::string long_tag_{"@long "};
    const std::string doc_at_tag_{"@doc_at "};
    const std::string solution1_tag_{"@solution1 "};
    const std::string solution2_tag_{"@solution2 "};
    const std::string solution3_tag_{"@solution3 "};
    const std::string another_tag_{"@"};

    void
    find_short(std::string const& line)
    {
        if (auto iter = std::string::npos; (iter = line.find(short_tag_)) != std::string::npos) {
            mode_            = &parser::fill_short;
            this->short_desc = line.substr(iter + short_tag_.size());
            return;
        }
        if (line.find(another_tag_) != std::string::npos) {
            throw unexpected_tag(errno, std::system_category(), "Unexpected tag " + another_tag_);
        }
    }

    void
    fill_short(std::string const& line)
    {
        if (auto iter = std::string::npos; (iter = line.find(report_tag_)) != std::string::npos) {
            mode_           = &parser::fill_report;
            this->report_as = line.substr(iter + report_tag_.size());
            return;
        }
        if (line.find(another_tag_) != std::string::npos) {
            throw unexpected_tag(errno, std::system_category(), "Unexpected tag " + another_tag_);
        }
        this->short_desc += " " + line;
    }

    void
    fill_report(std::string const& line)
    {
        if (auto iter = std::string::npos; (iter = line.find(because_tag_)) != std::string::npos) {
            mode_         = &parser::fill_because;
            this->because = line.substr(iter + because_tag_.size());
            return;
        }
        if (line.find(another_tag_) != std::string::npos) {
            throw unexpected_tag(errno, std::system_category(), "Unexpected tag " + another_tag_);
        }
        this->report_as += " " + line;
    }

    void
    fill_because(std::string const& line)
    {
        if (auto iter = std::string::npos; (iter = line.find(long_tag_)) != std::string::npos) {
            mode_           = &parser::fill_long;
            this->long_desc = line.substr(iter + long_tag_.size());
            return;
        }
        if (line.find(another_tag_) != std::string::npos) {
            throw unexpected_tag(errno, std::system_category(), "Unexpected tag " + another_tag_);
        }
        this->because += " " + line;
    }

    void
    fill_long(std::string const& line)
    {
        if (auto iter = std::string::npos; (iter = line.find(doc_at_tag_)) != std::string::npos) {
            mode_               = &parser::fill_doc_at;
            this->documented_at = line.substr(iter + doc_at_tag_.size());
            return;
        }
        if (line.find(another_tag_) != std::string::npos) {
            throw unexpected_tag(errno, std::system_category(), "Unexpected tag " + another_tag_);
        }
        this->long_desc += " " + line;
    }

    void
    fill_doc_at(std::string const& line)
    {
        if (auto iter = std::string::npos; (iter = line.find(solution1_tag_)) != std::string::npos) {
            mode_           = &parser::fill_solution1;
            this->solution1 = line.substr(iter + solution1_tag_.size());
            return;
        }
        if (line.find(another_tag_) != std::string::npos) {
            throw unexpected_tag(errno, std::system_category(), "Unexpected tag " + another_tag_);
        }
        this->documented_at += " " + line;
    }

    void
    fill_solution1(std::string const& line)
    {
        if (auto iter = std::string::npos; (iter = line.find(solution2_tag_)) != std::string::npos) {
            mode_           = &parser::fill_solution2;
            this->solution2 = line.substr(iter + solution2_tag_.size());
            return;
        }
        if (line.find(another_tag_) != std::string::npos) {
            throw unexpected_tag(errno, std::system_category(), "Unexpected tag " + another_tag_);
        }
        this->solution1 += " " + line;
    }

    void
    fill_solution2(std::string const& line)
    {
        if (auto iter = std::string::npos; (iter = line.find(solution3_tag_)) != std::string::npos) {
            mode_           = &parser::fill_solution3;
            this->solution3 = line.substr(iter + solution3_tag_.size());
            return;
        }
        if (line.find(another_tag_) != std::string::npos) {
            throw unexpected_tag(errno, std::system_category(), "Unexpected tag " + another_tag_);
        }
        this->solution2 += " " + line;
    }

    void
    fill_solution3(std::string const& line)
    {
        if (line.find(another_tag_) != std::string::npos) {
            throw unexpected_tag(errno, std::system_category(), "Unexpected tag " + another_tag_);
        }
        this->solution3 += " " + line;
    }
};

}    // namespace xitren::parsers::errors
