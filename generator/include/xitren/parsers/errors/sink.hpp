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
#include <xitren/problems_pool.hpp>

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
    ~sink() override
    {
        try {
            nlohmann::json out_data;
            out_data[problems_pool::version_key] = pool_.version();
            for (auto const& item : pool_) {
                out_data[problems_pool::problems_key].push_back(problems_pool::convert(item.second));
            }
            out_ << out_data;
        } catch (...) {}
    }

    sink&
    operator=(sink const& other)
        = delete;
    sink&
    operator=(sink const&& other)
        = delete;
    sink(sink const& val) = delete;
    sink(sink&& val)      = delete;

    explicit sink(std::string const& name) : pool_{name}
    {
        out_ = std::ofstream{name};
        if (!out_) {
            throw std::system_error(errno, std::system_category(), "failed to open " + name);
        }
    }

    void
    data(void const* /*src*/, data_type const& nd) final
    {
        using namespace std::string_literals;
        pool_.push(nd);
    }

private:
    std::ofstream out_;
    problems_pool pool_;

    const std::string another_tag_{"@"};
};

}    // namespace xitren::parsers::errors
