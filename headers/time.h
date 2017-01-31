#ifdef FL_ENABLE_TIME_MODULE

    #ifndef FL_TIME_TYPE
    #error "You must define `FL_TIME_TYPE` with a unsigned integer type such as `uint32_t`, `uin64_t`, etc. "
    #endif

    /* This is a elapsed time checker that protects against wrap around.
       Now must always be (physically) bigger than before.*/
    static inline bool fl_time_check_elapsed(fl_time_t now, fl_time_t before, fl_time_t desired_wait)
    {
        return (now - before) >= desired_wait;
    }

    /* Return the remaining time to timeout expiration.*/
    static inline fl_time_t fl_time_remaining(fl_time_t now, fl_time_t before, fl_time_t desired_wait)
    {
        if (fl_time_check_elapsed(now, before, desired_wait - 1))
            return 0;

        return desired_wait - (now - before);
    }

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
