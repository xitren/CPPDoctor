#include <array>
#include <iostream>
#include <optional>

namespace xitren {

class problem_pool;

class problem {
    friend problem_pool;

public:
    enum class error_type { trace, debug, info, warning, error, critical };

    problem()  = delete;
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
    index() const
    {
        return index_;
    }

    auto&
    report_as() const
    {
        return report_as_;
    }

    auto&
    short_desc() const
    {
        return short_desc_;
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
#endif

    auto&
    add_short_desc(std::string_view const& desc)
    {
#ifndef SHORT_PROBLEM_SOLVERS
        short_desc_ = desc;
#endif
        return *this;
    }

    auto&
    add_report_as(error_type desc)
    {
#ifndef SHORT_PROBLEM_SOLVERS
        report_as_ = desc;
#endif
        return *this;
    }

    auto&
    add_because(std::string_view const& desc)
    {
#ifndef SHORT_PROBLEM_SOLVERS
        because_ = desc;
#endif
        return *this;
    }

    auto&
    add_long_desc(std::string_view const& desc)
    {
#ifndef SHORT_PROBLEM_SOLVERS
        long_desc_ = desc;
#endif
        return *this;
    }

    auto&
    add_documented_at(std::string_view const& desc)
    {
#ifndef SHORT_PROBLEM_SOLVERS
        documented_at_ = desc;
#endif
        return *this;
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
    problem(std::string_view const& short_desc, error_type report_as, std::size_t index)
        : index_{index}, report_as_{report_as}, short_desc_{short_desc}
    {}

    std::size_t                     index_{};
    error_type                      report_as_;
    std::optional<std::string_view> short_desc_{std::nullopt};
#ifndef SHORT_PROBLEM_SOLVERS
    std::optional<std::string_view>                because_{std::nullopt};
    std::optional<std::string_view>                long_desc_{std::nullopt};
    std::optional<std::string_view>                documented_at_{std::nullopt};
    std::size_t                                    solutions_number_{};
    std::array<std::optional<std::string_view>, 3> solutions_{{{std::nullopt}, {std::nullopt}, {std::nullopt}}};
#endif
};

class problem_pool {
public:
    static problem_pool&
    instance()
    {
        static problem_pool inst{};
        return inst;
    }

    auto
    create_problem(std::string_view const& short_desc, problem::error_type report_as)
    {
        problem new_problem(short_desc, report_as, count_);
        vault_[count_++] = std::pair<problem, std::size_t>{new_problem, hash(short_desc)};
        return new_problem;
    }

    static std::optional<problem>
    get_problem(std::string_view const& short_desc)
    {
        const auto hash_id = hash(short_desc);
        for (const auto& problem : instance().vault_) {
            if (problem.second == hash_id) {
                if (problem.first->short_desc_ == short_desc) {
                    return problem.first;
                }
            }
        }
        return std::nullopt;
    }

private:
    // clang-format off
    problem_pool() {
        create_problem("Some problem", problem::error_type::error)
            .add_because("Just")
            .add_long_desc("Some problem happened")
            .add_documented_at("http://")
            .add_solution("Have no idea!")
            .add_solution("Maybe this")
            .add_solution("Матерь божья!");
        create_problem("Some problem 2", problem::error_type::error)
            .add_because("Just")
            .add_long_desc("Some problem happened")
            .add_documented_at("http://")
            .add_solution("Have no idea!")
            .add_solution("Maybe this")
            .add_solution("Матерь божья!");
        create_problem("Some problem 3", problem::error_type::error)
            .add_because("Just")
            .add_long_desc("Some problem happened")
            .add_documented_at("http://")
            .add_solution("Have no idea!")
            .add_solution("Maybe this")
            .add_solution("Матерь божья!");
    }
    // clang-format on

    static inline const std::hash<std::string_view>                      hash;
    std::size_t                                                          count_{};
    std::array<std::pair<std::optional<problem>, std::size_t>, 3> vault_{};
};

auto& problems = problem_pool::instance();

}    // namespace xitren
