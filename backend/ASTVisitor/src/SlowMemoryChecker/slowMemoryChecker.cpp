/**
 * @file slowMemoryChecker.cpp
 * @author 叶宙果
 * @version v2
 */

/*! \mainpage SlowMemoryChecker Index Page
 *
 * \section SlowMemoryChecker 主要检测慢速内存操作缺陷
 * 慢速内存操作缺陷:对较大的内存区域的清零，拷贝，比较等操作，使用低速逐byte方式操作, 或者每次操作内存大小相对于总内存区域的比例过小
 */
#include "slowMemoryChecker.h"

//#define SlowMemoryOper 1

/**
 * debug
 * @param s 输出至控制台的字符串
 */
void SlowMemoryChecker::debug(const char * s)
{
  //printf("%s\n",s);
}

/**
 * 在visitStmt中调用
 * 主要功能是判断目前所在的语句类型
 * 并且更新Checker所需的变量值
 * @param s 目前的语句
 */
void SlowMemoryChecker::setFlags(Stmt *s)
{
    SourceLocation beginLoc = s->getBeginLoc();

    //string beginLocString = beginLoc.printToString(*SM); //change
    //beginLoc.dump(CTX->getSourceManager());
    string beginLocString = beginLoc.printToString(CTX->getSourceManager());
    
    char delims[] = ":";
    char *tmp = NULL;
    tmp = strtok((char *)beginLocString.c_str(), delims);
    tmp = strtok(NULL, delims);
    int BinaryOperatorLine = atoi(tmp);
    tmp = strtok(NULL, delims);
    int BinaryOperatorCol = atoi(tmp);

    //printf("BinaryOperatorLine: %d\nForStmtEndLine: %d\n\n",BinaryOperatorLine,ForStmtEndLine);
    if(BinaryOperatorLine > ForStmtEndLine)
    {
      this->inForStmt=false;
      this->ForStmtEndLine=-1;
      this->numOfForLoop=0;
      //printf("numofForloop now is 0!\n");

    }

    //printf("BinaryOperatorLine: %d\nWhileStmtEndLine: %d\n\n",BinaryOperatorLine,WhileStmtEndLine);
    if(BinaryOperatorLine > WhileStmtEndLine)
    {
      this->inWhileStmt=false;
      this->WhileStmtEndLine=-1;
      this->numOfWhileLoop=0;
      //printf("numofWhileloop now is 0!\n");
    }

    //printf("BinaryOperatorLine: %d\nDoWhileStmtEndLine: %d\n\n",BinaryOperatorLine,DoWhileStmtEndLine);
    if(BinaryOperatorLine > DoWhileStmtEndLine)
    {
      this->inDoWhileStmt=false;
      this->DoWhileStmtEndLine=-1;
      this->numOfDoWhileLoop=0;
      //printf("numofDoWhileloop now is 0!\n");
    }
}

/**
 * SlowMemoryChecker的入口
 * 从这里开始调用不同语句的Checke函数
 * @param bo 目前的操作符
 */
void SlowMemoryChecker::BinaryOperationCheck(BinaryOperator* bo,int BinaryOperatorLine,int ForStmtEndLine,int WhileStmtEndLine)
{
    int endLine=0;

    if(this->inForStmt==true)
    {
      endLine = this->ForStmtEndLine;
      BinaryOperationCheckInForStmt(bo);
    }
    else if(this->inWhileStmt == true)
    {
      endLine = this->WhileStmtEndLine;
      BinaryOperationCheckInWhileStmt(bo);
    }
    else if(this->inDoWhileStmt == true)
    {
      endLine = this->DoWhileStmtEndLine;
      BinaryOperationCheckInDoWhileStmt(bo);
    }

}


/**
 * DoWhile语句的判断函数
 * 其中默认循环变量初值为零
 * 根据推算的循环次数以及变量大小决定是否告警
 * @param bo 目前的操作符
 */
