#ifndef _FL_LIB_H_
#define _FL_LIB_H_

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#define fl_typeof(___zarg) typeof(___zarg)

#define fl_auto_type(___zcxzzarg) fl_typeof(___zcxzzarg)

#include <stddef.h>
#define fl_offsetof(___j1h3oi1bob,___j1h3oi1bob2) offsetof(___j1h3oi1bob,___j1h3oi1bob2)

#define fl_container_of(ptr, type, member) ({ \
        const typeof( ((type *)0)->member ) *__mptr = (ptr); \
        (type *)( (char *)__mptr - fl_offsetof(type,member) );})

#define FL_BUILD_BUG_ON_ZERO(e) \
   (sizeof(struct { int:-!!(e)*1234; }))

#if defined(__GNUC__) && defined(__GNUC_MINOR__)
    #define FL_GNUC_VERSION \
        (__GNUC__ << 16) + __GNUC_MINOR__
    #define FL_GNUC_PREREQ(maj, min) \
        (FL_GNUC_VERSION >= ((maj) << 16) + (min))
#else
    #define FL_GNUC_PREREQ(maj, min) 0
#endif

#include <stddef.h>
#define fl_sizeof(___j1h32314141) sizeof(___j1h32314141)

#if FL_GNUC_PREREQ(3, 1)
#define FL_SAME_TYPE(a, b) \
    __builtin_types_compatible_p(fl_typeof(a), fl_typeof(b))
#define FL_MUST_BE_ARRAY(a) \
    FL_BUILD_BUG_ON_ZERO(FL_SAME_TYPE((a), &(*a)))
#else
#define FL_MUST_BE_ARRAY(a) \
    FL_BUILD_BUG_ON_ZERO(fl_sizeof(a) % fl_sizeof(*a))
#endif

#define FL_ARRAY_SIZE(a) ( \
    (fl_sizeof(a) / fl_sizeof(*a)) \
    + FL_MUST_BE_ARRAY(a))

struct fl_struct_array_private
{
    size_t max;
    size_t top;
};

typedef struct fl_struct_array_private fl_array_t;

static inline void fl_array_init(fl_array_t * obj, size_t max_length)
{
    obj->max = max_length;
    obj->top = 0;
}

static inline void fl_array_deinit(fl_array_t * obj)
{
    obj->max = 0;
    obj->top = 0;
}

static inline void fl_array_clear(fl_array_t * obj)
{
    obj->top = 0;
}

static inline size_t fl_array_length(fl_array_t * obj)
{
    return obj->top;
}

static inline bool fl_array_full(fl_array_t * obj)
{
    return obj->top == obj->max;
}

static inline size_t _fl_array_insert_prepare(fl_array_t * obj)
{
    if (fl_array_full(obj))
        obj->top--;

    return obj->top;
}

static inline void _fl_array_insert_end(fl_array_t * obj)
{
    obj->top ++;
}

#define fl_array_insert(obj, buffer, data) \
({ \
    fl_array_t * _o = (obj); \
    buffer[_fl_array_insert_prepare(_o)] = data; \
    _fl_array_insert_end(_o); \
})

static inline size_t _fl_array_remove_prepare(fl_array_t * obj, size_t pos)
{
    if (pos >= obj->top)
        return 0;

    return obj->top - 1 - pos;
}

static inline void _fl_array_remove_end(fl_array_t * obj)
{
    obj->top --;
}

#define fl_array_remove(obj, buffer, pos) \
({ \
    fl_array_t * _o = (obj); \
    size_t _p = (pos); \
    fl_auto_type (buffer) _b = (buffer); \
    size_t n = _fl_array_remove_prepare(_o, _p); \
    for (size_t i = 0; i < n; i++) \
        _b[_p + i] = _b[_p + i + 1]; \
    if (n) _fl_array_remove_end(_o); \
})

#define fl_array_free(obj, buffer, pos, freefunc) \
({ \
    fl_array_t * _o = (obj); \
    size_t _p = (pos); \
    fl_auto_type (buffer) _b = (buffer); \
    fl_auto_type (freefunc) _f = (freefunc); \
    _f(_b[_p]); \
    for (size_t i = 0; i < _fl_array_remove_prepare(_o, _p); i++) \
        _b[_p + i] = _b[_p + i + 1]; \
    _fl_array_remove_end(_o); \
})

#define fl_array_find(obj, buffer, comparefunc, seed) \
({ \
    int _r = -1; \
    fl_array_t * _o = (obj); \
    fl_auto_type (buffer) _b = (buffer); \
    fl_auto_type (seed) _s = (seed); \
    fl_auto_type (comparefunc) _f = (comparefunc); \
    for (size_t i = 0; i < fl_array_length(_o); i++) \
        if (_f(_b[i], _s)) { _r = (int)i; break; } \
    _r; \
})

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

