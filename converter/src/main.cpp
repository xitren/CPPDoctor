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
#include <xitren/solvers.hpp>

#include <fmt/core.h>

#include <fstream>
#include <iostream>
#include <sstream>

using json = nlohmann::json;
template <class Name>
using argv_parser = xitren::func::argv_parser<Name>;

int
main(int argv, char const* argc[])
{
    struct options {
        std::string problems_file{"problems.json"};
        std::string proto_header_file{"problem.hpp"};
        std::string output_header_file{"problem_desc.hpp"};
        bool        short_description{false};
    };

    auto parser = argv_parser<options>::instance({{"--problems", &options::problems_file},
                                                  {"--header", &options::proto_header_file},
                                                  {"--output", &options::output_header_file},
                                                  {"--short", &options::short_description}});

    auto detected_opts = parser->parse(argv, argc);

    std::cout << "problems file = " << detected_opts.problems_file << std::endl;
    std::cout << "proto header file = " << detected_opts.proto_header_file << std::endl;
    std::cout << "output header file = " << detected_opts.output_header_file << std::endl;
    std::cout << "short description = " << detected_opts.short_description << std::endl;

    try {
        std::ifstream f(detected_opts.problems_file);
        json          data     = json::parse(f);
        auto          problems = data["problems"];
        std::ifstream infile(detected_opts.proto_header_file);
        std::string   line;
        std::ofstream outfile(detected_opts.output_header_file);

        if (!f) {
            throw std::system_error(errno, std::system_category(), "failed to open " + detected_opts.problems_file);
        }
        if (!infile) {
            throw std::system_error(errno, std::system_category(), "failed to open " + detected_opts.proto_header_file);
        }
        if (!outfile) {
            throw std::system_error(errno, std::system_category(),
                                    "failed to open " + detected_opts.output_header_file);
        }
        
        xitren::find_definitions_paste(infile, outfile, problems, detected_opts.short_description);
        xitren::find_solvers_paste(infile, outfile, problems, detected_opts.short_description);
        xitren::find_problems_counter_paste(infile, outfile, problems, detected_opts.short_description);

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
