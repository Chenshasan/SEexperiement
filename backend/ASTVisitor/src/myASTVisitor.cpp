//------------------------------------------------------------------------------
// Clang rewriter sample. Demonstrates:
//
// * How to use RecursiveASTVisitor to find interesting AST nodes.
// * How to use the Rewriter API to rewrite the source code.
//
// Eli Bendersky (eliben@gmail.com)
// This code is in the public domain
//------------------------------------------------------------------------------
#include <cstdio>
#include <memory>
#include <sstream>
#include <string>
#include <iostream>

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/FileManager.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Basic/TargetOptions.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Parse/ParseAST.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Rewrite/Frontend/Rewriters.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/raw_ostream.h"
#include "clang-c/Index.h"

#include "pointerChecker.h"

#define WARNING_TRIGGER_VARIABLE_SIZE (1*1024*1024*8)

using namespace clang;
using namespace std;

double bitToMb(double bits);

SourceManager *SM;
ASTContext *CTX;
int ForStmtEndLine = 0;
int Pointer::numsOfPointer;

// By implementing RecursiveASTVisitor, we can specify which AST nodes
// we're interested in by overriding relevant methods.
class MyASTVisitor : public RecursiveASTVisitor<MyASTVisitor>
{
public:
  MyASTVisitor(Rewriter &R) : TheRewriter(R) {}

  bool VisitStmt(Stmt *s)
  {
    // Only care about For statements.
    if (isa<ForStmt>(s))
    {
      ForStmt *ForStatement = cast<ForStmt>(s);
      SourceLocation beginLoc = ForStatement->getBeginLoc();
      string beginLocString = beginLoc.printToString(*SM);
      Stmt *body = ForStatement->getBody();
      SourceLocation endLoc = ForStatement->getEndLoc();
      string endLocString = endLoc.printToString(*SM);
      char delims[] = ":";
      char *tmp = NULL;
      tmp = strtok((char *)endLocString.c_str(), delims);
      tmp = strtok(NULL, delims);
      ForStmtEndLine = atoi(tmp);
      Stmt::child_range range = body->children();
      for (Stmt::child_iterator r = range.begin(); r != range.end(); r++)
      {
        VisitStmt(*r);
      }
      string locString = beginLoc.printToString(*SM);
      //printf("for loop detected::%s\n", locString.c_str());
    }
    else if (isa<DeclStmt>(s))
    {
      DeclStmt* ds = cast<DeclStmt>(s);
      SourceLocation beginLoc = ds->getBeginLoc();
      //Decl* dcl=ds->getDeclGroup().getSingleDecl();   //will hit assertion when ds->getDeclGroup() is not SingleDecl
      Decl* dcl=NULL;
      if(ds->getDeclGroup().isSingleDecl())
      {
        dcl=ds->getDeclGroup().getSingleDecl();
      }
      else
      {
        dcl=*(ds->getDeclGroup().begin());
      }

      if(isa<VarDecl>(dcl))
      {
        VarDecl* vd = cast<VarDecl>(dcl);
        QualType qt = vd->getType();
        uint64_t tsize = vd->getASTContext().getTypeSize(vd->getType());
        if (tsize >=  WARNING_TRIGGER_VARIABLE_SIZE) {
          string qtstr = qt.getAsString();
          string locString = beginLoc.printToString(*SM);
          cout << "Warning: variable is too big::" << locString.c_str() << ": " << qtstr << ": " << bitToMb(tsize) << "Mb" << endl;
        }
        if(qt->isPointerType())
        {
          string pname=vd->getNameAsString();
          //std::cout<<pname<<'\n';
          Pointer* p;
          if(vd->hasInit())
          {
            Expr* vgi=vd->getInit()->IgnoreImpCasts();
            //vgi->dumpColor();
            if(isa<CXXNewExpr>(vgi))
            {
              p=new Pointer(pname,isVALID,true);
              pc.declPointer(p);
            }
            else if(isa<DeclRefExpr>(vgi))
            {
              DeclRefExpr* dre=cast<DeclRefExpr>(vgi);
              std::string rpname=dre->getNameInfo().getAsString();
              Pointer* rhs=pc.getPointerByName(rpname);
              p=new Pointer(pname,isVALID);
              pc.declPointer(p);
              if(rhs)
              {
                pc.assignPointer(*p,*rhs);
                //std::cout<<"INIT ASSIGN"<<p->getName()<<" "<<rhs->getName()<<std::endl;
              }
            }
            else
            {
              p=new Pointer(pname,isVALID);
              pc.declPointer(p);
            }
          }
          else
          {
            p=new Pointer(pname);
            pc.declPointer(p);
          }
          //pc.declPointer(p);
          //p->dump();
          //std::cout<<"decl "<<p->getName()<<'\n';
        }
      }
    }
    return true;
  }

