#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE (1024*1024)

void my_memcpy(void* dest, void* src, unsigned int len)
{
    // 逐 short 复制
    short* c_dest = (short*) dest;
    short* c_src  = (short*) src;
    for(short i = 0; i < len; ++i)
    {
        *c_dest++ = *c_src++;
    }
}

short my_memcmp(void* a, void* b, unsigned int len)
{
    // 逐 short 比较
    short* c_a = (short*) a;
    short* c_b = (short*) b;
    for(short i = 0; i < len; ++i)
    {
        short a = *c_a++;
        short b = *c_b++;
        if(a>b) return 1;
        if(b>a) return -1;
    }
    return 0;
}


int main(){
    short* a = NULL, *b = NULL;
    a = (short*)malloc(SIZE*sizeof(short));
    b = (short*)malloc(SIZE*sizeof(short));
    // do something for a and b
    // 对于较大的内存区域，采取逐 short 比较或者复制操作,
    my_memcpy(a, b, SIZE);
    return 0;
}