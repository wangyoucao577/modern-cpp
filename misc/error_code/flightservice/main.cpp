
#include "flights_err.h"
#include "seats_err.h"

#include <iostream>

int main()
{
    std::error_code flights_err = flightservice::FlightsErr::NoFlightsFound;
    std::cout << flights_err << std::endl;

    std::error_code seats_err = flightservice::SeatsErr::NoSeatAvailable;
    std::cout << seats_err << std::endl; 

    return 0;
}