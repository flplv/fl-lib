#include <typeinfo>
#include <type_traits>

#define fl_auto_type(___zcxzzarg) auto
#define fl_typeof(___zarg) std::remove_reference<decltype(___zarg)>::type

template<class P, class M>
size_t fl_offsetof(const M P::*member)
{
    return (size_t) &( reinterpret_cast<P*>(0)->*member);
}
