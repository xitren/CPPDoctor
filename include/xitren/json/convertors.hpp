/*!
_ _
__ _(_) |_ _ _ ___ _ _
\ \ / |  _| '_/ -_) ' \
/_\_\_|\__|_| \___|_||_|
* @date 05.08.2024
*/
#pragma once
#include <nlohmann/json.hpp>
#include <xitren/json/convertors.hpp>
#include <xitren/problem.hpp>
#include <xitren/problems_pool.hpp>

namespace ns {
void
to_json(nlohmann::json& j, xitren::problem const& p)
{
    using namespace xitren;
    j = nlohmann::json{
        {problem::short_key, p.short_desc()},         {problem::unique_tag_key, p.unique_tag()},
        {problem::module_key, p.module_desc()},       {problem::report_key, problem::report_desc(p.report())},
        {problem::because_key, p.because()},          {problem::long_key, p.long_desc()},
        {problem::documented_key, p.documented_at()}, {problem::solution1_key, p.solution1()},
        {problem::solution2_key, p.solution2()},      {problem::solution3_key, p.solution3()}};
}

void
from_json(nlohmann::json const& j, xitren::problem& p)
{
    using namespace xitren;
    std::string short_desc    = j.at(problem::short_key);
    std::string unique_tag    = j.at(problem::unique_tag_key);
    std::string module_desc   = j.at(problem::module_key);
    std::string report        = j.at(problem::report_key);
    std::string because       = j.at(problem::because_key);
    std::string long_desc     = j.at(problem::long_key);
    std::string documented_at = j.at(problem::documented_key);
    std::string solution1     = j.at(problem::solution1_key);
    std::string solution2     = j.at(problem::solution2_key);
    std::string solution3     = j.at(problem::solution3_key);

    p.short_desc(short_desc);
    p.unique_tag(unique_tag);
    p.module_desc(module_desc);
    p.report(problem::report_by_desc(report));
    p.because(because);
    p.long_desc(long_desc);
    p.documented_at(documented_at);
    p.clear_solutions();
    p.add_solution(solution1);
    p.add_solution(solution2);
    p.add_solution(solution3);
}
}    // namespace ns
