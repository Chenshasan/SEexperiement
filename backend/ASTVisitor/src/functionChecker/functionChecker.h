#include "func.h"
using FCMap = std::map<std::string, SPAFunc *>;
class FuncChecker
{
public:
    FuncChecker()
    {
        fcMap.clear();
        curFuncName = "";
        processingFunc = nullptr;
    }
    FuncChecker(const FuncChecker &fc) = delete;
    void stepInFunc(std::string funcName, std::vector<std::string> pm)
    {
        if (processingFunc == nullptr) //first function
        {
            processingFunc = new SPAFunc(funcName, pm);
        }
        else
        {
            fcMap[curFuncName] = processingFunc;
            processingFunc = new SPAFunc(funcName, pm);
        }
        curFuncName = funcName;
    }
    void setProcFuncPtrFreeByName(std::string pname)
    {
        processingFunc->freePtrByName(pname);
    }
    void setProcFuncPtrNullByName(std::string pname)
    {
        processingFunc->nullPtrByName(pname);
    }
    bool isFuncParamFreeByFuncNameAndIndex(std::string fname, int index)
    {
        if (fcMap.count(fname) < 1)
        {
            std::cout << "ERR calling non exist function\n";
            exit(-1);
        }
        SPAFunc *f = fcMap[fname];
        return f->isNthParamFreed(index);
    }
    bool isFuncParamNullByFuncNameAndIndex(std::string fname, int index)
    {
        if (fcMap.count(fname) < 1)
        {
            std::cout << "ERR calling non exist function\n";
            exit(-1);
        }
        SPAFunc *f = fcMap[fname];
        return f->isNthParamNulled(index);
    }
    bool isProcFuncHasParam(std::string name)
    {
        return processingFunc->funcHasParam(name);
    }

private:
    SPAFunc *processingFunc;
    std::string curFuncName;
    FCMap fcMap;
};