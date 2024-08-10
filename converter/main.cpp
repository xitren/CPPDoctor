/*!
_ _
__ _(_) |_ _ _ ___ _ _
\ \ / |  _| '_/ -_) ' \
/_\_\_|\__|_| \___|_||_|
* @date 24.07.2024
*/
#include <nlohmann/json.hpp>
#include <xitren/func/argv_parser.hpp>
#include <xitren/problem_const.hpp>

#include <fmt/core.h>

#include <fstream>
#include <iostream>
#include <sstream>

using json = nlohmann::json;
template <class Name>
using argv_parser = xitren::func::argv_parser<Name>;

const std::array<std::string_view, 6> error_types = {{{"problem::error_type::trace"},
                                                      {"problem::error_type::debug"},
                                                      {"problem::error_type::info"},
                                                      {"problem::error_type::warning"},
                                                      {"problem::error_type::error"},
                                                      {"problem::error_type::critical"}}};
auto
convert_from_json(std::size_t id, json el) {
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

void find_definitions_paste(std::ifstream& infile, std::ofstream& outfile, json problems) {
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
        id++;
        xitren::problem_const data = convert_from_json(id, el);

        // clang-format off
        outfile << fmt::format("        const problem_const {}", data.unique_tag) << " {" << "\n";
        outfile << fmt::format("            .index           = {},", data.index) << "\n";
        outfile << fmt::format("            .short_desc_hash = problem_const::hash({}),", data.short_desc) << "\n";
        outfile << fmt::format("            .short_desc      = {},", data.short_desc) << "\n";
        outfile << fmt::format("            .unique_tag      = {},", data.unique_tag) << "\n";
        outfile << fmt::format("            .module_desc     = {},", data.module_desc) << "\n";
        outfile << fmt::format("            .report          = problem_const::report_by_desc({}),",
                                xitren::problem_const::report_desc(data.report)) << "\n";
        outfile << fmt::format("            .because         = {},", data.because) << "\n";
        outfile << fmt::format("            .long_desc       = {},", data.long_desc) << "\n";
        outfile << fmt::format("            .documented_at   = {},", data.documented_at) << "\n";
        outfile << fmt::format("            .solutions       = {{{}, {}, {}}}",
                                data.solutions[0], data.solutions[1], data.solutions[2]) << "\n";
        outfile << "        };" << "\n";
        // clang-format on
    }
}

void find_solvers_paste(std::ifstream& infile, std::ofstream& outfile, json problems) {
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
        id++;
        auto data = convert_from_json(id, el);

        // clang-format off
        outfile << fmt::format("        vault_[{}] = &{}", data.index, data.unique_tag) << "\n";
        // clang-format on
    }
}

int
main(int argv, char const* argc[])
{
    struct options {
        std::string problems_file{"problems.json"};
        std::string proto_header_file{"problem.hpp"};
        std::string output_header_file{"problem_desc.hpp"};
    };

    auto parser = argv_parser<options>::instance({{"--problems", &options::problems_file},
                                                  {"--header", &options::proto_header_file},
                                                  {"--output", &options::output_header_file}});

    auto detected_opts = parser->parse(argv, argc);

    std::cout << "problems file = " << detected_opts.problems_file << std::endl;
    std::cout << "proto header file = " << detected_opts.proto_header_file << std::endl;
    std::cout << "output header file = " << detected_opts.output_header_file << std::endl;

    try {
        std::ifstream f(detected_opts.problems_file);
        json          data     = json::parse(f);
        auto          problems = data["problems"];
        std::ifstream infile(detected_opts.proto_header_file);
        std::string   line;
        std::ofstream out(detected_opts.output_header_file);

        if (!f) {
            throw std::system_error(errno, std::system_category(), "failed to open " + detected_opts.problems_file);
        }
        if (!infile) {
            throw std::system_error(errno, std::system_category(), "failed to open " + detected_opts.proto_header_file);
        }
        if (!out) {
            throw std::system_error(errno, std::system_category(),
                                    "failed to open " + detected_opts.output_header_file);
        }
        
        find_definitions_paste(infile, out, problems);
        find_solvers_paste(infile, out, problems);
        
        while (std::getline(infile, line)) {
            const std::string str = "PROBLEMS";
            if (size_t start = 0; (start = line.find(str)) != std::string::npos) {
                auto replace_str = std::to_string(problems.size());
                line.replace(start, str.size(), replace_str.data());
            }
            out << line << "\n";
        }
    } catch (json::parse_error& err) {
        std::cout << R"(\033[1;31mError parsing problems file\033[0m\n:)" << std::endl;
        std::cout << err.what() << std::endl;
        return -1;
    } catch (json::exception& err) {
        std::cout << R"(\033[1;31mError, guess: no field exist\033[0m\n:)" << std::endl;
        std::cout << err.what() << std::endl;
        return -1;
    } catch (std::system_error& err) {
        std::cout << R"(\033[1;31mCannot open file for read or write or file doesn't here!\033[0m\n:)" << std::endl;
        std::cout << "Code: " << err.code() << " problem: " << err.what() << std::endl;
        return -1;
    } catch (std::exception& err) {
        std::cout << err.what() << std::endl;
        return -1;
    }
    return 0;
}
