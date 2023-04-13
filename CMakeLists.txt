cmake_minimum_required(VERSION 3.16..3.26)

project(
  RaycasterTest
  VERSION 0.1
  DESCRIPTION "Raycaster test"
  LANGUAGES C
)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set(CMAKE_C_EXTENSIONS OFF)
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)
  include(CTest)
endif()

add_subdirectory(src)
add_subdirectory(apps)
