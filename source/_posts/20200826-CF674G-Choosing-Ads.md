---
title: 「CF674G」Choosing Ads
categories: 题解
urlname: CF674G
date: 2020-08-26 15:49:23
tags:
- 线段树
top:
---

## 题意简述

[CF 674G](https://codeforces.com/contest/674/problem/G)

维护序列，支持：
- 区间推平。
- 区间查询出现频率不小于 $p\%$ 的数，**保证有解**。
$n, m\ge 1.5\times 10^5, 20\le p\le 100$。

<!-- more -->

## 主要思路

先考虑 $p = 50$。

这便是一个经典问题：长度为 $n$ 的数列中存在一个出现次数不小于 $n/2$ 的数，设计一个算法找到它。

只要每次删除两个不同的数，最后留下的那个数（或那些数，但这些数全部相同）就是要求的答案。

原理是，如果一个数出现了 $a$ 次，其中 $a\ge n−a$。
则两边都减去 $1$，仍有 $a−1\ge n−a−1=(n−2)−(a−1)$。

拓展情况可以如法炮制。令 $k = \lfloor\dfrac{100}{p}\rfloor$，则每次删除 $k + 1$ 个数，则对的数一定会留下来。

两个区间合并的过程显然可以 $O(k^2)$ 一堆分类讨论。上棵线段树，做完了，时间复杂度 $O(n k^2\log n)$。

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
typedef long long LL;
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

const int N = 150010;
int n, m;
inline int init(){
    n = read(), m = read();
    return 100 / read();
}
const int K = init();

struct Node{
    int d[6], c[6];
    Node(){
        memset(d, 0, 6 * sizeof(int));
        memset(c, 0, 6 * sizeof(int));
    }
    inline void print(){
        Rint t = 0;
        FOR(i, 0, K - 1) if(d[i]) ++t;
        printf("%d", t);
        FOR(i, 0, K - 1) if(d[i]) printf(" %d", d[i]);
        putchar(10);
    }
    friend Node operator +(reg Node L, reg Node R){
        FOR(i, 0, K - 1){
            reg bool flag(0);
            FOR(j, 0, K - 1) if(L.d[j] == R.d[i]){
                L.c[j] += R.c[i];
                flag = 1;
                break;
            }//有相同的数，就合并
            if(flag) continue;
            int o = K, mn = R.c[i];
            FOR(j, 0, K - 1) if(chkmin(mn, L.c[j])) o = j;
            FOR(j, 0, K - 1) L.c[j] -= mn;
            if(o < K) L.d[o] = R.d[i], L.c[o] = R.c[i] - mn;
            //没有相同的，就每种都删去 最小的一种的个数
        }
        return L;
    }
}tr[N << 2];
int len[N << 2], tag[N << 2];
#define lc (t << 1)
#define rc (t << 1 | 1)
#define mid ((l + r) >> 1)

inline void cover(int t, int x){
    tr[t] = Node();
    tr[t].d[0] = tag[t] = x;
    tr[t].c[0] = len[t];
}

inline void p_d(int t){
    if(tag[t]){
        cover(lc, tag[t]), cover(rc, tag[t]);
        tag[t] = 0;
    }
}

void build(int t, int l, int r){
    len[t] = r - l + 1;
    if(l == r){
        tr[t].d[0] = read(), tr[t].c[0] = 1;
        return;
    }
    build(lc, l, mid), build(rc, mid + 1, r);
    tr[t] = tr[lc] + tr[rc];
}

void update(int t, int l, int r, int L, int R, int x){
    if(L <= l && r <= R) return cover(t, x);
    p_d(t);
    if(L <= mid) update(lc, l, mid, L, R, x);
    if(R > mid) update(rc, mid + 1, r, L, R, x);
    tr[t] = tr[lc] + tr[rc];
}

Node query(int t, int l, int r, int L, int R){
    if(L <= l && r <= R) return tr[t];
    p_d(t);
    reg Node res;
    if(L <= mid) res = query(lc, l, mid, L, R);
    if(R > mid) res = res + query(rc, mid + 1, r, L, R);
    return res;
}

int main() {
    build(1, 1, n);
    Rint opt, l, r;
    while(m --){
        opt = read(), l = read(), r = read();
        if(opt == 2) query(1, 1, n, l, r).print();
        else update(1, 1, n, l, r, read());
    }
    return 0;
}
```
{% endfold %}