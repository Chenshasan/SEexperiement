#include <iostream>
void freer(int* tofree)
{
    delete tofree;
}
void nuller(int* tonull)
{
    tonull=nullptr;
}
int multinuller(int* n1,int* n2 ,int* n3)
{
    n1=n2=n3=nullptr;
    return 1;
}
int main()
{
    int* p1=new int(666);
    delete p1;
    //freer(p1);
    /*-------*/
    int a=2;
    int *p2=&a;
    //nuller(p2);
    int b=*p2;
    int *p3=new int(8);

    a=multinuller(p1,p2,p3);

    return 0;
}