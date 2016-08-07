#pragma once

class money {
public:
    virtual ~money() { }
public:
    virtual float get_amount_in_dollars() const = 0;
};

class dollar : public money {
public:
    float amount_ {0.0f};
    float get_amount_in_dollars() const override { return amount_; }
public:
    dollar(float dollars) : amount_(dollars) { }
};

