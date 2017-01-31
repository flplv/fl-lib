#if defined(__GNUC__) && defined(__GNUC_MINOR__)
    #define FL_GNUC_VERSION \
        (__GNUC__ << 16) + __GNUC_MINOR__
    #define FL_GNUC_PREREQ(maj, min) \
        (FL_GNUC_VERSION >= ((maj) << 16) + (min))
#else
    #define FL_GNUC_PREREQ(maj, min) 0
#endif
