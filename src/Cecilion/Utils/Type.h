#pragma once
#include <string>
#include <typeinfo>
#include <type_traits>

namespace Cecilion {
    template<typename T>
    struct remove_all_pointers : std::conditional_t<
            std::is_pointer_v<T>,
            remove_all_pointers<
                    std::remove_pointer_t<T>
            >,
            std::type_identity<T>
    >
    {};

    template<typename T>
    using remove_all_pointers_t = typename remove_all_pointers<T>::type;

    std::string demangle(const char* name);

    template <class T>
    std::string type(const T&) {
        typedef typename std::remove_all_extents<T>::type Type;
        typedef typename std::remove_pointer<Type>::type  T2;
        return demangle(typeid(T2).name());
    }

    template<> std::string type(const std::type_info& t);
}

