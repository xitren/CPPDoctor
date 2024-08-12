/*!
_ _
__ _(_) |_ _ _ ___ _ _
\ \ / |  _| '_/ -_) ' \
/_\_\_|\__|_| \___|_||_|
* @date 06.08.2024
*/

#include "xitren/parsers/errors/locator_pipe.hpp"
#include <nlohmann/json.hpp>
#include <xitren/comm/observer.hpp>
#include <xitren/parsers/errors/exceptions.hpp>
#include <xitren/parsers/errors/parser.hpp>
#include <xitren/problem.hpp>

#include <gtest/gtest.h>

#include <fstream>
#include <iostream>
#include <sstream>

using namespace xitren::parsers::errors;
using namespace xitren::comm;

constexpr int number_of_errors_in_files = 6;
constexpr int number_of_files           = 20;

class listener : public observer<comment_block> {
public:
    comment_block record;
    std::size_t   counter{};

protected:
    void
    data(void const* /*src*/, comment_block const& nd) final
    {
        counter++;
        record = nd;
    }
};

TEST(base_ideal_test, locator)
{
    listener     catcher;
    locator_pipe pipe;

    pipe.add_observer(catcher);
    for (int i = 0; i < number_of_files; i++) {
        pipe.data(nullptr, fmt::format("test{}.cpp", i));
    }

    pipe.join();
    EXPECT_EQ(catcher.counter, number_of_errors_in_files * number_of_files);
}

void
create_dummy_file(std::string const& name)
{
    static int    counter{};
    std::ofstream out(name);
    if (!out) {
        return;
    }
    out << "#include <gtest/gtest.h>"
        << "\n";
    out << ""
        << "\n";
    out << "using namespace xitren;"
        << "\n";
    out << ""
        << "\n";

    // clang-format off
    for (int i = 0;i < number_of_errors_in_files;i++) {
        out << "/**" << "\n";
        out << fmt::format(" * Constructs a new interval_event object {}.", counter) << "\n";
        out << " *" << "\n";
        out << " * @short function The function to be called periodically" << "\n";
        out << fmt::format(" * @unique_tag UNIQUE_ERROR_{}", counter) << "\n";
        out << " * @module main_module" << "\n";
        out << " * @report ERROR" << "\n";
        out << " * @because wait_between_checks The interval between checks for whether the function should be called, in" << "\n";
        out << " * milliseconds." << "\n";
        out << " * @long function The function to be called periodically." << "\n";
        out << " * @doc_at function The function to be called periodically." << "\n";
        out << " * @solution1 function The function to be called periodically." << "\n";
        out << " * @solution2 function The function to be called periodically." << "\n";
        out << " * @solution3 function The function to be called periodically." << "\n";
        out << " */" << "\n";

        out << "TEST(base_test, base)" << "\n";
        out << "{" << "\n";
        out << R"(    problems.create_problem("Simple problem example", problem::error_type::error))"
        << R"(.add_because("Because");)" << "\n";
        out << "}" << "\n";
        out << "" << "\n";
        counter++;
    }
    out << "" << std::endl;
    // clang-format on
}

int
main(int argc, char** argv)
{
    for (int i = 0; i < number_of_files; i++) {
        create_dummy_file(fmt::format("test{}.cpp", i));
    }
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
