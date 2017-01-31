template <typename T, size_t N>
char ( &FL_ARRAY_SIZE_HELPER( T (&array)[N] ))[N];

#define FL_ARRAY_SIZE( array ) \
     (sizeof( FL_ARRAY_SIZE_HELPER( array ) ))
