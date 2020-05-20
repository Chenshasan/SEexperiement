#include <map>
#include "pointer.h"
#include "unionFind.h"
const int UF_CAPACITY=100;
class PointerChecker
{
public:
    PointerChecker():UF(UF_CAPACITY)
    {
        pMap.clear();
    }
    PointerChecker(const PointerChecker& pc) = delete;
    int nullDerefCheck(Pointer p2deref);
    int freeCheck(Pointer p2free);
    void declPointer(Pointer* p2decl);
    Pointer* getPointerByName(const std::string& s);
    void assignPointer(Pointer lhs,Pointer rhs);
    void freePointer(Pointer p2free,bool &success);
private:
    UnionFind UF;
    std::map<int,Pointer*> pMap;
    std::map<std::string,Pointer*> pMapByName;
};