  bool VisitBinaryOperator(BinaryOperator *stmt)
  {
    //stmt->dumpColor();
    SourceLocation beginLoc = stmt->getBeginLoc();
    string beginLocString = beginLoc.printToString(*SM);
    char delims[] = ":";
    char *tmp = NULL;
    tmp = strtok((char *)beginLocString.c_str(), delims);
    tmp = strtok(NULL, delims);
    int BinaryOperatorLine = atoi(tmp);
    if(BinaryOperatorLine > ForStmtEndLine)
    {
      //pointer assign? 
      Expr *lhs = stmt->getLHS()->IgnoreImpCasts();
      Expr *rhs = stmt->getRHS()->IgnoreImpCasts();
      if(lhs->getType()->isPointerType() && rhs->getType()->isPointerType())
        ;
        //std::cout<<"Pointer assign"<<std::endl;
      else return true;
      std::string lname,rname;
      DeclRefExpr* ldre,*rdre;
      //TODO
      if(isa<DeclRefExpr>(lhs))
        ldre=cast<DeclRefExpr>(lhs);
      else return true;
      if(isa<DeclRefExpr>(rhs))
        rdre=cast<DeclRefExpr>(rhs);
      else return true;
      lname=ldre->getNameInfo().getAsString();
      rname=rdre->getNameInfo().getAsString();
      Pointer* lp,*rp;
      lp=pc.getPointerByName(lname);
      rp=pc.getPointerByName(rname);
      pc.assignPointer(*lp,*rp);
      //std::cout<<"ASSIGN POINTER:"<<lname<<" "<<rname<<std::endl;
      return true;
    }
    PresumedLoc PLoc = (*SM).getPresumedLoc(beginLoc);
    const char * fname = PLoc.getFilename();
    int line = PLoc.getLine();
    int col = PLoc.getColumn();
    Expr *lhs = stmt->getLHS()->IgnoreImpCasts();
    Expr *rhs = stmt->getRHS()->IgnoreImpCasts();
    QualType ltype = lhs->getType();
    QualType rtype = rhs->getType();
    if (ltype->isPointerType())
    {
      ltype = ltype->getPointeeType();
    }
    if (rtype->isPointerType())
    {
      rtype = rtype->getPointeeType();
    }
    CharUnits lcu = CTX->getTypeSizeInChars(ltype);
    int lsize = lcu.getQuantity();
    CharUnits rcu = CTX->getTypeSizeInChars(rtype);
    int rsize = rcu.getQuantity();

    if ( lsize<=2 )
    {
      printf("SlowMemoryOperation::%s:%d:%d Type:%s SizeOfType:%d\n", fname, line, col,ltype.getAsString().c_str(),lsize);
    }
    else if ( rsize<=2 )
    {
      printf("SlowMemoryOperation::%s:%d:%d Type:%s SizeOfType:%d\n", fname, line, col,rtype.getAsString().c_str(),rsize);
    }
    return true;
  }
  bool VisitArraySubscriptExpr(ArraySubscriptExpr* ase)
  {
    std::cout<<"inarr\n";
    Expr *lhs = ase->getBase()->IgnoreImpCasts();
    Expr *rhs = ase->getIdx();
    //lhs->dumpColor();
    //rhs->dumpColor();
    return true;
  }
  bool VisitUnaryOperator(UnaryOperator* u)
  {
    SourceLocation beginLoc = u->getBeginLoc();
    string beginLocString = beginLoc.printToString(*SM);
    //std::cout<<"Unary OP"<<'\n';
    if(isa<DeclRefExpr>(u->getSubExpr()->IgnoreImpCasts()))
    {
      DeclRefExpr* dre=cast<DeclRefExpr>(u->getSubExpr()->IgnoreImpCasts());
      std::string opstr=u->getOpcodeStr(u->getOpcode()).str();
      if(opstr.find('*')!=opstr.npos)
      {
        std::string pname=dre->getNameInfo().getAsString();
        pc.nullDerefCheck(*(pc.getPointerByName(pname)));
        std::cout<<" ::"<<beginLocString<<'\n';
      }
    }
    //std::cout<<u->getOpcodeStr(u->getOpcode()).str()<<'\n';
    return true;
  }
  bool VisitCXXDeleteExpr(CXXDeleteExpr* cde)
  {
    //std::cout<<"CXXDEL"<<'\n';
    SourceLocation beginLoc = cde->getBeginLoc();
    string beginLocString = beginLoc.printToString(*SM);
    DeclRefExpr* dre=cast<DeclRefExpr>(cde->getArgument()->IgnoreImpCasts());
    //dre->dumpColor();
    Pointer* p2free=pc.getPointerByName(dre->getNameInfo().getAsString());
    //p2free->dump();
    bool success=false;
    pc.freePointer(*p2free,success);
    if(!success)
    {
      std::cout<<" ::"<<beginLocString<<'\n';
    }
    return true;
  }
  bool VisitSwitchStmt(SwitchStmt* s)
  {
    QualType condType = s->getCond()->getType();
    for (SwitchCase* sc = s->getSwitchCaseList(); sc; sc = sc->getNextSwitchCase())
    {
      if (isa<CaseStmt>(sc))
      {
        CaseStmt* cs = cast<CaseStmt>(sc);
        if (cs->getLHS()->IgnoreImpCasts()->getType() != condType) {
          SourceLocation beginLoc = cs->getBeginLoc();
          string locString = beginLoc.printToString(*SM);
          cout << "Warning: switch case type mismatches cond type::" << locString.c_str() << endl;
        }
      }
    }
    return true;
  }

private:
  Rewriter &TheRewriter;
  PointerChecker pc;
};

