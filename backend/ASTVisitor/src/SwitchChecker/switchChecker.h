#ifndef __SWITCHCHECKER_H__
#define __SWITCHCHECKER_H__

#include <string>
#include <vector>
#include <unordered_map>

#include "clang/AST/Stmt.h"
#include "clang/AST/Type.h"

#include "../Common/printer.h"
#include "../Common/errNo.h"

using namespace clang;
using namespace std;

extern const vector<string> VALID_COND_TYPE_LIST;

extern const unordered_map<string, vector<string>> VALID_CAST_TABLE;

struct SwitchLocation
{
    string switch_stmt;

    string filename;

    int row;

    int col;

    SwitchLocation(string s, string f, int r, int c) : switch_stmt(s),
                                                       filename(f),
                                                       row(r),
                                                       col(c)
    {
    }
};

extern vector<SwitchLocation> switch_loc_list;

class SwitchChecker : public Printer
{

public:
    SwitchChecker();

    SwitchChecker(const SwitchChecker &sc) = delete;

    void typeMismatchCheck(SwitchStmt *ss);

    void enumIncompleteCheck(SwitchStmt *ss);

    static void floatAndStringCheck(string filename);

private:
    vector<SwitchCase *> getCaseByOrder(SwitchStmt *ss);

    bool isCastAccepted(QualType caseType, QualType condType);

    string getFilteredTypeAsString(QualType type);

    string getEnumNameAsString(QualType type);

    static string getFileAllContent(string filename);

    static vector<SwitchLocation> getSwitchStmts(string filename);
};

#endif