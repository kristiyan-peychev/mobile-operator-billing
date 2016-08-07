// luxoft.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "service_provider.h"
#include "account.h"
#include "Windows.h"

int main()
{
    service_provider prov;
    account acc1(&prov, phone("1234"));
    account acc2(&prov, phone("05012342"));
    auto first = std::chrono::system_clock::now();
    Sleep(2);
    auto *EMONE = acc1.make_call(acc2.get_personal_number(), first, std::chrono::system_clock::now());
    EMONE->get_amount_in_dollars();

    return 0;
}

