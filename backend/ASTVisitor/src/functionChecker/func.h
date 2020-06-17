#pragma once
#include <map>
#include <string>
#include <vector>
#include <iostream>
#include "paramCond.h"
using PStateMap = std::map<std::string, int>;
class SPAFunc
{
public:
    SPAFunc() = delete;
    SPAFunc(const SPAFunc &sf) = delete;
    explicit SPAFunc(const std::string &gfuncName,
                     std::vector<std::string> paramNames) : funcName(gfuncName)
    {
        ParameterCond initPC;
        initPC.isFreed = initPC.isNulled = false;
        size_t ptrParamNum = paramNames.size();
        for (int i = 0; i != ptrParamNum; ++i)
        {
            pointParamConds.push_back(initPC);
            pointParamMap[paramNames.at(i)] = i;
        }
    }
    bool isNthParamFreed(int nTH)
    {
        return pointParamConds.at(nTH).isFreed;
    }
    bool isNthParamNulled(int nTH)
    {
        return pointParamConds.at(nTH).isNulled;
    }
    void freePtrByName(std::string name)
    {
        if (pointParamMap.count(name) < 1)
        {
            std::cout << "ERR Func has no param name " << name;
            exit(-1);
        }
        pointParamConds.at(pointParamMap[name]).isFreed = true;
    }
    void nullPtrByName(std::string name)
    {
        if (pointParamMap.count(name) < 1)
        {
            std::cout << "ERR Func has no param name " << name;
            exit(-1);
        }
        pointParamConds.at(pointParamMap[name]).isNulled = true;
    }
    bool funcHasParam(std::string name)
    {
        bool ret=(pointParamMap.count(name) >= 1);
        return ret;
    }
private:
    std::string funcName;
    PStateMap pointParamMap;
    std::vector<ParameterCond> pointParamConds;
};