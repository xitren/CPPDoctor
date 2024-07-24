/*!
_ _
__ _(_) |_ _ _ ___ _ _
\ \ / |  _| '_/ -_) ' \
/_\_\_|\__|_| \___|_||_|
* @date 24.07.2024
*/
#include <nlohmann/json.hpp>
#include <xitren/func/argv_parser.hpp>

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

        while (std::getline(infile, line)) {
            if (line.find("PROBLEM_SOLVERS_PASTE") != std::string::npos) {
                break;
            }
            out << line << "\n";
        }
        for (auto& el : problems.items()) {
            std::cout << el.key() << " : " << el.value() << "\n";
            std::string_view error_type;
            if (el.value()["type"] == "TRACE") {
                error_type = error_types[0];
            } else if (el.value()["type"] == "DEBUG") {
                error_type = error_types[1];
            } else if (el.value()["type"] == "INFO") {
                error_type = error_types[2];
            } else if (el.value()["type"] == "WARNING") {
                error_type = error_types[3];
            } else if (el.value()["type"] == "ERROR") {
                error_type = error_types[4];
            } else if (el.value()["type"] == "CRITICAL") {
                error_type = error_types[5];
            } else {
                error_type = error_types[4];
            }
            std::string short_desc      = el.value()["short_desc"];
            std::string because         = el.value()["because"];
            std::string long_desc       = el.value()["long_desc"];
            std::string documented_at   = el.value()["documented_at"];
            std::string solution_first  = el.value()["solution_first"];
            std::string solution_second = el.value()["solution_second"];
            std::string solution_third  = el.value()["solution_third"];
            out << fmt::format("        problem_pool::instance().create_problem(\"{}\", {})", short_desc, error_type)
                << "\n";
            out << fmt::format("            .add_because(\"{}\")", because) << "\n";
            out << fmt::format("            .add_long_desc(\"{}\")", long_desc) << "\n";
            out << fmt::format("            .add_documented_at(\"{}\")", documented_at) << "\n";
            out << fmt::format("            .add_solution(\"{}\")", solution_first) << "\n";
            out << fmt::format("            .add_solution(\"{}\")", solution_second) << "\n";
            out << fmt::format("            .add_solution(\"{}\");", solution_third) << "\n";
        }
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
