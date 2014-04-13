#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include "raycaster.h"
#include "helperfuncs.h"

#define mapWidth 24
#define mapHeight 24

Vec2 cameraPos = { { 22.0f, 12.0f } };
Vec2 cameraRot = { { 0.0f, 0.0f } };
float fieldOfView = 60.0f;

static const Vec2 startCameraDir = { { -1.0f, 0.0f } };
static Vec2 cameraDir = { { -1.0f, 0.0f } };
static Vec2 startProjectionPlane;
static Vec2 projectionPlane;

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
static int screenWidth;
static int screenHeight;

static void UpdateScreenBuffer();

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

bool InitRaycaster( int w, int h ) {
	screenWidth = w;
	screenHeight = h;
	startProjectionPlane = NewVec2( 0, 1 * tanf( fieldOfView / 2 * DEG2RAD ) );
	projectionPlane = startProjectionPlane;
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
	UpdateScreenBuffer();

	return true;
}

static void DrawVerticalLine( int x, int drawStart, int drawEnd, Vec3 color ) {
	for ( int i = drawStart; i < drawEnd; i++ ) {
		int index = x + screenWidth * ( i - 1 );
		screen.buffer[ index * 3 + 0 ] = color.x;
		screen.buffer[ index * 3 + 1 ] = color.y;
		screen.buffer[ index * 3 + 2 ] = color.z;
	}
}

void UpdateScreenBuffer() {
	for ( int x = 0; x < screenWidth; x++ ) {
		float cameraX = 2 * x / (float)screenWidth - 1;
		Vec2 rayPos = cameraPos;
		Vec2 rayDir = AddVec2( cameraDir, MulVec2f( projectionPlane, cameraX ) );
		Vec2 mapPos = { { (int)rayPos.x, (int)rayPos.y } };

		Vec2 sideDist = { { 0.0f, 0.0f } };
		Vec2 deltaDist = { { 
			sqrtf( 1 + ( rayDir.y * rayDir.y ) / ( rayDir.x * rayDir.x ) ),
			sqrtf( 1 + ( rayDir.x * rayDir.x ) / ( rayDir.y * rayDir.y ) ),
			}
		};
		float perpWallDist;

		Vec2 step;

		bool hit = false;
		int side;
		if ( rayDir.x < 0 ) {
			step.x = -1;
			sideDist.x = ( rayPos.x - mapPos.x ) * deltaDist.x;
		} else {
			step.x = 1;
			sideDist.x = ( mapPos.x + 1.0f - rayPos.x ) * deltaDist.x;
		}

		if ( rayDir.y < 0 ) {
			step.y = -1;
			sideDist.y = ( rayPos.y - mapPos.y ) * deltaDist.y;
		} else {
			step.y = 1;
			sideDist.y = ( mapPos.y + 1.0f - rayPos.y ) * deltaDist.y;
		}

		while ( !hit ) {
			if ( sideDist.x < sideDist.y ) {
				sideDist.x += deltaDist.x;
				mapPos.x += step.x;
				side = 0;
			} else {
				sideDist.y += deltaDist.y;
				mapPos.y += step.y;
				side = 1;
			}

			if ( mapPos.x > mapWidth || mapPos.x < 0 || mapPos.y > mapHeight || mapPos.y < 0 ) {
				break;
			}

			if ( worldMap[(int)mapPos.x][(int)mapPos.y] > 0 ) {
				hit = true;
			}
		}

		if ( hit ) {
			if ( side == 0 ) {
				perpWallDist = fabs( ( mapPos.x - rayPos.x + ( 1 - step.x ) / 2 ) / rayDir.x );
			} else {
				perpWallDist = fabs( ( mapPos.y - rayPos.y + ( 1 - step.y ) / 2 ) / rayDir.y );
			}

			int lineHeight = abs( (int)( screenWidth / 2 / tanf( fieldOfView / 2 * DEG2RAD ) / perpWallDist ) );
			int drawStart = -lineHeight / 2 + screenHeight / 2;
	      	if( drawStart <= 0 ) { 
	      		drawStart = 1;
	      	}
	      	int drawEnd = lineHeight / 2 + screenHeight / 2;
	      	if ( drawEnd >= screenHeight ) { 
	      		drawEnd = screenHeight - 1;
	      	}

	      	Vec3 color;
	      	switch ( worldMap[(int)mapPos.x][(int)mapPos.y] ) {
	      		case 1: color = NewVec3( 255, 0, 0 ); break;
	      		case 2: color = NewVec3( 0, 255, 0 ); break;
	      		case 3: color = NewVec3( 0, 0, 255 ); break;
	      		case 4: color = NewVec3( 255, 255, 255 ); break;
	      		default: color = NewVec3( 255, 255, 0 ); break;
	      	}

	      	if ( side == 1 ) {
	      		color = MulVec3f( color, 0.5f );
	      	}
	      	DrawVerticalLine( x, drawStart, drawEnd, color );
      	}
	}

	 //both camera direction and camera plane must be rotated
    cameraDir = RotateVec2( startCameraDir, cameraRot.x * DEG2RAD );
    projectionPlane = RotateVec2( startProjectionPlane, cameraRot.x * DEG2RAD );

	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, screen.buffer );
}

void RenderRaycaster() {
	for ( int i = 0; i < screenWidth * screenHeight * 3; i++ ) {
		screen.buffer[i] = 0;
	}
	UpdateScreenBuffer();
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