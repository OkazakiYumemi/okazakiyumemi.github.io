---
title: 「AGC043C」Giant Graph
categories: 题解
urlname: AGC043C
date: 2020-08-27 11:12:32
tags:
- AtCoder
- 图论
- 博弈论
top:
---

## 题意简述

[AGC 043C](https://atcoder.jp/contests/agc043/tasks/agc043_c)

给三个点数均为 $n$ 的无向图 $G_0, G_1, G_2$，构造一张新无向图 $W$，点数为 $n^3$，每个点的形式为 $(x_0, x_1, x_2)$。
对于原来 $G_0$ 中的一条边 $u, v$，连接所有 $(u, x_1, x_2), (v, x_1, x_2)$。$G_1, G_2$ 中的边同理。
点 $(x_0, x_1, x_2)$ 的点权为 $V^{x_0 + x_1 + x_2}$，其中 $V = 10^{18}$。
求 $W$ 的最大权独立集的权值。$n, m_0, m_1, m_2\le 10^5$。

<!-- more -->

## 主要思路

显然贪心优先选 $x_0 + x_1 + x_2$ 大的是最优的。不妨设所有的边的方向为从权值小的点连向权值大的。
那么一个点被选仅当它的出边中没有点被选。
不妨将其想象成一个公平游戏，则 $sg(x_0, x_1, x_2) = \operatorname{mex}\limits_{e((x_0, x_1, x_2), (y_0, y_1, y_2))}sg(y_0, y_1, y_2)$，为 $0$ 则选。
然后观察到**三个维度是独立的**，可以分开来做。
注意到这样的 SG 函数的上界是 $O(\sqrt{m})$ 的。
于是甚至不用写 FWT，暴力枚举就好了。

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
#define pb push_back
typedef long long i64;
#define Templ(T) template <typename T>
inline int read() {
    reg int ans = 0, f = 1;
    reg char c = getchar();
    while (!isdigit(c)) f ^= (c == '-'), c = getchar();
    for (; isdigit(c); c = getchar()) ans = (ans << 1) + (ans << 3) + (c ^ 48);
    return f ? ans : -ans;
}
Templ(_Tp) inline int chkmin(_Tp &x, _Tp y) { return x > y ? x = y, 1 : 0; }
Templ(_Tp) inline int chkmax(_Tp &x, _Tp y) { return x < y ? x = y, 1 : 0; }
#define mod 998244353
inline void inc(int &x, const int &y) { x += y; if (x >= mod) x -= mod; }
inline void dec(int &x, const int &y) { x -= y; if (x < 0) x += mod; }
inline void qmo(int &x){ x += (x >> 31) & mod; }
} // namespace my_std
using namespace my_std;

#define N 100010
#define BN 610
const i64 V = i64(1e18) % mod;

int n;
struct Graph{
    vector<int> E[N];
    int sg[N], n, m, val[BN], mxsg;
    inline void build(){
        n = ::n, m = read();
        Rint u, v;
        FOR(i, 1, m){
            u = read(), v = read();
            if(u > v) u ^= v ^= u ^= v;
            E[u].pb(v);
        }
    }
    inline void topo(){
        int *c = new int[BN];
        mxsg = 0;
        memset(c, 0, sizeof(int) * BN);
        ROF(u, n, 1){
            for(Rint v: E[u]) c[sg[v]] = 1;
            Rint t(0);
            while(c[t]) ++t;
            chkmax(mxsg, sg[u] = t);
            for(Rint v: E[u]) c[sg[v]] = 0;
        }
        delete c;
        Rint x(V);
        FOR(i, 1, n){
            qmo(val[sg[i]] += x - mod);
            x = V * x % mod;
        }
    }
}G[3];

int main() {
    n = read();
    FOR(i, 0, 2) G[i].build(), G[i].topo();
    Rint ans(0);
    FOR(i, 0, G[0].mxsg) FOR(j, 0, G[1].mxsg){
        qmo(ans += ((i64)G[0].val[i] * G[1].val[j] % mod
            * G[2].val[i ^ j] % mod) - mod);
    }
    printf("%d", ans);
    return 0;
}
```
{% endfold %}