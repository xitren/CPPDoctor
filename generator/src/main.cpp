/*!
_ _
__ _(_) |_ _ _ ___ _ _
\ \ / |  _| '_/ -_) ' \
/_\_\_|\__|_| \___|_||_|
* @date 24.07.2024
*/
#include <nlohmann/json.hpp>
#include <xitren/parsers/errors/locator.hpp>
#include <xitren/parsers/errors/locator_pipe.hpp>
#include <xitren/parsers/errors/parser.hpp>

#include <fmt/core.h>

#include <fstream>
#include <iostream>
#include <sstream>

using json = nlohmann::json;

using namespace xitren::parsers::errors;
parser       sink{"problems.json"};
locator_pipe source{};

int
main(int argv, char const* argc[])
{
    using namespace std::string_literals;
    if (argv < 2) {
        std::cout << "Not enough parameters!" << std::endl;
        return -1;
    }
    if (strcmp(argc[1], "-o")) {
        std::cout << "Format error! Should be '-o'!" << std::endl;
        return -1;
    }

    source.add_observer(sink);

    source.data(nullptr, "test.cpp"s);
    for (int i = 3;i < argv;i++) {
        std::string filename{argc[3]};
        source.data(nullptr, filename);
    }
    return 0;
}
