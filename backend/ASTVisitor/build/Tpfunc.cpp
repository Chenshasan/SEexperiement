#include <iostream>
#define NULL 0
void pfree(int *p)
{
    //int* a=new int(1);
    delete p;
    //delete p;
}

void pnull(int *a,int *b,int r)
{
    b=NULL;
    int i=*b;
}
int main()
{
    int* p=new int(9);
    pfree(p);
    delete p;
    int *pp=new int(1);
    pnull(p,pp,1);
    int r=*pp;
    return 0;
}
// int main()
// {
//     int *p;
//     p = nullptr;
//     int a=*p;
//     return 0;
// }