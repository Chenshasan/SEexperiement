#include "function.h"
#include "../pointerChecker/pointer.h"
#include "../pointerChecker/printer.h"
class FunctionChecker:public printer
{
public:
    astFunc* getFuncByName(std::string funcName)
    {
        for(auto af:funcs)
        {
            if(af->getName()==funcName)
                return af;
        }
        return nullptr;
    }
    void func2DerefCheck(std::string funcName,std::vector<Pointer*> params)
    {
        astFunc* af=getFuncByName(funcName);
        if(params.size()!=af->getParamNum()) return;
        for(int i=0;i<params.size();++i)
        {
            if(params[i]->getState()==isNULL && af->getm2d(i))
                of<<"func null deref detected "<<params[i]->getName();
            if(params[i]->getState()==isFREED && af->getmnd(i))
                of<<"func 2 free detected "<<params[i]->getName();
        }
    }
    void setFunc(astFunc* a)
    {
        funcs.push_back(a);
    }
private:
    std::vector<astFunc*> funcs;
};