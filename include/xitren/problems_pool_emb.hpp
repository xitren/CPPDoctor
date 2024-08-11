/*!
_ _
__ _(_) |_ _ _ ___ _ _
\ \ / |  _| '_/ -_) ' \
/_\_\_|\__|_| \___|_||_|
* @date 28.07.2024
*/

#include <xitren/problem_const.hpp>

#include <array>
#include <iostream>
#include <optional>

namespace xitren {

class problems_pool_emb {
public:
    static problems_pool_emb&
    instance()
    {
        static problems_pool_emb inst{};
        return inst;
    }

    static std::optional<problem_const>
    get_problem(std::string_view const& short_desc)
    {
        auto const hash_id = hash(short_desc);
        for (auto const& problem : instance().vault_) {
            if (problem->short_desc_hash == hash_id) {
                if (problem->short_desc == short_desc) {
                    return *problem;
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

    static inline const std::hash<std::string_view>    hash;
    std::array<problem_const const*, PROBLEMS_COUNTER> vault_{};
};

auto const& problems = problems_pool_emb::instance();

}    // namespace xitren