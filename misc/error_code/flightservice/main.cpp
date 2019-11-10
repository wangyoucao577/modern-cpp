
#include "failure_source.h"
#include "flights_err.h"
#include "seats_err.h"

#include <iostream>

void handle_errors(const std::error_code& ec)
{
    std::error_condition err_cond;
    if (ec == flightservice::FailureSource::BadUserInput) {
        err_cond = flightservice::FailureSource::BadUserInput;
    } else if (ec == flightservice::FailureSource::InternalError) {
        err_cond = flightservice::FailureSource::InternalError;
    } else if (ec == flightservice::FailureSource::NoSolution) {
        err_cond = flightservice::FailureSource::NoSolution;
    }
    std::cout << "  FailureSource message: " << err_cond.message() << std::endl;
}

int main()
{
    std::error_code flights_err = flightservice::FlightsErr::NonexistentLocations;
    std::cout << flights_err << std::endl;
    std::cout << "  value: " << flights_err.value() << " msg: " << flights_err.message() << std::endl;
    handle_errors(flights_err);

    std::error_code seats_err = flightservice::SeatsErr::NoSeatAvailable;
    std::cout << seats_err << std::endl;
    std::cout << "  value: " << seats_err.value() << " msg: " << seats_err.message() << std::endl;
    handle_errors(seats_err);

    return 0;
}