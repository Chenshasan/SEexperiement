#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE (1024*1024)

void my_memcpy(void* dest, void* src, unsigned int len)
{
    // 逐 int 复制
    int* c_dest = (int*) dest;
    int* c_src  = (int*) src;
    for(int i = 0; i < len; ++i)
    {
        *c_dest++ = *c_src++;
    }
}

int my_memcmp(void* a, void* b, unsigned int len)
{
    // 逐 int 比较
    int* c_a = (int*) a;
    int* c_b = (int*) b;
    for(int i = 0; i < len; ++i)
    {
        int a = *c_a++;
        int b = *c_b++;
        if(a>b) return 1;
        if(b>a) return -1;
    }
    return 0;
}



int main(){
    int* a = NULL, *b = NULL;
    a = (int*)malloc(SIZE*sizeof(int));
    b = (int*)malloc(SIZE*sizeof(int));
    // do something for a and b
    // 对于较大的内存区域，采取逐 int 比较或者复制操作,
    // no warning
    my_memcpy(a, b, SIZE);
    return 0;
}