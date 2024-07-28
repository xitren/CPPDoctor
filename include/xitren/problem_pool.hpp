/*!
_ _
__ _(_) |_ _ _ ___ _ _
\ \ / |  _| '_/ -_) ' \
/_\_\_|\__|_| \___|_||_|
* @date 28.07.2024
*/

#include <array>
#include <iostream>
#include <optional>
#include <xitren/problem.hpp>

namespace xitren {

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
        auto const hash_id = hash(short_desc);
        for (auto const& problem : instance().vault_) {
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
        PROBLEM_SOLVERS_PASTE;
    }
    // clang-format on

    static inline const std::hash<std::string_view>                      hash;
    std::size_t                                                          count_{};
    std::array<std::pair<std::optional<problem>, std::size_t>, PROBLEMS> vault_{};
};

auto& problems = problem_pool::instance();

}    // namespace xitren