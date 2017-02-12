#include "catch.hpp"

extern "C" {

    void check (bool assert, const char * error) {
        if (!assert) FAIL(error);
    }

    void test_auto_type (void (*checkfunc)(bool, const char *));
    void test_container_of (void (*checkfunc)(bool, const char *));
    void test_array_size (void (*checkfunc)(bool, const char *));
    void test_linked_list (void (*checkfunc)(bool, const char *));
    void test_ring_fifo (void (*checkfunc)(bool, const char *));
}

#define A_C_TEST(func_name) SECTION(#func_name) {func_name(check);}

TEST_CASE ("c_code: C Wrapper test group")
{
    A_C_TEST(test_auto_type);
    A_C_TEST(test_container_of);
    A_C_TEST(test_array_size);
    A_C_TEST(test_linked_list);
    A_C_TEST(test_ring_fifo);
}
