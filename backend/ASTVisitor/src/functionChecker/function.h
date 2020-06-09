#pragma once
#include <string>
#include <vector>
#include <map>
class astFunc
{
public:
    astFunc(std::string givName,int gvpn):funcName(givName),\
    paramNum(gvpn),maybe2Deref(false),maybeNullDeref(false)
    {
        for(int i=0;i<paramNum;++i)
        {
            PointerParam.push_back(false);
            nullType.push_back(false);
            derefType.push_back(false);
        }
    }
    astFunc()=delete;
    astFunc(const astFunc& af)=delete;
    void setIndByName(std::string pname,int ind)
    {
        name2ind[pname]=ind;
    }
    int getIndByName(std::string pname)
    {
        return name2ind[pname];
    }
    bool getm2d()
    {
        return maybe2Deref;
    }
    bool getm2d(int ind)
    {
        return derefType[ind];
    }
    bool getmnd()
    {
        return maybeNullDeref;
    }
    bool getmnd(int ind)
    {
        return nullType[ind];
    }
    void setm2d(bool b)
    {
        maybe2Deref=b;
    }
    void setm2d(bool b,int i)
    {
        derefType[i]=b;
    }
    void setmnd(bool b)
    {
        maybeNullDeref=b;
    }
    void setmnd(bool b,int i)
    {
        nullType[i]=b;
    }
    std::string getName(){return funcName;}
    int getParamNum(){return paramNum;}
private:
    const std::string funcName;
    const int paramNum;
    std::vector<bool> PointerParam;
    bool maybe2Deref;
    bool maybeNullDeref;
    std::vector<bool> derefType;
    std::vector<bool> nullType;
    std::map<std::string,int> name2ind;
};