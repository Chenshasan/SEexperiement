#include "clang/AST/Decl.h"

#include "printer.h"

#define WARNING_TRIGGER_VARIABLE_SIZE (1*1024*1024*8)

using namespace clang;
using namespace std;

class BigVariableChecker: public Printer
{

public:

    BigVariableChecker();

    BigVariableChecker(const BigVariableChecker& bvc) = delete;

    void bigVariableCheck(VarDecl* vd);

};

double bitToMb(double bits);