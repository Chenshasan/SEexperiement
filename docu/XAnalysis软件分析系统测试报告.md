

# XAnalysis软件分析系统项目测试报告

## 文档修改历史

| 修改人员 |   日期   | 修改原因 | 版本号 |
| :------: | :------: | :------: | :----: |
|  李俊杰  | 2020.7.1 |   初稿   | v1.0.0 |



[TOC]

## 引言

### 编制目的

本报告详细完成对软件分许系统的测试过程总结，达到指导后续软件构造的目的，同时实现和测试人员及用户的沟通。
本报告面向开发人员、测试人员及最终用户而编写，是了解系统的导航。

### 定义

xanalysis软件分析系统：用于cpp项目静态分析

### 参考资料

1. IEEE标准
2. 本系统用例需求规格说明文档
3. 软件工程与计算（卷二）软件开发的技术基础

### 单元测试

#### Switch

```cpp
#include <iostream>
using namespace std;
int main()
{
    int a;
    scanf("%d",&a);
    int b;
    switch (a)
    {
        case 1:
            b=1;
            break;
        case 2:
            b=3;
            break;
        default:
            break;
    }
    return 0;
}
```

##### 测试结果

```
Tswitch2.cpp:9:5:类型不匹配::'YELLOW'
Tswitch2.cpp:21:5:类型不匹配:
```

#### SlowMemory

```cpp
#include <stdio.h>
#include <stdlib.h>
#define SIZE (1024*1024)

void my_memcpy(void* dest, void* src, unsigned int len){
    // 浣庨€熼€?byte 澶嶅埗
    unsigned char* c_dest = (unsigned char*) dest;
    unsigned char* c_src  = (unsigned char*) src;
    for(int i = 0; i < len; ++i)
        *c_dest++ = *c_src++;
}

int my_memcmp(void* a, void* b, unsigned int len){
    // 浣庨€熼€?byte 姣旇緝
    unsigned char* c_a = (unsigned char*) a;
    unsigned char* c_b = (unsigned char*) b;
    for(int i = 0; i < len; ++i){
        unsigned char a = *c_a++;
        unsigned char b = *c_b++;
        if(a>b) return 1;
        if(b>a) return -1;
    }
    return 0;
}

int my_memcmp_short(void* a, void* b, unsigned int len){
    // 閫?short 姣旇緝
    short* c_a = (short*) a;
    short* c_b = (short*) b;
    for(int i = 0; i < len; ++i){
        short a = *c_a++, b = *c_b++;
        if(a>b) return 1;
        if(b>a) return -1;
    }
    return 0;
}

int my_memcmp_int(void* a, void* b, unsigned int len){
    // 閫?int 姣旇緝
    int* c_a = (int*) a;
    int* c_b = (int*) b;
    for(int i = 0; i < len; ++i){
        int a = *c_a++, b = *c_b++;
        if(a>b) return 1;
        if(b>a) return -1;
    }
    return 0;
}

int main(){
    unsigned char* a = NULL, *b = NULL;
    a = (unsigned char*)malloc(SIZE*sizeof(unsigned char));
    b = (unsigned char*)malloc(SIZE*sizeof(unsigned char));
    // do something for a and b
    // 瀵逛簬杈冨ぇ鐨勫唴瀛樺尯鍩燂紝閲囧彇浣庝織鐨勯€?byte 姣旇緝鎴栬€呭鍒舵搷浣?
    printf("%d\n",my_memcmp(a, b, SIZE));
    my_memcpy(a, b, SIZE);
    printf("%d\n",my_memcmp(a, b, SIZE));
    return 0;
}
```

##### 测试结果

```
Tmemory.cpp:10:9:慢速内存:
```

#### nullRef

```cpp
#include<iostream>
using namespace std;
int main()
{
    int* p=nullptr;
    int a=*p;
    return 0;

}
```

##### 测试结果

```
Tnullderef.cpp:6:12:空指针:指针名--p
```

#### 数组越界

```cpp
using namespace std;
int main() {
	int x[3];
	int b;
	b = 0;
	x[0] = 1;
	x[3] = 0;
}
```



#### bigVar

```cpp
#include <iostream>

int main()
{
    int a[100000000];

    return 0;
}
```

##### 测试结果

```
TBigVar.cpp:5:5:大型变量:大小--381.47MB
```



五月中旬理解提供框架代码的结构。回顾软件分析/编译原理内容，组员间先一起完成AST建立，数据流分析等共通工作，之后确定这些缺陷的语法结构，做到正确定位缺陷

六月中旬根据定位的缺陷，记录用户代码的缺陷类型，行号等。Debug后可以通过简单->复杂的测试用例，正确报告不符合C/C++语法分析/语义分析的输入

六月底将完成的后端程序接入前端网站。为最后成型的代码编写符合国标的软件文档

实际开发基本依照计划进度进行。

### 集成测试

```cpp
#include <iostream>
#include <cstdio>
#include <cstring>
#include <queue>
#include <vector>
using namespace std;
using intp=int *;
enum NEWT
{
    T1,T2,T3,T4,T5
};
const int Ni = 10000;
const int INF = 1 << 27;
struct node
{
    int point, value;
    node(int a, int b)
    {
        point = a;
        value = b;
    }
    bool operator<(const node &a) const
    {
        if (value == a.value)
        {
            return point < a.point;
        }
        else
        {
            return value > a.value;
        }
    }
};
vector<node> e[Ni];
int dis[Ni], n;
priority_queue<node> pq;
void dijkstra();
int main()
{
    intp p[9];
    for (int i = 0; i < 1000000; ++i)
        p[i & 9] = NULL;
    p[10] = NULL;

    NEWT i[1000000];
    int *q;
    q = NULL;

    int ss = *q;

    switch (i[0])
    {
    case T1:
    case T3:
    case T5:
        q = new int(10);
    case T2:
        q = NULL;
    }
    int *r = new int(10);
    delete r;
    delete r;

    char dest_seexp[2048];
    int ii = 0;
    
    do
    {
        dest_seexp[ii] = 'n';
    } while (++ii < 2048);

    int a, b, c, m;
    scanf("%d%d", &n, &m);
    while (m--)
    {
        scanf("%d%d%d", &a, &b, &c);
        e[a].push_back(node(b, c));
        e[b].push_back(node(a, c));
    }
    memset(dis, 0x3f, sizeof(dis));
    dis[1] = 0;
    pq.push(node(1, dis[1]));

    dijkstra();

    for (int i = 1; i <= n; i++)
    {
        printf("%d ", dis[i]);
    }
    system("pause");
    return 0;
}
void dijkstra()
{
    while (!pq.empty())
    {
        node x = pq.top();
        pq.pop();
        for (int i = 0; i < e[x.point].size(); i++)
        {
            node y = e[x.point][i];
            if (dis[y.point] > dis[x.point] + y.value)
            {
                dis[y.point] = dis[x.point] + y.value;
                pq.push(node(y.point, dis[y.point]));
            }
        }
    }
}
```

##### 测试结果

![](https://pinru.oss-cn-shanghai.aliyuncs.com/QQ20200723143611.png)

![](https://pinru.oss-cn-shanghai.aliyuncs.com/QQ20200723143732.png)
