---
title: 「CodeChef TRIPS」Children Trips
categories: 题解
urlname: CodeChef-TRIPS
date: 2020-08-31 02:30:44
tags:
- CodeChef
- 树论
- 树分块
- 根号分治
top:
---

## 题意简述

[CodeChef TRIPS](https://www.codechef.com/problems/TRIPS)

一棵树，边有 $1$ 或 $2$ 的边权，多次询问从点 $u$ 到 $v$，每次移动最多能走 $w$（不能在两点之间停下），要几次才能到达。
$n, m\le 10^5$。

<!-- more -->

## 主要思路

这个题感觉有一车 $O(n\sqrt{n})$ 做法？这里介绍一种在线的树分块做法。
设 $M = \sqrt{n}$。
先钦定根，定义点 $u$ 的深度 $dep(u)$ 为到根路径上的边数。

那么我们将 $M | dep(u)$ 的节点设为关键点，于是这棵树就被分为若干块（关键点属于其下面的块）。（若一块大小不足 $M$ 跟上面的块合并。）

这样每个块的大小就不小于 $M$，个数就不大于 $M$，且直径最大为 $2M$（此处直径指连通块中两点路径间最多的边的数量）。

显然对于询问 $(u, v, w)$，从两个人两边出发直到碰头所用的总步数和单人走完是相同的。

那么若 $u, v$ 在同一块中，则直接暴力每次将较深的一个往上**跳一条边**就行了。

若在不同块中，则由于块数量也为 $O(M)$，我们希望一次能够令一个点上**跳一个块**。

假设点 $u$，一次走的距离为 $i$（$i\le 2M$）。
设 $db(u)$ 为到最近的不同块的祖先的距离，$du(u)$ 为到父亲的距离；
$rc(u, i)$ 为向上走一次能到达的最远的点；
$sp(u, i)$ 表示向上走最少几次能跳出块，$rm(u, i)$ 为最后一次会剩余多少未使用距离。

考虑步长为 $w$，当前向上跳的点为 $u$，根号分治：
- 对 $w\le 2M$，由于处理了 $sp, rm$，一次可以**直接跳出一个块**；
- 对 $w > 2M$，由于块数 $O(M)$，可以把一次移动**拆成多次长为 $2M$ 的移动**，一步只跳一个块。

预处理？对 $rc$，继承父亲的；对 $sp, rm$ 继承 $rc(u, i)$ 的。
嘛还有些讨论细节啥的，不过不重要。

反正这样就 $O(n\sqrt{n})$ 了。

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
#define GO(x, e, i, v) for (register int i = head[x], v; i; i = e[i].link)
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

#define N 100010
#define BN 350
#define M 317
#define M2 634
#define swap(a, b) (a ^= b ^= a ^= b)
struct Edge{
    int link, to, wgh;
}e[N << 1];
int ecnt, head[N];
inline void A_E(int u, int v, int w){
    e[++ecnt] = (Edge){head[u], v, w};
    head[u] = ecnt;
}

int n, m;
int fth[N], du[N], dep[N], siz[N];
int key[BN], bc, bl[N];
int sk[N], st;
#define TB(u) key[bl[u]]

void dfs0(int u){
    siz[u] = 1;
    sk[st++] = u;
    GO(u, e, i, v){
        v = e[i].to;
        if(v == fth[u]) continue;
        fth[v] = u, du[v] = e[i].wgh;
        dep[v] = dep[u] + 1;
        dfs0(v);
        siz[u] += siz[v];
    }
    if(!(dep[u] % M) && siz[u] >= M){
        key[++bc] = u, bl[u] = bc;
        while(sk[--st] != u){
            bl[sk[st]] = bc;
        }
    }
}

int rc[N][BN << 1], db[N], sp[N][BN << 1], rm[N][BN << 1];
//点 u，跳一次距离 i：
//到达的点；与最近的块外祖先距离；跳几步能跳出块；最后一步会剩下多少；

void dfs1(int u){
    reg const int d(du[u]), f(fth[u]);
    rc[u][0] = u;
    rc[u][1] = d == 1 ? f : u;
    FOR(i, 2, M2){
        rc[u][i] = (i >= d) ? rc[f][i - d] : f;
    }
    if(bl[u] == bl[f]){
        Rint v;
        FOR(i, 2, M2){
            v = rc[u][i];
            if(bl[u] == bl[v]){
                sp[u][i] = sp[v][i] + 1;
                rm[u][i] = rm[v][i];
            }
            else{
                sp[u][i] = 1;
                rm[u][i] = rm[f][i] - d;
            }
        }
        db[u] = db[f] + d;
    }
    else{
        FOR(i, 2, M2){
            sp[u][i] = 1;
            rm[u][i] = i - d;
        }
        db[u] = d;
    }
    GO(u, e, i, v){
        v = e[i].to;
        if(v != f) dfs1(v);
    }
}

inline void init(){
    n = read();
    Rint u, v, w;
    FOR(i, 1, n - 1){
        u = read(), v = read(), w = read();
        A_E(u, v, w), A_E(v, u, w);
    }
    dfs0(1);
    dfs1(1);
    m = read();
}
//同一块中暴力跳
inline int queryb(int *u, int *s, const int &p){
    int ans(0);
    while(u[0] != u[1]){
        if(dep[u[0]] < dep[u[1]]) swap(u[0], u[1]), swap(s[0], s[1]);
        if(*s >= du[*u]) *s -= du[*u];
        else *s = p - du[*u], ++ans;
        *u = fth[*u];
    }
    if(s[0] + s[1] >= p) --ans;
    return ans;
}
//不同块，步长小，一步跳一块
inline int query0(int *u, const int &p){
    int ans(0), s[2] = {};
    while(bl[u[0]] != bl[u[1]]){
        if(bl[u[0]] > bl[u[1]]) swap(u[0], u[1]), swap(s[0], s[1]);
        if(db[*u] > *s){
            *u = rc[*u][*s];
            ans += sp[*u][p], *s = rm[*u][p];
        }
        else *s = rm[*u][*s];
        *u = fth[TB(*u)];
    }
    return ans + queryb(u, s, p);
}
//不同块，步长大，一步跳一块
inline int query1(int *u, const int &p){
    int ans(0), s[2] = {};
    while(bl[u[0]] != bl[u[1]]){
        if(bl[u[0]] > bl[u[1]]) swap(u[0], u[1]), swap(s[0], s[1]);
        if(db[*u] <= *s) *s -= db[*u], *u = fth[TB(*u)];
        else{
            if(*s > M2){
                *s -= M2 - (rc[*u][M2] == rc[*u][M2 - 1]);
                *u = rc[*u][M2];
            }
            else *u = rc[*u][*s], ++ans, *s = p;
        }
    }
    return ans + queryb(u, s, p);
}

int main() {
    init();
    Rint u[2], w;
    while(m--){
        u[0] = read(), u[1] = read(), w = read();
        printf("%d\n", (w <= M ? query0 : query1)(u, w));
    }
    return 0;
}
```
{% endfold %}