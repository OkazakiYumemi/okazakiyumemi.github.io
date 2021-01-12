---
title: 「CF1408H」Rainbow Triples
categories: 题解
urlname: CF1408H
date: 2021-01-12 21:26:40
tags:
- Codeforces
- 模拟网络流
- log数据结构
top:
---

被模拟网络流打爆了。

## 题意简述

[CF1408H](https://codeforces.com/contest/1408/problem/H)

给定一个长度为 $n$ 的序列 $a$。
每次可以选出三个数删掉，需要保证左边和右边的都是 $0$，中间的不是 $0$ 且不能是之前删过的数。
求删掉数的最大次数。$n\le 5\times 10^5$。

<!-- more -->

## 主要思路

设 $cnt(x)$ 为序列中 $x$ 的数量，那么显然答案上界为 $m=\lfloor{cnt(0)\over 2}\rfloor$。
假设一个位置左边至少有 $m$ 个 $0$，那么其左边一定可以选出一个 $0$ 来合成答案。右边同理。
不难发现每个点总会满足上述性质中的一条，于是可以据此将节点划分为 $L, R$ 两个集合，$L$ 表示满足右边至少有 $m$ 个点的点集，$R$ 反之。

同时不难发现，如果 $L$ 中有数 $x$ 出现至少两次，更右的出现位置更优。$R$ 中同理。
所以对于每种颜色 $x$，仅保留两个节点，记为 $l(x), r(x)$。

考虑网络流建图：
- 源点 $S$ 向每个颜色 $x$ 连一条流量为 $1$ 的边（颜色边）；
- $x$ 向 $l(x),r(x)$ 连一条流量为 $1$ 的边；
- $L$ 中每个下标 $i$ 向 $i - 1$ 连边，$R$ 中每个下标 $i$ 向 $i + 1$ 连边，流量均为 $+\infty$；
- 每个 $0$ 向汇点 $T$ 连一条流量为 $1$ 的边（零边）。

此时，该图的最大流就是需要求的答案。
注意到，最大流等于最小割，考虑求解最小割。

显然，我们只会割去颜色边和零边。并且如果颜色边 $x$ 未被割去，则 $l(x)$ 前面的所有零边，$r(x)$ 后面的所有零边都需要被割去。
因此，最小割方案必定为割去一段前缀零边，一段后缀零边，一部分颜色边。

枚举割走的 $L$ 中前缀零边，则对于每一段 $R$ 中的零后缀，割走这一段零边的代价是前缀长度、后缀长度、**前后缀中均不存在的颜色数量**的和。

不难推出，只需维护一个支持区间减一、全局最值的数据结构即可。

于是做完了，时间复杂度 $O(n\log n)$。

## 参考代码

{% fold %}
```cpp
#include <bits/stdc++.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a), ed_##i = (b); i <= ed_##i; ++i)
#define ROF(i, a, b) for (int i = (a), ed_##i = (b); i >= ed_##i; --i)
inline int read() {
    int x = 0;
    char c = getchar();
    while (!isdigit(c)) c = getchar();
    for (; isdigit(c); c = getchar())
        x = x * 10 + c - '0';
    return x;
}
#define Templ(T) template <typename T>
Templ(_Tp) inline int chkmin(_Tp &x, _Tp y) { return x > y ? x = y, 1 : 0; }
Templ(_Tp) inline int chkmax(_Tp &x, _Tp y) { return x < y ? x = y, 1 : 0; }

#define N 500010
int n, a[N], m, Lm, Rm;
int vis[N], pre[N], suf[N], ans;
int Lw[N], Rw[N];
int lim, cc;

namespace DS{
#define lc t << 1
#define rc t << 1 | 1
#define ls lc, l, mid
#define rs rc, mid + 1, r
int mn[N << 2], del[N << 2];
inline void Pu(int t){
    mn[t] = min(mn[lc], mn[rc]);
}
inline void Pd(int t){
    if(del[t]){
        const int d = del[t];
        del[t] = 0;
        mn[lc] += d, mn[rc] += d;
        del[lc] += d, del[rc] += d;
    }
}
void Bld(int t, int l, int r){
    del[t] = 0;
    if(l == r) return mn[t] = l, void();
    int mid = (l + r) >> 1;
    Bld(ls), Bld(rs);
    Pu(t);
}
void Upd(int t, int l, int r, int L, int R){
    if(L <= l && r <= R) return --mn[t], --del[t], void();
    int mid = (l + r) >> 1;
    Pd(t);
    if(L <= mid) Upd(ls, L, R);
    if(R > mid) Upd(rs, L, R);
    Pu(t);
}
inline void Upd(int l, int r){
    if(l > r) return;
    Upd(1, 0, lim, l, r);
}
}

inline void work(){
    n = read(), cc = 0;
    memset(vis, 0, sizeof(int) * (n + 1));
    FOR(i, 1, n) vis[a[i] = read()] = 1;
    suf[n + 1] = 0;
    FOR(i, 1, n) if(vis[i]) ++cc;
    FOR(i, 1, n) pre[i] = pre[i - 1] + (!a[i]);
    ROF(i, n, 1) suf[i] = suf[i + 1] + (!a[i]);
    m = pre[n] >> 1, ans = min(m, cc);

    Lm = 0;
    FOR(i, 1, n) Lm += pre[i] <= m;
    Rm = Lm + 1;
    FOR(i, 1, n) Lw[i] = 0, Rw[i] = n + 1;
    FOR(i, 1, Lm) Lw[a[i]] = i;
    ROF(i, n, Rm) Rw[a[i]] = i;

    lim = suf[Rm];
    DS::Bld(1, 0, lim);
    FOR(i, 1, n) if(vis[i] && !Lw[i]){
        DS::Upd(suf[Rw[i]], lim);
        chkmin(ans, cc + DS::mn[1]);
    }
    FOR(i, 1, Lm) if(Lw[a[i]] == i){
        DS::Upd(suf[Rw[a[i]]], lim);
        chkmin(ans, cc + DS::mn[1] + pre[i]);
    }
    printf("%d\n", ans);
}

int main() {
    int T = read();
    while(T--) work();
    return 0;
}
```
{% endfold %}