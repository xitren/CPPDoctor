set(CONV_LIBRARY_NAME
    converter_lib
    )

set(GLOBAL_NAMESPACE
    xitren
    )

project(${CONV_LIBRARY_NAME}
        DESCRIPTION "Converter library"
        VERSION 1.0.1
        LANGUAGES CXX
        )

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${CONV_LIBRARY_NAME} INTERFACE)
add_library(${GLOBAL_NAMESPACE}::${CONV_LIBRARY_NAME} ALIAS ${CONV_LIBRARY_NAME})
target_compile_options(${CONV_LIBRARY_NAME} INTERFACE -std=c++20 -Wc++20-compat)
target_include_directories(
        ${CONV_LIBRARY_NAME} INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>)
target_link_libraries(${CONV_LIBRARY_NAME} INTERFACE ${LIBRARY_NAME_HOST})
message(STATUS "Adding library project \"${GEN_LIBRARY_NAME}\"")

set(_target converter)
add_executable(${_target} "src/main.cpp")

target_include_directories(${_target} PUBLIC
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                           $<INSTALL_INTERFACE:include>
                           )
target_link_libraries(${_target} ${CONV_LIBRARY_NAME})
target_compile_features(${_target} PUBLIC cxx_std_20)
target_compile_options(${_target}
                       PRIVATE
                       -Wall -Wextra -Wpedantic -Weffc++ -std=c++20 -Wc++20-compat)
target_link_options(${_target}
                    PRIVATE
                    -Wl,-demangle)

enable_testing()
add_subdirectory(tests)