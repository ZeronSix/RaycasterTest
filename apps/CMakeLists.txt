add_executable(boosh_test boosh_test.c)
target_compile_features(boosh_test PRIVATE c_std_17)
target_link_libraries(boosh_test PRIVATE boosh)
