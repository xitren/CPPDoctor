/*!
     _ _
__ _(_) |_ _ _ ___ _ _
\ \ / |  _| '_/ -_) ' \
/_\_\_|\__|_| \___|_||_|
* @date 24.07.2024
*/

#define PROBLEM_SOLVERS_PASTE
#define PROBLEMS 100
// #define SHORT_PROBLEM_SOLVERS

#include <xitren/problem_const.hpp>
#include <xitren/problems_pool.hpp>
#include <xitren/problems_pool_emb.hpp>

#include <gtest/gtest.h>

using namespace xitren;

TEST(base_test, base)
{
    problems.create_problem("Simple problem example", problem::error_type::error).because("Because");
}
