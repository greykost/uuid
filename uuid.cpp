#include "uuid.h"


static std::vector<std::string> split(const std::string &str, const char sep);

static std::string unite(const std::vector<std::string> &list, const char sep);



UUID::UUID() : key(""), sep('-'), correct(false)
{

}


void UUID::set(const std::string &id)
{
    const std::lock_guard<std::mutex> lock(mtx);

    correct = check(id);

    if(correct)
        key = id;
    else
        std::cout << "Error ID format!!! " << id << std::endl;
}


std::string UUID::inc()
{
    const std::lock_guard<std::mutex> lock(mtx);

    std::vector<std::string> pairs = split(key, sep);

    const char maxnum = nums[nums.length() - 1];
    const char maxsym = syms[syms.length() - 1];

    if(pairs.size() == 1)
    {
        size_t pos = 0;
        const char cursym = pairs[pos][0];
        const char curnum = pairs[pos][1];

        if(curnum < maxnum)
        {
            pairs[pos][1] = nextnum(curnum);
        }
        else if(curnum == maxnum)
        {
            if(cursym < maxsym)//  < 'Z'
            {
                pairs[pos][0] = nextsym(cursym);
                pairs[pos][1] = nums[0];
            }
            else if(cursym == maxsym) // == 'Z' Следующий после «Z9» имеет вид «A1-A1», потом «A1-A2» и так далее.
            {
                pairs[pos][0] = syms[0];
                pairs[pos][1] = nums[0];
                std::string s = "";
                s.append(&syms[0], 1);
                s.append(&nums[0], 1);
                pairs.push_back(s);
            }
        }
    }
    else if(pairs.size() > 1)
    {
        size_t pos = pairs.size() - 1;
        size_t prevpos = pos - 1;
        const char cursym = pairs[pos][0];
        const char curnum = pairs[pos][1];

        if(curnum < maxnum) // < 9
        {
            pairs[pos][1] = nextnum(curnum);
        }
        else if(curnum == maxnum) // == 9
        {
            if(cursym < maxsym) // < 'Z'
            {
                pairs[pos][0] = nextsym(cursym);
                pairs[pos][1] = nums[0];
            }
            else if(cursym == maxsym) // == 'Z' После «A1-Z9» следует «A2-A1».
            {
                // Предыдущая пара
                const char prevsym = pairs[prevpos][0];
                const char prevnum = pairs[prevpos][1];

                if(prevsym < maxsym) // < 'Z'
                {
                    if(prevnum < maxnum)
                    {
                        pairs[prevpos][1] = nextnum(prevnum);
                    }
                    else if(prevnum == maxnum)
                    {
                        pairs[prevpos][0] = nextsym(prevsym);
                        pairs[prevpos][1] = nums[0];
                    }

                    pairs[pos][0] = syms[0];
                    pairs[pos][1] = nums[0];
                }
                else if(prevsym == maxsym) // == 'Z'
                {
                    if(prevnum < maxnum)
                    {
                        pairs[prevpos][1] = nextnum(prevnum);
                        pairs[pos][0] = syms[0];
                        pairs[pos][1] = nums[0];
                    }
                    else if(prevnum == maxnum)
                    {
                        if(pairs.size() < 10)
                        {
                            pairs[pos][0] = syms[0];
                            pairs[pos][1] = nums[0];
                            std::string s = "";
                            s.append(&syms[0], 1);
                            s.append(&nums[0], 1);
                            pairs.push_back(s);
                        }
                        else
                            return key;
                    }
                }
            }
        }
    }

    key = unite(pairs, sep);

    return key;
}


bool UUID::check(const std::string &id)
{
    std::vector<std::string> strings = split(id, sep);

    if(strings.empty() || strings.size() > 10)
        return false;

    for(size_t i = 0; i < strings.size(); i++)
    {
        if(strlen(strings.at(i).c_str()) != 2)
            return false;

        if((syms.find(strings.at(i)[0], 0) == std::string::npos) || (nums.find(strings.at(i)[1], 0) == std::string::npos))
            return false;
    }

    return true;
}


static std::vector<std::string> split(const std::string &str, const char sep)
{
    std::vector<std::string> list;
    std::istringstream iss(str);
    std::string line;

    while (getline(iss, line, sep))
    {
        list.push_back(line);
    }

    return list;
}


static std::string unite(const std::vector<std::string> &list, const char sep)
{
    std::string s = "";

    for(size_t i = 0; i < list.size(); i++)
    {
        s.append(list.at(i));
        s.append(&sep, 1);
    }

    if(!s.empty())
        s.erase(s.length() - 1, 1);

    return s;
}