void SlowMemoryChecker::BinaryOperationCheckInDoWhileStmt(BinaryOperator* bo)
{
  debug("do");
  
  SourceLocation beginLoc = bo->getBeginLoc();

    string beginLocString = beginLoc.printToString(*SM);
    char delims[] = ":";
    char *tmp = NULL;
    tmp = strtok((char *)beginLocString.c_str(), delims);
    tmp = strtok(NULL, delims);
    int BinaryOperatorLine = atoi(tmp);
    tmp = strtok(NULL, delims);
    int BinaryOperatorCol = atoi(tmp);

    int initVal=0;
    

    //bool BinaryOperatorInCond = this->isBinaryOpratorInCond(bo);//todo : dowhile's cond is in the end of Dostmt

    //printf("Incond : %d , DowhileLoop :%d\n",BinaryOperatorInCond,this->numOfDoWhileLoop);


    if(this->numOfDoWhileLoop == 0)
    {
      this->numOfDoWhileLoop = this->getNumOfDoWhileLoop(BinaryOperatorLine,BinaryOperatorCol,initVal);
      //printf("DowhileLoop :%d \n",this->numOfDoWhileLoop);
    }

    PresumedLoc PLoc = (*SM).getPresumedLoc(beginLoc);
    const char * fname = PLoc.getFilename();
    int line = PLoc.getLine();
    int col = PLoc.getColumn();

    Expr *lhs = bo->getLHS()->IgnoreImpCasts();
    Expr *rhs = bo->getRHS()->IgnoreImpCasts();

    QualType ltype = lhs->getType();
    QualType rtype = rhs->getType();
    bool isSameType = sameType(ltype,rtype);

    CharUnits lcu = CTX->getTypeSizeInChars(ltype);
    int lsize = lcu.getQuantity();
    CharUnits rcu = CTX->getTypeSizeInChars(rtype);
    int rsize = rcu.getQuantity();

    //printf("DoWhile :: initVal : %d numOfDoWhileLoop: %d lsize:%d \n\n",initVal,this->numOfDoWhileLoop,lsize);
    if(lsize<=1 && (this->numOfDoWhileLoop > this->minNumOfLoop))
    {
#ifdef OOP
      printf("SlowMemoryOperation::%s:%d:%d Type:%s SizeOfType:%d\n", fname, line, col, ltype.getAsString().c_str(), lsize);
      char tmpwarn[100];
      sprintf(tmpwarn, "SlowMemoryOperation::%s:%d:%d Type:%s SizeOfType:%d\n", fname, line, col, ltype.getAsString().c_str(), lsize);
      std::string tmpwarns(tmpwarn);
      pprint(tmpwarns);
#else
      string warns = lhs->getBeginLoc().printToString(*SM) + ':' + static_cast<char>('0' + SlowMemoryOper) + '\n';
      pprint(warns);
      std::cout << warns;
#endif

      this->VarDeclInForHead=false;
      this->ForInitVarDecl=NULL;
      this->DoWhileCond=NULL;

    }
    else if (rsize<=1 && (this->numOfDoWhileLoop > this->minNumOfLoop))
    {
#ifdef OOP
      printf("SlowMemoryOperation::%s:%d:%d Type:%s SizeOfType:%d\n", fname, line, col, rtype.getAsString().c_str(), rsize);
      char tmpwarn[100];
      sprintf(tmpwarn, "SlowMemoryOperation::%s:%d:%d Type:%s SizeOfType:%d\n", fname, line, col, rtype.getAsString().c_str(), rsize);
      std::string tmpwarns(tmpwarn);
      pprint(tmpwarns);
#else
      string warns = rhs->getBeginLoc().printToString(*SM) + ':' + static_cast<char>('0' + SlowMemoryOper) + '\n';
      pprint(warns);
      std::cout << warns;
#endif

      this->VarDeclInForHead=false;
      this->ForInitVarDecl=NULL;
      this->DoWhileCond=NULL;
    }
}


/**
 * While语句的判断函数
 * 其中默认循环变量初值为零
 * 根据推算的循环次数以及变量大小决定是否告警
 * @param bo 目前的操作符
 */
