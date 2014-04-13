#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include "raycaster.h"
#include "helperfuncs.h"

#define mapWidth 24
#define mapHeight 24
int worldMap[mapWidth][mapHeight]= {
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

static Screen screen;
static float screenWidth;
static float screenHeight;
static const float fov = 60.0f;

static GLuint CreateShader( const char* vertSource, const char* fragSource ) {
	GLuint program = glCreateProgram();
	GLint status;

	GLuint vertShader = glCreateShader( GL_VERTEX_SHADER );
	glShaderSource( vertShader, 1, &vertSource, NULL );
	glCompileShader( vertShader );

	glGetShaderiv( vertShader, GL_COMPILE_STATUS, &status );
	if ( status == GL_FALSE ) {
		char buffer[1024];
		glGetShaderInfoLog( vertShader, sizeof( buffer ), 0, buffer );
		fprintf( stderr, "Vertex shader compilation error: %s\n", buffer );
		glDeleteShader( vertShader );
		glDeleteProgram( program );
		return 0;
	}

	GLuint fragShader = glCreateShader( GL_FRAGMENT_SHADER );
	glShaderSource( fragShader, 1, &fragSource, NULL );
	glCompileShader( fragShader );

	glGetShaderiv( fragShader, GL_COMPILE_STATUS, &status );
	if ( status == GL_FALSE ) {
		char buffer[1024];
		glGetShaderInfoLog( fragShader, sizeof( buffer ), 0, buffer );
		fprintf( stderr, "Fragment shader compilation error: %s\n", buffer );
		glDeleteShader( fragShader );
		glDeleteProgram( program );
		return 0;
	}

	glAttachShader( program, vertShader );
	glAttachShader( program, fragShader );
	glDeleteShader( vertShader ); // we don't need shaders after attaching them
	glDeleteShader( fragShader );

	glLinkProgram( program );
	glGetShaderiv( program, GL_LINK_STATUS, &status );
	if ( status == GL_FALSE ) {
		char buffer[1024];
		glGetProgramInfoLog( program, sizeof( buffer ), 0, buffer );
		fprintf( stderr, "Shader linking error: %s\n", buffer );
		glDeleteProgram( program );
		return 0;
	}

	return program;
}

bool InitRaycaster( float w, float h ) {
	screenWidth = w;
	screenHeight = h;

	GLfloat vertices[] = {
   		 //  Position Texcoords
        -1.0f,  1.0f, 0.0f, 0.0f, // Top-left
         1.0f,  1.0f, 1.0f, 0.0f, // Top-right
         1.0f, -1.0f, 1.0f, 1.0f, // Bottom-right
        -1.0f, -1.0f, 0.0f, 1.0f  // Bottom-left
    };
    GLuint indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    glGenVertexArrays( 1, &screen.vao );
    glBindVertexArray( screen.vao );
    glGenBuffers( 1, &screen.vbo );
    glBindBuffer( GL_ARRAY_BUFFER, screen.vbo );
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );
    glGenBuffers( 1, &screen.ebo ); 
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, screen.ebo );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices ), indices, GL_STATIC_DRAW );

    screen.shader = CreateShader( LoadStringFromFile( "..\\assets\\shaders\\screen.vs" ), LoadStringFromFile( "..\\assets\\shaders\\screen.fs" ) );
	if ( !screen.shader ) {
		return false;
	}
	glUseProgram( screen.shader );

	GLint posAttr = glGetAttribLocation( screen.shader, "vert" );
    glEnableVertexAttribArray( posAttr );
    glVertexAttribPointer( posAttr, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0 );

    GLint uvAttr = glGetAttribLocation( screen.shader, "uv" );
    glEnableVertexAttribArray( uvAttr );
    glVertexAttribPointer( uvAttr, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)( 2 * sizeof(GLfloat) ) );

	glGenTextures( 1, &screen.texture );
	glBindTexture( GL_TEXTURE_2D, screen.texture );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	screen.buffer = malloc( sizeof( ubyte_t ) * screenWidth * screenHeight * 3 );

	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, screen.buffer );
	return true;
}

static void UpdateScreenBuffer() {
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, screen.buffer );
}

void RenderRaycaster() {
	glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );
}

void ShutdownRaycaster() {
	glBindTexture( GL_TEXTURE_2D, 0 );
	glBindVertexArray( 0 );
	glUseProgram( 0 );

	glDeleteTextures( 1, &screen.texture );
	glDeleteBuffers( 1, &screen.vbo );
	glDeleteVertexArrays( 1, &screen.vao );
	glDeleteProgram( screen.shader );
}