#if !defined(__Account__)
# define __Account__
# pragma once

# include "stdafx.h"
# include "phone.h"
# include "service_provider.h"
# include <chrono>

class account : public service_provider::customer {
public:
   ~account();
    account(service_provider *provider, phone& number);
public:
    money *make_call(const phone &other_side_number, const call_timestamp &start_time, const call_timestamp &end_time);
};

#endif
