#include "pointerChecker.h"

int PointerChecker::nullDerefCheck(Pointer p2deref)
{
    if(pMap[p2deref.getid()]->getState()!=isVALID)
    {
        std::cout<<"Warning: Deref a null or uninitialized pointer : "<<p2deref.getName();
        pprint("Warning: Deref a null or uninitialized pointer : ");
        pprint(p2deref.getName());
        return -1;
    }
    return 1;
}
int PointerChecker::freeCheck(Pointer p2free)
{
    if(p2free.getNew() && p2free.getState()==isVALID) return 1;
    else if(p2free.getNew() && p2free.getState()==isFREED)
    {
        std::cout<<"Warning: Double free detected : "<<p2free.getName();
        pprint("Warning: Double free detected : ");
        return -1;
    }
    else
    {
        std::cout<<"Warning: Try to free a null ,uninit or a pointer that is not defined by new operator : "<<p2free.getName();
        pprint("Warning: Try to free a null ,uninit or a pointer that is not defined by new operator : ");
    }
    pprint(p2free.getName());
    return -1;
}
/*
    void declPointer(Pointer p2decl);
    void assignPointer(Pointer lhs,Pointer rhs);
*/
void PointerChecker::declPointer(Pointer* p2decl)
{
    pMap[p2decl->getid()]=p2decl;
    pMapByName[p2decl->getName()]=p2decl;
}
Pointer* PointerChecker::getPointerByName(const std::string& s)
{
    if(pMapByName.count(s)>0)
        return pMapByName.at(s);
    else
        return nullptr;
}
void PointerChecker::assignPointer(Pointer lhs,Pointer rhs)
{
    //UF.reset(lhs.getid());
    for(int i=0;i<UF_CAPACITY;++i)
    {
        if(UF.isConnected(i,lhs.getid()))
        {
            pMap[i]->setState(rhs.getState());
            pMap[i]->setisNew(rhs.getNew());
            UF.unionElement(i,rhs.getid());
        }
    }
    lhs.setState(rhs.getState());
    UF.unionElement(lhs.getid(),rhs.getid());
}
void PointerChecker::freePointer(Pointer p2free,bool &success)
{
    success=false;
    if(freeCheck(p2free)<0) return;
    else for(int i=0;i<UF_CAPACITY;++i)
    {
        if(UF.isConnected(i,p2free.getid()))
        {
            pMap[i]->setState(isFREED);
            pMap[i]->setisNew(true);
        }
    }
    success=true;
}