void SlowMemoryChecker::BinaryOperationCheckInWhileStmt(BinaryOperator* bo)
{
  debug("while");
    SourceLocation beginLoc = bo->getBeginLoc();

    string beginLocString = beginLoc.printToString(*SM);
    char delims[] = ":";
    char *tmp = NULL;
    tmp = strtok((char *)beginLocString.c_str(), delims);
    tmp = strtok(NULL, delims);
    int BinaryOperatorLine = atoi(tmp);
    tmp = strtok(NULL, delims);
    int BinaryOperatorCol = atoi(tmp);

    int initVal=0;
    

    bool BinaryOperatorInCond = this->isBinaryOpratorInCond(bo);

    
    if(BinaryOperatorInCond == true && this->numOfWhileLoop == 0)
    {
      //printf("\n\nincond!\n\n");
      this->numOfWhileLoop = getNumOfLoop(bo,BinaryOperatorLine,BinaryOperatorCol,initVal);
    }
    
    //printf("numOfWhileLoop: %d\n",this->numOfWhileLoop);

    PresumedLoc PLoc = (*SM).getPresumedLoc(beginLoc);
    const char * fname = PLoc.getFilename();
    int line = PLoc.getLine();
    int col = PLoc.getColumn();

    Expr *lhs = bo->getLHS()->IgnoreImpCasts();
    Expr *rhs = bo->getRHS()->IgnoreImpCasts();

    QualType ltype = lhs->getType();
    QualType rtype = rhs->getType();
    bool isSameType = sameType(ltype,rtype);

    if(ltype->isDependentType() || rtype->isDependentType())
    {
        return ;
    }

    CharUnits lcu = CTX->getTypeSizeInChars(ltype);
    int lsize = lcu.getQuantity();
    CharUnits rcu = CTX->getTypeSizeInChars(rtype);
    int rsize = rcu.getQuantity();

    //printf("While :: initVal : %d numOfWhileLoop: %d lsize:%d \n\n",initVal,this->numOfWhileLoop,lsize);
    if(lsize<=1 && (this->numOfWhileLoop > this->minNumOfLoop))
    {
#ifdef OOP
      printf("SlowMemoryOperation::%s:%d:%d Type:%s SizeOfType:%d\n", fname, line, col, rtype.getAsString().c_str(), lsize);
      char tmpwarn[100];
      sprintf(tmpwarn, "SlowMemoryOperation::%s:%d:%d Type:%s SizeOfType:%d\n", fname, line, col, rtype.getAsString().c_str(), lsize);
      std::string tmpwarns(tmpwarn);
      pprint(tmpwarns);
#else
      string warns = rhs->getBeginLoc().printToString(*SM) + ':' + static_cast<char>('0' + SlowMemoryOper) + '\n';
      pprint(warns);
      std::cout << warns;
#endif

      this->VarDeclInForHead=false;
      this->ForInitVarDecl=NULL;

    }
    else if (rsize<=1 && (this->numOfWhileLoop > this->minNumOfLoop))
    {
#ifdef OOP
      printf("SlowMemoryOperation::%s:%d:%d Type:%s SizeOfType:%d\n", fname, line, col, rtype.getAsString().c_str(), rsize);
      char tmpwarn[100];
      sprintf(tmpwarn, "SlowMemoryOperation::%s:%d:%d Type:%s SizeOfType:%d\n", fname, line, col, rtype.getAsString().c_str(), rsize);
      std::string tmpwarns(tmpwarn);
      pprint(tmpwarns);
#else
      string warns = rhs->getBeginLoc().printToString(*SM) + ':' + static_cast<char>('0' + SlowMemoryOper) + '\n';
      pprint(warns);
      std::cout << warns;
#endif

      this->VarDeclInForHead=false;
      this->ForInitVarDecl=NULL;

    }
}


/**
 * For语句的判断函数
 * 根据for语句头获取循环变量初值
 * 根据推算的循环次数以及变量大小决定是否告警
 * @param bo 目前的操作符
 */
void SlowMemoryChecker::BinaryOperationCheckInForStmt(BinaryOperator* bo)
{
debug("for");
    SourceLocation beginLoc = bo->getBeginLoc();

    string beginLocString = beginLoc.printToString(*SM);
    char delims[] = ":";
    char *tmp = NULL;
    tmp = strtok((char *)beginLocString.c_str(), delims);
    tmp = strtok(NULL, delims);
    int BinaryOperatorLine = atoi(tmp);
    tmp = strtok(NULL, delims);
    int BinaryOperatorCol = atoi(tmp);
debug("For 1");
    int initVal=0;
    if(this->VarDeclInForHead==true)
    {
      initVal = getInitValFromVarDecl(this->ForInitVarDecl);
      debug("initial");
    }
    else
    {
      if(bo->isAssignmentOp())
      {
        bool AssignmentOpInForHead = this->isAssignmentOpInForHead(bo);
        if(AssignmentOpInForHead)
        {
          initVal = getInitVal(bo);
        }

      }
    }
debug("For 2");
    bool BinaryOperatorInCond = this->isBinaryOpratorInCond(bo);

    
    if(BinaryOperatorInCond == true && this->numOfForLoop == 0)
    {
      //printf("\n\nincond!\n\n");
      this->numOfForLoop = getNumOfLoop(bo,BinaryOperatorLine,BinaryOperatorCol,initVal);
    }
    
    //printf("numOfForLoop: %d\n",this->numOfForLoop);
debug("For 3");
    PresumedLoc PLoc = (*SM).getPresumedLoc(beginLoc);
    const char * fname = PLoc.getFilename();
    int line = PLoc.getLine();
    int col = PLoc.getColumn();

    Expr *lhs = bo->getLHS()->IgnoreImpCasts();
    Expr *rhs = bo->getRHS()->IgnoreImpCasts();

    QualType ltype = lhs->getType();
    QualType rtype = rhs->getType();
    bool isSameType = sameType(ltype,rtype);

    //ltype->dump();
    if(ltype->isDependentType() || rtype->isDependentType())
    {
        return ;
    }
    CharUnits lcu = CTX->getTypeSizeInChars(ltype); //might cause core dump
    int lsize = lcu.getQuantity();  
    CharUnits rcu = CTX->getTypeSizeInChars(rtype);
    int rsize = rcu.getQuantity();


    //printf("For :: initVal : %d numOfForLoop: %d lsize:%d \n\n",initVal,this->numOfForLoop,lsize);
    if(lsize<=1 && (this->numOfForLoop > this->minNumOfLoop))
    {
#ifdef OOP
      printf("SlowMemoryOperation::%s:%d:%d Type:%s SizeOfType:%d\n", fname, line, col, rtype.getAsString().c_str(), lsize);
      char tmpwarn[100];
      sprintf(tmpwarn, "SlowMemoryOperation::%s:%d:%d Type:%s SizeOfType:%d\n", fname, line, col, rtype.getAsString().c_str(), lsize);
      std::string tmpwarns(tmpwarn);
      pprint(tmpwarns);
#else
      string warns = rhs->getBeginLoc().printToString(*SM) + ':' + static_cast<char>('0' + SlowMemoryOper) + '\n';
      pprint(warns);
      std::cout << warns;
#endif

      this->VarDeclInForHead=false;
      this->ForInitVarDecl=NULL;
      //this->numOfLoop=0;
    }
    else if (rsize<=1 && (this->numOfForLoop > this->minNumOfLoop))
    {
#ifdef OOP
      printf("SlowMemoryOperation::%s:%d:%d Type:%s SizeOfType:%d\n", fname, line, col, rtype.getAsString().c_str(), rsize);
      char tmpwarn[100];
      sprintf(tmpwarn, "SlowMemoryOperation::%s:%d:%d Type:%s SizeOfType:%d\n", fname, line, col, rtype.getAsString().c_str(), rsize);
      std::string tmpwarns(tmpwarn);
      pprint(tmpwarns);
#else
      string warns = rhs->getBeginLoc().printToString(*SM) + ':' + static_cast<char>('0' + SlowMemoryOper) + '\n';
      pprint(warns);
      std::cout << warns;
#endif

      this->VarDeclInForHead=false;
      this->ForInitVarDecl=NULL;
      //this->numOfLoop=0;
    }
}


