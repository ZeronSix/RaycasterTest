#ifndef LINALG_H
#define LINALG_H
#include <math.h>

#define DEG2RAD 3.14159265359 / 180.0f

typedef union Vec2 {
	float m[2];
	struct {
		float x;
		float y;
	};
} Vec2;

Vec2 NewVec2( float x, float y );
Vec2 AddVec2( Vec2 a, Vec2 b );
Vec2 MulVec2f( Vec2 a, float b );
Vec2 RotateVec2( Vec2 vec, float angle );

typedef union Vec3 {
	float m[3];
	struct {
		float x;
		float y;
		float z;
	};
} Vec3;

Vec3 NewVec3( float x, float y, float z );
Vec3 MulVec3f( Vec3 a, float b );

#endif