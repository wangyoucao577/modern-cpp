
#include "flights_err.h"

namespace flightservice {

namespace { // anonymous namespace for category definition

    struct FlightsErrCategory : public std::error_category {
        const char* name() const noexcept override;
        std::string message(int ev) const override;
    };

    const char* FlightsErrCategory::name() const noexcept
    {
        return "FlightsErr";
    }

    std::string FlightsErrCategory::message(int ev) const
    {
        switch (static_cast<FlightsErr>(ev)) {
        case FlightsErr::NonexistentLocations:
            return "nonexistent airport name in request";
        case FlightsErr::DatesInThePast:
            return "request for a date from the past";
        case FlightsErr::InvertedDates:
            return "requested flight return date before departure date";
        case FlightsErr::NoFlightsFound:
            return "no flight combination found";
        case FlightsErr::ProtocolViolation:
            return "received malformed request";
        case FlightsErr::ConnectionError:
            return "could not connect to server";
        case FlightsErr::ResourceError:
            return "insufficient resources";
        case FlightsErr::Timeout:
            return "processing timed out";
        default:
            break;
        }
        return "(unrecognized error)";
    }

    // global object for unify this category
    const FlightsErrCategory the_flights_err_category {};
}

std::error_code make_error_code(flightservice::FlightsErr e) noexcept
{
    return { static_cast<int>(e), the_flights_err_category };
}

}