/**
 * 根据For语句的条件语句
 * 判断声明语句是否在For语句的头部
 * @param vd 目前的声明语句
 * @retval true 语句在For语句的初始化语句中
 * @retval false 语句不在For语句的初始化语句中
 */
bool SlowMemoryChecker::isVarDeclInForHead(VarDecl *vd)
{
    SourceLocation beginLoc = vd->getBeginLoc();

    string beginLocString = beginLoc.printToString(*SM);
    char delims[] = ":";
    char *tmp = NULL;
    tmp = strtok((char *)beginLocString.c_str(), delims);
    tmp = strtok(NULL, delims);
    int VarDeclLine = atoi(tmp);
    tmp = strtok(NULL, delims);
    int VarDeclCol = atoi(tmp);

    int condBeginLine = this->fscl.getBeginLine();
    int condBeginCol = this->fscl.getBeginCol();
    int condEndLine = this->fscl.getEndLine();
    int condEndCol = this->fscl.getEndCol();

    bool LineMatch=false;
    bool ColMatch=false;

    if(VarDeclLine == condBeginLine)
    {
      LineMatch=true;
      if(VarDeclCol < condBeginCol)
      {
        ColMatch=true;
      }
    }

    if(LineMatch == true && ColMatch == true)
    {
      //printf("BinaryOperatorLine: %d Col: %d \n",BinaryOperatorLine,BinaryOperatorCol);
      this->ForInitVarDecl=vd;
      this->VarDeclInForHead=true;                                                                                                                                                              
      return true;
    }
    else
    {
      this->ForInitVarDecl=NULL;
      this->VarDeclInForHead=false;
      return false;
    }
}

/**
 * 根据目前的声明语句
 * 获取For语句的循环变量的初始值
 * 在BinaryOperationCheckInForStmt中被调用
 * （For语句头部中有声明语句的情况被调用）
 * @param vd 目前的声明语句
 * @return val For语句的循环变量的初始值
 */
int SlowMemoryChecker::getInitValFromVarDecl(VarDecl *vd)
{
  clang::Expr::EvalResult result;
  clang::APValue *apvalue;
  
  apvalue = vd->evaluateValue();
  

  //TODO
  //get the name of VarDecl?

  int val=0;
  
  QualType type = vd->getType();
  if(type->isDependentType())
  {
    return 0;
  }
  
  if(apvalue->isInt())
  {
    
    llvm::APSInt intval = apvalue->getInt();
    //printf("its Int,in line %d col %d\n",line,col);
    

    QualType ty = vd->getType();
    string valString = apvalue->getAsString(*CTX,ty);
    val = atoi(valString.c_str());
    //printf("val : %d\n",val);
    //printf("aspint: %s\n",intval.toString(10).c_str());
  }
  return val;
}


