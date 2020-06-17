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
  vector<SwitchCase*> sces = getCaseByOrder(ss);
  for (const auto& sc : sces)
  {
    CaseStmt* cs = cast<CaseStmt>(sc);
    QualType caseType = cs->getLHS()->IgnoreImpCasts()->getType();
    if (!isCastAccepted(caseType, condType))
    {
      SourceLocation beginLoc = cs->getBeginLoc();
      string locString = beginLoc.printToString(*SM);
      cout << locString.c_str() << ':' << ' ' <<
        "warning: there is a mismatch between case type and cond type" << endl;
      stringstream ssr;
      ssr << locString.c_str() << ':' << ' ' <<
        "warning: there is a mismatch between case type and cond type" << endl;
      pprint(ssr.str());
    }
  }
}

vector<SwitchCase*> SwitchChecker::getCaseByOrder(SwitchStmt* ss)
{
  vector<SwitchCase*> ordered_sc;
  for (SwitchCase* sc = ss->getSwitchCaseList(); sc; sc = sc->getNextSwitchCase())
  {
    if (isa<CaseStmt>(sc)) {
      ordered_sc.insert(ordered_sc.begin(), sc);
    }
  }
  return ordered_sc;
}

bool SwitchChecker::isCastAccepted(QualType caseType, QualType condType)
{
  uint64_t caseSize = CTX->getTypeSize(caseType);
  uint64_t condSize = CTX->getTypeSize(condType);
  return caseSize <= condSize;
}