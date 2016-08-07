#include "stdafx.h"
#include "service_provider.h"

bool operator == (const phone &first, const phone &second) { return first.get_number() == second.get_number(); }

bool operator<(const service_provider::customer_ref &first, const service_provider::customer_ref &second)
{
    return first.c_ < second.c_;
}

bool operator==(const service_provider::customer_ref &first, const service_provider::customer_ref &second)
{
    return first.c_ == second.c_;
}

bool operator==(const service_provider::customer_ref &first, const phone &second)
{
    return first.c_ == second;
}

bool service_provider::customer::register_to_provider(service_provider *provider) 
{
    if (nullptr == provider || false == provider->make_registration(*this)) {
        return false;
    } else {
        provider_ = provider;
        return true;
    }
}

void service_provider::customer::unregister_from_provider()
{
    if (nullptr != provider_)
        provider_->delete_registration(*this);
}

money *service_provider::customer::make_call(call_duration duration, const phone &other_side, bool is_weekend)
{
    return get_provider()->tax_call(*this, other_side, duration, is_weekend);// FIXME
}

bool service_provider::make_registration(customer &account)
{
    return customers_.insert(customer_ref(account)).second;
}

void service_provider::delete_registration(customer &account)
{
    auto cust = customers_.find(account);
    if (cust != customers_.end())
        customers_.erase(cust);
}

money * service_provider::tax_local_currency_amount(float amount)
{
    return new dollar(get_base_call_cost().get_amount_in_dollars() + amount);
}

money* service_provider::tax_inside_call(customer &client, const phone &other_side, call_duration duration)
{
    call_duration remaining_duration = call_duration(0);
    const auto &time_inside = client.get_time_talked_inside_operator();

    if (time_inside < FREE_TIME_FOR_INSIDE_CALLS) {
        if ((time_inside + duration) < FREE_TIME_FOR_INSIDE_CALLS) {
            client.set_time_talked_inside_operator(time_inside + duration);
        } else {
            remaining_duration = duration - (FREE_TIME_FOR_INSIDE_CALLS - time_inside);
            client.set_time_talked_inside_operator(FREE_TIME_FOR_INSIDE_CALLS);
        }
    } else {
        remaining_duration = duration;
    }

    if (remaining_duration == call_duration(0))
        return tax_local_currency_amount(0);

    return tax_call_per_minute(duration, get_inside_call_cost());
}

money *service_provider::tax_outside_call(customer &client, const phone &other_side, call_duration duration)
{
    return tax_call_per_minute(duration, get_outside_call_cost());
}

money *service_provider::tax_call_per_minute(call_duration duration, const money &cost_per_minute)
{
    int add_additional_minute = 0;
    long long duration_in_seconds = std::chrono::seconds(int(duration.count())).count();
    if ((duration_in_seconds % 60) > 0)
        add_additional_minute = 1;

    float call_cost = cost_per_minute.get_amount_in_dollars() * (duration_in_seconds / 60);
    call_cost += add_additional_minute * cost_per_minute.get_amount_in_dollars();
    return tax_local_currency_amount(call_cost);
}

bool service_provider::is_home_network_number(const phone &number)
{ // TODO: unhardcode this
    if (0 == number.get_number().compare(0, 3, "050")) {
        return true;
    } else if (0 == number.get_number().compare(0, 3, "066")) {
        return true;
    } else if (0 == number.get_number().compare(0, 3, "095")) {
        return true;
    } else if (0 == number.get_number().compare(0, 3, "099")) {
        return true;
    }

    return false;
}

void service_provider::update_total_time_talked(customer &client, call_duration duration)
{
    auto new_total_time_talked = client.get_total_time_talked() + duration;
    client.set_total_time_talked(new_total_time_talked);
}

call_duration service_provider::tax_weekend_call(call_duration duration)
{
    if (duration < FREE_TIME_FOR_WEEKEND_CALLS) {
        return call_duration(std::chrono::minutes(0));
    } else {
        return duration - FREE_TIME_FOR_WEEKEND_CALLS;
    }
}

money *service_provider::tax_call(customer &client, const phone &other_side, call_duration duration, bool is_weekday)
{
    call_duration real_duration = duration;

    update_total_time_talked(client, duration);

    if (is_weekday)
        real_duration = tax_weekend_call(duration);

    if (real_duration == call_duration(std::chrono::minutes(0)))
        return tax_local_currency_amount(0.0f);

    if (is_home_network_number(other_side))
        return tax_inside_call(client, other_side, duration);

# if 0
    if (customers_.size() > 0)
        for (auto &itr : customers_)
            if (itr.c_.get_personal_number() == other_side)
                return tax_inside_call(client, other_side, duration);
# endif

    return tax_outside_call(client, other_side, duration);
}


service_provider::service_provider(float base_call_cost, float inside_call_cost, float outside_call_cost)
    : base_call_cost_(base_call_cost)
    , inside_call_cost_(inside_call_cost)
    , outside_call_cost_(outside_call_cost)
    , customers_()
{
}

