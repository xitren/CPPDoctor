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

#include <xitren/problem.hpp>

#include <gtest/gtest.h>

using namespace xitren;

TEST(base_test, base)
{
    problem_pool::instance()
        .create_problem("Simple problem example", problem::error_type::error)
        .add_because("Because");
}
