## How to run

```shell
$ cd ${project.root}/backend/ASTVisitor
$ clang++ -emit-ast -c sample.cpp
$ ./build/myASTVisitor astlist.txt