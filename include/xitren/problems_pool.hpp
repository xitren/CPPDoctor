/*!
_ _
__ _(_) |_ _ _ ___ _ _
\ \ / |  _| '_/ -_) ' \
/_\_\_|\__|_| \___|_||_|
* @date 05.08.2024
*/
#pragma once
#include <nlohmann/json.hpp>
#include <xitren/problem.hpp>

#include <array>
#include <iostream>
#include <map>
#include <optional>
#include <unordered_map>

namespace xitren {

class problems_pool : public std::unordered_map<std::string, problem> {
public:
    static constexpr std::string_view version_key{"version"};
    static constexpr std::string_view problems_key{"problems"};

    static problem
    parse(nlohmann::json el)
    {
        std::string     short_desc      = el[xitren::problem::short_key];
        std::string     unique_tag      = el[xitren::problem::unique_tag_key];
        std::string     module_desc     = el[xitren::problem::module_key];
        std::string     report          = el[xitren::problem::report_key];
        std::string     because         = el[xitren::problem::because_key];
        std::string     long_desc       = el[xitren::problem::long_key];
        std::string     documented_at   = el[xitren::problem::documented_key];
        std::string     solution_first  = el[xitren::problem::solution1_key];
        std::string     solution_second = el[xitren::problem::solution2_key];
        std::string     solution_third  = el[xitren::problem::solution3_key];
        std::string     filename        = el[xitren::problem::filename_key];
        std::size_t     line            = el[xitren::problem::line_key];
        xitren::problem problem{short_desc, unique_tag, module_desc,   xitren::problem::report_by_desc(report),
                                because,    long_desc,  documented_at, filename,
                                line};
        problem.add_solution(solution_first);
        problem.add_solution(solution_second);
        problem.add_solution(solution_third);
        return problem;
    }

    static nlohmann::json
    convert(problem const& el)
    {
        nlohmann::json data;
        data[xitren::problem::short_key]      = el.short_desc();
        data[xitren::problem::unique_tag_key] = el.unique_tag();
        data[xitren::problem::module_key]     = el.module_desc();
        data[xitren::problem::report_key]     = xitren::problem::report_desc(el.report());
        data[xitren::problem::because_key]    = el.because();
        data[xitren::problem::long_key]       = el.long_desc();
        data[xitren::problem::documented_key] = el.documented_at();
        data[xitren::problem::solution1_key]  = el.solution1();
        data[xitren::problem::solution2_key]  = el.solution2();
        data[xitren::problem::solution3_key]  = el.solution3();
        data[xitren::problem::filename_key]   = el.filename();
        data[xitren::problem::line_key]       = el.line();
        return data;
    }

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

    explicit problems_pool(std::string const& name)
    {
        std::ifstream in_file{name};
        if (!in_file) {
            return;
        }
        auto pool     = nlohmann::json::parse(in_file);
        auto problems = pool[problems_key];
        version_      = pool[version_key];
        for (auto const& el : problems.items()) {
            auto problem                          = parse(el.value());
                 operator[](problem.unique_tag()) = problem;
        }
    }

    explicit problems_pool(nlohmann::json const& data) : version_{data[version_key]}
    {
        auto problems = data[problems_key];
        for (auto const& el : problems.items()) {
            auto problem                          = parse(el.value());
                 operator[](problem.unique_tag()) = problem;
        }
    }

    void
    push(problem const& desc)
    {
        if (contains(desc.unique_tag())) {
            if (desc.hash() == operator[](desc.unique_tag()).hash()) {
                return;
            }
            operator[](desc.unique_tag()) = desc;
            inc_version();
            return;
        }
        operator[](desc.unique_tag()) = desc;
        inc_version();
    }

    void
    push(nlohmann::json const& data)
    {
        auto problem = parse(data);
        if (contains(problem.unique_tag())) {
            if (problem.hash() == operator[](problem.unique_tag()).hash()) {
                return;
            }
            operator[](problem.unique_tag()) = problem;
            inc_version();
            return;
        }
        operator[](problem.unique_tag()) = problem;
        inc_version();
    }

    auto&
    version() const
    {
        return version_;
    }

private:
    void
    inc_version()
    {
        if (!changed) {
            version_++;
            changed = true;
        }
    }

    std::size_t version_{};
    bool        changed{false};
};

}    // namespace xitren