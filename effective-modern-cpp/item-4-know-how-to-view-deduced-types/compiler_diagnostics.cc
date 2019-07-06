
#include "compiler_diagnostics.hpp"

int main() {

    const int the_answer = 42;

    auto x = the_answer;
    auto y = &the_answer;

    // elicit errors containing x's and y's types
    TD<decltype(x)> x1;
    TD<decltype(y)> y1;

    return 0;
}