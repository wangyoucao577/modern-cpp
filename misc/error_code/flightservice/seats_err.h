
#pragma once
#include <system_error>

namespace flightservice {

enum class SeatsErr {
    // no 0
    // 0 represents OK/Success is std::error_code's convention.

    // internal error
    InvalidRequest = 1, // e.g., bad XML
    CouldNotConnect, // could not connect to server
    InternalError, // service run short of resources
    NoResponse, // did not respond in time

    // illogical request
    NonexistentClass, // requested class does not exist

    // bad luck
    NoSeatAvailable, // all seats booked
};

// overload for `std::make_error_code` for SeatsErr.
// http://naipc.uchicago.edu/2014/ref/cppreference/en/cpp/error/error_code/error_code.html
std::error_code make_error_code(flightservice::SeatsErr e) noexcept;

}

// specialize template for SeatsErr to indicate that
// the type is eligible for `std::error_code` and `std::error_condition` automatic conversions.
// http://naipc.uchicago.edu/2014/ref/cppreference/en/cpp/error/error_code/is_error_code_enum.html
template <>
struct std::is_error_code_enum<flightservice::SeatsErr> : public std::true_type {
};
