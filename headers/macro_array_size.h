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
