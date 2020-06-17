#pragma once
#include <string>
#include <iostream>
#include <utility>
const int isNULL = 1; //null pointer
const int isVALID = 2; //valid pointer--can be deref
const int isUNPREDICTABLE = 3; //uninitialized pointer or pointer after free/delete
const int isFREED = 4;//pointer that has been freed
class Pointer
{
public:
    Pointer() = default;
    Pointer(const Pointer& p) = default;
    static int numsOfPointer;
    static void numsAdd()
    {
        ++numsOfPointer;
    }
    explicit Pointer(std::string name):
    _name(std::move(name)),
    _state(isUNPREDICTABLE),
    _id(numsOfPointer),
    _isnewed(false)
    {
        numsAdd();
    }
    Pointer(std::string name,const int &ps):
    _name(std::move(name)),
    _state(ps),
    _id(numsOfPointer),
    _isnewed(false)
    {
        numsAdd();
    }
    Pointer(std::string name,const int &ps,const bool &nw):
    _name(std::move(name)),
    _state(ps),
    _id(numsOfPointer),
    _isnewed(nw)
    {
        numsAdd();
    }
    int getState() const
    {
        return _state;
    }
    int getid() const
    {
        return _id;
    }
    bool getNew() const
    {
        return _isnewed;
    }
    std::string getName() const
    {
        return _name;
    }
    int setState(int newState)
    {
        if(newState >=1 && newState <=4) _state=newState;
        else return -1;
        return 1;
    }
    void setisNew(bool b)
    {
        _isnewed=b;
    }
    bool operator==(Pointer& rhs) const {return this->_id==rhs.getid();}
    bool operator!=(Pointer& rhs) const {return !(*this==rhs);}
    void dump()
    {
        std::cout<<"___POINTER DUMP____"<<std::endl;
        std::string strstate;
        switch(this->_state)
        {
            case 1:strstate="isNULL";break;
            case 2:strstate="isVALID";break;
            case 3:strstate="isUNP";break;
            case 4:strstate="isFREED";break;
            default:break;
        }
        std::cout<<"P name:"<<_name<<" \\P state:"<<strstate<<" \\P id:"<<_id;
        if(_isnewed) std::cout<<"  NEWED";
        std::cout<<'\n';
    }
private:
    std::string _name;
    bool _isnewed;
    int _state;
    int _id;
};