cmake_minimum_required(VERSION 3.16..3.26)

project(
  Boosh
  VERSION 0.1
  DESCRIPTION "Boosh"
  LANGUAGES C
)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set(CMAKE_C_EXTENSIONS OFF)
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)
  include(CTest)
endif()

add_subdirectory(extern)
add_subdirectory(src)
add_subdirectory(apps)

if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR BOOSH_CMAKE_BUILD_TESTING) AND BUILD_TESTING)
  add_subdirectory(test)
endif()
