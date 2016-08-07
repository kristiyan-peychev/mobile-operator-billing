#include "account.h"
#include "service_provider.h"
#include <chrono>
#include <ctime>
#include <exception>

account::~account()
{
    unregister_from_provider();
}

account::account(service_provider *provider, phone &number)
{
    if (!register_to_provider(provider))
        throw std::exception("Could not register with service provider");

    set_personal_number(number);
}

static bool is_weekend(const call_timestamp &time_point)
{
    std::time_t unix_time = std::chrono::system_clock::to_time_t(time_point);
    long long hours = unix_time / 3600;
    long long days = hours / 24;
    long long day_of_week = days % 7;
    return (day_of_week == 0 || day_of_week == 6);
}

money *account::make_call(const phone &other_side_number, const call_timestamp &start_time, const call_timestamp &end_time)
{
    if (start_time == end_time)
        throw std::exception("The call did not exist");
    else if (start_time > end_time)
        throw std::exception("A call cannot start after it has ended");

    call_duration duration = end_time - start_time;
    return service_provider::customer::make_call(duration, other_side_number, is_weekend(start_time));
}

