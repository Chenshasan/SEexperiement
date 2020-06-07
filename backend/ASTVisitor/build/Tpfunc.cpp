#include <iostream>
void freer(int* free)
{
    delete free;
}
void nuller(int* tonull)
{
    tonull=nullptr;
}
int main()
{
    int* p1=new int(666);
    delete p1;
    freer(p1);
    /*-------*/
    int a=2;
    int *p2=&a;
    nuller(p2);
    int b=*p2;

    return 0;
}