#include "linalg.h"

Vec2 NewVec2( float x, float y ) {
	Vec2 v = { { x, y } };
	return v;
}

Vec2 AddVec2( Vec2 a, Vec2 b ) {
	Vec2 v = { { a.x + b.x, a.y + b.y } };
	return v;
}

Vec2 MulVec2f( Vec2 a, float b ) {
	Vec2 v = { { a.x * b, a.y * b } };
	return v;
}

Vec2 RotateVec2( Vec2 vec, float angle ) {
	Vec2 v;
	v.x = vec.x * cos( angle ) - vec.y * sin( angle );
	v.y = vec.x * sin( angle ) + vec.y * cos( angle );
	return v;
}

Vec3 NewVec3( float x, float y, float z ) {
	Vec3 v = { { x, y, z } };
	return v;
}

Vec3 MulVec3f( Vec3 a, float b ) {
	Vec3 v = { { a.x * b, a.y * b, a.z * b } };
	return v;
}