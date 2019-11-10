
#include "seats_err.h"

namespace flightservice {

namespace { // anonymous namespace for category definition

    struct SeatsErrCategory : public std::error_category {
        const char* name() const noexcept override;
        std::string message(int ev) const override;
    };

    const char* SeatsErrCategory::name() const noexcept
    {
        return "SeatsErr";
    }

    std::string SeatsErrCategory::message(int ev) const
    {
        switch (static_cast<SeatsErr>(ev)) {
        case SeatsErr::InvalidRequest: 
            return "invalid request";
        case SeatsErr::CouldNotConnect:
            return "could not connect to server";
        case SeatsErr::InternalError:
            return "service run short of resources";
        case SeatsErr::NoResponse:
            return "did not respond in time";
        case SeatsErr::NonexistentClass:
            return "requested class does not exist";
        case SeatsErr::NoSeatAvailable:
            return "all seats booked";
        default:
            break;
        }
        return "(unrecognized error)";
    }

    // global object for unify this category 
    const SeatsErrCategory the_seats_err_category {};
}

std::error_code make_error_code(flightservice::SeatsErr e) noexcept
{
    return {static_cast<int>(e), the_seats_err_category};
}

}