class MyASTConsumer : public ASTConsumer
{
public:
  MyASTConsumer(Rewriter &R) : Visitor(R) {}

  virtual bool HandleTopLevelDecl(DeclGroupRef DR)
  {
    for (DeclGroupRef::iterator b = DR.begin(), e = DR.end(); b != e; ++b)
      // Traverse the declaration using my AST visitor.
      Visitor.TraverseDecl(*b);
    return true;
  }

private:
  MyASTVisitor Visitor;
};

int main(int argc, char *argv[])
{

  if (argc != 2)
  {
    llvm::errs() << "Usage: myASTVisitor <filename>\n";
    return 1;
  }

  CompilerInstance TheCompInst;
  TheCompInst.createDiagnostics();

  LangOptions &lo = TheCompInst.getLangOpts();
  lo.CPlusPlus = 1;

  auto TO = std::make_shared<TargetOptions>();
  TO->Triple = llvm::sys::getDefaultTargetTriple();
  TargetInfo *TI =
      TargetInfo::CreateTargetInfo(TheCompInst.getDiagnostics(), TO);
  TheCompInst.setTarget(TI);

  TheCompInst.createFileManager();
  FileManager &FileMgr = TheCompInst.getFileManager();
  TheCompInst.createSourceManager(FileMgr);
  SourceManager &SourceMgr = TheCompInst.getSourceManager();
  SM = &SourceMgr;
  TheCompInst.createPreprocessor(TU_Module);
  TheCompInst.createASTContext();

  Rewriter TheRewriter;

  TheRewriter.setSourceMgr(SourceMgr, TheCompInst.getLangOpts());

  // Set the main file
  const FileEntry *FileIn = FileMgr.getFile(argv[1]);
  SourceMgr.setMainFileID(
      SourceMgr.createFileID(FileIn, SourceLocation(), SrcMgr::C_User));
  TheCompInst.getDiagnosticClient().BeginSourceFile(
      TheCompInst.getLangOpts(), &TheCompInst.getPreprocessor());

  MyASTConsumer TheConsumer(TheRewriter);

  ASTContext &context = TheCompInst.getASTContext();
  CTX = &context;

  // Parse the file to AST, registering our consumer as the AST consumer.
  ParseAST(TheCompInst.getPreprocessor(), &TheConsumer,
           TheCompInst.getASTContext());

  // const RewriteBuffer *RewriteBuf =
  //     TheRewriter.getRewriteBufferFor(SourceMgr.getMainFileID());
  // llvm::outs() << std::string(RewriteBuf->begin(), RewriteBuf->end());

  return 0;
}

double bitToMb(double bits) {
  return bits / 8 / 1024 / 1024;
}