#include <stdio.h>
#include <string.h>
#include<fstream>
#include<iostream>

using namespace std;
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
ofstream OutFile("Test.txt");//���ù��캯������txt�ı������Ҵ򿪸��ı�

 OutFile << "This is a Test12!";//���ַ�������"This is a Test!"��д��Test.txt�ļ�

 OutFile.close();


    return 0;
}
