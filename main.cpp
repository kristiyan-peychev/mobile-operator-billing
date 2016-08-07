// luxoft.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "service_provider.h"
#include "account.h"
//#include "Windows.h"

int main()
{
    service_provider prov;
    phone acc1_phone = "1234";
    phone acc2_phone = "050123421234";
    account acc1(&prov, acc1_phone);
    account acc2(&prov, acc2_phone);
    auto first = std::chrono::system_clock::now();
  //  Sleep(2);
    auto *EMONE = acc1.make_call(acc2.get_personal_number(), first, std::chrono::system_clock::now());
    EMONE->get_amount_in_dollars();

    return 0;
}

