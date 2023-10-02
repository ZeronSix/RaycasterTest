#include <ptest/ptest.h>

#include <boosh/cgmath.h>

void test_vec2_zero(void) {
    boosh_vec2_t value = boosh_vec2_zero();

    PT_ASSERT(value.x == 0.0F);
    PT_ASSERT(value.y == 0.0F);
}

void suite_cgmath(void) {
    pt_add_test(test_vec2_zero, "Test boosh_vec2_zero", "Suite CGMath");
}
