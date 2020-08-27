---
title: 「AGC006F」Blackout
categories: 题解
urlname: AGC006F
date: 2020-08-27 17:08:46
tags:
- AtCoder
- 图论
- 猜结论
top:
---

## 题意简述

[AGC 006F](https://atcoder.jp/contests/agc006/tasks/agc006_f)

给定平面上的 $m$ 个整点 $(x, y), x, y\in[1, n]$，将这 $n$ 个点染黑。然后任取两个黑色的点 $(a, b), (b, c)$ 并将点 $(c, a)$ 染黑，直至无法再染黑任何点为止。

求最后平面上有多少点被染黑。$n, m\le 5\times 10^5$。

<!-- more -->

## 主要思路

不妨建出一个 $n$ 个点的图，对于每一对给定的 $(x, y)$ 则在图中连边 $x\to y$。
则对于任何一条长为 $2$ 的链，其形态会变成一个三元环。

考虑对每个弱连通块三染色。
- 如果染色成功，则最后连通块中所有颜色 $0,1,2$ 的点会分别向所有颜色 $1,2,0$ 的点连边。
- 如果不成功但用了每种颜色，则可知一定出现了长度不为 $3$ 的倍数的环，这个连通块会被连成完全图（包括自环）。
- 如果不成功且没有用到每种颜色，则不会新加边。

于是做完了。

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
#define GO(x, p, e, i, v) for (register int i = p[x].head, v; i; i = e[i].link)
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
} // namespace my_std
using namespace my_std;

#define N 500010
int n, m;
int f[3];
int comp;
struct Edge{
    int link, to;
}e[N << 1];
struct Vertice{
    int head, col;
}p[N];
int ecnt(0);
#define col(x) p[x].col
#define head(x) p[x].head
inline void A_E(int u, int v){
    e[++ecnt] = (Edge){head(u), v};
    head(u) = ecnt;
}
inline int qm3(reg const int &x){
    return x > 2 ? x - 3 : x;
}
inline i64 sqr(reg const int &x){
    return (i64)x * x;
}

inline void dfs(int u){
    Rint t;
    ++f[col(u)];
    GO(u, p, e, i, v){
        v = e[i].to;
        if(i & 1) ++ecnt;
        t = qm3(col(u) + 1 + (!(i & 1)));
        if(col(v) == -1){
            col(v) = t, dfs(v);
        }
        else{
            if(col(v) != t) comp = 1;
        }
    }
}

int main() {
    n = read(), m = read();
    Rint u, v;
    FOR(i, 1, m){
        u = read(), v = read();
        A_E(u, v), A_E(v, u);
    }
    FOR(i, 1, n) col(i) = -1;
    reg i64 ans(0);
    FOR(i, 1, n) if(col(i) == -1){
        col(i) = 0, f[0] = f[1] = f[2] = 0;
        ecnt = 0, comp = 0;
        dfs(i);
        if(comp){
            ans += sqr(f[0] + f[1] + f[2]);
            continue;
        }
        if(!f[0] || !f[1] || !f[2]){
            ans += ecnt;
            continue;
        }
        ans += (i64)f[0] * f[1] + (i64)f[1] * f[2] + (i64)f[2] * f[0];
    }
    printf("%lld\n", ans);
    return 0;
}
```
{% endfold %}