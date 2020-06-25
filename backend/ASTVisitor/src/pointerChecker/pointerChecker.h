#include <map>
#include "pointer.h"
#include "../common/printer.h"
#include "../common/unionFind.h"
const int UF_CAPACITY = 100;
using PNMap = std::map<std::string, Pointer *>;
using UFMap = std::map<std::string, UnionFind *>;
using PIMap = std::map<int, Pointer *>;
class PointerChecker : public printer
{
public:
    PointerChecker()
    {
        if (!of.is_open())
            of.open("warn.dat", std::ios::out | std::ios::trunc);
        pIndexMaps.clear();
        pMapsByName.clear();
    }
    ~PointerChecker()
    {
        for (auto iter = UFMapByName.begin(); iter != UFMapByName.end(); ++iter)
        {
            delete iter->second;
        }
        for (auto iter = pMapsByName.begin(); iter != pMapsByName.end(); ++iter)
        {
            delete iter->second;
        }
    }
    PointerChecker(const PointerChecker &pc) = delete;
    int nullDerefCheck(const Pointer &p2deref, const std::string &locString);
    int freeCheck(const Pointer &p2free, const std::string &locString);
    void declPointer(Pointer *p2decl);
    Pointer *getPointerByName(const std::string &s);
    void assignPointer(Pointer lhs, const Pointer &rhs);
    void freePointer(const Pointer &p2free, bool &success, const std::string &locString);
        std::string getFuncName() { return currentFuncName; }
    void stepInFunc(const std::string &funcName);

private:
    //UnionFind UF;
    std::string currentFuncName;
    UFMap UFMapByName;
    std::map<std::string, PIMap *> pIndexMaps;
    //std::map<int, Pointer *> pMap;
    std::map<std::string, PNMap *> pMapsByName;
};