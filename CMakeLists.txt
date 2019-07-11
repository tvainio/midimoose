cmake_minimum_required (VERSION 2.6)
project (ClockDivider)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
 
# Link runTests with what we want to test and the GTest and pthread library
add_executable(runTests tests.cpp ../clock_divider.cpp)
target_link_libraries(runTests ${GTEST_LIBRARIES} pthread)

