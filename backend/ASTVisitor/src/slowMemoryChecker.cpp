#include "slowMemoryChecker.h"



void SlowMemoryChecker::BinaryOperationCheck(BinaryOperator* stmt,int BinaryOperatorLine,int ForStmtEndLine,int WhileStmtEndLine)
{
    
    if(ForStmtEndLine==-1 && WhileStmtEndLine==-1)
    { 
      //outside of For or While
      return ;
    }

    int endLine=0;

    //Find the close one
    if(ForStmtEndLine > WhileStmtEndLine)
    {
      endLine = ForStmtEndLine;
    }
    else
    {
      endLine = WhileStmtEndLine;
    }
    
    SourceLocation beginLoc = stmt->getBeginLoc();

    string beginLocString = beginLoc.printToString(*SM);
    // char delims[] = ":";
    // char *tmp = NULL;
    // tmp = strtok((char *)beginLocString.c_str(), delims);
    // tmp = strtok(NULL, delims);
    // int BinaryOperatorLine = atoi(tmp);


    //outside of While or For
    if(BinaryOperatorLine > endLine)
    {
      // printf("endLine : %d\n",endLine);
      // printf("return !!! BinaryOperatorLine: %d\nForStmtEndLine: %d\nWhileStmtEndLine: %d\n\n",BinaryOperatorLine,ForStmtEndLine,WhileStmtEndLine);
      return ;
    }
    
    PresumedLoc PLoc = (*SM).getPresumedLoc(beginLoc);
    const char * fname = PLoc.getFilename();
    int line = PLoc.getLine();
    int col = PLoc.getColumn();

    Expr *lhs = stmt->getLHS()->IgnoreImpCasts();
    Expr *rhs = stmt->getRHS()->IgnoreImpCasts();

    QualType ltype = lhs->getType();
    QualType rtype = rhs->getType();
    bool isSameType = sameType(ltype,rtype);

    CharUnits lcu = CTX->getTypeSizeInChars(ltype);
    int lsize = lcu.getQuantity();
    CharUnits rcu = CTX->getTypeSizeInChars(rtype);
    int rsize = rcu.getQuantity();

    if ( lsize<=2 )
    {
      printf("SlowMemoryOperation::%s:%d:%d Type:%s SizeOfType:%d\n", fname, line, col,ltype.getAsString().c_str(),lsize);
      char tmpwarn[100];
      sprintf(tmpwarn,"SlowMemoryOperation::%s:%d:%d Type:%s SizeOfType:%d\n", fname, line, col,ltype.getAsString().c_str(),lsize);
      std::string tmpwarns(tmpwarn);
      pprint(tmpwarns);
    }
    else if ( rsize<=2 )
    {
      printf("SlowMemoryOperation::%s:%d:%d Type:%s SizeOfType:%d\n", fname, line, col,rtype.getAsString().c_str(),rsize);
      char tmpwarn[100];
      sprintf(tmpwarn,"SlowMemoryOperation::%s:%d:%d Type:%s SizeOfType:%d\n", fname, line, col,rtype.getAsString().c_str(),rsize);
      std::string tmpwarns(tmpwarn);
      pprint(tmpwarns);
    }
}

bool SlowMemoryChecker::sameType(QualType ltype,QualType rtype)
{
    if (ltype->isPointerType())
    {
      ltype = ltype->getPointeeType();
    }
    if (rtype->isPointerType())
    {
      rtype = rtype->getPointeeType();
    }
    return ltype==rtype;
}

SlowMemoryChecker::SlowMemoryChecker()
{
    ;
}

SlowMemoryChecker::SlowMemoryChecker(const SlowMemoryChecker &other)
{
    ;
}

void SlowMemoryChecker::printCallExprName(CallExpr *c)
{
    FunctionDecl *fd = c->getDirectCallee();
    const char *funcname = fd->getNameAsString().c_str();
    printf("CallExpr: %s\n",funcname);
}

void SlowMemoryChecker::printCXXCallExprName(CXXOperatorCallExpr *c)
{
    FunctionDecl *fd = c->getDirectCallee();
    const char *funcname = fd->getNameAsString().c_str();
    printf("CXXCallExpr: %s\n",funcname);
}

int SlowMemoryChecker::findWhileStmtEndLine(WhileStmt *ws)
{
    SourceLocation beginLoc = ws->getBeginLoc();
    string beginLocString = beginLoc.printToString(*SM);
    Stmt *body = ws->getBody();
    SourceLocation endLoc = ws->getEndLoc();
    string endLocString = endLoc.printToString(*SM);
    char delims[] = ":";
    char *tmp = NULL;
    tmp = strtok((char *)endLocString.c_str(), delims);
    tmp = strtok(NULL, delims);
    int WhileStmtEndLine = atoi(tmp);
    return WhileStmtEndLine;
}

bool SlowMemoryChecker::FindExprNameByToken(Expr *e)
{
    bool printNameByToken = false;
    SourceLocation loc = e->getBeginLoc();
    std::pair<FileID, unsigned> locInfo = SM->getDecomposedLoc(loc);
    bool invalidTemp = false;
    llvm::StringRef file = SM->getBufferData(locInfo.first, &invalidTemp);
    if (invalidTemp)
      return false;
    // tokenBegin即为loc对应源码内容的起始点
    const char *tokenBegin = file.data() + locInfo.second;

    clang::LangOptions opts;
    Lexer *lexer = new Lexer(SM->getLocForStartOfFile(locInfo.first), opts, file.begin(), tokenBegin, file.end());
    Token tok;
    lexer->LexFromRawLexer(tok); // 取得第一个tok，反复调用可以获取一段token流

    clang::IdentifierInfo *II;
    const char * tokenName = tok.getName();
    tok.setIdentifierInfo(II);

    if(tok.isAnyIdentifier() && printNameByToken)
    {
      llvm::StringRef sr = tok.getRawIdentifier();
      llvm::StringRef *sr1 = new StringRef("memcmp");
      //
      if(sr.contains(*sr1))
      {
        string tmp = sr.str();
        if(strcmp(tmp.c_str(),"my_memcmp"))
        {
          printf("tokenName : %s Iden:%s loc: %s \n"
          ,tokenName
          ,tmp.c_str()
          ,tok.getLocation().printToString(*SM).c_str());
        }
      }
    }
    return true;
}