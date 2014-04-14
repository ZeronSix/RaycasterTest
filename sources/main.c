#include <stdio.h>
#include <stdbool.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "raycaster.h"
#include "linalg.h"

static const int windowWidth = 1280;
static const int windowHeight = 720;
static const int contextVersionMajor = 3;
static const int contextVersionMinor = 3;
static const float movementSpeed = 5.0f;

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

	glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );

	return true;
}

void FrameLoop( float deltaTime ) {
	// draw fps counter
	char buffer[128];
	sprintf( buffer, "Raycasting @ fps: %.2f", 1.0f / deltaTime );
	glfwSetWindowTitle( window, buffer );

	// update input
	if ( glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS ) {
		glfwSetWindowShouldClose( window, true );
	}

	Vec2 movement = { { 0, 0 } };
	if ( glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS ) {
		movement.x += movementSpeed * deltaTime;
	} else if ( glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS ) {
		movement.x -= movementSpeed * deltaTime;
	}

	if ( glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS ) {
		movement.y += movementSpeed * deltaTime;
	} else if ( glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS ) {
		movement.y -= movementSpeed * deltaTime;
	}
	movement = RotateVec2( movement, ( cameraRot.x - 90 ) * DEG2RAD );
	cameraPos = AddVec2( cameraPos, movement );

	static double oldMouseX;
	double mouseX;
	glfwGetCursorPos( window, &mouseX, NULL );
	printf( "%f", mouseX );
	cameraRot.x -= (float)( mouseX - oldMouseX ) * movementSpeed * deltaTime;
	oldMouseX = mouseX;

	glClear( GL_COLOR_BUFFER_BIT );
       
	RenderRaycaster();

	glfwSwapBuffers( window );
	glfwPollEvents();
}

void ShutdownGL() {
	ShutdownRaycaster();
	glfwTerminate();
}