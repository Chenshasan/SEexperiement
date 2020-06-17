#include <vector>

#include "clang/AST/Stmt.h"
#include "clang/AST/Type.h"

#include "../common/printer.h"

using namespace clang;
using namespace std;

class SwitchChecker: public printer
{

public:

    SwitchChecker();

    SwitchChecker(const SwitchChecker& sc) = delete;

    void typeMismatchCheck(SwitchStmt* ss);

private:

    vector<SwitchCase*> getCaseByOrder(SwitchStmt* ss);

    bool isCastAccepted(QualType caseType, QualType condType);

};