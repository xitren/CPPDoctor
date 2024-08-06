/*!
_ _
__ _(_) |_ _ _ ___ _ _
\ \ / |  _| '_/ -_) ' \
/_\_\_|\__|_| \___|_||_|
* @date 05.08.2024
*/
#pragma once
#include <nlohmann/json.hpp>
#include <xitren/comm/observer.hpp>
#include <xitren/parsers/errors/exceptions.hpp>

#include <fmt/core.h>

#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <string>

namespace xitren::parsers::errors {

class sink : public comm::observer<nlohmann::json> {
    using data_type = nlohmann::json;

public:
    sink() = delete;
    ~sink() override { out_ << pool_; }
    sink&
    operator=(sink const& other)
        = delete;
    sink&
    operator=(sink const&& other)
        = delete;
    sink(sink const& val) = delete;
    sink(sink&& val)      = delete;

    explicit sink(std::string const& name)
    {
        std::ifstream in{name};
        if (in) {
            nlohmann::json data = nlohmann::json::parse(in);
            version_            = data["version"];
            version_++;
        }
        pool_["version"] = version_;
        out_             = std::ofstream{name};
        if (!out_) {
            throw std::system_error(errno, std::system_category(), "failed to open " + name);
        }
    }

    void
    data(void const* /*src*/, data_type const& nd) final
    {
        using namespace std::string_literals;
        pool_["problems"].push_back(nd);
    }

private:
    int            version_{};
    nlohmann::json pool_{};
    std::ofstream  out_;

    const std::string another_tag_{"@"};
};

}    // namespace xitren::parsers::errors
