#include "catch.hpp"

#include <stdint.h>
#define FL_ENABLE_TIME_MODULE
#define FL_TIME_TYPE uint32_t
#define FL_TIME_INT_TYPE int64_t /* only used for remaining function */
#include <fl-lib.hpp>

TEST_CASE("time")
{
    const fl_time_t max = (fl_time_t)-1;

    SECTION ("elapsed")
    {

        /* Comparator margin */
        CHECK_FALSE(fl_time_check_elapsed(10, 0, 11));
        CHECK (fl_time_check_elapsed(10, 0, 10));

        /* Considering timer wrap */
        CHECK_FALSE(fl_time_check_elapsed(max - 1, max - 1, 3));
        CHECK_FALSE(fl_time_check_elapsed(max, max - 1, 3));
        CHECK_FALSE(fl_time_check_elapsed(max + 1, max - 1, 3));
        CHECK (fl_time_check_elapsed(max + 2, max - 1, 3));
        CHECK (fl_time_check_elapsed(max + 3, max - 1, 3));
        CHECK (fl_time_check_elapsed(max + 4, max - 1, 3));
        CHECK (fl_time_check_elapsed(max + 5, max - 1, 3));
        CHECK (fl_time_check_elapsed(max + 6, max - 1, 3));

        CHECK_FALSE(fl_time_check_elapsed(max - 1, max - 1, 1));
        CHECK (fl_time_check_elapsed(max, max - 1, 1));
        CHECK (fl_time_check_elapsed(max + 1, max - 1, 1));
        CHECK (fl_time_check_elapsed(max + 2, max - 1, 1));
        CHECK (fl_time_check_elapsed(max + 3, max - 1, 1));
        CHECK (fl_time_check_elapsed(max + 4, max - 1, 1));
        CHECK (fl_time_check_elapsed(max + 5, max - 1, 1));
        CHECK (fl_time_check_elapsed(max + 6, max - 1, 1));

        CHECK_FALSE(fl_time_check_elapsed(max - 1, max + 1, max));
        CHECK (fl_time_check_elapsed(max, max + 1, max));

        CHECK_FALSE(fl_time_check_elapsed(0xE, 0x10, max));
        CHECK (fl_time_check_elapsed(0xF, 0x10, max));

        /* Tests pass because the subtraction made in check_elapsed
         * removes the wrap effect.
         * It only works if "now" is bigger than "before", considering
         * we are dealing with time, now is bigger than before, always.
         */

        /* Elapsed 0 always return true! */
        CHECK (fl_time_check_elapsed(max - 2, max - 1, 0));
        CHECK (fl_time_check_elapsed(max - 1, max - 1, 0));

        /* Ok, what if I wanted to test now against a number in the future?
         * Simple! Transform later in before with a stupid big number and
         *  check against it! */
        fl_time_t stupid_big_number = (max/2) + 1;
        fl_time_t later = 0x00000010;
        fl_time_t before = later - stupid_big_number;

        CHECK_FALSE(fl_time_check_elapsed(max - 255, before, stupid_big_number));
        CHECK_FALSE(fl_time_check_elapsed(max, before, stupid_big_number));
        CHECK_FALSE(fl_time_check_elapsed(max + 1, before, stupid_big_number));
        CHECK_FALSE(fl_time_check_elapsed(0x0F, before, stupid_big_number));
        CHECK (fl_time_check_elapsed(0x10, before, stupid_big_number));
        CHECK (fl_time_check_elapsed(0x11, before, stupid_big_number));
    }

    SECTION ("reached")
    {
        /* As we are good programmers, we wrapped the algorithm in a new function */
        CHECK_FALSE(fl_time_check_reached(0x10, max - 255));
        CHECK_FALSE(fl_time_check_reached(0x10, max));
        CHECK_FALSE(fl_time_check_reached(0x10, max + 1));
        CHECK_FALSE(fl_time_check_reached(0x10, 0x0F));
        CHECK (fl_time_check_reached(0x10, 0x10));
        CHECK (fl_time_check_reached(0x10, 0x11));

        /* Wrap around whooooooooooooooooooooooooooooooo? */
    }

    SECTION ("remaining")
    {
        CHECK (fl_time_remaining(0, 0, 10) == 10);
        CHECK (fl_time_remaining(8, 0, 10) == 2);
        CHECK (fl_time_remaining(9, 0, 10) == 1);
        CHECK (fl_time_remaining(10, 0, 10) == 0);
        CHECK (fl_time_remaining(11, 0, 10) < 0);
    }
}
