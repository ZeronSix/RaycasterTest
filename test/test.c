#include <ptest/ptest.h>

void suite_cgmath(void);

int main(int argc, char** argv) {
    (void)argc;
    (void)argv;

    pt_add_suite(suite_cgmath);

    return pt_run();
}
