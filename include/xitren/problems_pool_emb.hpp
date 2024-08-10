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
#include <xitren/problem_const.hpp>

namespace xitren {

class problems_pool_emb {
public:
    static problems_pool_emb&
    instance()
    {
        static problems_pool_emb inst{};
        return inst;
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

    // clang-format off
    PROBLEM_DEFINITIONS_PASTE;
    // clang-format on

private:
    // clang-format off
    problems_pool_emb() {
        PROBLEM_SOLVERS_PASTE;
    }
    // clang-format on

    static inline const std::hash<std::string_view>                      hash;
    std::array<std::pair<std::optional<problem>, std::size_t>, PROBLEMS_COUNTER> vault_{};
};

auto& problems = problems_pool_emb::instance();

}    // namespace xitren