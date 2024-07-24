include(FetchContent)
FetchContent_Declare(
        patterns
        GIT_REPOSITORY https://github.com/xitren/patterns.git
        GIT_TAG v1.0.2
)
FetchContent_MakeAvailable(patterns)

FetchContent_Declare(fmt
                     GIT_REPOSITORY https://github.com/fmtlib/fmt.git
                     GIT_TAG master
                     )
FetchContent_MakeAvailable(fmt)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

set(_target converter)
add_executable(${_target} "main.cpp")

target_link_libraries(${_target} nlohmann_json::nlohmann_json patterns_lib fmt::fmt)
target_compile_features(${_target} PUBLIC cxx_std_20)
target_compile_options(${_target}
                       PRIVATE
                       -Wall -Wextra -Wpedantic -Weffc++ -std=c++20 -Wc++20-compat)
target_link_options(${_target}
                    PRIVATE
                    -Wl,-demangle)
