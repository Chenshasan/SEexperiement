#include <iostream>
#include <sstream>

#include "clang/AST/Stmt.h"
#include "clang/AST/Type.h"
#include "clang/AST/RecursiveASTVisitor.h"

#include "myASTVisitor.h"
#include "switchChecker.h"

using namespace clang;
using namespace std;

SwitchChecker::SwitchChecker()
{
  if (!of.is_open()) of.open("warn.dat", std::ios::out | std::ios::trunc);
}

void SwitchChecker::typeMismatchCheck(SwitchStmt* ss)
{
  QualType condType = ss->getCond()->getType();
  for (SwitchCase* sc = ss->getSwitchCaseList(); sc; sc = sc->getNextSwitchCase())
  {
    if (isa<CaseStmt>(sc))
    {
      CaseStmt* cs = cast<CaseStmt>(sc);
      if (cs->getLHS()->IgnoreImpCasts()->getType() != condType)
      {
        SourceLocation beginLoc = cs->getBeginLoc();
        string locString = beginLoc.printToString(*SM);
        cout << "Warning: switch case type mismatches cond type::" << locString.c_str() << endl;
        stringstream ssr;
        ssr << "Warning: switch case type mismatches cond type::" << locString.c_str() << endl;
        pprint(ssr.str());
      }
    }
  }
}