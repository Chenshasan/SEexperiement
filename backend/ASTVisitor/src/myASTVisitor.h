  
#include <string>
#include <unordered_map>
#include "clang/AST/Decl.h"

#include "clang/Basic/SourceManager.h"
#include "clang/Frontend/CompilerInstance.h"

using namespace std;
using namespace clang;

extern SourceManager *SM;
extern ASTContext *CTX;
extern CompilerInstance TheCompInst;
extern unordered_map<string, EnumDecl*> EDs;