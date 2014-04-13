#ifndef RAYCASTER_H
#define RAYCASTER_H
#include <stdbool.h>
#include "linalg.h"

typedef unsigned char ubyte_t;

typedef struct Screen {
	GLuint shader;
	GLuint texture;
	GLuint vao;
	GLuint vbo;
	GLuint ebo;

	ubyte_t* buffer;
} Screen;

extern Vec2 cameraPos;
extern Vec2 cameraRot;
extern float fieldOfView;

bool InitRaycaster( int w, int h );
void RenderRaycaster();
void ShutdownRaycaster();

#endif