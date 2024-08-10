/*!
     _ _
__ _(_) |_ _ _ ___ _ _
\ \ / |  _| '_/ -_) ' \
/_\_\_|\__|_| \___|_||_|
* @date 28.07.2024
*/

#include <nlohmann/json.hpp>
#include <xitren/comm/observer.hpp>
#include <xitren/parsers/errors/parser.hpp>
#include <xitren/parsers/errors/exceptions.hpp>
#include <xitren/problem.hpp>

#include <gtest/gtest.h>

using namespace xitren::parsers::errors;
using namespace xitren::comm;

class listener : public observer<nlohmann::json> {
public:
    nlohmann::json record;
protected:
    void
    data(void const*  /*src*/, nlohmann::json const& nd) final {
        std::cout << nd << std::endl;
        record = nd;
    }
};

TEST(base_ideal_test, parser) {
    using namespace std::string_literals;
    parser translator{};
    listener                sink{};
    std::list<std::string>  code{};

    translator.add_observer(sink);
    code.push_back("/**"s);
    code.push_back(" * Constructs a new interval_event object."s);
    code.push_back(" *"s);
    code.push_back(" * @short function The function to be called periodically"s);
    code.push_back(" * @unique_tag UNIQUE_ERROR"s);
    code.push_back(" * @module main_module"s);
    code.push_back(" * @report ERROR"s);
    code.push_back(" * @because wait_between_checks The interval between checks for whether the function should be called, in"s);
    code.push_back(" * milliseconds."s);
    code.push_back(" * @long function The function to be called periodically."s);
    code.push_back(" * @doc_at function The function to be called periodically."s);
    code.push_back(" * @solution1 function The function to be called periodically."s);
    code.push_back(" * @solution2 function The function to be called periodically."s);
    code.push_back(" * @solution3 function The function to be called periodically."s);
    code.push_back(" */"s);

    translator.data(nullptr, code);
    EXPECT_EQ(sink.record[xitren::problem::short_key], "function The function to be called periodically"s);
    EXPECT_EQ(sink.record[xitren::problem::unique_tag_key], "UNIQUE_ERROR"s);
    EXPECT_EQ(sink.record[xitren::problem::module_key], "main_module"s);
    EXPECT_EQ(sink.record[xitren::problem::report_key], "ERROR"s);
    EXPECT_EQ(sink.record[xitren::problem::because_key], "wait_between_checks The interval between checks for whether the function should be called, in milliseconds."s);
    EXPECT_EQ(sink.record[xitren::problem::long_key], "function The function to be called periodically."s);
    EXPECT_EQ(sink.record[xitren::problem::documented_key], "function The function to be called periodically."s);
    EXPECT_EQ(sink.record[xitren::problem::solution1_key], "function The function to be called periodically."s);
    EXPECT_EQ(sink.record[xitren::problem::solution2_key], "function The function to be called periodically."s);
    EXPECT_EQ(sink.record[xitren::problem::solution3_key], "function The function to be called periodically."s);
}

TEST(base_no_field_test, parser) {
    using namespace std::string_literals;
    parser translator{};
    listener                sink{};
    std::list<std::string>  code{};

    translator.add_observer(sink);
    code.push_back("/**"s);
    code.push_back(" * Constructs a new interval_event object."s);
    code.push_back(" *"s);
    code.push_back(" * @short function The function to be called periodically"s);
    code.push_back(" * @unique_tag UNIQUE_ERROR"s);
    code.push_back(" * @report repeat_every The interval at which the function should be called, in milliseconds."s);
    code.push_back(" * @because wait_between_checks The interval between checks for whether the function should be called, in"s);
    code.push_back(" * milliseconds."s);
    code.push_back(" * @long function The function to be called periodically."s);
    code.push_back(" * @doc_at function The function to be called periodically."s);
    code.push_back(" * @solution1 function The function to be called periodically."s);
    code.push_back(" * @solution2 function The function to be called periodically."s);
    code.push_back(" * @solution3 function The function to be called periodically."s);
    code.push_back(" */"s);

    EXPECT_THROW(translator.data(nullptr, code), unexpected_tag);
}

TEST(base_change_field_order_test, parser) {
    using namespace std::string_literals;
    parser translator{};
    listener                sink{};
    std::list<std::string>  code{};

    translator.add_observer(sink);
    code.push_back("/**"s);
    code.push_back(" * Constructs a new interval_event object."s);
    code.push_back(" *"s);
    code.push_back(" * @short function The function to be called periodically"s);
    code.push_back(" * @unique_tag UNIQUE_ERROR"s);
    code.push_back(" * @report repeat_every The interval at which the function should be called, in milliseconds."s);
    code.push_back(" * @because wait_between_checks The interval between checks for whether the function should be called, in"s);
    code.push_back(" * milliseconds."s);
    code.push_back(" * @doc_at function The function to be called periodically."s);
    code.push_back(" * @long function The function to be called periodically."s);
    code.push_back(" * @solution1 function The function to be called periodically."s);
    code.push_back(" * @solution2 function The function to be called periodically."s);
    code.push_back(" * @solution3 function The function to be called periodically."s);
    code.push_back(" */"s);

    EXPECT_THROW(translator.data(nullptr, code), unexpected_tag);
}

TEST(base_format_error_test, parser) {
    using namespace std::string_literals;
    parser translator{};
    listener                sink{};
    std::list<std::string>  code{};

    translator.add_observer(sink);
    code.push_back("/**"s);
    code.push_back(" * Constructs a new interval_event object."s);
    code.push_back(" *"s);
    code.push_back(" * @short function The function to be called periodically"s);
    code.push_back(" * @unique_tag UNIQUE_ERROR"s);
    code.push_back(" * @module main_module"s);
    code.push_back(" *@report ERROR"s);
    code.push_back(" * @because wait_between_checks The interval between checks for whether the function should be called, in"s);
    code.push_back(" *milliseconds."s);
    code.push_back(" * @long function The function to be called periodically."s);
    code.push_back(" * @doc_at function The function to be called periodically."s);
    code.push_back(" * @solution1 function The function to be called periodically."s);
    code.push_back(" * @solution2 function The function to be called periodically."s);
    code.push_back(" * @solution3 function The function to be called periodically."s);
    code.push_back(" */"s);

    EXPECT_THROW(translator.data(nullptr, code), unexpected_tag);
}

TEST(base_clamp_error_test, parser) {
    using namespace std::string_literals;
    parser translator{};
    listener                sink{};
    std::list<std::string>  code{};

    translator.add_observer(sink);
    code.push_back("/***/"s);

    EXPECT_THROW(translator.data(nullptr, code), unexpected_tag);
}

