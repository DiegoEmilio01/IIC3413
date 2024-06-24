cmake_minimum_required(VERSION 3.12)

# Set project name and version
project(IIC3413DB VERSION 0.5.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Enable export of compile commands output
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set output directories
set(EXECUTABLE_OUTPUT_PATH bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY lib)

# Define the compiler flags
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic -pthread -march=native -funroll-loops -fno-operator-names")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g3 -fsanitize=undefined,address -fno-omit-frame-pointer")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -g0")

# When compiling with Clang generate debug info specifically for lldb
if(CMAKE_CXX_COMPILER_ID STREQUAL Clang)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -glldb -fstandalone-debug")
endif()

if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated")
    set(FS "")
else()
    set(FS "stdc++fs")
endif(APPLE)

# Add include directories
include_directories(${CMAKE_SOURCE_DIR}/src)

# Define sources
file(GLOB_RECURSE SRCS src/*.cc src/*.cpp)
file(GLOB_RECURSE BIN_SRCS src/bin/*.cc)
# file(GLOB_RECURSE TEST_SRCS src/tests/*.cc)

# Remove unwanted files from common sources
list(REMOVE_ITEM SRCS ${BIN_SRCS})
# list(REMOVE_ITEM SRCS ${TEST_SRCS})

# Third party dependencies
add_subdirectory(${CMAKE_SOURCE_DIR}/third_party/antlr4-runtime-4.13.1)
include_directories(${CMAKE_SOURCE_DIR}/third_party/antlr4-runtime-4.13.1/src)
# include_directories(${CMAKE_SOURCE_DIR}/third_party/boost_1_81/include)

# Define targets
add_library(iic3413db STATIC ${SRCS})

set(BUILD_TARGETS
    execute_query
    create_example_db
)

# set(TEST_TARGETS
# )

# Build targets
foreach(target ${BUILD_TARGETS})
    add_executable(${target} ${CMAKE_SOURCE_DIR}/src/bin/${target}.cc)
    target_link_libraries(${target}
        iic3413db
        antlr4_cpp_runtime
        ${FS}
    )
endforeach(target)

# Test targets
# enable_testing()
# foreach(target ${TEST_TARGETS})
#     add_executable(${target} ${CMAKE_SOURCE_DIR}/src/tests/${target}.cc)
#     set_target_properties(${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests")
#     target_link_libraries(${target}
#         iic3413db
#         antlr4_cpp_runtime
#         ${FS}
#     )
#     add_test(
#         NAME ${target}
#         COMMAND $<TARGET_FILE:${target}>
#         WORKING_DIRECTORY tests
#     )
# endforeach(target)