/**
 * 根据单元位置获取目前的文件信息
 * 用以区分无需分析的头文件以及需要分析的源文件
 * @param beginLoc 目前的单元开始位置
 * @retval true 目前分析的文件是源文件
 * @retval false 目前分析的文件是头文件或其他文件
 */
bool SlowMemoryChecker::checkeFileName(SourceLocation beginLoc)
{
    PresumedLoc PLoc = (*SM).getPresumedLoc(beginLoc);
    if(PLoc.isInvalid())
    {
      return false;
    }
    const char * fname = PLoc.getFilename();
    //debug(fname);
    if(strstr(fname,".cpp"))
    {
      this->debug(fname);
      return true;
    }
    else
    {
      return false;
    }
}

/**
 * 根据目前的操作符
 * 获取For语句的循环变量的初始值
 * 在BinaryOperationCheckInForStmt中被调用
 * （For语句头部中有赋值语句的情况被调用）
 * @param bo 目前的二元操作符
 * @return val For语句的循环变量的初始值
 */
int SlowMemoryChecker::getInitVal(BinaryOperator *bo)
{
  Expr *LHS = bo->getLHS();
  Expr *RHS = bo->getRHS();

  
  clang::Expr::EvalResult result;
  if(RHS->isEvaluatable(*CTX))
  {
    RHS->EvaluateAsInt(result,*CTX);
  }

  int val=0;
  clang::APValue apvalue = result.Val;
  if(apvalue.isInt())
  {
    llvm::APSInt intval = apvalue.getInt();
    //printf("its Int,in line %d col %d\n",line,col);
    

    QualType ty = RHS->getType();
    string valString = apvalue.getAsString(*CTX,ty);
    val = atoi(valString.c_str());
    //printf("val : %d\n",val);
    //printf("aspint: %s\n",intval.toString(10).c_str());
  }
  return val;
}


/**
 * 根据目前的操作符
 * 获取For语句的循环变量的初始值
 * 在BinaryOperationCheckInForStmt中被调用
 * （For语句头部中有赋值语句的情况被调用）
 * @param bo 目前的二元操作符
 * @return numOfLoop DoWhile语句的循环次数
 */
int SlowMemoryChecker::getNumOfDoWhileLoop(int line,int col,int initVal)
{
  BinaryOperator *bo =  cast<BinaryOperator>(this->DoWhileCond);
  clang::BinaryOperator::Opcode opcode = bo->getOpcode();
  static llvm::StringRef opcodeStrRef = bo->getOpcodeStr(opcode);
  const char *opcodeStr = opcodeStrRef.str().c_str();
  //printf("opcode : %s\n",opcodeStr);
  int val=0;
  Expr *LHS = bo->getLHS();
  Expr *RHS = bo->getRHS();

  clang::Expr::EvalResult result;
  if(RHS->isEvaluatable(*CTX))
  {
    RHS->EvaluateAsInt(result,*CTX);
    
    clang::APValue apvalue = result.Val;
    if(apvalue.isInt())
    {
      llvm::APSInt intval = apvalue.getInt();
      //printf("its Int,in line %d col %d\n",line,col);


      QualType ty = RHS->getType();
      string valString = apvalue.getAsString(*CTX,ty);
      val = atoi(valString.c_str());
      //printf("val : %d\n",val);
      //printf("aspint: %s\n",intval.toString(10).c_str());
    }
  }
  else
  {
    //if it's Unevaluable, then we should give the warning
    val=this->minNumOfLoop+1;
  }

  

  //printf("initval: %d , val: %d\n",initVal,val);
  int numOfLoop=0;
  //what kind of ComparisonOperator
  if(strcmp(opcodeStr,"<") == 0)
  {
    numOfLoop=val-initVal;
  }
  else if(strcmp(opcodeStr,">") == 0)
  {
    numOfLoop=initVal-val;
  }
  else if(strcmp(opcodeStr,"!=") == 0)
  {
    if(initVal<=val)
    {
      numOfLoop=val-initVal;
    }
    else
    {
      numOfLoop=initVal-val;
    }
  }
  
  return numOfLoop;
}


/**
 * 根据目前的操作符以及循环变量初始值
 * 尝试获取For语句的循环次数
 * 在BinaryOperationCheckInForStmt中被调用
 * @param bo 目前的二元操作符
 * @param initVal 循环变量初始值
 * @return numOfLoop For语句和While语句的循环次数
 */
