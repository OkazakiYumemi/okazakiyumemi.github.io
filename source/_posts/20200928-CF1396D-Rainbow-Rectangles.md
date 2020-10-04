---
title: 「CF1396D」Rainbow Rectangles
categories: 题解
urlname: CF1396D
date: 2020-09-27 22:57:00
tags:
- Codeforces
- log数据结构
- 线段树
top:
---

## 题意简述

[CF1396D](https://codeforces.com/contest/1396/problem/D)

有 $n$ 个豆子，第 $i$ 个坐标为 $(x_i + \varepsilon, y_i + \varepsilon)$，颜色 $c_i$（$\varepsilon = 0.5$）。总的坐标范围 $[0, lim]$，求有多少个格点矩形能够包含所有不同颜色的豆子。$n\le 2000$。

<!-- more -->

## 主要思路

离散化，枚举矩形左边界做 $O(n)$ 次。
考虑把右边界从 $lim$ 枚举回左边界。

那么现在左右边界确定。考虑对每一个纵坐标 $y$ 维护 $a_y$，满足对于下边界为 $y$ 时，只要上边界大于 $a_y$ 整个矩形就包含所有颜色。
不难发现，任意时刻 $a_y$ 单调不降；右边界从右往左枚举的时候，一次只会把 $a$ 的一个后缀对某个数取 max。
线段树，$O(n^2\log n)$。

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
#define mod 1000000007
// inline int qmo(const int &x){ return x + (((mod - x) >> 31) & mod); }
inline int qmo(const int &x){ return x >= mod ? x - mod : x; }
typedef long long i64;
Templ(_Tp) inline int chkmin(_Tp &x, _Tp y) { return x > y ? x = y, 1 : 0; }
Templ(_Tp) inline int chkmax(_Tp &x, _Tp y) { return x < y ? x = y, 1 : 0; }
} // namespace my_std
using namespace my_std;

#define N 2010
int n, m, lim;

struct Vertice{
    int x, y, i, c;
}a[N];
int Y[N], C[N], F[N];

struct QRY{
    int lst[N], nxt[N];
    inline QRY(){}
    inline QRY(const QRY &_Q){
        memcpy(lst, _Q.lst, sizeof(lst));
        memcpy(nxt, _Q.nxt, sizeof(nxt));
    }
    inline void del(const int &i){
        lst[nxt[i]] = lst[i];
        nxt[lst[i]] = nxt[i];
    }
}Q;
int lsc[N];
int bar[N];

namespace DS{
#define mid ((l + r) >> 1)
#define lc ((t) << 1)
#define rc ((t) << 1 | 1)
#define ls lc, l, mid
#define rs rc, mid + 1, r
struct Node{
    int mn, mx, sum, tag;
    inline Node(int _mn = 0, int _mx = 0, int _sum = 0, int _tag = 0)
        : mn(_mn), mx(_mx), sum(_sum), tag(_tag){}
    inline Node operator +(const Node &rhs)const{
        return Node(min(mn, rhs.mn), max(mx, rhs.mx), qmo(sum + rhs.sum), 0);
    }
}a[N << 2];
inline void nd(int t, int l, int r, int v){
    a[t] = Node(v, v, (i64)(Y[r] - Y[l - 1]) * v % mod, v);
}
void build(int t, int l, int r){
    if(l == r) return nd(t, l, r, F[l]);
    build(ls);
    build(rs);
    a[t] = a[lc] + a[rc];
}
void update(int t, int l, int r, int L, int v){
    if(a[t].mn >= v) return;
    if(a[t].mx <= v && l == L) return nd(t, l, r, v);
    if(a[t].tag){
        nd(ls, a[t].tag);
        nd(rs, a[t].tag);
        a[t].tag = 0;
    }
    if(L <= mid) update(ls, L, v);
    update(rs, max(mid + 1, L), v);
    a[t] = a[lc] + a[rc];
}
#undef mid
#undef lc
#undef rc
#undef ls
#undef rs
}

int ans;

int main() {
    n = read(), m = read(), lim = read();
    FOR(i, 1, n) a[i].x = read(), a[i].y = read(), a[i].c = read();
    sort(a + 1, a + n + 1, [&](const Vertice &lhs, const Vertice &rhs){
        return lhs.y < rhs.y;
    });
    a[0].x = a[0].y = Y[0] = -1;
    a[n + 1].x = a[n + 1].y = Y[n + 1] = lim;
    FOR(i, 1, n) a[i].i = i, Y[i] = a[i].y;
    FOR(i, 1, n){
        Q.nxt[Q.lst[i] = lsc[a[i].c]] = i;
        Q.nxt[i] = n + 1;
        lsc[a[i].c] = i;
    }
    sort(a + 1, a + n + 1, [&](const Vertice &lhs, const Vertice &rhs){
        return lhs.x < rhs.x;
    });
    for(Rint i(1); i <= n; Q.del(a[i].i), ++i){
        if(a[i].x == a[i - 1].x) continue;
        FOR(j, 1, i - 1) C[a[j].i] = 0;
        FOR(j, i, n) C[a[j].i] = a[j].c;
        for(Rint l(1), r(1), t(0); l <= n; ++l){
            for(; r <= n && t < m; ++r) C[r] && !bar[C[r]]++ && ++t;
            F[l] = t < m ? lim : Y[r - 1];
            C[l] && !--bar[C[l]] && --t;
        }
        DS::build(1, 1, n);
        QRY q(Q);
        Rint res(0), id;
        ROF(j, n, i){
            res = qmo(res + ((Y[n] + 1ll) * lim - DS::a[1].sum) % mod * 
                (a[j + 1].x - a[j].x) % mod);
            id = a[j].i;
            DS::update(1, 1, n, q.lst[id] + 1, Y[q.nxt[id]]);
            q.del(id);
        }
        res = (i64)res * (a[i].x - a[i - 1].x) % mod;
        ans = qmo(ans + res);
    }
    printf("%d\n", ans);
    return 0;
}
```
{% endfold %}

突然发现我开始染上了新的恶习：

对于几个`if`短句喜欢直接拿`&&`连接。

```cpp
if(A) if(B) C;
(A) && (B) && (C);
```