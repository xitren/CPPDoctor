/*!
_ _
__ _(_) |_ _ _ ___ _ _
\ \ / |  _| '_/ -_) ' \
/_\_\_|\__|_| \___|_||_|
* @date 11.08.2024
*/
#pragma once

#include <nlohmann/json.hpp>
#include <xitren/func/argv_parser.hpp>
#include <xitren/problem_const.hpp>

#include <fmt/core.h>

#include <fstream>
#include <iostream>
#include <sstream>

namespace xitren {

auto
convert_from_json(std::size_t id, nlohmann::json el) {
    std::string short_desc      = el["short_desc"];
    std::string unique_tag      = el["unique_tag"];
    std::string module_desc     = el["module"];
    std::string report          = el["type"];
    std::string because         = el["because"];
    std::string long_desc       = el["long_desc"];
    std::string documented_at   = el["documented_at"];
    std::string solution_first  = el["solution_first"];
    std::string solution_second = el["solution_second"];
    std::string solution_third  = el["solution_third"];
    std::transform(unique_tag.begin(), unique_tag.end(), unique_tag.begin(),
                   [](unsigned char c){ return std::tolower(c); });

    return xitren::problem_const{
        .index           = id,
        .short_desc_hash = xitren::problem_const::hash(short_desc),
        .short_desc      = short_desc,
        .unique_tag      = unique_tag,
        .module_desc     = module_desc,
        .report          = xitren::problem_const::report_by_desc(report),
        .because         = because,
        .long_desc       = long_desc,
        .documented_at   = documented_at,
        .solutions       = {{solution_first, solution_second, solution_third}}
    };
}

void find_problems_counter_paste(std::ifstream& infile, std::ofstream& outfile, nlohmann::json problems, bool short_desc) {
    using namespace std::string_view_literals;
    std::string   line;
    while (std::getline(infile, line)) {
        const std::string str = "PROBLEMS_COUNTER";
        if (size_t start = 0; (start = line.find(str)) != std::string::npos) {
            auto replace_str = std::to_string(problems.size());
            line.replace(start, str.size(), replace_str.data());
        }
        outfile << line << "\n";
    }
}

void find_definitions_paste(std::ifstream& infile, std::ofstream& outfile, nlohmann::json problems, bool short_desc) {
    using namespace std::string_view_literals;
    std::string   line;
    while (std::getline(infile, line)) {
        if (line.find("PROBLEM_DEFINITIONS_PASTE") != std::string::npos) {
            break;
        }
        outfile << line << "\n";
    }
    std::size_t id{};
    for (const auto& el : problems.items()) {
        xitren::problem_const data = convert_from_json(id, el.value());
        id++;

        // clang-format off
        outfile << fmt::format(R"(    const problem_const {})", data.unique_tag) << " {" << "\n";
        outfile << fmt::format(R"(        .index           = {},)", data.index) << "\n";
        outfile << fmt::format(R"(        .short_desc_hash = problem_const::hash("{}"),)", data.short_desc) << "\n";
        outfile << fmt::format(R"(        .short_desc      = "{}",)", data.short_desc) << "\n";
        outfile << fmt::format(R"(        .unique_tag      = "{}",)", data.unique_tag) << "\n";
        outfile << fmt::format(R"(        .module_desc     = "{}",)", data.module_desc) << "\n";
        if (!short_desc) {
            outfile << fmt::format(R"(        .report          = problem_const::report_by_desc("{}"),)",
                                    xitren::problem_const::report_desc(data.report)) << "\n";
            outfile << fmt::format(R"(        .because         = "{}",)", data.because) << "\n";
            outfile << fmt::format(R"(        .long_desc       = "{}",)", data.long_desc) << "\n";
            outfile << fmt::format(R"(        .documented_at   = "{}",)", data.documented_at) << "\n";
            outfile << fmt::format(R"(        .solutions       = {{"{}", "{}", "{}"}})",
                                    data.solutions[0], data.solutions[1], data.solutions[2]) << "\n";
        } else {
            outfile << fmt::format(R"(        .report          = problem_const::report_by_desc("{}"))",
                                    xitren::problem_const::report_desc(data.report)) << "\n";
        }
        outfile << "    };" << "\n";
        // clang-format on
    }
}

void find_solvers_paste(std::ifstream& infile, std::ofstream& outfile, nlohmann::json problems, bool short_desc) {
    using namespace std::string_view_literals;
    std::string   line;
    while (std::getline(infile, line)) {
        if (line.find("PROBLEM_SOLVERS_PASTE") != std::string::npos) {
            break;
        }
        outfile << line << "\n";
    }
    std::size_t id{};
    for (const auto& el : problems.items()) {
        auto data = convert_from_json(id, el.value());
        id++;

        // clang-format off
        outfile << fmt::format("        vault_[{}] = &{};", data.index, data.unique_tag) << "\n";
        // clang-format on
    }
}

}