int SlowMemoryChecker::getNumOfLoop(BinaryOperator *bo,int line,int col,int initVal)
{
  if(!isComparisonOperator(bo))
  {
      return 0;
  }
  clang::BinaryOperator::Opcode opcode = bo->getOpcode();
  static llvm::StringRef opcodeStrRef = bo->getOpcodeStr(opcode);
  const char *opcodeStr = opcodeStrRef.str().c_str();
  //printf("opcode : %s\n",opcodeStr);

  Expr *LHS = bo->getLHS();
  Expr *RHS = bo->getRHS();
  
  int val=0;
  clang::Expr::EvalResult result;
  if(RHS->isEvaluatable(*CTX))
  {
    RHS->EvaluateAsInt(result,*CTX);
    //printf("getloop of While\n");

    clang::APValue apvalue = result.Val;
    if(apvalue.isInt())
    {
      llvm::APSInt intval = apvalue.getInt();
      //printf("its Int,in line %d col %d\n",line,col);
      
      QualType ty = RHS->getType();
      string valString = apvalue.getAsString(*CTX,ty);
      val = atoi(valString.c_str());
      //printf("val : %d\n",val);
      //printf("aspint: %s\n",intval.toString(10).c_str());
    }
  }
  else
  {
    //if it's Unevaluable, then we should give the warning
    val=this->minNumOfLoop+1;
  }

  //printf("initval: %d , val: %d\n",initVal,val);
  int numOfLoop=0;
  //what kind of ComparisonOperator
  if(strcmp(opcodeStr,"<") == 0)
  {
    numOfLoop=val-initVal;
  }
  else if(strcmp(opcodeStr,">") == 0)
  {
    numOfLoop=initVal-val;
  }
  else if(strcmp(opcodeStr,"!=") == 0)
  {
    if(initVal<=val)
    {
      numOfLoop=val-initVal;
    }
    else
    {
      numOfLoop=initVal-val;
    }
  }
  
  return numOfLoop;
}


/**
 * 判断目前的二元操作符是否是比较操作符
 * @param bo 目前的二元操作符
 * @retval true 目前的二元操作符是比较操作符
 * @retval false 目前的二元操作符不是比较操作符
 */
bool SlowMemoryChecker::isComparisonOperator(BinaryOperator *bo)
{
  clang::BinaryOperator::Opcode opcode = bo->getOpcode();
  bo->getID(*CTX);
  Stmt *stmt = cast<Stmt>(bo);
  
  if(bo->isComparisonOp() || bo->isEqualityOp())
  {
    return true;
  }
  else
  {
    return false;
  }
  return true;
}

/**
 * 判断两个类型是否一致
 * @param ltype 类型一
 * @param rtype 类型二
 * @retval true 两个类型一致
 * @retval false 两个类型不一致
 */
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

/**
 * 构造函数
 * 这里读取了配置文件中的最小循环次数（告警阈值）
 * 如果读取配置文件错误，会将最小循环次数（告警阈值）设置为默认的1024
 */
SlowMemoryChecker::SlowMemoryChecker()
{
    this->numOfWhileLoop=0;
    this->numOfForLoop=0;
    this->numOfDoWhileLoop=0;
    char minloop[16];
    const char *profile = "./smChecker.conf";
    const char *appname = "smchecker";
    const char *varname = "minloop";
    if(this->rc.GetProfileString(profile,appname,varname,minloop)==0)
    { 
      //printf("minloop: %s \n",minloop);
      this->minNumOfLoop=atoi(minloop);
      //printf("minNumOfLoop: %d\n",this->minNumOfLoop);
    }
    else
    {
      this->minNumOfLoop=1024;
    }
    this->ForStmtEndLine=-1;
    this->DoWhileStmtEndLine=-1;
    this->WhileStmtEndLine=-1;
    this->inForStmt=false;
    this->inWhileStmt=false;
    this->inDoWhileStmt=false;
}

/**
 * 判断赋值语句是否在For语句头部
 * @param bo 目前二元操作符（赋值）
 * @retval true 该赋值语句在For语句头部
 * @retval false 该赋值语句不在For语句头部
 */
bool SlowMemoryChecker::isAssignmentOpInForHead(BinaryOperator *bo)
{
    SourceLocation beginLoc = bo->getBeginLoc();

    string beginLocString = beginLoc.printToString(*SM);
    char delims[] = ":";
    char *tmp = NULL;
    tmp = strtok((char *)beginLocString.c_str(), delims);
    tmp = strtok(NULL, delims);
    int BinaryOperatorLine = atoi(tmp);
    tmp = strtok(NULL, delims);
    int BinaryOperatorCol = atoi(tmp);

    int condBeginLine = this->fscl.getBeginLine();
    int condBeginCol = this->fscl.getBeginCol();
    int condEndLine = this->fscl.getEndLine();
    int condEndCol = this->fscl.getEndCol();

    bool LineMatch=false;
    bool ColMatch=false;

    if(BinaryOperatorLine == condBeginLine)
    {
      LineMatch=true;
      if(BinaryOperatorCol < condBeginCol)
      {
        ColMatch=true;
      }
    }

    if(LineMatch == true && ColMatch == true)
    {
      //printf("BinaryOperatorLine: %d Col: %d \n",BinaryOperatorLine,BinaryOperatorCol);
      return true;
    }
    else
    {
      return false;
    }
}

