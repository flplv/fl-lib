#include "catch.hpp"
#include <fl-lib.hpp>
#include <climits>

TEST_CASE("fl_ring_fifo: initialization size")
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

    sz = fl_ring_fifo_init(&cut, 7);
    CHECK (4 == sz);

    sz = fl_ring_fifo_init(&cut, 8);
    CHECK (8 == sz);

    sz = fl_ring_fifo_init(&cut, 9);
    CHECK (8 == sz);

    sz = fl_ring_fifo_init(&cut, 15);
    CHECK (8 == sz);

    sz = fl_ring_fifo_init(&cut, 16);
    CHECK (16 == sz);

    sz = fl_ring_fifo_init(&cut, 17);
    CHECK (16 == sz);

    sz = fl_ring_fifo_init(&cut, 31);
    CHECK (16 == sz);

    sz = fl_ring_fifo_init(&cut, 32);
    CHECK (32 == sz);

    sz = fl_ring_fifo_init(&cut, 255);
    CHECK (128 == sz);

    sz = fl_ring_fifo_init(&cut, 256);
    CHECK (256 == sz);

    sz = fl_ring_fifo_init(&cut, 0x200);
    CHECK (0x200 == sz);

    sz = fl_ring_fifo_init(&cut, 0x400);
    CHECK (0x400 == sz);

    sz = fl_ring_fifo_init(&cut, 0x800);
    CHECK (0x800 == sz);

    sz = fl_ring_fifo_init(&cut, 0x80000000);
    CHECK (0x80000000 == sz);
}

TEST_CASE ("fl_ring_fifo: read and write")
{
    fl_ring_fifo_t cut;

    char reference_buffer[] = "abcdefghijklmn";
    char buffer[16];

    int sz = fl_ring_fifo_init(&cut, 16);
    CHECK (16 == sz);

    int i = 0;

    while (!fl_ring_fifo_full(&cut))
    {
        buffer[fl_ring_fifo_write_index(&cut)] = reference_buffer[i++];
        fl_ring_fifo_write_increment(&cut);
    }

    CHECK_THAT( reference_buffer, Catch::Matchers::Equals( buffer ) );
    CHECK (15 == fl_ring_fifo_count(&cut));


    i = 0;

    while(!fl_ring_fifo_empty(&cut))
    {
        int n = i++;
        CHECK (reference_buffer[n] == buffer[fl_ring_fifo_read_index(&cut)]);
        fl_ring_fifo_read_increment(&cut);
    }

    int n = 3;
    while (n--)
    {
        i = 0;

        while (!fl_ring_fifo_full(&cut))
        {
            buffer[fl_ring_fifo_write_index(&cut)] = reference_buffer[i++];
            fl_ring_fifo_write_increment(&cut);
        }

        CHECK (15 == fl_ring_fifo_count(&cut));

        i = 0;

        while(!fl_ring_fifo_empty(&cut))
        {
            int m = i++;
            CHECK (reference_buffer[m] == buffer[fl_ring_fifo_read_index(&cut)]);
            fl_ring_fifo_read_increment(&cut);
        }
    }
}


TEST_CASE ("fl_ring_fifo: buffer boundaries")
{
    fl_ring_fifo_t cut;
    int buffer[1026];
    buffer[0] = INT_MAX;
    buffer[1025] = INT_MAX;
    int * const buf = buffer + 1;
    int c;

    int sz = fl_ring_fifo_init(&cut, 1024);
    CHECK (1024 == sz);

    c = 0;
    while (!fl_ring_fifo_full(&cut))
    {
        buf[fl_ring_fifo_write_index(&cut)] = c++;
        fl_ring_fifo_write_increment(&cut);
    }

    CHECK (INT_MAX == buffer[0]);
    CHECK (INT_MAX == buffer[1025]);
}
