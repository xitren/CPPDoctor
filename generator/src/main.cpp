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
#include <xitren/parsers/errors/sink.hpp>

#include <fmt/core.h>

#include <fstream>
#include <iostream>
#include <sstream>

using json = nlohmann::json;

using namespace xitren::parsers::errors;

int
main(int argv, char const* argc[])
{
    using namespace std::string_literals;

    if (argv < 4) {
        std::cout << "Not enough parameters! Should be 4." << std::endl;
        return -1;
    }
    if (strcmp(argc[1], "-o") != 0) {
        std::cout << "Format error! Should be '-o'!" << std::endl;
        return -1;
    }

    sink         file{argc[2]};
    parser       parser{};
    locator_pipe source{};

    source.add_observer(parser);
    parser.add_observer(file);

    for (int i = 3; i < argv; i++) {
        std::string filename{argc[i]};
        source.data(nullptr, filename);
    }
    source.join();
    return 0;
}