/**
 * 判断操作符是否在循环的条件语句中
 * @param bo 目前二元操作符
 * @retval true 操作符在循环的条件语句中
 * @retval false 操作符不在循环的条件语句中
 */
bool SlowMemoryChecker::isBinaryOpratorInCond(BinaryOperator *bo)
{
    SourceLocation beginLoc = bo->getBeginLoc();

    string beginLocString = beginLoc.printToString(*SM);
    char delims[] = ":";
    char *tmp = NULL;
    tmp = strtok((char *)beginLocString.c_str(), delims);
    tmp = strtok(NULL, delims);
    int BinaryOperatorLine = atoi(tmp);
    tmp = strtok(NULL, delims);
    int BinaryOperatorCol = atoi(tmp);

    int condBeginLine = this->fscl.getBeginLine();
    int condBeginCol = this->fscl.getBeginCol();
    int condEndLine = this->fscl.getEndLine();
    int condEndCol = this->fscl.getEndCol();

    bool LineMatch=false;
    bool ColMatch=false;
    if(BinaryOperatorLine >= condBeginLine && BinaryOperatorLine <= condEndLine)
    {
      LineMatch=true;
      if(BinaryOperatorCol >= condBeginCol && BinaryOperatorCol <= condEndCol)
      {
        ColMatch=true;
      }
    }
    
    if(LineMatch == true && ColMatch == true)
    {
      //printf("BinaryOperatorLine: %d Col: %d \n",BinaryOperatorLine,BinaryOperatorCol);
      return true;
    }
    else
    {
      return false;
    }
}


/**
 * 打印目前的调用函数名称
 * 测试用
 */
void SlowMemoryChecker::printCallExprName(clang::CallExpr *c)
{
    FunctionDecl *fd = c->getDirectCallee();
    const char *funcname = fd->getNameAsString().c_str();
    printf("CallExpr: %s\n",funcname);
}

/**
 * 打印目前的调用函数名称
 * 测试用
 */
void SlowMemoryChecker::printCXXCallExprName(CXXOperatorCallExpr *c)
{
    FunctionDecl *fd = c->getDirectCallee();
    const char *funcname = fd->getNameAsString().c_str();
    printf("CXXCallExpr: %s\n",funcname);
}

/**
 * 找到While语句的结尾行号
 * 用来判断之后的语句类型
 * @param ws 目前的While语句
 * @return WhileStmtEndLine 最近的While语句的结尾行号
 */
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

/**
 * 找到For语句的结尾行号
 * 用来判断之后的语句类型
 * @param fs 目前的For语句
 * @return ForStmtEndLine 最近的For语句的结尾行号
 */
int SlowMemoryChecker::findForStmtEndLine(ForStmt *fs)
{
    SourceLocation beginLoc = fs->getBeginLoc();
    string beginLocString = beginLoc.printToString(*SM);
    Stmt *body = fs->getBody();
    SourceLocation endLoc = fs->getEndLoc();
    string endLocString = endLoc.printToString(*SM);
    char delims[] = ":";
    char *tmp = NULL;
    tmp = strtok((char *)endLocString.c_str(), delims);
    tmp = strtok(NULL, delims);
    int ForStmtEndLine = atoi(tmp);
    return ForStmtEndLine;
}

/**
 * 找到DoWhile语句的结尾行号
 * 用来判断之后的语句类型
 * @param ds 目前的DoWhile语句
 * @return DoWhileStmtEndLine 最近的DoWhile语句的结尾行号
 */
int SlowMemoryChecker::findDoWhileStmtEndLine(DoStmt *ds)
{
    SourceLocation beginLoc = ds->getBeginLoc();
    string beginLocString = beginLoc.printToString(*SM);
    Stmt *body = ds->getBody();
    SourceLocation endLoc = ds->getEndLoc();
    string endLocString = endLoc.printToString(*SM);
    char delims[] = ":";
    char *tmp = NULL;
    tmp = strtok((char *)endLocString.c_str(), delims);
    tmp = strtok(NULL, delims);
    int DoWhileStmtEndLine = atoi(tmp);
    return DoWhileStmtEndLine;
}

/**
 * 使用token流寻找表达式的名字
 * 测试用
 */
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

/**
 * 找到条件语句的行号
 * @param cond 循环的条件语句
 * @return condBeginLine 条件语句的初始行号
 */
