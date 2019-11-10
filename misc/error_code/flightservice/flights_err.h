
#pragma once
#include <system_error>

namespace flightservice {

enum class FlightsErr {
    // no 0
    // 0 represents OK/Success is std::error_code's convention.

    // illogical request
    NonexistentLocations = 10, // requested airport doesn't exist
    DatesInThePast, // booking flight for yesterday
    InvertedDates, // returning before departure

    // bad luck
    NoFlightsFound = 20, // did not find any combination

    // internal error
    ProtocolViolation = 30, // e.g., bad XML
    ConnectionError, // could not connect to server
    ResourceError, // service run short of resources
    Timeout, // did not respond in time
};

// overload for `std::make_error_code` for FlightsErr.
// http://naipc.uchicago.edu/2014/ref/cppreference/en/cpp/error/error_code/error_code.html
std::error_code make_error_code(flightservice::FlightsErr e) noexcept;

// for category compare in condition checking
const std::error_category& get_flights_err_category() noexcept;

}

// specialize template for FlightsErr to indicate that
// the type is eligible for `std::error_code` and `std::error_condition` automatic conversions.
// http://naipc.uchicago.edu/2014/ref/cppreference/en/cpp/error/error_code/is_error_code_enum.html
template <>
struct std::is_error_code_enum<flightservice::FlightsErr> : public std::true_type {
};
