/*!
_ _
__ _(_) |_ _ _ ___ _ _
\ \ / |  _| '_/ -_) ' \
/_\_\_|\__|_| \___|_||_|
* @date 05.08.2024
*/
#pragma once

#include <array>
#include <iostream>
#include <optional>

namespace xitren {

struct problem_const {
    static constexpr std::string_view short_key{"short_desc"};
    static constexpr std::string_view unique_tag_key{"unique_tag"};
    static constexpr std::string_view module_key{"module"};
    static constexpr std::string_view report_key{"type"};
    static constexpr std::string_view because_key{"because"};
    static constexpr std::string_view long_key{"long_desc"};
    static constexpr std::string_view documented_key{"documented_at"};
    static constexpr std::string_view solution1_key{"solution_first"};
    static constexpr std::string_view solution2_key{"solution_second"};
    static constexpr std::string_view solution3_key{"solution_third"};
    enum class error_type { trace, debug, info, warning, error, critical, undefined };

    static std::string_view
    report_desc(error_type type)
    {
        using namespace std::string_literals;
        switch (type) {
            using enum xitren::problem_const::error_type;
        case trace:
            return "TRACE";
        case debug:
            return "DEBUG";
        case info:
            return "INFO";
        case warning:
            return "WARNING";
        case error:
            return "ERROR";
        case critical:
            return "CRITICAL";
        default:
            return "Undefined";
        }
    }

    static error_type
    report_by_desc(std::string_view const& desc)
    {
        using namespace std::string_literals;
        using enum xitren::problem_const::error_type;
        if (desc == "TRACE") {
            return trace;
        }
        if (desc == "DEBUG") {
            return debug;
        }
        if (desc == "INFO") {
            return info;
        }
        if (desc == "WARNING") {
            return warning;
        }
        if (desc == "ERROR") {
            return error;
        }
        if (desc == "CRITICAL") {
            return critical;
        }
        return undefined;
    }

    const std::size_t index;
    const std::size_t short_desc_hash;
    const std::string short_desc;
    const std::string unique_tag;
    const std::string module_desc;
    const error_type  report;
#ifndef SHORT_PROBLEM_SOLVERS
    const std::string                because;
    const std::string                long_desc;
    const std::string                documented_at;
    const std::array<std::string, 3> solutions;
#endif
    static inline const std::hash<std::string_view> hash;
};

}    // namespace xitren
