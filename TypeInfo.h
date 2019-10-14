#pragma once

namespace serious
{

template<typename T> // c++11 -> std::is_fundamental<T>
struct is_primitive
{
    enum { value = false };
};

#define DECL_PRIMITIVE_TYPE(type) \
template<>  \
struct is_primitive<type> \
{ \
    enum { value = true }; \
} \

DECL_PRIMITIVE_TYPE(int);
DECL_PRIMITIVE_TYPE(short);
DECL_PRIMITIVE_TYPE(long);
DECL_PRIMITIVE_TYPE(signed char);
DECL_PRIMITIVE_TYPE(unsigned char);
DECL_PRIMITIVE_TYPE(float);
DECL_PRIMITIVE_TYPE(double);
DECL_PRIMITIVE_TYPE(long long int);

template<typename T> // c++ -> std::is_empty<T>
struct is_empty
{
    struct __empty : T { int x; }; // Empty class base optimization (ECBO)
    enum { value = sizeof(__empty) == sizeof(int) };
};

}
