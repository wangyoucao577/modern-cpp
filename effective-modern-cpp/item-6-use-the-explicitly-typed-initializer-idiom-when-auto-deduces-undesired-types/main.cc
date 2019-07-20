
#include <iostream>
#include <vector>

#if defined(BOOST_SUPPORT)
#include <boost/type_index.hpp>
#endif

std::vector<bool> get_features(){
    std::vector<bool> b;
    b.push_back(true);
    b.push_back(false);
    b.push_back(true);
    return b;
}

int main() {

    /* std::vector<bool> is a possibly space-efficient specialization of std::vector for the type bool.
    * Exposes class std::vector<bool>::reference as a method of accessing individual bits. In particular, objects of this class are returned by operator[] by value.
    */ 

    bool explicitly_b1 = get_features()[1];
    auto auto_b1 = get_features()[1];
    auto forced_auto_b1 = static_cast<bool>(get_features()[1]);

#if defined(BOOST_SUPPORT)
    std::cout << "-- reports by boost::type_index:" << std::endl;
    std::cout << "boost::typeindex::type_id_with_cvr<decltype(explicitly_b1)>().pretty_name(): " 
        << boost::typeindex::type_id_with_cvr<decltype(explicitly_b1)>().pretty_name() << std::endl;
    std::cout << "boost::typeindex::type_id_with_cvr<decltype(auto_b1)>().pretty_name(): " 
        << boost::typeindex::type_id_with_cvr<decltype(auto_b1)>().pretty_name() << std::endl;
    std::cout << "boost::typeindex::type_id_with_cvr<decltype(forced_auto_b1)>().pretty_name(): " 
        << boost::typeindex::type_id_with_cvr<decltype(forced_auto_b1)>().pretty_name() << std::endl;
#endif


    return 0;
}