/*!
     _ _
__ _(_) |_ _ _ ___ _ _
\ \ / |  _| '_/ -_) ' \
/_\_\_|\__|_| \___|_||_|
* @date 24.07.2024
*/

#define PROBLEM_SOLVERS_PASTE
#define PROBLEM_DEFINITIONS_PASTE
#define PROBLEMS_COUNTER 100
// #define SHORT_PROBLEM_SOLVERS

#include <xitren/problem_const.hpp>
#include <xitren/problems_pool_emb.hpp>

#include <gtest/gtest.h>

using namespace xitren;

TEST(base_test, base)
{
    const problem_const problem{.index           = 1,
                                .short_desc_hash = problem_const::hash("short_desc"),
                                .short_desc      = "short_desc",
                                .unique_tag      = "unique_tag",
                                .module_desc     = "module_desc",
                                .report          = problem_const::report_by_desc("ERROR"),
                                .because         = "because",
                                .long_desc       = "long_desc",
                                .documented_at   = "documented_at",
                                .solutions       = {{"solution1", "solution2", "solution3"}}};
}
