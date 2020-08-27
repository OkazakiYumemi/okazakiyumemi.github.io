---
title: 「CF1063E」Lasers and Mirrors
urlname: CF1063E
date: 2020-08-21 20:18:25
tags:
- Codeforces
- 构造
categories: 题解
top:
---

## 题意简述

一个 $n\times n$ 的网格，在每列上方各有一个激光发射器，下方各有一个激光接收器。

第 $i$ 个接收器希望接收来自第 $p_i$ 个发射器的激光。你可以在一些位置 45 度放置镜子，使得最多的接收器满足其需求。

求出最多数量并给出一种放置镜子的方案，保证 $\{a_n\}$ 为排列，$n\le 1000$。

[[CF 1063E]](https://codeforces.com/problemset/problem/1063/E)

{% asset_img sample.png 这张图片可以方便地理解题意。 %}

<!-- more -->

## 主要思路

简单构造题。

容易发现除非对于任意 $i$ 均有 $i = p_i$ 时答案为 $n$，其余答案均为 $n - 1$。

等价地，设第 $i$ 个发射器希望射到 $a_i$ 列。

不妨钦定第 $1$ 个发射器无法满足，则对于点 $1$ 所在的环 $S$，容易使用 $|S| - 1$ 行对该环中的其他元素均满足，从第 $a_1$ 个发射器开始，依次处理即可。

对任意其他环 $T$，可以使用 $|T|$ 行使其中所有元素均满足：找到一个位置使得 $x > a_x, a_x < a_{a_x}$，则可以利用第一列满足该环——从第 $a_x$ 发射器开始依次处理即可。

以下是一个简单例子：

```plain
a:
4 6 2 7 3 5 1
. . . . . . .
/ . / . . . .
. . / . / . .
. . . . / / .
\ \ . . . \ .
/ . . . . . /
. . . \ . . \
```

## 参考代码

{% fold %}
```cpp
#include <bits/stdc++.h>
namespace my_std {
using namespace std;
#define reg register
#define Rint register int
#define FOR(i, a, b) for (register int i = (a), ed_##i = (b); i <= ed_##i; ++i)
#define ROF(i, a, b) for (register int i = (a), ed_##i = (b); i >= ed_##i; --i)
inline int read() {
    Rint ans = 0, f = 1;
    reg char c = getchar();
    while (!isdigit(c)) {
        f ^= (c == '-');
        c = getchar();
    }
    for (; isdigit(c); c = getchar())
        ans = ans * 10 + c - '0';
    return f ? ans : -ans;
}
} // namespace my_std
using namespace my_std;

const int N = 1010;
#define pcr(x) putchar(x)
#define pb push_back
inline void write(int x){
    static char tmp[10], *t = tmp;
    do{
        *++t = (x % 10) ^ 48, x /= 10;
    }while(x);
    while(t != tmp) pcr(*t--);
    pcr(10);
}

int n, a[N], b[N];
vector<int> v[N];
char g[N][N];
// '\' -
// '/' +

inline bool Identity(){
    FOR(i, 1, n) if(a[i] != i) return 0;
    return 1;
}
inline int Check(int i){
    int x = i;
    ROF(j, n, 1) if(g[j][x] != '.'){
        if(g[j][x] == '/'){
            ++x;
            while(x <= n && g[j][x] != '/') ++x;
            if(x > n){
                printf("%d %d: %d +\n", i, b[i], j);
                return 0;
            }
        }
        else{
            --x;
            while(x > 0 && g[j][x] != '\\') --x;
            if(x < 1){
                printf("%d %d: %d -\n", i, b[i], j);
                return 0;
            }
        }
    }
    if(x != b[i]) printf("%d %d %d\n", i, b[i], x);
    return x == b[i];
}
inline void Check(){
    FOR(i, 1, n) b[a[i]] = i;
    FOR(i, 1, n) Check(i);
}

int main() {
    n = read();
    FOR(i, 1, n) a[read()] = i;
    if(Identity()){
        write(n);
        FOR(i, 1, n){
            FOR(i, 1, n) pcr('.');
            pcr(10);
        }
        return 0;
    }
    write(n - 1);
    Rint c = n;
    {
        int x = a[1];
        while(x != 1){
            if(x < a[x]) v[c].pb(-x), v[c].pb(-a[x]);
            else v[c].pb(x), v[c].pb(a[x]);
            b[x] = 1, x = a[x], --c;
        }
    }
    FOR(i, 2, n) if(!b[i] && i != a[i]){
        const int t = c;
        int y = i;
        while(y < a[y] || a[y] > a[a[y]]) y = a[y];
        int x = a[y];
        while(x != y){
            if(x < a[x]) v[c].pb(-x), v[c].pb(-a[x]);
            else v[c].pb(x), v[c].pb(a[x]);
            b[x] = 1, x = a[x], --c;
        }
        v[c].pb(1), v[c].pb(x);
        v[t].pb(-1);
        b[y] = 1, --c;
    }
    FOR(i, 1, n) FOR(j, 1, n) g[i][j] = '.';
    FOR(i, 1, n){
        for(int x: v[i]){
            x > 0 ? g[i][x] = '/' : g[i][-x] = '\\';
        }
    }
    FOR(i, 1, n){
        FOR(j, 1, n) pcr(g[i][j]);
        pcr(10);
    }
    // Check();
    return 0;
}
```
{% endfold %}