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

#include <xitren/problem_gen.hpp>

#include <gtest/gtest.h>

using namespace xitren;

TEST(base_test, base)
{
    auto problem = problems_pool_emb::get_problem("Some problem 2");
    EXPECT_TRUE(problem.has_value());
}
