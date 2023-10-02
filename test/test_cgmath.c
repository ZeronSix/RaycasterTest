#include <ptest/ptest.h>

#include <boosh/cgmath.h>

void test_vec2_zero(void) {
    boosh_vec2_t value = boosh_vec2_zero();

    PT_ASSERT(value.x == 0.0F);
    PT_ASSERT(value.y == 0.0F);
}

void test_vec2_new(void) {
    const float x = 1.0;
    const float y = 2.0;

    boosh_vec2_t value = boosh_vec2_new(x, y);

    PT_ASSERT(value.x == x);
    PT_ASSERT(value.y == y);
}

void test_vec2_eq_approx(void) {
    const boosh_vec2_t zero = boosh_vec2_new(0, 0);
    const boosh_vec2_t almost_zero = boosh_vec2_new(1e-10F, 1e-10F);
    const boosh_vec2_t one = boosh_vec2_new(1.0F, 1.0F);

    PT_ASSERT(boosh_vec2_eq_approx(zero, zero));
    PT_ASSERT(boosh_vec2_eq_approx(zero, almost_zero));
    PT_ASSERT(!boosh_vec2_eq_approx(zero, one));
}

void test_vec2_add(void) {
    const boosh_vec2_t first = boosh_vec2_new(1.0F, 2.0F);
    const boosh_vec2_t second = boosh_vec2_new(3.0F, 4.0F);
    const boosh_vec2_t expected_sum = boosh_vec2_new(4.0F, 6.0F);

    boosh_vec2_t sum = boosh_vec2_add(first, second);
    PT_ASSERT(boosh_vec2_eq_approx(sum, expected_sum));
}

void test_vec2_mul(void) {
    const boosh_vec2_t vec = boosh_vec2_new(1.0F, 2.0F);
    const float scalar = 2.0F;
    const boosh_vec2_t expected_product = boosh_vec2_new(2.0F, 4.0F);

    boosh_vec2_t product = boosh_vec2_mul(vec, scalar);
    PT_ASSERT(boosh_vec2_eq_approx(product, expected_product));
}

void suite_cgmath(void) {
    pt_add_test(test_vec2_zero, "test_vec2_zero", "suite_cgmath");
    pt_add_test(test_vec2_new, "test_vec2_new", "suite_cgmath");
    pt_add_test(test_vec2_eq_approx, "test_vec2_eq_approx", "suite_cgmath");
    pt_add_test(test_vec2_add, "test_vec2_add", "suite_cgmath");
    pt_add_test(test_vec2_mul, "test_vec2_mul", "suite_cgmath");
}
