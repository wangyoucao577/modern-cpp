#include <iostream>
#include <typeinfo>
#include <type_traits>

#if defined(_MSC_VER)
//https://docs.microsoft.com/en-us/cpp/preprocessor/predefined-macros?view=vs-2019
#define FUNCTION_WITH_SIGNATURE __FUNCSIG__
#else
// https://gcc.gnu.org/onlinedocs/gcc/Function-Names.html
#define FUNCTION_WITH_SIGNATURE __PRETTY_FUNCTION__
#endif

#define PRINT_FUNC_WITH_SIGNATURE_AND_TYPE_NAME_OF_T \
    std::cout << "\"" << FUNCTION_WITH_SIGNATURE << "\", type name of T: " << typeid(T).name() << std::endl; 

/**** Case 1: ParamType is a reference or a pointer, but not a universal reference.  */

template<typename T>
void func_param_type_reference(T&){
    PRINT_FUNC_WITH_SIGNATURE_AND_TYPE_NAME_OF_T;
}

template<typename T>
void func_param_type_const_reference(const T&){
    PRINT_FUNC_WITH_SIGNATURE_AND_TYPE_NAME_OF_T;
}

template<typename T>
void func_param_type_pointer(T*){
    PRINT_FUNC_WITH_SIGNATURE_AND_TYPE_NAME_OF_T;
}

template<typename T>
void func_param_type_pointer_to_const(const T*){
    PRINT_FUNC_WITH_SIGNATURE_AND_TYPE_NAME_OF_T;
}

/**** Case 1: ParamType is a reference or a pointer, but not a universal reference.  */

/**** Case 2: ParamType is a universal reference. */

template<typename T>
void func_param_type_universal_reference(T&&){
    PRINT_FUNC_WITH_SIGNATURE_AND_TYPE_NAME_OF_T;
}

/**** Case 2: ParamType is a universal reference. */

/**** Case 3: ParamType is neither a pointer nor a reference, i.e. pass by value. */

template<typename T>
void func_param_type_by_value(T){
    PRINT_FUNC_WITH_SIGNATURE_AND_TYPE_NAME_OF_T;
}

template<typename T>
void func_param_type_by_const_value(const T){
    PRINT_FUNC_WITH_SIGNATURE_AND_TYPE_NAME_OF_T;
}


/**** Case 3: ParamType is neither a pointer nor a reference, i.e. pass by value. */


void case1(){

    /**** Case 1: ParamType is a reference or a pointer, but not a universal reference. 
     */
    std::cout << "Case 1: ParamType is a reference or a pointer, but not a universal reference." << std::endl;


    int x = 100;
    const int x1 = x;
    int& x2 = x;
    const int& x3 = x;

    func_param_type_reference(x);  // T is int, T& is int&
    func_param_type_reference(x1); // T is const int, T& is const int&
    func_param_type_reference(x2); // T is int, T& is int&
    func_param_type_reference(x3); // T is const int, T& is const int&

    func_param_type_const_reference(x);  // T is int, const T& is const int&
    func_param_type_const_reference(x1); // T is int, const T& is const int&
    func_param_type_const_reference(x2); // T is int, const T& is const int&
    func_param_type_const_reference(x3); // T is int, const T& is const int&

    int* y1 = &x;
    const int* y2 = &x;
    const int* const y3 = &x;

    func_param_type_pointer(y1);    // T is int, T* is int*
    func_param_type_pointer(y2);    // T is const int, T* is const int*
    func_param_type_pointer(y3);    // T is const int, T* is const int*

    func_param_type_pointer_to_const(y1);   // T is int, const T* is const int*
    func_param_type_pointer_to_const(y2);   // T is int, const T* is const int*
    func_param_type_pointer_to_const(y3);   // T is int, const T* is const int*

}

void case2(){
    /**** Case 2: ParamType is a universal reference. 
     */
    std::cout << "Case 2: ParamType is a universal reference." << std::endl;

    int x = 100;
    const int x1 = x;
    int& x2 = x;
    const int& x3 = x;

    func_param_type_universal_reference(x);     // T is int&, T&& is int&
    func_param_type_universal_reference(x1);    // T is const int&, T&& is const int&
    func_param_type_universal_reference(x2);    // T is int&, T&& is int&
    func_param_type_universal_reference(x3);    // T is const int&, T&& is const int&

    func_param_type_universal_reference(std::move(x));      // T is int, T&& is int&&
    func_param_type_universal_reference(std::move(x1));     // T is const int, T&& is const int&&
    func_param_type_universal_reference(100);               // T is int, T&& is int&&
}

void case3(){
    /**** Case 3: ParamType is neither a pointer nor a reference, i.e. pass by value. 
    */
    std::cout << "Case 3: ParamType is neither a pointer nor a reference, i.e. pass by value." << std::endl;

    int x = 100;
    const int x1 = x;
    const int& x2 = x;
    const int&& x3 = std::move(x);
    int* y = &x;
    const int* y1 = y;
    const int* const y2 = y;

    func_param_type_by_value(x);    // T is int
    func_param_type_by_value(x1);   // T is int
    func_param_type_by_value(x2);   // T is int
    func_param_type_by_value(x3);   // T is int

    func_param_type_by_value(y);    // T is int*
    func_param_type_by_value(y1);   // T is const int*
    func_param_type_by_value(y2);   // T is const int*

    func_param_type_by_const_value(x);    // T is const int
    func_param_type_by_const_value(x1);   // T is const int
    func_param_type_by_const_value(x2);   // T is const int
    func_param_type_by_const_value(x3);   // T is const int

    func_param_type_by_const_value(y);    // T is int* const
    func_param_type_by_const_value(y1);   // T is const int* const
    func_param_type_by_const_value(y2);   // T is const int* const
}

void case_array_arguments(){
    std::cout << "Case Array Arguments." << std::endl;

    const char name[] = "J. P. Briggs";
    func_param_type_by_value(name); // T is const char*
    func_param_type_pointer(name);  // T is const char, T* is const char*
    func_param_type_reference(name);// T is const char [13], T& is const char(&)[13]
}

void some_func(int, double){}
void case_function_arguments(){
    std::cout << "Case Function Arguments." << std::endl;

    func_param_type_by_value(some_func);    // T is void(*)(int, double)
    func_param_type_pointer(some_func);     // T is void(int, double), T* is void(*)(int, double)
    func_param_type_reference(some_func);   // T is void(int, double), T& is void(&)(int, double)
}

int main() {

    case1();
    std::cout << std::endl;

    case2();
    std::cout << std::endl;

    case3();
    std::cout << std::endl;

    case_array_arguments();
    std::cout << std::endl;

    case_function_arguments();
    std::cout << std::endl;

    return 0;
}