#include "pointerChecker.h"

int PointerChecker::nullDerefCheck(const Pointer &p2deref)
{
    PIMap *pMap = pIndexMaps[currentFuncName];
    if (pMap->at(p2deref.getid())->getState() != isVALID)
    {
        std::string msg = "Warning: Deref a null or uninitialized pointer : " + p2deref.getName();
        msg += "  @func: ";
        msg += getFuncName();
        msg += '\n';
        std::cout << msg;
        pprint(msg);
        return -1;
    }
    return 1;
}
int PointerChecker::freeCheck(const Pointer &p2free)
{
    if (p2free.getNew() && p2free.getState() == isVALID)
        return 1;
    else if (p2free.getNew() && p2free.getState() == isFREED)
    {
        std::string msg = "Warning: Double free detected : " + p2free.getName();
        msg += "  @func: ";
        msg += getFuncName();
        msg += '\n';
        std::cout << msg;
        pprint(msg);
        //return -1;
    }
    else
    {
        std::string msg = "Warning: Try to free a null ,uninit or a pointer that is not defined by new operator : " + p2free.getName();
        msg += "  @func: ";
        msg += getFuncName();
        msg += '\n';
        std::cout << msg;
        pprint(msg);
    }
    return -1;
}
/*
    void declPointer(Pointer p2decl);
    void assignPointer(Pointer lhs,Pointer rhs);
*/
void PointerChecker::declPointer(Pointer *p2decl)
{
    PIMap *pMap = pIndexMaps[currentFuncName];
    pMap->insert(std::make_pair(p2decl->getid(), p2decl));
    //pMap->at(p2decl->getid()) = p2decl;
    if (pMapsByName.count(currentFuncName) > 0)
    {
        auto pMapByName = pMapsByName.at(currentFuncName);
        pMapByName->insert(std::make_pair(p2decl->getName(), p2decl));
        //pMapByName[p2decl->getName()] = p2decl;
    }
    else
    {
        std::cout << "ERROR: pMapsName doesn't contains this Map" << std::endl;
        exit(-1);
    }
}
Pointer *PointerChecker::getPointerByName(const std::string &s)
{
    if (pMapsByName.count(currentFuncName) > 0)
    {
        auto pMapByName = pMapsByName.at(currentFuncName);
        if (pMapByName->count(s) > 0)
            return pMapByName->at(s);
        else
            return nullptr;
    }
    else
    {
        std::cout << "ERROR: pMapsName doesn't contains this Map" << std::endl;
        exit(-1);
    }
}
void PointerChecker::assignPointer(Pointer lhs, const Pointer &rhs)
{
    //UF.reset(lhs.getid());
    if (UFMapByName.count(currentFuncName) > 0)
    {
        PIMap *pMap = pIndexMaps[currentFuncName];
        UnionFind *UF = UFMapByName.at(currentFuncName);
        for (int i = 0; i < UF_CAPACITY; ++i)
        {
            if (UF->isConnected(i, lhs.getid()))
            {
                pMap->at(i)->setState(rhs.getState());
                pMap->at(i)->setisNew(rhs.getNew());
                UF->unionElement(i, rhs.getid());
            }
        }
        lhs.setState(rhs.getState());
        UF->unionElement(lhs.getid(), rhs.getid());
    }
    else
    {
        std::cout << "ERROR: UFMap doesn't contains this UF" << std::endl;
        exit(-1);
    }
}
void PointerChecker::freePointer(const Pointer &p2free, bool &success)
{
    success = false;
    if (UFMapByName.count(currentFuncName) > 0)
    {
        PIMap *pMap = pIndexMaps[currentFuncName];
        UnionFind *UF = UFMapByName.at(currentFuncName);
        if (freeCheck(p2free) < 0)
            return;
        else
            for (int i = 0; i < UF_CAPACITY; ++i)
            {
                if (UF->isConnected(i, p2free.getid()))
                {
                    pMap->at(i)->setState(isFREED);
                    pMap->at(i)->setisNew(true);
                }
            }
        success = true;
    }
    else
    {
        std::cout << "ERROR: UFMap doesn't contains this UF" << std::endl;
        exit(-1);
    }
}
void PointerChecker::stepInFunc(const std::string &funcName)
{
    currentFuncName = funcName;
    UnionFind *UF = new UnionFind(UF_CAPACITY);
    UFMapByName[currentFuncName] = UF;
    PNMap *NM = new PNMap;
    NM->clear();
    pMapsByName[currentFuncName] = NM;
    PIMap *IM = new PIMap;
    IM->clear();
    pIndexMaps[currentFuncName] = IM;
}