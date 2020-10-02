---
title: 「CF1404D」Game of Pairs
categories: 题解
urlname: CF1404D
date: 2020-09-23 18:19:44
tags:
- Codeforces
- 交互题
- 图论
top:
---

## 题意简述

[CF1404D](https://codeforces.com/contest/1404/problem/D)

交互题。给定一个正整数 $n$，两人玩游戏。游戏分两步：

1. 先手把 $1$ 到 $2n$ 拆成 $n$ 对。
2. 后手在每对中，选择恰好一个数。

如果选出的数之和为 $2n$ 的倍数，则后手胜，否则先手胜。

自己选择扮演先手还是后手，并给出必胜策略。$n\le 5\times 10^5$。

<!-- more -->

## 主要思路

$n$ 是偶数时，先手必胜。

构造 $(i, n + i)$ 即可。此时无论怎么选，最后和都 $\bmod\ n$ 为 $\binom{n}{2}$。

那么下面考虑 $n$ 为奇数。

发现若已经得到了和 $\bmod\ 2n$ 为 $n$ 的方案，取不在方案内的 $n$ 个数即满足和为 $2n$ 的倍数。

于是条件就弱化为「和为 $n$ 的倍数」。

发现总有一种方案使得 $\bmod\ n = 0, 1, \cdots, n - 1$ 的数各出现一次。证明如下：

考虑一张 $n$ 个点的图，并对于每一对数 $(x, y)$，将 $x\bmod n, y\bmod n$ 间连一条边。

那么每个点的度都为 $2$，故整张图必定是若干个环。

考虑一个大小为 $m$ 的环实际上是 $m$ 对数，每对数只能选一个；$\bmod n$ 相等的也恰好组成 $m$ 对。所以直接选出 $m$ 个数两两不同对且不 $\bmod n$ 相等即可。

于是做完了，复杂度线性。

## 参考代码

{% fold %}
```cpp
#include <bits/stdc++.h>
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

#define N 500010
int n;
int a[N << 1];
int f[N << 1];
int vis[N];

inline int C(int x){
    return (x > n) ? x - n : x + n;
}

inline int sol_even(){
    puts("First");
    FOR(i, 1, n) printf("%d ", i);
    FOR(i, 1, n) printf("%d ", i);
    puts("");
    fflush(stdout);
    return 0;
}
inline int sol_odd(){
    puts("Second");
    fflush(stdout);
    Rint x;
    FOR(i, 1, n << 1){
        x = read();
        if(f[x]) a[f[x]] = i, a[i] = f[x], f[x] = 0;
        else f[x] = i;
    }
    ROF(i, n << 1, 1){
        if(f[i] || f[C(i)]) continue;
        f[x = i] = 1;
        while(x = a[C(x)], !f[x] && !f[C(x)]){
            f[x] = 1;
        }
    }
    x = 0;
    FOR(i, 1, n << 1) x ^= i & f[i];
    FOR(i, 1, n << 1) if(f[i] ^= x) printf("%d ", i);
    puts("");
    fflush(stdout);
    return 0;
}

int main() {
    return (n = read()) & 1 ? sol_odd() : sol_even();
}
```
{% endfold %}