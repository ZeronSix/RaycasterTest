#ifndef RAYCASTER_H
#define RAYCASTER_H
#include <stdbool.h>

typedef unsigned char ubyte_t;

typedef struct Screen {
	GLuint shader;
	GLuint texture;
	GLuint vao;
	GLuint vbo;
	GLuint ebo;

	ubyte_t* buffer;
} Screen;

bool InitRaycaster( float w, float h );
void RenderRaycaster();
void ShutdownRaycaster();

#endif