static inline size_t fl_ring_fifo_count(fl_ring_fifo_t * obj)
{
    return obj->wrIdx - obj->rdIdx;
}

static inline bool fl_ring_fifo_full(fl_ring_fifo_t * obj)
{
    return fl_ring_fifo_count (obj) == fl_ring_fifo_num_of_slots (obj);
}

struct fl_struct_linked_list_head
{
    struct fl_struct_linked_list_head * next;
    struct fl_struct_linked_list_head * prev;
};

typedef struct fl_struct_linked_list_head fl_linked_list_t;

/**
 * Initialize a fl_linked_list_t within structure
 * @param
 */
#define fl_linked_list_init(structure, field) \
({ \
        fl_auto_type(structure) ___str = structure; \
        if (___str) {\
        	___str->field.next = (fl_linked_list_t *)0;\
        	___str->field.prev = (fl_linked_list_t *)0;\
        } \
})

/**
 * Get the number of nodes of the list.
 *
 * @param ptr_to_current
 * @return Pointer to the last node of the list, including ptr_to_current.
 */
#define fl_linked_list_count(ptr_to_current, field) \
({ \
        int ___ret = 0; \
        fl_auto_type(ptr_to_current) ___cur = fl_linked_list_first(ptr_to_current, field); fl_linked_list_t * ___head;\
        if (___cur) ___ret++; \
        if (___ret) \
        { \
            ___head = &___cur->field; \
            if (___head->next) { while (___head->next){ ___head = ___head->next; ___ret++;}} \
        } \
        ___ret; \
})


/**
 * Get the next node.
 * @param ptr_to_current
 * @return Pointer to the next node.
 */
#define fl_linked_list_next(ptr_to_current, field) \
({ \
        fl_auto_type(ptr_to_current) ___ret = ptr_to_current; fl_linked_list_t * ___head;\
        if (___ret) \
        { \
           ___head = ___ret->field.next; \
           if (___head) { ___ret  = fl_container_of(___head, fl_typeof(*(ptr_to_current)), field); }\
           else {___ret = 0; } \
        } \
        ___ret; \
})

/**
 * Get the previous node.
 * @param ptr_to_current
 * @return Pointer to the previous node.
 */
#define fl_linked_list_previous(ptr_to_current, field) \
({ \
        fl_auto_type(ptr_to_current) ___ret = ptr_to_current; fl_linked_list_t * ___head;\
        if (___ret) \
        { \
           ___head = ___ret->field.prev; \
           if (___head) { ___ret  = fl_container_of(___head, fl_typeof(*(ptr_to_current)), field); }\
           else {___ret = 0; } \
        } \
        ___ret; \
})

/**
 * Get the last node of the list, including ptr_to_current.
 *
 * @param ptr_to_current
 * @return Pointer to the last node of the list, including ptr_to_current.
 */
#define fl_linked_list_last(ptr_to_current, field) \
({ \
        fl_auto_type(ptr_to_current) ___ret = ptr_to_current; fl_linked_list_t * ___head;\
        if (___ret) \
        { \
            ___head = &___ret->field; \
            if (___head->next) { while (___head->next) ___head = ___head->next;} \
            if (___head) { ___ret  = fl_container_of(___head, fl_typeof(*(ptr_to_current)), field); } \
            else {___ret = 0; } \
        } \
        ___ret; \
})

/**
 * Get the first node of the list, including ptr_to_current.
 *
 * @param ptr_to_current
 * @return Pointer to the first node of the list, including ptr_to_current.
 */
#define fl_linked_list_first(ptr_to_current, field) \
({ \
        fl_auto_type(ptr_to_current) ___ret2 = ptr_to_current; fl_linked_list_t * ___head;\
        if (___ret2) \
        { \
            ___head = &___ret2->field; \
            if (___head->prev) { while (___head->prev) ___head = ___head->prev;} \
            if (___head) { ___ret2  = fl_container_of(___head, fl_typeof(*(ptr_to_current)), field); } \
            else {___ret2 = 0; } \
        } \
        ___ret2; \
})

/**
 * Insert new node between current and current's next.
 *
 * @param ptr_to_current
 * @param ptr_to_new
 */
#define fl_linked_list_insert_after(ptr_to_current, ptr_to_new, field) \
({ \
        fl_auto_type(ptr_to_current) ___curr = ptr_to_current; \
        fl_auto_type(ptr_to_new) ___new = ptr_to_new; \
        if (___curr && ___new) \
        { \
            ___new->field.next = ___curr->field.next; \
            ___new->field.prev = &___curr->field; \
            if (___curr->field.next) ___curr->field.next->prev = &___new->field; \
            ___curr->field.next = &___new->field; \
        } \
})

