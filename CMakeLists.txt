cmake_minimum_required(VERSION 3.23)

set(LIBRARY_NAME
    cpp_doctor_lib
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

add_subdirectory(generator)
add_subdirectory(converter)

enable_testing()
add_subdirectory(tests)
