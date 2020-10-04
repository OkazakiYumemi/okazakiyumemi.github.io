---
title: 「CF1407E」Egor in the Republic of Dagestan
categories: 题解
urlname: CF1407E
date: 2020-09-27 23:31:16
tags:
- Codeforces
- dp
top:
---

## 题意简述

[CF1407E](https://codeforces.com/contest/1407/problem/E)

$n$ 个点 $m$ 条边的有向图，边有黑白两种颜色。现在要给点染色，每个点染成黑或白，白点只能走它连出的白色边，黑点只能走它连出的黑色边，求一种染色方案使得 $1\to n$ 的最短路径最长，输出方案。

($1 \le n, m \le 500000$)

<!-- more -->

## 主要思路

发现正着做不好做，于是把边反过来倒着做。设 $f(x, c)$ 为当点 $x$ 颜色为 $c$ 时 $n\to x$ 的最短路径最长长度。直接 bfs 就行了。

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
#define Templ(T) template <typename T>
inline int read() {
    Rint ans = 0;
    reg char c = getchar();
    while (!isdigit(c)) c = getchar();
    for (; isdigit(c); c = getchar())
        ans = ans * 10 + c - '0';
    return ans;
}
Templ(_Tp) inline int chkmin(_Tp &x, _Tp y) { return x > y ? x = y, 1 : 0; }
Templ(_Tp) inline int chkmax(_Tp &x, _Tp y) { return x < y ? x = y, 1 : 0; }
} // namespace my_std
using namespace my_std;

#define N 500010
#define inf 0x3f3f3f3f
int n, m;
vector<int> E[N][2];
int f[N][2];
int Q[N];
#define f(x) max(f[x][0], f[x][1])

inline void bfs(){
    Rint *ql(Q), *qr(Q);
    *qr++ = n;
    f[n][0] = f[n][1] = 0;
    while(ql != qr){
        Rint u = *ql++;
        for(Rint v: E[u][0]){
            if(f[v][0] <= n) continue;
            f[v][0] = f(u) + 1;
            if(f(v) < n) *qr++ = v;
        }
        for(Rint v: E[u][1]){
            if(f[v][1] <= n) continue;
            f[v][1] = f(u) + 1;
            if(f(v) < n) *qr++ = v;
        }
    }
}

int main() {
    n = read(), m = read();
    Rint u, v, w;
    FOR(i, 1, m){
        u = read(), v = read(), w = read();
        E[v][w].push_back(u);
    }
    memset(f, 0x3f, sizeof(f));
    bfs();
    printf("%d\n", f(1) > n ? -1 : f(1));
    FOR(i, 1, n) printf("%d", f[i][0] < f[i][1]);
    return 0;
}
```
{% endfold %}