/**
 * Insert new node between current and current's previous.
 *
 * @param ptr_to_current
 * @param ptr_to_new
 */
#define fl_linked_list_insert_before(ptr_to_current, ptr_to_new, field) \
({ \
        fl_auto_type(ptr_to_current) ___curr = ptr_to_current; \
        fl_auto_type(ptr_to_new) ___new = ptr_to_new; \
        if (___curr && ___new) \
        { \
            ___new->field.prev = ___curr->field.prev; \
            ___new->field.next = &___curr->field; \
            if(___curr->field.prev) ___curr->field.prev->next = &___new->field; \
            ___curr->field.prev = &___new->field; \
        } \
})

/**
 * Remove the current node and return it.
 *
 * @param current
 * @return Return the new or old beginning of the list.
 */
#define fl_linked_list_remove(ptr_to_current, field) \
({ \
        fl_auto_type(ptr_to_current) ___cur = ptr_to_current; \
        fl_auto_type(ptr_to_current) ___ret = (fl_typeof(ptr_to_current))0; \
        fl_linked_list_t * ___head_next = (fl_linked_list_t*)0; \
        fl_linked_list_t * ___head_prev = (fl_linked_list_t*)0; \
        if (___cur) \
        { \
           ___head_next = ___cur->field.next; \
           ___head_prev = ___cur->field.prev; \
           if (___head_prev) ___head_prev->next = ___head_next; \
           if (___head_next) ___head_next->prev = ___head_prev; \
           fl_linked_list_init(___cur, field); \
           if (___head_prev) ___ret = fl_linked_list_first(fl_container_of(___head_prev, fl_typeof(*(ptr_to_current)), field), field); \
           else if(___head_next) ___ret = fl_linked_list_first(fl_container_of(___head_next, fl_typeof(*(ptr_to_current)), field), field); \
        } \
        ___ret; \
})

/**
 * Free all list members,
 * but instead calling free itself, this function calls
 * user free function passed on free_callback.
 *
 * @param ptr_to_current
 *
 * @warning
 *      Any pointer to the a list node will be invalid.
 */
#define fl_linked_list_free(ptr_to_current, field, free_callback) \
({ \
        fl_auto_type(ptr_to_current) ___item = fl_linked_list_first(ptr_to_current, field); \
        fl_typeof(___item) ___item_to_delete; \
        while (___item) \
        { \
            ___item_to_delete = ___item; \
            ___item = fl_linked_list_next(___item_to_delete, field); \
            free_callback(___item_to_delete); \
        } \
})

/**
 */
#define fl_linked_list_find(ptr_to_current, field, comparator_func, comparator_seed) \
({ \
        fl_auto_type(ptr_to_current) ___item = ptr_to_current; \
        while (___item) \
        { \
        	if (comparator_func(comparator_seed, ___item) == true) break; \
            ___item = fl_linked_list_next(___item, field); \
        } \
        ___item; \
})

#ifdef FL_ENABLE_TIME_MODULE

    #ifndef FL_TIME_TYPE
    #error "You must define `FL_TIME_TYPE` with a unsigned integer type such as `uint32_t`, `uin64_t`, etc. "
    #endif

    typedef FL_TIME_TYPE fl_time_t;

    /* This is a elapsed time checker that protects against wrap around.
       Now must always be (physically) bigger than before.*/
    static inline bool fl_time_check_elapsed(fl_time_t now, fl_time_t before, fl_time_t desired_wait)
    {
        return (now - before) >= desired_wait;
    }

    #ifdef FL_TIME_INT_TYPE
        /* Return the remaining time to timeout expiration.*/
        static inline FL_TIME_INT_TYPE fl_time_remaining(fl_time_t now, fl_time_t before, fl_time_t desired_wait)
        {
            return (FL_TIME_INT_TYPE)desired_wait - (FL_TIME_INT_TYPE)(now - before);
        }
    #endif

    /* This is a reached time checker that protects against wrap around.
       It checks if now has reached timestamp. Timestamp must always be bigger than
       now when first stored. */
    static inline bool fl_time_check_reached(fl_time_t timestamp, fl_time_t now)
    {
        /* First we subtract from timestamp a stupid_big_number,
           to create a "before" number.
           Then we use a stupid_big_number as the elapsed variable
           to the check_elapsed function. */

        const fl_time_t max = (fl_time_t)-1;
        const fl_time_t stupid_big_number = (max/2) + 1;
        fl_time_t before = timestamp - stupid_big_number;

        return fl_time_check_elapsed(now, before, stupid_big_number);
    }

#endif /* FL_ENABLE_TIME_MODULE */

#endif /* _FL_LIB_H_ */
