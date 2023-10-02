cmake_minimum_required(VERSION 3.1...3.26)

project(
  ptest
  VERSION 1.0
  LANGUAGES C
)

add_library(ptest include/ptest/ptest.h src/ptest.c)
target_include_directories(ptest PUBLIC include)
add_library(ptest::ptest ALIAS ptest)
