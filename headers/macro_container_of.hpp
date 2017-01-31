template<class P, class M>
P* fl_container_of_impl(M* ptr, const M P::*member)
{
    return (P*)( (char*)ptr - fl_offsetof(member));
}

#define fl_container_of(ptr, type, member) \
     fl_container_of_impl (ptr, &type::member)
