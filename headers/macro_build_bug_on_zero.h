#define FL_BUILD_BUG_ON_ZERO(e) \
   (sizeof(struct { int:-!!(e)*1234; }))
