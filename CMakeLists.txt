cmake_minimum_required(VERSION 2.9)
project(atm)

file(GLOB atm_src CONFIGURE_DEPENDS "src/*.cc")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-g -O3 -Wall -Wextra -pedantic")

add_executable(atm ${atm_src})
