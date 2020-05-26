#include "clang/AST/Stmt.h"

#include "printer.h"

using namespace clang;
using namespace std;

class SwitchChecker: public Printer
{

public:

    SwitchChecker();

    SwitchChecker(const SwitchChecker& sc) = delete;

    void typeMismatchCheck(SwitchStmt* ss);

};