int ForStmtCondLoc::findCondBeginLine(Expr *cond)
{
      // cond->getBeginLoc().dump(TheCompInst.getSourceManager());
      // cond->getEndLoc().dump(TheCompInst.getSourceManager());
      SourceLocation beginLoc = cond->getBeginLoc();
      SourceLocation endLoc = cond->getEndLoc();
      string beginLocString = beginLoc.printToString(*SM);
      char delims[] = ":";
      char *tmp = NULL;
      tmp = strtok((char *)beginLocString.c_str(), delims);
      tmp = strtok(NULL, delims);
      int condBeginLine = atoi(tmp);
      tmp = strtok(NULL, delims);
      int condBeginCol = atoi(tmp);
      //printf("%d %d\n",condBeginLine,condBeginCol);
      this->condBeginLine=condBeginLine;
      return condBeginLine;
}

/**
 * 找到条件语句的列号
 * @param cond 循环的条件语句
 * @return condBeginCol 条件语句的初始列号
 */
int ForStmtCondLoc::findCondBeginCol(Expr *cond)
{
      // cond->getBeginLoc().dump(TheCompInst.getSourceManager());
      // cond->getEndLoc().dump(TheCompInst.getSourceManager());
      SourceLocation beginLoc = cond->getBeginLoc();
      SourceLocation endLoc = cond->getEndLoc();
      string beginLocString = beginLoc.printToString(*SM);
      char delims[] = ":";
      char *tmp = NULL;
      tmp = strtok((char *)beginLocString.c_str(), delims);
      tmp = strtok(NULL, delims);
      int condBeginLine = atoi(tmp);
      tmp = strtok(NULL, delims);
      int condBeginCol = atoi(tmp);
      //printf("%d %d\n",condBeginLine,condBeginCol);
      this->condBeginCol=condBeginCol;
      return condBeginCol;
      
}

/**
 * 找到条件语句的结尾行号
 * @param cond 循环的条件语句
 * @return condEndLine 条件语句的结尾行号
 */
int ForStmtCondLoc::findCondEndLine(Expr *cond)
{
      // cond->getBeginLoc().dump(TheCompInst.getSourceManager());
      // cond->getEndLoc().dump(TheCompInst.getSourceManager());
      SourceLocation beginLoc = cond->getBeginLoc();
      SourceLocation endLoc = cond->getEndLoc();
      string EndLocString = endLoc.printToString(*SM);
      char delims[] = ":";
      char *tmp = NULL;
      tmp = strtok((char *)EndLocString.c_str(), delims);
      tmp = strtok(NULL, delims);
      int condEndLine = atoi(tmp);
      tmp = strtok(NULL, delims);
      int condEndCol = atoi(tmp);
      //printf("%d %d\n",condEndLine,condEndCol);
      this->condEndLine=condEndLine;
      return condEndLine;
}

/**
 * 找到条件语句的结尾列号
 * @param cond 循环的条件语句
 * @return condEndCol 条件语句的结尾列号
 */
int ForStmtCondLoc::findCondEndCol(Expr *cond)
{
      // cond->getBeginLoc().dump(TheCompInst.getSourceManager());
      // cond->getEndLoc().dump(TheCompInst.getSourceManager());
      SourceLocation beginLoc = cond->getBeginLoc();
      SourceLocation endLoc = cond->getEndLoc();
      string EndLocString = endLoc.printToString(*SM);
      char delims[] = ":";
      char *tmp = NULL;
      tmp = strtok((char *)EndLocString.c_str(), delims);
      tmp = strtok(NULL, delims);
      int condEndLine = atoi(tmp);
      tmp = strtok(NULL, delims);
      int condEndCol = atoi(tmp);
      //printf("%d %d\n",condEndLine,condEndCol);
      this->condEndCol=condEndCol;
      return condEndCol;
}

/**
 * 寻找条件语句的位置信息
 * @param cond 循环的条件语句
 */
void ForStmtCondLoc::getCondLoc(Expr *cond)
{
  //printf("in getcondloc\n");
  this->findCondBeginLine(cond);
  this->findCondBeginCol(cond);
  this->findCondEndLine(cond);
  this->findCondEndCol(cond);
  //printf("out getcondloc\n");
}

/**
 * @return 条件语句的开始行号
 */
int ForStmtCondLoc::getBeginLine()
{
  return this->condBeginLine;
}

/**
 * @return 条件语句的开始列号
 */
int ForStmtCondLoc::getBeginCol()
{
  return this->condBeginCol;
}

/**
 * @return 条件语句的结尾行号
 */
int ForStmtCondLoc::getEndLine()
{
  return this->condEndLine;
}

/**
 * @return 条件语句的结尾列号
 */
int ForStmtCondLoc::getEndCol()
{
  return this->condEndCol;
}

/* 
 * ForStmtCondLoc类的构造函数
 */
ForStmtCondLoc::ForStmtCondLoc()
{
    ;
}

