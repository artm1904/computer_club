cmake_minimum_required(VERSION 3.10)
project(computer_club VERSION 0.1.0)



include_directories(${PROJECT_NAME}
    PRIVATE
    include/
)

set(SOURCES
    src/main.cpp
    src/Club.cpp
    src/Event.cpp
    src/Parser.cpp
    src/Time.cpp
    src/Table.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})


add_library(computer_club_lib
  include/Club.hpp
  include/Event.hpp
  include/Parser.hpp
  include/Table.hpp
  include/Time.hpp
  src/Club.cpp
  src/Event.cpp
  src/Parser.cpp
  src/Table.cpp
  src/Time.cpp
  src/main.cpp 

target_include_directories(computer_club_lib PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)


if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(${PROJECT_NAME} PUBLIC -O3)
endif()
