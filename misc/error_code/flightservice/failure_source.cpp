
#include "failure_source.h"
#include "flights_err.h"
#include "seats_err.h"

namespace flightservice {

namespace { // anonymous namespace for category definition

    struct FailureSourceCategory : public std::error_category {
        const char* name() const noexcept override;
        std::string message(int ev) const override;
        bool equivalent(const std::error_code& code,
            int condition) const noexcept override;
    };

    const char* FailureSourceCategory::name() const noexcept
    {
        return "FailureSource";
    }

    std::string FailureSourceCategory::message(int ev) const
    {
        switch (static_cast<FailureSource>(ev)) {
        case FailureSource::BadUserInput:
            return "invalid user request";
        case FailureSource::InternalError:
            return "internal error";
        case FailureSource::NoSolution:
            return "no solution found for specified request";
        default:
            break;
        }
        return "(unrecognized error)";
    }

    bool FailureSourceCategory::equivalent(const std::error_code& ec,
        int condition) const noexcept
    {
        switch (static_cast<FailureSource>(condition)) {
        case FailureSource::BadUserInput: {
            // for SeatsErr
            if (ec == SeatsErr::NonexistentClass) {
                return true;
            }

            // for FlightsErr
            if (ec.category() == get_flights_err_category()) {
                return ec.value() >= 10 && ec.value() < 20;
            }

            // for any other error_code enum:
            return false;
        }
        case FailureSource::InternalError: {
            // for SeatsErr
            if (ec.category() == get_seats_err_category()) {
                return ec.value() >= 1 && ec.value() < 10;
            }

            // for FlightsErr
            if (ec.category() == get_flights_err_category()) {
                return ec.value() >= 30;
            }

            // for any other error_code enum:
            return false;
        }
        case FailureSource::NoSolution: {
            // for SeatsErr
            if (ec == SeatsErr::NoSeatAvailable) {
                return true;
            }

            // for FlightsErr
            if (ec == FlightsErr::NoFlightsFound) {
                return true;
            }

            // for any other error_code enum:
            return false;
        }
        default:
            break;
        }
        return false;
    }

    // global object for unify this category
    const FailureSourceCategory the_failure_source_category {};
}

std::error_condition make_error_condition(FailureSource e) noexcept
{
    return { static_cast<int>(e), the_failure_source_category };
}
}