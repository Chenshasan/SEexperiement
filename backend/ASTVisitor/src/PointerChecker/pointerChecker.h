#include <map>
#include "pointer.h"
#include "../Common/printer.h"
#include "../Common/unionFind.h"
/*
 * 并查集的大小，在初始化并查集的时候提供给它的构造函数
 */
const int UF_CAPACITY = 100;

using PNMap = std::map<std::string, Pointer *>;
using UFMap = std::map<std::string, UnionFind *>;
using PIMap = std::map<int, Pointer *>;


class PointerChecker : public Printer
{
/*
 * PointerChecker类 检测指针问题
 * 作者：刘浩文
 */
public:
/*
 * 构造函数，检测错误文件的打开程度，如果没有打开的话就打开这个文件
 */
    PointerChecker()
    {
        if (!of.is_open())
            of.open("warn.dat", std::ios::out | std::ios::trunc);
        pIndexMaps.clear();
        pMapsByName.clear();
    }
/*
 * 析构函数
 */
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
/* 
 * 逻辑上不允许拷贝构造函数，通知编译器禁用拷贝构造函数
 */
    PointerChecker(const PointerChecker &pc) = delete;
/* 
 * 检查空指针解引用函数
 * @param p2deref 待检查的指针的引用
 * @param locstring 输出用的定位字符串
 * @return 1：成功 -1：失败，出现问题
 */
    int nullDerefCheck(const Pointer &p2deref, const std::string &locString);
/* 
 * 检查空指针解引用函数
 * @param p2deref 待检查的指针的引用
 * @param locstring 输出用的定位字符串
 * @return 1：成功 -1：失败
 */
    int freeCheck(const Pointer &p2free, const std::string &locString);
/* 
 * 声明一个指针
 * @param p2decl 待声明的一个指针的指针
 */
    void declPointer(Pointer *p2decl);
/* 
 * 根据名称得到一个指针的指针
 * @param s 这个指针的名字
 * @return 指针的指针 nullptr表示没有这个名字的指针
 */
    Pointer *getPointerByName(const std::string &s);
/* 
 * 指针之间的赋值
 * @param lhs，rhs 左右手侧的指针
 * lhs=rhs
 */
    void assignPointer(Pointer lhs, const Pointer &rhs);
/* 
 * 释放一个指针
 * @param p2free 需要释放的指针的名称
 * @param success 表示释放成功与否的引用
 * @param locstring 输出用的定位字符串
 */
    void freePointer(const Pointer &p2free, bool &success, const std::string &locString);
/* 
 * 得到当前正在处理的函数的名称
 */
    std::string getFuncName() { return currentFuncName; }
/* 
 * 进入一个函数，对于当前处理的函数和map进行一些操作
 * @param funcName 即将进入的函数的名称
 */
    void stepInFunc(const std::string &funcName);

private:
    //UnionFind UF;

    /* 
    * 当前正在处理的函数名称
    */
    std::string currentFuncName;
    /* 
    * map结构 K=函数名 V=指向一个并查集的指针
    * 指引某个函数的并查集
    */ 
    UFMap UFMapByName;
    /* 
    * map结构 K=函数名 V=另一个map[K=某个函数内的指针序号 V=这个指针的指针]
    * 指引某个函数的根据序号寻找指针的索引
    */ 
    std::map<std::string, PIMap *> pIndexMaps;
    /* 
    * map结构 K=函数名 V=另一个map[K=某个函数内的指针名称 V=这个指针的指针]
    * 指引某个函数的根据名称寻找指针的索引
    */ 
    std::map<std::string, PNMap *> pMapsByName;
};