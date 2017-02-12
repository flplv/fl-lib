#include <typeinfo>
#include <type_traits>

#define fl_auto_type(___zcxzzarg) auto
#define fl_typeof(___zarg) std::remove_reference<decltype(___zarg)>::type
