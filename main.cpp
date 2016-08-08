#include "stdafx.h"
#include "service_provider.h"
#include "account.h"
#include <sstream>
#include <cassert>
#include <iostream>
#include <map>
#include <vector>
#include <utility>
#include <functional>
#include <cstdlib>

std::vector<account*> g_accounts;
service_provider g_operator1;

static size_t get_till_character(const char *string, char c)
{
    size_t ret = 0;
    while (string && *string && *string != c) {
        ++string;
        ++ret;
    }

    return ret;
}

static auto get_till_space = std::bind(get_till_character, std::placeholders::_1,' ');

static size_t shell_add(const char *string, std::string &message)
{
    size_t offset = get_till_character(string, ' ');
    if (offset == 0)
        return 0;

    std::string phone_number(string, offset);
    phone _phone(phone_number);

    auto *acc = new account(&g_operator1, _phone);
    size_t idx = g_accounts.size();
    g_accounts.push_back(acc);

    std::stringstream str;
    str << "New account(" << phone_number << ") index is " << idx << std::endl;
    message = str.str();

    return offset + 1;
}

static int get_index(const char *string, std::string &message, size_t &off)
{
    size_t offset = get_till_space(string);
    if (offset == 0)
        return -1;

    for (size_t i = 0; i < offset; ++i) {
        if (0 == isdigit(string[i])) {
            message = "Index missing\n";
            return -1;
        }
    }

    std::string index(string, offset);
    int idx = std::stoi(index);
    if (size_t(idx) >= g_accounts.size()) {
        message = "Index is out of range\n";
        return -1;
    }

    off = offset;

    return idx;
}

static size_t shell_remove(const char *string, std::string &message)
{
    size_t offset = 0;
    int idx = get_index(string, message, offset);
    if (idx < 0)
        return 0;

    g_accounts.erase(g_accounts.begin() + idx);

    return 0;
}

// syntax: call <acc idx> <phone>
static size_t shell_call(const char *string, std::string &message)
{
    size_t offset = 0;
    int idx = get_index(string, message, offset);
    if (idx < 0)
        return 0;

    string += offset;
    std::string dial_phone(string + offset);

    auto start_time = std::chrono::system_clock::now();
    std::cout << "Type anything to end call...";
    char dummy;
    std::cin >> dummy;

    std::stringstream str;
    money *call_cost = g_accounts[idx]->make_call(dial_phone, start_time, std::chrono::system_clock::now());
    str << "Call cost is $" << call_cost->get_amount_in_dollars() << std::endl;
    message = str.str();

    return offset + 1;
}

static size_t shell_list(const char *string, std::string &message)
{
    std::stringstream str;
    size_t idx = 0;
    str << "Listing accounts\n";
    for (auto &itr : g_accounts)
        str << "Account index " << idx++ << " phone " << itr->get_personal_number().get_number() << std::endl;

    str << "Accounts listing finished\n";
    message = str.str();

    return 0;
}

static size_t shell_exit(const char *string, std::string &message)
{
    exit(0);
}

static size_t shell_help(const char *string, std::string &message)
{
    std::stringstream str;
    str << "Shell usage: \n"
        << "Command: add; syntax: `add <phone number>`; meaning: add an account from which we can call\n"
        << "Command: remove; syntax `remove <account index>`; meaning: remove an account from the accounts list\n"
        << "Command: call; syntax `call <account index> <phone number>; meaning: call a number from a selected account\n"
        << "Command: list; syntax `list`; meaning: list all the accounts\n"
        << "Command: exit; syntax `exit`; meaning: exit the shell\n"
        << "Command: help; syntax `help`; meaning: print this message\n";
    message = str.str();

    return 1;
}

static std::map<std::string, size_t (*)(const char *, std::string &)> g_tokens_map
{
    std::make_pair("add", shell_add),
    std::make_pair("remove", shell_remove),
    std::make_pair("call", shell_call),
    std::make_pair("list", shell_list),
    std::make_pair("exit", shell_exit),
    std::make_pair("help", shell_help),
};

static size_t parse_token(const char *string)
{
    size_t offset = get_till_character(string, ' ');
    if (offset == 0)
        return 0;

    std::string token(string, offset);
    string += offset;
    while (string && *string && *string == ' ') ++string;

    size_t token_result = 0;
    auto search_itr = g_tokens_map.find(token);
    if (search_itr != g_tokens_map.end()) {
        std::string message;
        token_result = search_itr->second(string, message);
        if (!message.empty())
            std::cout << message;
    } else {
        std::cout << "Command not found; type `help` for more info\n";
    }

    return offset + token_result;
}

bool handle_input(const char *input)
{
    const char *tokens = input;
    while (tokens) {
        while (tokens && *tokens == ' ') ++tokens;

        size_t offset = parse_token(tokens);
        if (offset == 0) 
            return false;

        tokens += offset;
    }
    return true;
}

int main()
{
    {
        std::string dummy;
        shell_help(nullptr, dummy);
        std::cout << dummy << std::endl;
    }

    char *buff = new char[42];
    while (std::cin) {
        using namespace std;
        cin.getline(buff, 42);
        handle_input(buff);
    }
    delete[] buff;

    return 0;
}

