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

using namespace clang;
using namespace std;

SourceManager *SM;
ASTContext *CTX;
int ForStmtEndLine = 0;

// By implementing RecursiveASTVisitor, we can specify which AST nodes
// we're interested in by overriding relevant methods.
class MyASTVisitor : public RecursiveASTVisitor<MyASTVisitor>
{
public:
  MyASTVisitor(Rewriter &R) : TheRewriter(R) {}

  bool VisitStmt(Stmt *s)
  {
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
    }
    else if (isa<DeclStmt>(s))
    {
      DeclStmt* ds=cast<DeclStmt>(s);
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
        VarDecl* vd=cast<VarDecl>(dcl);
        QualType qt=vd->getType();
        std::string qtgas=qt.getAsString();
        if(qtgas.find('*')!=qtgas.npos)
        {
          string locString = beginLoc.printToString(*SM);
          printf("pointer decl detected::%s\n", locString.c_str());
        }
      }
    }
    return true;
  }

  bool VisitBinaryOperator(BinaryOperator *stmt)
  {
    SourceLocation beginLoc = stmt->getBeginLoc();
    string beginLocString = beginLoc.printToString(*SM);
    char delims[] = ":";
    char *tmp = NULL;
    tmp = strtok((char *)beginLocString.c_str(), delims);
    tmp = strtok(NULL, delims);
    int BinaryOperatorLine = atoi(tmp);
    if(BinaryOperatorLine > ForStmtEndLine)
    {
      return false;
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

private:
  Rewriter &TheRewriter;
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
