add_executable(test_boosh test.c test_cgmath.c)
target_compile_features(test_boosh PRIVATE c_std_17)
target_link_libraries(test_boosh PRIVATE boosh ptest::ptest)

add_test(NAME test_boosh COMMAND test_boosh)
