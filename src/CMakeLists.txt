set(HEADER_LIST "${Boosh_SOURCE_DIR}/include/boosh/boosh.h")

find_package(SDL2 REQUIRED)

add_library(boosh boosh.c ${HEADER_LIST})
target_include_directories(
  boosh
  PUBLIC ../include
  PRIVATE ${SDL2_INCLUDE_DIRS}
)
target_link_libraries(boosh PRIVATE ${SDL2_LIBRARIES})
target_compile_features(boosh PUBLIC cxx_std_11)

source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "Header Files"
  FILES ${HEADER_LIST}
)
