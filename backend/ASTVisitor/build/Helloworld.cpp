#include <stdio.h>
#include <string.h>

int inc(int a)
{
    return a++;
}

int main()
{
    int a =0;
    for(int i=0;i<10;i++)
    {
        a = inc(a);
    }
    if(a>1)
    {
        printf("maior\n");
    }
    else
    {
        printf("menor\n");
    }


    return 0;
}
