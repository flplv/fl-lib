/*
    fl_ring_fifo
    - Single consumer / single Producer safe
    - FIFO's available slots will be rounded down to a power of two integer, 
       it is a requirement of the SCSP safety algorithm. Use power of two
       integer in buffer sizes and `num_of_slots` init parameter to avoid 
       rounding.
    - size_t must be atomic in the platform
*/

typedef struct fl_struct_ring_fifo_private {
    size_t mask;
    size_t wrIdx;
    size_t rdIdx;
} fl_ring_fifo_t;

/*
 * num_of_slots must be base of two, i.e. 2, 4, 8, 16, 32, 64, ...
 */
static inline int fl_ring_fifo_init(fl_ring_fifo_t * obj, size_t num_of_slots)
{
    size_t pre_mask = (SIZE_MAX >> 1) + 1;

    while (pre_mask)
    {
        if (num_of_slots & pre_mask) {
            break;
        }

        pre_mask >>= 1;
    }

    if (pre_mask <= 1) {
        obj->mask = 0;
        return -1;
    }

    obj->mask = pre_mask - 1;
    obj->rdIdx = 0;
    obj->wrIdx = 0;

    return (int)pre_mask;
}

static inline size_t fl_ring_fifo_num_of_slots(fl_ring_fifo_t * obj)
{
    return obj->mask + 1;
}

static inline size_t fl_ring_fifo_write_index(fl_ring_fifo_t * obj)
{
    return obj->mask & obj->wrIdx;
}

static inline size_t fl_ring_fifo_read_index(fl_ring_fifo_t * obj)
{
    return obj->mask & obj->rdIdx;
}

static inline void fl_ring_fifo_write_increment(fl_ring_fifo_t * obj)
{
    obj->wrIdx++;
}

static inline void fl_ring_fifo_read_increment(fl_ring_fifo_t * obj)
{
    obj->rdIdx++;
}

static inline void fl_ring_fifo_read_reset(fl_ring_fifo_t * obj)
{
    obj->rdIdx = 0;
    obj->wrIdx = 0;
}

static inline bool fl_ring_fifo_empty(fl_ring_fifo_t * obj)
{
    return obj->rdIdx == obj->wrIdx;
}

static inline bool fl_ring_fifo_full(fl_ring_fifo_t * obj)
{
    return (obj->mask & obj->rdIdx) == (obj->mask & (obj->wrIdx+1));
}

static inline size_t fl_ring_fifo_count(fl_ring_fifo_t * obj)
{
    return obj->mask & (obj->wrIdx - obj->rdIdx);
}
