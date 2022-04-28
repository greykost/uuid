#ifndef UUID_H
#define UUID_H

#include <string>
#include <string.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <mutex>

class UUID
{
public:
    UUID();
    const std::string syms = "ABCEHIKLNOPRSTUWXYZ";
    const std::string nums = "123456789";

    void set(const std::string &id);
    std::string inc();

private:
    char nextsym(char sym) { return syms[syms.find(sym, 0) + 1]; }
    char nextnum(char num) { return nums[nums.find(num, 0) + 1]; }
    bool check(const std::string &id);

private:
    std::mutex mtx;
    std::string key;
    char sep;
    bool correct;
};

#endif // UUID_H
