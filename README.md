# fl-lib

**Yet another single header C/CPP authorial library**

Fully GNU C compliant, 100% type checked, tested and safe.

These are the last released single headers:
  - [fl-lib.h](https://github.com/flplv/fl-lib/releases/download/v0.0.0/fl-lib.h) for C code, and/or;
  - [fl-lib.hpp](https://github.com/flplv/fl-lib/releases/download/v0.0.0/fl-lib.hpp) for C++ code:

## How to use it?

Have a look at the tests folder, there you'll find usage examples. Anyways, enjoy this ring FIFO snipped below:

```c
#include <fl-lib.h>

void look_how_beautiful_i_get_with_ring_fifos()
{
    fl_ring_fifo_t my_fifo;
    whatever_type_you_want_t my_fifo_buffer[128];

    fl_ring_fifo_init(&my_fifo, FL_ARRAY_SIZE(my_fifo_buffer));

    while (!fl_ring_fifo_full(&my_fifo))
    {
        my_fifo_buffer[fl_ring_fifo_write_index(&my_fifo)] = data[i++];
        fl_ring_fifo_write_increment(&my_fifo);
    }
}
```

## Planned next features

  - Push/pop/peek signatures for the storage classes

  Whould you like a feature? Please, ask.

## Licensing

Use as you want. Have fun. Code as is, no grantees.
