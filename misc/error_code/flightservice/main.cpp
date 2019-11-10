
#include "flights_err.h"
#include "seats_err.h"

#include <iostream>

int main()
{
    std::error_code flights_err = flightservice::FlightsErr::NoFlightsFound;
    std::cout << flights_err << ", value: " << flights_err.value() << " msg: " << flights_err.message() << std::endl;

    std::error_code seats_err = flightservice::SeatsErr::NoSeatAvailable;
    std::cout << seats_err << ", value: " << seats_err.value() << " msg: " << seats_err.message() << std::endl;

    return 0;
}