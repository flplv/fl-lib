#include "catch.hpp"
#include <fl-lib.hpp>
#include <string.h>

TEST_CASE ("macros")
{

    SECTION ("auto_type")
    {
        const char * t;
        fl_auto_type(t) s = "blablabla";
        CHECK(strcmp(s, "blablabla") == 0);
    }

    SECTION ("container_of")
    {
        struct s_container {
            int a, b, c;
            char d;
            char t;
            int z;
        } container;

        CHECK(&container == fl_container_of (&container.t, s_container, t));
    }

    SECTION ("array_size")
    {
        const char s[] = "blablabla";
        CHECK(FL_ARRAY_SIZE(s) == 10);
    }
}
