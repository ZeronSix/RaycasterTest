#ifndef BOOSH_CGMATH_H_
#define BOOSH_CGMATH_H_

typedef union {
    float data[2];
    struct {
        float x;
        float y;
    };
} boosh_vec2_t;

boosh_vec2_t boosh_vec2_zero();

#endif
