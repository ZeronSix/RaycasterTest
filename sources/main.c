#include <stdio.h>
#include <stdbool.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "raycaster.h"

static const int windowWidth = 960;
static const int windowHeight = 540;
static const int contextVersionMajor = 3;
static const int contextVersionMinor = 3;

static GLFWwindow* window;

bool InitGL();
void FrameLoop( float dt );
void ShutdownGL();

int main() {
	if ( !InitGL() ) {
		return 1;
	}
	if ( !InitRaycaster( windowWidth, windowHeight) ) {
		return 1;
	}

	float lastTime = glfwGetTime();
	while ( !glfwWindowShouldClose( window ) ) {
		float currentTime = glfwGetTime();
		FrameLoop( currentTime - lastTime );
		lastTime = currentTime;
	}

	ShutdownGL();
	return 0;
}

bool InitGL() {
	if ( !glfwInit() ) {
		fprintf( stderr, "Failed to initialize GLFW!\n" );
		return false;
	}

	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, contextVersionMajor );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, contextVersionMinor );
	glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	window = glfwCreateWindow( windowWidth, windowHeight, "Raycasting Demo", NULL, NULL );
	if ( !window ) {
		fprintf( stderr, "Failed to create a window!" );
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent( window );

	glewExperimental = GL_TRUE;
	if ( glewInit() != GLEW_OK ) {
		fprintf( stderr, "Failed to initialize GLEW!" );
		glfwTerminate();
		return false;
	}

	return true;
}

void FrameLoop( float deltaTime ) {
	// draw fps counter
	char buffer[128];
	sprintf( buffer, "Raycasting @ fps: %.2f", 1.0f / deltaTime );
	glfwSetWindowTitle( window, buffer );

	glClear( GL_COLOR_BUFFER_BIT );
       
	RenderRaycaster();

	glfwSwapBuffers( window );
	glfwPollEvents();
}

void ShutdownGL() {
	ShutdownRaycaster();
	glfwTerminate();
}