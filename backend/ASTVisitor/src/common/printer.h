#pragma once
#include <fstream>
class Printer
{
public:
    Printer()
    {
    }
    void pprint(const std::string &s)
    {
        of << s;
    }

protected:
    static std::ofstream of;
};