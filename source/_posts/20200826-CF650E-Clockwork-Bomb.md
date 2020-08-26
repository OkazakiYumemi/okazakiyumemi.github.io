---
title: 「CF650E」Clockwork Bomb
categories: 题解
urlname: CF650E
date: 2020-08-26 21:19:50
tags:
- 树论
- 并查集
top:
---

## 题意简述

[CF 650E](https://codeforces.com/contest/650/problem/E)

给两棵树 $T_0, T_1$，每次删去 $T_0$ 的一条边后加入一条新边，要求加入后仍为树。求最少次数使得将 $T_0$ 修改为 $T_1$ 并构造方案。$n\le 500000$。

<!-- more -->

## 主要思路

~~可以强行用LCT做动态最小生成树无脑肝过去~~

显然改掉两棵树都有的边不优，猜测其他边均只需改变一次。

考虑对于 $T_0$ 中的一对父子关系 $f_0(v) = u$，若 $(u, v)$ 不在 $T_1$ 中出现应在断掉后如何连边。

一个简单的想法是直接连接 $(f_1(v), v)$，问题在于这条边可能已经在 $T_0$ 中连了。

想象一下由 $T_0, T_1$ 中都存在的边构成的子图，发现这其实是一堆连通块并且块内的结构在过程中是不变的。

设 $find(v)$ 为在**上述子图中** $v$ **所在连通块内**的在 $T_1$ 中**最浅的**祖先 $w$。根据上述子图的定义，$(f_1(w), w)$ 在 $T_0$ 中不存在且不在上述子图中的同一个连通块内，故删去 $(u, v)$ 后连接 $(f_1(w), w)$ 仍能保证其为一棵树。

求 $find(v)$ 可以并查集。复杂度 $O(n\alpha(n))$。

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
} // namespace my_std
using namespace my_std;

#define N 500010

vector<int> E[2][N];
int fth[2][N];
int ans[N][4], acnt;
int n;

int pr[N];
int find(int u){
    return u == pr[u] ? u : pr[u] = find(pr[u]);
}

inline bool chk_pr(int x, int y){
    if(!x || !y) return 0;
    return fth[0][x] == y || fth[0][y] == x;
}

void dfs_ff(const int &id, int u){
    for(Rint v: E[id][u]){
        if(v == fth[id][u]) continue;
        fth[id][v] = u, dfs_ff(id, v);
    }
}
void dfs_ce(int u){
    Rint t;
    for(Rint v: E[0][u]){
        if(v == fth[0][u]) continue;
        dfs_ce(v);
        if(fth[1][v] != u && fth[1][u] != v){
            t = find(v);
            ans[++acnt][0] = u;
            ans[acnt][1] = v;
            ans[acnt][2] = t;
            ans[acnt][3] = fth[1][t];
        }
    }
}

int main() {
    n = read();
    FOR(o, 0, 1){
        Rint u, v;
        FOR(i, 1, n - 1){
            u = read(), v = read();
            E[o][u].pb(v), E[o][v].pb(u);
        }
        dfs_ff(o, 1);
    }
    FOR(i, 1, n){
        pr[i] = chk_pr(i, fth[1][i]) ? fth[1][i] : i;
    }
    dfs_ce(1);
    printf("%d\n", acnt);
    FOR(i, 1, acnt){
        FOR(j, 0, 3) printf("%d ", ans[i][j]);
        putchar(10);
    }
    return 0;
}
```
{% endfold %}