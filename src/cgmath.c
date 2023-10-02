#include <boosh/cgmath.h>

#include <math.h>

boosh_vec2_t boosh_vec2_zero() {
    boosh_vec2_t vec = {{0.0F, 0.0F}};

    return vec;
}

boosh_vec2_t boosh_vec2_new(float x, float y) {
    boosh_vec2_t vec = {{x, y}};

    return vec;
}

bool boosh_vec2_eq_approx(boosh_vec2_t lhs, boosh_vec2_t rhs) {
    const float EPS = 1e-9;
    return fabsf(lhs.x - rhs.x) < EPS && fabsf(lhs.y - rhs.y) < EPS;
}

boosh_vec2_t boosh_vec2_add(boosh_vec2_t lhs, boosh_vec2_t rhs) {
    return boosh_vec2_new(lhs.x + rhs.x, lhs.y + rhs.y);
}

boosh_vec2_t boosh_vec2_mul(boosh_vec2_t vec, float scalar) {
    return boosh_vec2_new(vec.x * scalar, vec.y * scalar);
}
