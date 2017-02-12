#include "catch.hpp"
#include <fl-lib.hpp>

#define CHECK_EQUAL(___a,___b) CHECK(___a == ___b)

struct test_t
{
    uint8_t b1;
    fl_linked_list_t h;
    uint8_t b2;
};

static bool compare (test_t * element, test_t * seed)
{
    return element == seed;
}

static void freefunc (test_t * element)
{
    element->b1 = 0xF1;
}

TEST_CASE ("fl_linked_list api only")
{
    test_t * root;
    test_t cut;

    root = NULL;
    cut = {0xAA, {0, 0}, 0x55};

    SECTION ("initialization")
    {
        fl_linked_list_init(&cut, h);
        CHECK_EQUAL(0, cut.h.next);
        CHECK_EQUAL(0, cut.h.prev);
    }

    SECTION ("tests with multiple entries using insert_after")
    {
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

        SECTION ("test insertions and end of list")
        {
            test_t * p = root;
            for (int i = 0; i<10; i++)
            {
                p = fl_linked_list_next(p, h);
                CHECK_EQUAL(&cuts[i], p);
            }

            p = fl_linked_list_next(p, h);
            CHECK_EQUAL(0, p);
        }

        SECTION ("remove one from the middle")
        {
            root = fl_linked_list_remove(&cuts[4], h);
            test_t * p = root;
            for (int i = 0; i<10; i++)
            {
                if (i == 4) continue;
                p = fl_linked_list_next(p, h);
                CHECK_EQUAL(&cuts[i], p);
            }

            p = fl_linked_list_next(p, h);
            CHECK_EQUAL(0, p);
        }

        SECTION ("remove the first")
        {
            /* Root relocation */
            root = fl_linked_list_remove(root, h);
            CHECK_EQUAL(root, &cuts[0]);

            test_t * p = &cuts[9];
            for (int i = 9; i>=0; i--)
            {
                CHECK_EQUAL(&cuts[i], p);
                p = fl_linked_list_previous(p, h);
            }

            CHECK_EQUAL(0, p);
        }

        SECTION ("traverse reverse")
        {
            test_t * p = &cuts[9];
            for (int i = 9; i>=0; i--)
            {
                CHECK_EQUAL(&cuts[i], p);
                p = fl_linked_list_previous(p, h);
            }
            CHECK_EQUAL(root, p);
        }

        SECTION ("find")
        {
            CHECK_EQUAL(&cuts[9], fl_linked_list_find(root, h, compare, &cuts[9]));
        }

        SECTION ("free")
        {
            fl_linked_list_free(root, h, freefunc);

            for (int i = 0; i<10; i++)
            {
                CHECK_EQUAL(0xF1, cuts[i].b1);
                cuts[i].b1 = 0xAA;
            }

            CHECK_EQUAL(0xF1, root->b1);
            root->b1 = 0xAA;
        }
    }

    SECTION ("tests with multiple entries using insert_before for reverse order")
    {
        test_t cuts[10];
        root = &cut;

        for (int i = 0; i<10; i++)
            fl_linked_list_init(&cuts[i], h);
        fl_linked_list_init(root, h);

        CHECK_EQUAL(root, fl_linked_list_last(root, h));

        for (int i = 0; i<10; i++)
            fl_linked_list_insert_before(
                    fl_linked_list_first(root, h),
                    &cuts[i],
                    h);

        /* root is the last node */
        CHECK_EQUAL(11, fl_linked_list_count(root, h));

        SECTION ("list in reverse order")
        {
            test_t * p = fl_linked_list_first(root, h);
            for (int i = 9; i>=0; i--)
            {
                CHECK_EQUAL(&cuts[i], p);
                p = fl_linked_list_next(p, h);
            }

            CHECK_EQUAL(root, p);
        }
    }

    SECTION ("remove the only one")
    {
        root = &cut;

        fl_linked_list_init(root, h);
        CHECK_EQUAL(root, fl_linked_list_last(root, h));

        root = fl_linked_list_remove(root, h);
        CHECK_EQUAL(0, root);
    }


    SECTION ("remove not inserted")
    {
        test_t not_inserted;
        fl_linked_list_init(&not_inserted, h);
        CHECK_EQUAL(0, fl_linked_list_remove(&not_inserted, h));
    }

    CHECK_EQUAL(0xAA, cut.b1);
    CHECK_EQUAL(0x55, cut.b2);
}

TEST_CASE ("fl_linked_list internals")
{
    struct s {
        fl_linked_list_t ll;
    } d0, d1, d2, d3, d4, * r;

    fl_linked_list_init(&d0, ll);
    fl_linked_list_init(&d1, ll);
    fl_linked_list_init(&d2, ll);
    fl_linked_list_init(&d3, ll);
    fl_linked_list_init(&d4, ll);

    r = &d0;
    CHECK_EQUAL(&d0, r);

    SECTION ("check next and prev pointers after some insertions")
    {
        fl_linked_list_insert_after(r, &d4, ll);

        CHECK_EQUAL(&d4.ll, d0.ll.next);
        CHECK_EQUAL(0, d0.ll.prev);

        CHECK_EQUAL(0, d4.ll.next);
        CHECK_EQUAL(&d0.ll, d4.ll.prev);

        fl_linked_list_insert_before(&d4, &d1, ll);

        CHECK_EQUAL(&d4.ll, d1.ll.next);
        CHECK_EQUAL(&d0.ll, d1.ll.prev);

        CHECK_EQUAL(&d1.ll, d0.ll.next);
        CHECK_EQUAL(&d1.ll, d4.ll.prev);

        fl_linked_list_insert_before(&d4, &d2, ll);

        CHECK_EQUAL(&d4.ll, d2.ll.next);
        CHECK_EQUAL(&d1.ll, d2.ll.prev);

        CHECK_EQUAL(&d2.ll, d1.ll.next);
        CHECK_EQUAL(&d2.ll, d4.ll.prev);

        fl_linked_list_insert_after(&d2, &d3, ll);

        CHECK_EQUAL(&d4.ll, d3.ll.next);
        CHECK_EQUAL(&d2.ll, d3.ll.prev);

        CHECK_EQUAL(&d3.ll, d2.ll.next);
        CHECK_EQUAL(&d3.ll, d4.ll.prev);

        SECTION ("now remove and re check")
        {
            fl_linked_list_remove(&d2, ll);

            CHECK_EQUAL(0, d2.ll.next);
            CHECK_EQUAL(0, d2.ll.prev);

            CHECK_EQUAL(&d3.ll, d1.ll.next);
            CHECK_EQUAL(&d1.ll, d3.ll.prev);

            CHECK_EQUAL(&d0.ll, d1.ll.prev);
            CHECK_EQUAL(&d4.ll, d3.ll.next);
        }
    }
}
