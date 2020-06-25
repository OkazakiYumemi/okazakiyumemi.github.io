---
title: 「CF1364E」X-OR
categories: 题解
date: 2020-06-25 11:23:12
tags:
- 交互题
- 随机算法
top:
---

手造 rand 太烂，换成`mt19937`过了。

## 题意简述

你需要猜一个 $0$ 到 $n - 1$ 的排列 $\langle a_n\rangle$，每次可以询问二元组 $(x, y)$($x\neq y$) ，表示询问 $a_x | a_y$ 的值，其中 $|$ 为按位或。

$3\le n\le 2048$，询问次数不能超过 $4269$ 次。

<!-- more -->

## 主要思路

本题有许多做法，下面放送本人认为最妙的一种方法。

假设现在有两个可能是 0 的位置 $x$ 与 $y$，设 $A(x, y) = a_x | a_y$。

考虑现在新加入一个可能是 0 的位置 $z$，如何排除一个呢？

- 如果 $A(z, y) < A(x, y)$，则 $x > 0$，否则 $A(x, y) = y \le A(z, y)$ 矛盾。
- 如果 $A(z, y) > A(x, y)$，则同理 $z > 0$。
- 如果 $A(z, y) = A(x, y)$，则 $y > 0$，否则 $A(z, y) = z = x = A(x, y)$ 非排列。

每个数都会被查询 1 次，并且如果出现上述最后一种情况，还应额外查询 $(x, z)$。

然后就得到了两个可能是 0 的位置。如何确定哪个是 0？随机一个 $d$，询问 $(x, d)$ 与 $(y, d)$ 直到二者答案不同，小的那个即为 0。

最后再让每个数都与 0 询问一次来获得整个排列。

明明有比 $2n$ 多的查询次数，为什么总次数是正确的？容易证明当我们**按随机顺序**判断每个数是否可能为 0 时，出现最后一种状况的概率极小。同样地，判断最后两个数哪个是 0 时，期望询问次数也不大。~~有兴趣的读者可以自己尝试证明。~~

由此可见，利用随机算法获取优秀的复杂度是解题的常用技巧之一。在数据结构中，典型的示例是随机堆。

## 参考代码

注意找到两个可能是 0 的位置也要随机顺序，否则可能导致最后一种情况过多导致询问次数超限。

{% fold %}
```cpp
#include <bits/stdc++.h>
using namespace std;
#define reg register
typedef pair<int, int> PII;
#define MP(x, y) make_pair(x, y)
inline int read() {
    reg int ans = 0, f = 1;
    reg char c = getchar();
    while (!isdigit(c)) f ^= (c == '-'), c = getchar();
    for (; isdigit(c); c = getchar()) ans = (ans << 1) + (ans << 3) + (c ^ 48);
    return f ? ans : -ans;
}
inline int chkmin(int &x, const int &y) { return x > y ? x = y, 1 : 0; }
inline int chkmax(int &x, const int &y) { return x < y ? x = y, 1 : 0; }
 
#define F_O fflush(stdout)
mt19937 mt(chrono::steady_clock::now().time_since_epoch().count());
const int N = 2050;
int n, dd[N], p[N];
//睦月、如月、弥生、卯月、皐月、水無月、文月、葉月、長月、神無月、霜月、師走
map<PII, int> Q;
 
inline int query(const int &x, const int &y){
    if(Q.count(MP(x, y))) return Q[MP(x, y)];
    printf("? %d %d\n", x, y), F_O;
    return Q[MP(x, y)] = Q[MP(y, x)] = read();
}
inline int rnd(const int &lim){
    return mt() % lim;
}
 
int main() {
    n = read();
    reg int mutsuki, kisaragi, yayoi;
    reg int uzuki, satsuki, minazuki;
    reg int fumizuki, hazuki, nagatsuki;
    reg int kannazuki, shimotsuki, shiwasu;
    for(kannazuki = 1; kannazuki <= n; ++kannazuki){
        p[kannazuki] = kannazuki;
    }
    for(shimotsuki = 1; shimotsuki <= n; ++shimotsuki){
        swap(p[shimotsuki], p[rnd(n) + 1]);
    }
    mutsuki = p[1], kisaragi = p[2], uzuki = query(p[1], p[2]);
    for(nagatsuki = 3; nagatsuki <= n; ++nagatsuki){
        yayoi = p[nagatsuki];
        satsuki = query(kisaragi, yayoi);
        if(chkmin(uzuki, satsuki)){
            mutsuki = yayoi;
        }
        else if(uzuki == satsuki){
            uzuki = query(mutsuki, kisaragi = yayoi);
        }
    }
    for(shiwasu = 1; shiwasu <= n; ++shiwasu){
        swap(p[shiwasu], p[rnd(n) + 1]);
    }
    fumizuki = hazuki = nagatsuki = 0;
    do{
        minazuki = p[++nagatsuki];
        if(minazuki == mutsuki || minazuki == kisaragi) continue;
        fumizuki = query(mutsuki, minazuki);
        hazuki = query(kisaragi, minazuki);
    }while(fumizuki == hazuki);
    if(fumizuki > hazuki) mutsuki = kisaragi;
    for(nagatsuki = 1; nagatsuki <= n; ++nagatsuki){
        if(nagatsuki == mutsuki) continue;
        dd[nagatsuki] = query(nagatsuki, mutsuki);
    }
    printf("! ");
    for(nagatsuki = 1; nagatsuki <= n; ++nagatsuki){
        printf("%d ", dd[nagatsuki]);
    }
    puts(""), F_O;
    return 0;
}
```
{% endfold %}