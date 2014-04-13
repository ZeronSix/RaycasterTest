cmake_minimum_required( VERSION 2.8 )
project( RaycasterTest )

set_property(GLOBAL PROPERTY USE_FOLDERS ON) 
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin )
set( CMAKE_C_FLAGS "-std=c99 -Wall -m64 -Os -g -L./" )

#--- Third-party libraries ---#
find_package( OpenGL REQUIRED )

include( cmake/FindGLFW3.cmake )
if ( NOT GLFW_FOUND ) 
	message( FATAL_ERROR "GLFW3 has been not found!" )
endif()
include_directories( ${GLFW_INCLUDE_DIRS} )

include( cmake/FindGLEW.cmake )
if ( NOT GLEW_FOUND ) 
	message( FATAL_ERROR "GLEW has been not found!" )
endif()
include_directories( ${GLEW_INCLUDE_DIR} )

add_subdirectory( sources )