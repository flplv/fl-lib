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
