#-----------------------------#
file( GLOB SOURCES *.c *.h )

add_executable( raycastertest ${SOURCES} )
target_link_libraries( raycastertest ${OPENGL_LIBRARY} ${GLEW_LIBRARY} ${GLFW_LIBRARIES})