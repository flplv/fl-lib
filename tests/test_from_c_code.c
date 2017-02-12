#include <fl-lib.h>
#include <string.h>
#include <stdarg.h>

#define CHECK(__condition) checkfunc (__condition, #__condition)
#define CHECK_EQUAL(__expected, __actual) CHECK(__expected==__actual)

void test_auto_type (void (*checkfunc)(bool, const char *))
{
    const char * t;
    fl_auto_type(t) s = "blablabla";
    CHECK(strcmp(s, "blablabla") == 0);
}

void test_container_of (void (*checkfunc)(bool, const char *))
{
    struct s_container {
        int a, b, c;
        char d;
        char t;
        int z;
    } container;

    CHECK(&container == fl_container_of (&container.t, struct s_container, t));
}

void test_array_size (void (*checkfunc)(bool, const char *))
{
    const char s[] = "blablabla";
    CHECK(FL_ARRAY_SIZE(s) == 10);
}

void test_linked_list (void (*checkfunc)(bool, const char *))
{
    struct test_t
    {
        uint8_t b1;
        fl_linked_list_t h;
        uint8_t b2;
    };
    typedef struct test_t test_t;

    test_t * root = NULL;
    test_t cut = {0xAA, {0, 0}, 0x55};

    fl_linked_list_init(&cut, h);
    CHECK_EQUAL(0, cut.h.next);
    CHECK_EQUAL(0, cut.h.prev);

    test_t cuts[10];
    root = &cut;

    for (int i = 0; i<10; i++)
        fl_linked_list_init(&cuts[i], h);
    fl_linked_list_init(root, h);

    CHECK_EQUAL(root, fl_linked_list_last(root, h));

    for (int i = 0; i<10; i++)
        fl_linked_list_insert_after(
                fl_linked_list_last(root, h),
                &cuts[i],
                h);

    CHECK_EQUAL(11, fl_linked_list_count(root, h));
}

void test_ring_fifo (void (*checkfunc)(bool, const char *))
{
    fl_ring_fifo_t cut;
    int sz;

    sz = fl_ring_fifo_init(&cut, 0);
    CHECK (-1 == sz);

    sz = fl_ring_fifo_init(&cut, 1);
    CHECK (-1 == sz);

    sz = fl_ring_fifo_init(&cut, 2);
    CHECK (2 == sz);

    sz = fl_ring_fifo_init(&cut, 3);
    CHECK (2 == sz);

    sz = fl_ring_fifo_init(&cut, 4);
    CHECK (4 == sz);
}
