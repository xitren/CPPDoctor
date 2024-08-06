set(_target converter)
add_executable(${_target} "main.cpp")

target_link_libraries(${_target} ${LIBRARY_NAME_HOST})
target_compile_features(${_target} PUBLIC cxx_std_20)
target_compile_options(${_target}
                       PRIVATE
                       -Wall -Wextra -Wpedantic -Weffc++ -std=c++20 -Wc++20-compat)
target_link_options(${_target}
                    PRIVATE
                    -Wl,-demangle)
