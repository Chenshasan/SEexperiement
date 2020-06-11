#pragma once
#include "myASTVisitor.h"

#include "printer.h"

using namespace clang;
using namespace std;

class SlowMemoryChecker: public Printer
{

public:

    SlowMemoryChecker();

    SlowMemoryChecker(const SlowMemoryChecker& smc) = delete;

    void BinaryOperationCheck(BinaryOperator* bo,int BinaryOperatorLine,int ForStmtEndLine,int WhileStmtEndLine);

    bool sameType(QualType ltype,QualType rtype);

    void printCallExprName(CallExpr *c);

    void printCXXCallExprName(CXXOperatorCallExpr *c);

    int findWhileStmtEndLine(WhileStmt *ws);

    bool FindExprNameByToken(Expr *e);

};