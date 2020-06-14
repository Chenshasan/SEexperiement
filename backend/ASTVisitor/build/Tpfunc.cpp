void pfree(int* p)
{
    delete p;
}
void pfreeparam(int a,int* p)
{
    *p=a;
    delete p;
}
void pnull(int* p)
{
    p=nullptr;
}
void pnullparam(int a,int* p)
{
    *p=a;
    p=nullptr;
}
void m1x(int a,int* b,int c,int* d)
{
    *b=a;
    b=nullptr;
    *d=c;
    delete d;
}
int main()
{
    int* p1=new int(1);
    int* p2=new int(2);
    int* p3=new int(3);
    int* p4=new int(4);

    int* p5=new int(5);
    int* p6=new int(6);

    delete p1;
    delete p2;
    pfree(p1); //err 1
    pfreeparam(4,p2); //err 2

    p3=nullptr;
    p4=nullptr;

    pnull(p3); //err 3
    pnullparam(5,p4);//err 4

    p5=nullptr;
    delete p6;
    m1x(666,p5,777,p6);
    
    return 0;
}