cmake_minimum_required(VERSION 3.20)
project(Graphs)

set(CMAKE_CXX_FLAGS "-std=c++2c")

add_subdirectory(src)
add_subdirectory(test)

find_program(CLANG_FORMAT "clang-format")

if (CLANG_FORMAT)

    file(GLOB_RECURSE ALL_PROJECT_FILES 
        "${PROJECT_SOURCE_DIR}/src/*.cpp" 
        "${PROJECT_SOURCE_DIR}/inc/*.h"
        "${PROJECT_SOURCE_DIR}/inc/*.hpp"
        "${PROJECT_SOURCE_DIR}/test/*.cpp" 
        "${PROJECT_SOURCE_DIR}/test/*.h" 
        "${PROJECT_SOURCE_DIR}/main.cpp")

    add_custom_target(format
        COMMAND clang-format 
        -i
        -style=file:/home/gerwant/repos/git-scripts/.clang-format
        ${ALL_PROJECT_FILES}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Running clang-format")
endif()