
#include "complex_sample.hpp"

class Widget{
    int value_ {0};
};

std::vector<Widget> create_vector(){
    std::vector<Widget> vw;
    vw.emplace_back(Widget());
    return vw;
}

int main(){

    const auto vw = create_vector();
    if (!vw.empty()){
        f(&vw[0]);
    }

    return 0;
}