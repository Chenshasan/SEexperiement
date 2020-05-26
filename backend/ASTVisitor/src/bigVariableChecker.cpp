#include <iostream>
#include <sstream>

#include "clang/AST/Decl.h"
#include "clang/AST/Type.h"
#include "clang/AST/RecursiveASTVisitor.h"

#include "myASTVisitor.h"
#include "bigVariableChecker.h"

BigVariableChecker::BigVariableChecker()
{
  if (!of.is_open()) of.open("warn.dat", std::ios::out | std::ios::trunc);
}

void BigVariableChecker::bigVariableCheck(VarDecl* vd)
{
  SourceLocation beginLoc = vd->getBeginLoc();
  QualType qt = vd->getType();
  uint64_t tsize = vd->getASTContext().getTypeSize(vd->getType());
  if (tsize >=  WARNING_TRIGGER_VARIABLE_SIZE) {
    string qtstr = qt.getAsString();
    string locString = beginLoc.printToString(*SM);
    stringstream ssr;
    cout << "Warning: variable is too big::" << locString.c_str() << ": " << qtstr << ": " << bitToMb(tsize) << "Mb" << endl;
    ssr << "Warning: variable is too big::" << locString.c_str() << ": " << qtstr << ": " << bitToMb(tsize) << "Mb" << endl;
    pprint(ssr.str());
  }
}

double bitToMb(double bits)
{
  return bits / 8 / 1024 / 1024;
}