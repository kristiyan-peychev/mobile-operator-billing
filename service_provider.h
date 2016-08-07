#if !defined(__Provider__)
# define __Provider__
# pragma once

# include "money.h"
# include "phone.h"
# include "stdafx.h"
# include <set>

class service_provider {
public:
    class customer {
        service_provider *provider_ {nullptr};
        phone personal_number_;

        call_duration total_time_talked_;
        call_duration time_talked_inside_operator_;

        std::chrono::time_point<std::chrono::system_clock> date_last_paid_;

        MAKE_GETTER(provider);
        MAKE_GETTER(personal_number);
        MAKE_SETTER(personal_number);
        MAKE_GETTER(time_talked_inside_operator);
        MAKE_SETTER(time_talked_inside_operator);
        MAKE_GETTER(total_time_talked);
        MAKE_SETTER(total_time_talked);
        MAKE_GETTER(date_last_paid);

    protected:
        bool register_to_provider(service_provider *provider);
        void unregister_from_provider();
    public:
        bool operator < (const customer &other) {
            return personal_number_.get_number() < other.get_personal_number().get_number();
        }
        bool operator == (const customer &other) {
            return personal_number_.get_number() == other.get_personal_number().get_number();
        }
        bool operator == (const phone &other) {
            return personal_number_.get_number() == other.get_number();
        }
    public:
        virtual ~customer() = default;
        customer() = default;
        virtual money* make_call(call_duration duration, const phone &other_side, bool is_weekend);
    };

protected:
    dollar base_call_cost_;
    MAKE_GETTER(base_call_cost);
    dollar inside_call_cost_;
    MAKE_GETTER(inside_call_cost);
    dollar outside_call_cost_;
    MAKE_GETTER(outside_call_cost);
protected:
    struct customer_ref {
        customer &c_;
        operator customer&() { return c_; }
        customer_ref(customer &c) : c_(c) { }
        customer_ref() = delete;
    };
    std::set<customer_ref> customers_;
    bool make_registration(customer &account);
    void delete_registration(customer &account);
protected:
    money *tax_local_currency_amount(float amount);
protected:
    void update_total_time_talked(customer &client, call_duration duration);
    call_duration tax_weekend_call(call_duration duration);
    money *tax_inside_call(customer &client, const phone &other_side, call_duration duration);
    money *tax_outside_call(customer &client, const phone &other_side, call_duration duration);
    money *tax_call_per_minute(call_duration duration, const money &cost_per_minute);
protected:
    bool is_home_network_number(const phone &number);
public:
    money *tax_call(customer &client, const phone &other_side, call_duration duration, bool is_weekday);
public:
  ~service_provider() = default;
   service_provider(float base_call_cost = SERVICE_PROVIDER_BASE_CALL_COST,
                    float inside_call_cost = SERVICE_PROVIDER_INSIDE_CALL_COST,
                    float outside_call_cost = SERVICE_PROVIDER_OUTSIDE_CALL_COST);

public:
   friend bool operator<(const service_provider::customer_ref &first, const service_provider::customer_ref &second);
   friend bool operator==(const service_provider::customer_ref &first, const service_provider::customer_ref &second);
   friend bool operator==(const service_provider::customer_ref &first, const phone &second);
};

#endif
