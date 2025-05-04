cmake_minimum_required(VERSION 3.10)
project(computer_club_test VERSION 0.1.0)

enable_testing()

add_executable(${PROJECT_NAME}
    src/SimpleTest.cpp
)

target_link_libraries(computer_club_test computer_club_lib GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(computer_club_test)