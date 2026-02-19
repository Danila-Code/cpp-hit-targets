find_package(GTest REQUIRED)

add_executable(tests src/tests.cpp)

target_link_libraries(tests PRIVATE
    GTest::gtest
    GTest::gtest_main
    quad_tree_lib
)

add_test(NAME test_quad_tree COMMAND test_quad_tree)
