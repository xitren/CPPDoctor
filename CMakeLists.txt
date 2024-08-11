cmake_minimum_required(VERSION 3.23)

set(LIBRARY_NAME
	cpp_doctor_lib
	)
set(LIBRARY_NAME_HOST
	cpp_doctor_json_lib
	)
set(GEN_LIBRARY_NAME
	cpp_doctor_gen_lib
	)

set(GLOBAL_NAMESPACE
	xitren
	)

project(${LIBRARY_NAME}
		DESCRIPTION "CPP Doctor library"
		VERSION 1.0.1
		LANGUAGES CXX
		)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${LIBRARY_NAME} INTERFACE)
add_library(${GLOBAL_NAMESPACE}::${LIBRARY_NAME} ALIAS ${LIBRARY_NAME})
target_compile_options(${LIBRARY_NAME} INTERFACE -std=c++20 -Wc++20-compat)
target_include_directories(
		${LIBRARY_NAME} INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>)
message(STATUS "Adding library project \"${LIBRARY_NAME}\"")

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

add_library(${LIBRARY_NAME_HOST} INTERFACE)
add_library(${GLOBAL_NAMESPACE}::${LIBRARY_NAME_HOST} ALIAS ${LIBRARY_NAME_HOST})
target_compile_options(${LIBRARY_NAME_HOST} INTERFACE -std=c++20 -Wc++20-compat)
target_include_directories(
		${LIBRARY_NAME_HOST} INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>)
target_link_libraries(${LIBRARY_NAME_HOST} INTERFACE nlohmann_json::nlohmann_json patterns_lib fmt::fmt)
message(STATUS "Adding library project \"${LIBRARY_NAME_HOST}\"")

add_library(${GEN_LIBRARY_NAME} INTERFACE)
add_library(${GLOBAL_NAMESPACE}::${GEN_LIBRARY_NAME} ALIAS ${GEN_LIBRARY_NAME})
target_compile_options(${GEN_LIBRARY_NAME} INTERFACE -std=c++20 -Wc++20-compat)
target_include_directories(
		${GEN_LIBRARY_NAME} INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>)
message(STATUS "Adding library project \"${GEN_LIBRARY_NAME}\"")

add_subdirectory(generator)
add_subdirectory(converter)

enable_testing()
add_subdirectory(tests)
