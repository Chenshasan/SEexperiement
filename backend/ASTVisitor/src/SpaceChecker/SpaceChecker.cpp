#include <iostream>
#include <sstream>

#include "clang/AST/Decl.h"
#include "clang/AST/Type.h"
#include "clang/AST/RecursiveASTVisitor.h"

#include "../myASTVisitor.h"
#include "../common/errNo.h"
#include "SpaceChecker.h"

SpaceChecker::SpaceChecker()
{
  if (!of.is_open())
    of.open("warn.dat", std::ios::out | std::ios::trunc);
}

void SpaceChecker::bigVariableCheck(VarDecl *vd)
{
  SourceLocation beginLoc = vd->getBeginLoc();
  QualType qt = vd->getType();
  uint64_t tsize = vd->getASTContext().getTypeSize(vd->getType());
  if (tsize >= WARNING_TRIGGER_VARIABLE_SIZE)
  {
    string qtstr = qt.getAsString();
#ifdef OOP
    string locString = beginLoc.printToString(*SM);
    cout << locString.c_str() << ':' << ' ' << "warning: variable" << ' ' << '\'' << qtstr << '\'' << ' ' << '(' << bitToMb(tsize) << "Mb" << ')' << ' ' << "is too big" << endl;
    stringstream ssr;
    ssr << locString.c_str() << ':' << ' ' << "warning: variable" << ' ' << '\'' << qtstr << '\'' << ' ' << '(' << bitToMb(tsize) << "Mb" << ')' << ' ' << "is too big" << endl;
    pprint(ssr.str());
#else
    string locString = beginLoc.printToString(*SM);
    stringstream ssr;
    ssr << locString.c_str() << ':' << static_cast<char>('0' + BigVar) << ':' << bitToMb(tsize) << endl;
    pprint(ssr.str());
    std::cout << ssr.str();
#endif
  }
}

double bitToMb(double bits)
{
  return bits / 8 / 1024 / 1024;
}