#include "catch.hpp"
#include "fff.hpp"
#include <fl-lib.hpp>
#include <climits>

DECLARE_FAKE_VOID_FUNC (freefunc, int);
DEFINE_FAKE_VOID_FUNC (freefunc, int);

static bool compare (int element, int seed)
{
    return element == seed;
}

TEST_CASE("fl_array")
{
    RESET_FAKE (freefunc);

    fl_array_t cut;
    int buffer[5 + 1];

    buffer[5] = 0;
    fl_array_init(&cut, 5);

    SECTION("Initialization")
    {
        CHECK (5 == cut.max);
        CHECK (0 == cut.top);
    }

    SECTION("Insert remove free and find")
    {
        fl_array_insert(&cut, buffer, 10);
        CHECK (10 == buffer[0]);
        fl_array_insert(&cut, buffer, 11);
        CHECK (11 == buffer[1]);
        fl_array_insert(&cut, buffer, 12);
        CHECK (12 == buffer[2]);
        fl_array_insert(&cut, buffer, 13);
        CHECK (13 == buffer[3]);

        CHECK (4 == cut.top);
        CHECK (false == fl_array_full(&cut));

        fl_array_insert(&cut, buffer, 14);
        CHECK (14 == buffer[4]);
        CHECK (cut.max == cut.top);
        CHECK (true == fl_array_full(&cut));

        /* Extra insert should replace last */
        fl_array_insert(&cut, buffer, 14);
        CHECK (14 == buffer[4]);
        CHECK (0 == buffer[5]);

        /* Now we remove one from the middle */
        fl_array_remove(&cut, buffer, 3);
        CHECK (10 == buffer[0]);
        CHECK (11 == buffer[1]);
        CHECK (12 == buffer[2]);
        CHECK (14 == buffer[3]);
        CHECK (0 == buffer[5]);
        CHECK (4 == cut.top);

        /* And then we remove an invalid position, nothing should happen */
        fl_array_remove(&cut, buffer, 312);
        CHECK (10 == buffer[0]);
        CHECK (11 == buffer[1]);
        CHECK (12 == buffer[2]);
        CHECK (14 == buffer[3]);
        CHECK (0 == buffer[5]);
        CHECK (4 == cut.top);

        /* Now we remove another with freefunc */
        fl_array_free(&cut, buffer, 1, freefunc);

        CHECK (1 == freefunc_fake.call_count);

        CHECK (10 == buffer[0]);
        CHECK (12 == buffer[1]);
        CHECK (14 == buffer[2]);
        CHECK (0 == buffer[5]);
        CHECK (3 == cut.top);


        /* Lets do a little of searching */
        int pos = fl_array_find(&cut, buffer, compare, 14);
        CHECK (2 == pos);
        pos = fl_array_find(&cut, buffer, compare, 23141241);
        CHECK (-1 == pos);

        /* bye bye array! */
        fl_array_clear(&cut);
        CHECK (5 == cut.max);
        CHECK (0 == cut.top);
    }

    fl_array_deinit(&cut);
};
