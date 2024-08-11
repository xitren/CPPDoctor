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

class problems_pool_emb;

class problem {
public:
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
    static constexpr std::string_view filename_key{"filename"};
    static constexpr std::string_view line_key{"file_line"};
    enum class error_type { trace, debug, info, warning, error, critical, undefined };

    static std::string_view
    report_desc(error_type type)
    {
        using namespace std::string_literals;
        switch (type) {
            using enum xitren::problem::error_type;
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
        using enum xitren::problem::error_type;
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

    problem()  = default;
    ~problem() = default;
    problem&
    operator=(problem const& other)
        = default;
    problem&
    operator=(problem&& other)
        = default;
    problem(problem const& val) = default;
    problem(problem&& val)      = default;

    auto&
    report() const
    {
        return report_;
    }

    auto&
    short_desc() const
    {
        return short_desc_;
    }

    auto&
    unique_tag() const
    {
        return unique_tag_;
    }

    auto&
    module_desc() const
    {
        return module_desc_;
    }

    void
    report(error_type value)
    {
        report_ = value;
    }

    void
    short_desc(std::string_view const& value)
    {
        short_desc_ = value;
    }

    void
    unique_tag(std::string_view const& value)
    {
        unique_tag_ = value;
    }

    void
    module_desc(std::string_view const& value)
    {
        module_desc_ = value;
    }

#ifndef SHORT_PROBLEM_SOLVERS
    auto&
    because() const
    {
        return because_;
    }

    auto&
    long_desc() const
    {
        return long_desc_;
    }

    auto&
    documented_at() const
    {
        return documented_at_;
    }

    auto&
    solution1() const
    {
        return solutions_[0];
    }

    auto&
    solution2() const
    {
        return solutions_[1];
    }

    auto&
    solution3() const
    {
        return solutions_[2];
    }

    void
    because(std::string_view const& value)
    {
        because_ = value;
    }

    void
    long_desc(std::string_view const& value)
    {
        long_desc_ = value;
    }

    void
    documented_at(std::string_view const& value)
    {
        documented_at_ = value;
    }
#else
    auto&
    because() const
    {
        return dummy;
    }

    auto&
    long_desc() const
    {
        return dummy;
    }

    auto&
    documented_at() const
    {
        return dummy;
    }

    auto&
    solution1() const
    {
        return dummy;
    }

    auto&
    solution2() const
    {
        return dummy;
    }

    auto&
    solution3() const
    {
        return dummy;
    }

    void
    because(std::string_view const& value)
    {}

    void
    long_desc(std::string_view const& value)
    {}

    void
    documented_at(std::string_view const& value)
    {}
#endif

    void
    clear_solutions()
    {
#ifndef SHORT_PROBLEM_SOLVERS
        solutions_number_ = 0;
#endif
    }

    auto&
    add_solution(std::string_view const& desc)
    {
#ifndef SHORT_PROBLEM_SOLVERS
        if (solutions_number_ >= solutions_.size()) {
            return *this;
        }
        solutions_[solutions_number_++] = desc;
#endif
        return *this;
    }

private:
    problem(std::string_view const& short_desc, std::string_view const& unique_tag, std::string_view const& module_desc,
            error_type const& report, std::string_view const& because, std::string_view const& long_desc,
            std::string_view const& documented_at)
        : short_desc_{short_desc},
          unique_tag_{unique_tag},
          module_desc_{module_desc},
          report_{report},
          because_{because},
          long_desc_{long_desc},
          documented_at_{documented_at}
    {}

    std::string short_desc_{};
    std::string unique_tag_{};
    std::string module_desc_{};
    error_type  report_{};
#ifndef SHORT_PROBLEM_SOLVERS
    std::string                because_{};
    std::string                long_desc_{};
    std::string                documented_at_{};
    std::size_t                solutions_number_{};
    std::array<std::string, 3> solutions_{{{}, {}, {}}};
#endif
    static inline const std::string dummy{};
};

}    // namespace xitren
