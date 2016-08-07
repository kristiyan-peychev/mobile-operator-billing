# pragma once

# include <chrono>

typedef std::chrono::duration<float> call_duration;
typedef std::chrono::time_point<std::chrono::system_clock> call_timestamp;

#define FREE_MINUTES_FOR_INSIDE_CALLS 30
#define FREE_TIME_FOR_INSIDE_CALLS call_duration(std::chrono::minutes(FREE_MINUTES_FOR_INSIDE_CALLS))
#define FREE_MINUTES_FOR_WEEKEND_CALLS 5
#define FREE_TIME_FOR_WEEKEND_CALLS call_duration(std::chrono::minutes(FREE_MINUTES_FOR_WEEKEND_CALLS))
#define SERVICE_PROVIDER_BASE_CALL_COST 0.33f
#define SERVICE_PROVIDER_INSIDE_CALL_COST 0.50f
#define SERVICE_PROVIDER_OUTSIDE_CALL_COST 0.95f

#define MAKE_GETTER(x) public: const decltype(x ## _) &get_ ## x () const { return x ## _; }
#define MAKE_SETTER(x) public: void set_ ## x (decltype(x ## _) &val) { x ## _ = val; }
#define MAKE_SETTER_NONREF(x) public: void set_ ## x (decltype(x ## _) val) { x ## _ = val; }
