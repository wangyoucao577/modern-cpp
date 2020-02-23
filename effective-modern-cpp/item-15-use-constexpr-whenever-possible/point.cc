#include <iostream>

class Point{
public:
    constexpr Point(double x = 0, double y = 0) noexcept
    : x_(x), y_(y) {}
    
    constexpr double x() const noexcept { return x_; }
    constexpr double y() const noexcept { return y_; }

#if __cplusplus >= 201402L
    constexpr
#endif
    void setX(double x) noexcept { x_ = x; }
#if __cplusplus >= 201402L
    constexpr
#endif
    void setY(double y) noexcept { y_ = y; }

private:
    double x_, y_;
};

std::ostream& operator << (std::ostream& os, const Point& p) {
    return os << "(" << p.x() << "," << p.y() << ")";
}

constexpr
Point midpoint(const Point& p1, const Point& p2) noexcept {
    return { (p1.x() + p2.x()) / 2, (p1.y() + p2.y()) / 2};
}

#if __cplusplus >= 201402L
constexpr 
Point reflection(const Point& p) noexcept {
    Point result; 
    result.setX(-p.x());
    result.setY(-p.y());
    return result;
}
#endif

int main() {

    constexpr Point p1(9.4, 27.7);  
    std::cout << "p1" << p1 << std::endl;
    constexpr Point p2(28.8, 5.3);
    std::cout << "p2" << p2 << std::endl;

    constexpr auto mp = midpoint(p1, p2);
    std::cout << "mid point" << mp << std::endl;

#if __cplusplus >= 201402L
    constexpr auto rp1 = reflection(p1);
    std::cout << "rp1" << rp1 << std::endl;
#endif
    return 0;
}