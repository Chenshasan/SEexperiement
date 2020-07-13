/**
 * @file slowMemoryChecker.h
 * @author 叶宙果
 * @version v2
 */
#pragma once
#include "../myASTVisitor.h"

#include "../Common/printer.h"
#include "../Common/errNo.h"

#include <cstdio>
#include <memory>
#include <sstream>
#include <string>
#include <iostream>


#include "clang/Lex/Lexer.h"
#include "clang/Analysis/CFG.h"
#include "clang/Analysis/AnalysisDeclContext.h"
#include "clang/Analysis/FlowSensitive/DataflowValues.h"
#include "clang/Analysis/Analyses/LiveVariables.h"
#include "clang/AST/APValue.h"

#include "slowMemoryChecker.h"
#include "VariableChecker.h"

#include "readConfig.h"

using namespace clang;
using namespace std;



/**
 * @class ForStmtCondLoc
 * 存储条件语句位置的类
 */
class ForStmtCondLoc: public Printer
{

public:
    
/**
 * 构造函数
 */
    ForStmtCondLoc();
/**
 * 拷贝构造函数（已禁用）
 */
    ForStmtCondLoc(const ForStmtCondLoc& fscl) = delete;
/**
 * 入口函数
 * 寻找条件语句的各个位置
 * @param cond 获取到的条件语句
 */
    void getCondLoc(Expr *cond);
/**
 * 返回条件语句的开始行
 */
    int getBeginLine();
/**
 * 返回条件语句的开始列
 */
    int getBeginCol();
/**
 * 返回条件语句的终结行
 */
    int getEndLine();
/**
 * 返回条件语句的终结列
 */
    int getEndCol();

private:
/**
 * 条件语句的开始行号
 */
    int condBeginLine;
/**
 * 条件语句的开始列号
 */
    int condBeginCol;
/**
 * 条件语句的终结行号
 */
    int condEndLine;
/**
 * 条件语句的终结列号
 */
    int condEndCol;
/**
 * 寻找条件语句的开始行
 * @param cond 获取到的条件语句
 */
    int findCondBeginLine(Expr *cond);
/**
 * 寻找条件语句的开始列
 * @param cond 获取到的条件语句
 */
    int findCondBeginCol(Expr *cond);
/**
 * 寻找条件语句的终结行
 * @param cond 获取到的条件语句
 */
    int findCondEndLine(Expr *cond);
/**
 * 寻找条件语句的终结列
 * @param cond 获取到的条件语句
 */
    int findCondEndCol(Expr *cond);

};


/**
 * @class SlowMemoryChecker
 * 检测慢速内存操作的主类
 */
class SlowMemoryChecker: public Printer
{

public:

    SlowMemoryChecker();

    SlowMemoryChecker(const SlowMemoryChecker& smc) = delete;

    void BinaryOperationCheck(BinaryOperator* bo,int BinaryOperatorLine,int ForStmtEndLine,int WhileStmtEndLine);

    bool sameType(QualType ltype,QualType rtype);

    void printCallExprName(clang::CallExpr *c);

    void printCXXCallExprName(CXXOperatorCallExpr *c);

    int findWhileStmtEndLine(WhileStmt *ws);

    int findForStmtEndLine(ForStmt *fs);

    int findDoWhileStmtEndLine(DoStmt *ds);

    bool FindExprNameByToken(Expr *e);
/**
 * 最近遍历的条件语句
 */
    ForStmtCondLoc fscl;

    bool isBinaryOpratorInCond(BinaryOperator *bo);

    bool isAssignmentOpInForHead(BinaryOperator *bo);

    int getNumOfLoop(BinaryOperator *bo,int line,int col,int initVal);

    bool isComparisonOperator(BinaryOperator *bo);

    int getInitVal(BinaryOperator *bo);

    bool isVarDeclInForHead(VarDecl *vd);
/**
 * For语句初始化中的变量声明语句
 */
    VarDecl *ForInitVarDecl;
/**
 * 标示变量声明语句是否在For语句初始化语句中
 */
    bool VarDeclInForHead;

    int getInitValFromVarDecl(VarDecl *vd);
/**
 * For循环的循环次数
 */
    int numOfForLoop;
/**
 * While循环的循环次数
 */
    int numOfWhileLoop;
/**
 * DoWhile循环的循环次数
 */
    int numOfDoWhileLoop;
/**
 * 阈值
 * 判断是否进行告警的最小循环次数
 * 在构造函数中从配置文件读入
 * 如果没有配置文件则默认为1024
 */
    int minNumOfLoop;
/**
 * 标示目前语句是否在For循环中
 */
    bool inForStmt;
/**
 * 最近的For循环终结行号
 */
    int ForStmtEndLine;
/**
 * 标示目前语句是否在While循环中
 */
    bool inWhileStmt;
/**
 * 最近的While循环终结行号
 */
    int WhileStmtEndLine;
/**
 * 标示目前语句是否在DoWhile循环中
 */
    bool inDoWhileStmt;
/**
 * 最近的DoWhile循环终结行号
 */
    int DoWhileStmtEndLine;

    void BinaryOperationCheckInForStmt(BinaryOperator *bo);

    void BinaryOperationCheckInWhileStmt(BinaryOperator* bo);

    void BinaryOperationCheckInDoWhileStmt(BinaryOperator* bo);

    void setFlags(Stmt *stmt);

    int getNumOfDoWhileLoop(int line,int col,int initVal);
/**
 * DoWhile语句的条件语句
 * 因为在语句最后
 * 特殊处理
 */
    Expr *DoWhileCond;
/**
 * 配置文件读取工具类的实例
 */
    ReadConfig rc;

    void debug(const char * s);

    bool checkeFileName(SourceLocation beginLoc);

};
