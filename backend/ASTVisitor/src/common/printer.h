#pragma once
#include <fstream>
class printer
{
public:
    printer()
    {

    }
    void pprint(const std::string& s)
    {
        of<<s;
    }
protected:
    static std::ofstream of;
};