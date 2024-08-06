/*!
_ _
__ _(_) |_ _ _ ___ _ _
\ \ / |  _| '_/ -_) ' \
/_\_\_|\__|_| \___|_||_|
* @date 05.08.2024
*/
#pragma once
#include <xitren/problem.hpp>

#include <array>
#include <iostream>
#include <map>
#include <optional>

namespace xitren {

class problems_pool {

public:
    problems_pool()  = delete;
    ~problems_pool() = default;
    problems_pool&
    operator=(problems_pool const& other)
        = delete;
    problems_pool&
    operator=(problems_pool&& other)
        = delete;
    problems_pool(problems_pool const& val) = default;
    problems_pool(problems_pool&& val)      = default;

    explicit problems_pool(std::size_t version) : version_{version} {}

    void
    push(problem const& desc)
    {
        if (pool_.contains(desc.unique_tag())) {
            return;
        }
        pool_[desc.unique_tag()] = desc;
    }

private:
    std::map<std::string, problem> pool_;
    std::size_t                    version_{};
};

}    // namespace xitren