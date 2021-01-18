---
title: 「GYM102798F」Skeleton Dynamization
categories: 题解
urlname: GYM102798F
date: 2021-01-18 16:27:25
tags:
- GYM
- 图论
top:
---

## 题意简述

[GYM102798F](https://codeforces.com/gym/102798/problem/F)

想象一只 skeleton，抽象其骨头为无向边，关节为顶点，可以得到一个无向连通图。假设定点数为 $J$，该图为 $G$。

这只 skeleton 开始动了起来，我们记录了其在连续 $F$ 帧中的动作，并且建立了一张 spatial-temporal graph。

spatial-temporal graph 是一个有 $F\times J$ 个节点的图。每个节点可以用 $(f, j)$ 唯一表示，前者为其出现的帧，后者为其在 $G$ 中对应的标号。

两点 $(f_1, j_1), (f_2, j_2)$ 间有边，仅当满足以下条件之一：

- $j_1 = j_2, f_1 + 1 = f_2$；
- $f_1 = f_2$，且在 $G$ 中有边 $(j_1, j_2)$。

现在 $G$ 丢失了，只剩下了标号被打乱的 spatial-temporal graph。请还原可能的 $F, J$ 并构造方案。
如有多个方案，请最大化 $F$。

$n\le 10^5, m\le 2\times 10^5$。

<!-- more -->

## 主要思路

~~上面讲了那么多废话，其实题意就是将给定的图看作分层图，每层内部边相同，相邻层只有对应点连边，最大化层数。~~

设度数最小的点为 $s$，显然其（在某个最优方案）一定是第一层的点。显然，$s$ 的度数不超过 $O(\sqrt{m})$。

枚举 $s$ 在第二层对应的点 $t$。
从 $s,t$ 开始 bfs，则以 $s$ 为起点 bfs 到的点即为第一层的点。

已知第一层的点，就可以确定第二层的点，然后确定所有层的点。

如果过程中出现矛盾，则退出。

最后，检查每一层内部的边是否都相同。

时间复杂度 $O(m\sqrt{m})$。

## 参考代码

{% fold %}
```cpp
#include <bits/stdc++.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a), ed_##i = (b); i < ed_##i; ++i)
#define ROF(i, a, b) for (int i = (a), ed_##i = (b); i > ed_##i; --i)
// Note the unusual definition
typedef long long i64;
typedef vector<int> VI;
#define pb emplace_back
#define X first
#define Y second
#define SZ(x) ((int)(x).size())
#define Templ(T) template <typename T>
inline int read() {
    int x = 0;
    char c = getchar();
    while (!isdigit(c)) c = getchar();
    for (; isdigit(c); c = getchar())
        x = x * 10 + c - '0';
    return x;
}
Templ(_Tp) inline int chkmin(_Tp &x, _Tp y) { return x > y ? x = y, 1 : 0; }
Templ(_Tp) inline int chkmax(_Tp &x, _Tp y) { return x < y ? x = y, 1 : 0; }

#define N 100010
int n, m;
int F, J;// Frames, Joints
int ans[N];
int S;
VI E[N];

inline void work(const int &T){
    static int B[N], Q[N], w[N], res[N];
    static VI e[N];
    memset(B, -1, n << 2);
    memset(w, -1, n << 2);
    memset(res, -1, n << 2);

    int *ql(Q), *qr(Q);
    B[S] = 0, B[T] = 1;
    *qr++ = S, *qr++ = T;
    while(ql != qr){
        int u = *ql++;
        for(int v: E[u]) if(B[v] == -1){
            B[v] = B[u];
            *qr++ = v;
        }
    }

    int j(0), f, t(0);
    // (now) joints, frames
    FOR(i, 0, n) if(B[i] == 0){
        res[j] = i;
        w[i] = j++;
        t += SZ(E[i]);
    };
    if(n % j != 0) return;
    f = n / j;
    if(f < F) return;
    t -= j;
    if(t & 1) return;
    t >>= 1;
    if(f * t + (f - 1) * j != m) return;

    FOR(o, 1, f){
        int c = 0, *a = res + (o - 1) * j;
        FOR(i, 0, j){
            int u = a[i];
            for(int v: E[u]) if(w[v] == -1){
                if(a[i + j] != -1) return;
                w[v] = w[u] + j;
                a[i + j] = v;
                ++c;
            }
        }
        if(c != j) return;
    }

    FOR(i, 0, n) e[i].resize(0);
    FOR(o, 0, f){
        int *a = res + o * j;
        FOR(i, 0, j){
            int u = a[i];
            for(int v: E[u]) if(w[v] / j == w[u] / j) e[w[v]].pb(w[u] % j);
        }
    }
    FOR(i, j, n) if(e[i] != e[i - j]) return;

    J = j;
    F = f;
    memcpy(ans, res, n << 2);
}

int main() {
    n = read(), m = read();
    FOR(i, 0, m){
        int u(read() - 1), v(read() - 1);
        E[u].pb(v), E[v].pb(u);
    }
    {
        int mn = m;
        FOR(i, 0, n) if(chkmin(mn, SZ(E[i]))) S = i;
    }

    F = 1, J = n;
    FOR(i, 0, n) ans[i] = i;

    for(int i: E[S]) work(i);

    printf("%d %d\n", F, J);
    FOR(i, 0, n) printf("%d%c", ans[i] + 1, " \n"[i % J == J - 1]);
    return 0;
}
```
{% endfold %}