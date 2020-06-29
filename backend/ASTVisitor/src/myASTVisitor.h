#include <string>
#include <unordered_map>

#include "clang/Basic/SourceManager.h"

using namespace std;
using namespace clang;

extern SourceManager *SM;
extern ASTContext *CTX;
extern unordered_map<string, EnumDecl*> EDs;

#pragma once
enum
{
    ArrayOutOfBound,
    SlowMemoryOper,
    SwitchMismatch,
    SpaceProblem,
    NullDeref,
    DoubleFree
};
