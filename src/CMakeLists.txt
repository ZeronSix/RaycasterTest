set(HEADER_LIST "${RaycasterTest_SOURCE_DIR}/include/raycaster_engine/raycaster_engine.h")

add_library(raycaster_engine raycaster_engine.c ${HEADER_LIST})
target_include_directories(raycaster_engine PUBLIC ../include)
target_compile_features(raycaster_engine PUBLIC cxx_std_11)

source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "Header Files"
  FILES ${HEADER_LIST})
