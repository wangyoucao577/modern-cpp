
#include <iostream>

template<typename T>
void processPointer(T *ptr){
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

#if defined(DELETE_OVERLOADS)
// delete instantiations
template<>
void processPointer<void>(void*) = delete;
template<>
void processPointer<char>(char*) = delete;
template<>
void processPointer<const void>(const void*) = delete;
template<>
void processPointer<const char>(const char*) = delete;
#endif

int main() {
    int* i = nullptr;
    void* v = nullptr;
    char* c = nullptr; 
    const void* cv = nullptr;
    const char* cc = nullptr;

    processPointer(i);

    // can not compile after deleted template instantiations
    processPointer(v);
    processPointer(c);
    processPointer(cv);
    processPointer(cc);

    return 0;
}