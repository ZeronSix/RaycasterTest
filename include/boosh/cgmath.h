#ifndef BOOSH_CGMATH_H_
#define BOOSH_CGMATH_H_

#include <stdbool.h>

typedef union {
    float data[2];
    struct {
        float x;
        float y;
    };
} boosh_vec2_t;

boosh_vec2_t boosh_vec2_zero();
boosh_vec2_t boosh_vec2_new(float x, float y);
bool boosh_vec2_eq_approx(boosh_vec2_t lhs, boosh_vec2_t rhs);
boosh_vec2_t boosh_vec2_add(boosh_vec2_t lhs, boosh_vec2_t rhs);
boosh_vec2_t boosh_vec2_mul(boosh_vec2_t vec, float scalar);

#endif
