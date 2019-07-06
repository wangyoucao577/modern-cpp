

#if defined(COMPILER_DIAGNOSTICS)
#include "compiler_diagnostics.hpp"
#endif

#include <vector>

template<typename T>
void f(const T& param) {
#if defined(COMPILER_DIAGNOSTICS)
    TD<decltype(param)> t;
#endif

}
