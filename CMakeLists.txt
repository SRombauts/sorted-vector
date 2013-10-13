# Copyright (c) 2013 Sebastien Rombauts (sebastien.rombauts@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)

cmake_minimum_required (VERSION 2.6)
project (sorted-vector)

if(MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
    set(CCPPLINT_ARG_OUTPUT "--output=vs7")
    add_definitions (/D_CRT_SECURE_NO_WARNINGS)
else()
    set(CCPPLINT_ARG_OUTPUT "--output=eclipse")
#   add_definitions (-std=c++0x)  # -std=c++11
endif()
set(CCPPLINT_ARG_VERBOSE "--verbose=3")

# add the application executable
add_executable (sorted-vector
 src/main.cpp
 src/utils/random.cpp src/utils/random.h
 src/utils/time.cpp src/utils/time.h
 src/utils/utils.h)
target_link_libraries (sorted-vector rt)

# add a cpplint target to the "all" target
add_custom_target(cpplint
 ALL
 COMMAND python cpplint.py ${CCPPLINT_ARG_OUTPUT} ${CCPPLINT_ARG_VERBOSE} src/*.* src/*/*.* 
 WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

# add a Doxygen target to the "all" target
add_custom_target(doxygen
 ALL
 COMMAND doxygen Doxyfile > /dev/null 
 WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

# add the subdirectory containing the CMakeLists.txt for the gtest library
add_subdirectory (googletest) 
include_directories ("${PROJECT_SOURCE_DIR}/googletest/include")

include_directories ("${PROJECT_SOURCE_DIR}/src")

# add the test executable
add_executable (test-sorted-vector
 src/utils/random.cpp src/utils/random.h src/utils/random_test.cpp
 src/utils/time.cpp src/utils/time.h src/utils/time_test.cpp)
target_link_libraries (test-sorted-vector rt gtest_main)
 
# add a "test" target:
enable_testing()

# does the application runs : no, it became to long for Travis CI
#add_test (ApplicationRuns sorted-vector)

# does the tests pass
add_test (TestsPass test-sorted-vector)

