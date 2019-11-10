
#pragma once
#include <system_error>

namespace flightservice {
enum class FailureSource {
    // no 0
    // 0 represents OK/Success is std::error_code's convention.

    BadUserInput = 1, // illogical request
    InternalError = 2, // internal error
    NoSolution = 3, // bad luck
};

// overload for `std::make_error_condition` for FailureSource.
// http://naipc.uchicago.edu/2014/ref/cppreference/en/cpp/error/error_condition/make_error_condition.html
std::error_condition make_error_condition(FailureSource e) noexcept;

}

// specialize template for FailureSource to indicate that
// the type is eligible for `std::error_code` and `std::error_condition` automatic conversions.
// http://naipc.uchicago.edu/2014/ref/cppreference/en/cpp/error/error_condition/is_error_condition_enum.html
template <>
struct std::is_error_condition_enum<flightservice::FailureSource>
    : public std::true_type {
};
