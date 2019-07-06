

#if defined(COMPILER_DIAGNOSTICS)
#include "compiler_diagnostics.hpp"
#endif

#if defined(BOOST_SUPPORT)
#include <boost/type_index.hpp>
#endif

#include <vector>
#include <iostream>

template<typename T>
void f(const T& param) {
#if defined(COMPILER_DIAGNOSTICS)
    TD<decltype(param)> t;
#endif

    std::cout << "-- reports by std:" << std::endl;
    std::cout << "typeid(T).name(): " << typeid(T).name() << std::endl;
    std::cout << "typeid(decltype(param)).name(): " << typeid(decltype(param)).name() << std::endl;
    std::cout << "std::is_const<T>(): " << std::is_const<T>() 
        << ", std::is_reference<T>(): " << std::is_reference<T>() << std::endl;
    std::cout << "std::is_const<decltype(param)>(): " << std::is_const<decltype(param)>() 
        << ", std::is_reference<decltype(param)>(): " << std::is_reference<decltype(param)>() << std::endl;
    std::cout << std::endl;

#if defined(BOOST_SUPPORT)
    std::cout << "-- reports by boost::type_index:" << std::endl;
    std::cout << "boost::typeindex::type_id_with_cvr<T>().pretty_name(): " 
        << boost::typeindex::type_id_with_cvr<T>().pretty_name() << std::endl;
    std::cout << "boost::typeindex::type_id_with_cvr<decltype(param)>().pretty_name(): " 
        << boost::typeindex::type_id_with_cvr<decltype(param)>().pretty_name() << std::endl;
#endif
}
