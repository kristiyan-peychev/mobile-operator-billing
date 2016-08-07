#if !defined(__Phone__)
# define __Phone__
# pragma once

# include <string>
# include "stdafx.h"

class phone {
    std::string number_;
    MAKE_GETTER(number)
    MAKE_SETTER(number)
public:
    operator std::string() { return number_; }
public:
    phone() = default;
    phone(const char *str) : number_(str) { }
    phone(std::string &str) : number_(str) { }
};

#endif
