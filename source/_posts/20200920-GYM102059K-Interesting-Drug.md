---
title: 「GYM102059K」Interesting Drug
categories: 题解
urlname: GYM102059K
date: 2020-10-02 22:43:59
tags:
- GYM
- dp
- log数据结构
top:
---

## 题意简述

[GYM102059K](https://codeforces.com/gym/102059/problem/K)

$n$ 个物品排列成一排。
从选择某个物品开始，每一步可以选择左/右最近的未被选择的物品，直到所有物品均被选择。
若第 $i$ 个物品恰好在第 $C_i$ 步被选择，则会贡献 $D_i$ 的价值。
对每个 $1\le i\le n$，求出第一步选择 $i$ 能得到的最大价值。
$n\le 300000$。

<!-- more -->

## 主要思路

选物品的过程可以看作是二位平面上从 $(0, 0)$ 开始向上和向右行走，从 $i$ 开始可以看作终点是 $(i - 1, n - i)$。获得的贡献对应两条边的边权。
提取出 $O(n)$ 个关键点 dp，用个树状数组维护即可 $O(n\log n)$。

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
typedef long long i64;
#define Templ(T) template <typename T>
Templ(_Tp) inline int chkmin(_Tp &x, _Tp y) { return x > y ? x = y, 1 : 0; }
Templ(_Tp) inline int chkmax(_Tp &x, _Tp y) { return x < y ? x = y, 1 : 0; }
} // namespace my_std
using namespace my_std;

#define MMR (1 << 23)
struct INPUT{
    char buf[MMR], *s;
    INPUT(){ s = buf, fread(buf, 1, MMR, stdin); }
    inline INPUT& operator >>(int &x){
        x = 0;
        while(*s < 48) ++s;
        while(*s > 47) x = x * 10 + *s++ - 48;
        return *this;
    }
}fin;
struct OUTPUT{
    char buf[MMR], *s;
    OUTPUT(){ s = buf; }
    inline OUTPUT& operator <<(i64 x){
        static char tmp[20];
        reg char *top = tmp;
        do{
            *++top = (x % 10) ^ 48, x /= 10;
        }while(x);
        while(top != tmp) *s++ = *top--;
        *s++ = 32;
        return *this;
    }
    ~OUTPUT(){ fwrite(buf, 1, s - buf, stdout); }
}fout;

#define N 300010
#define inf 1145141919810364364ll
int n;
int C[N], D[N];

struct QRY{
    int x, y, w, tp;
    QRY(int _x = 0, int _y = 0, int _w = 0, int _tp = 0):
        x(_x), y(_y), w(_w), tp(_tp){}
    inline bool operator <(const QRY &r)const{
        return x == r.x ? (tp == r.tp ? y > r.y : tp < r.tp) : x < r.x;
    }
}Q[N << 1];
int qc;

struct BIT{
    i64 a[N];
    inline void upd(int x, const i64 &v){
        for(; x; x -= (-x) & x) chkmax(a[x], v);
    }
    inline i64 qry(int x){
        reg i64 v(-inf);
        for(; x <= n; x += (-x) & x) chkmax(v, a[x]);
        return v;
    }
}B;

int main() {
    fin >> n;
    FOR(i, 1, n) fin >> C[i];
    FOR(i, 1, n) fin >> D[i];
    FOR(i, 1, n){
        Q[++qc] = QRY(i + 1, i + C[i] - 1, D[i], 0);
        Q[++qc] = QRY(i - C[i] + 1, i - 1, D[i], 1);
    }
    sort(Q + 1, Q + qc + 1);
    const QRY *ed(Q + qc + 1);
    reg QRY *t(Q + 1);
    while(t != ed && t->x < 1) ++t;
    FOR(i, 1, n){
        for(; t != ed && t->x <= i; ++t){
            // if(t->tp) B.upd(t->y, B.qry(t->y + 1) + t->w);
            // else B.upd(t->y, B.qry(t->y) + t->w);
            B.upd(t->y, B.qry(t->y + t->tp) + t->w);
        }
        fout << B.qry(i) + (C[i] == 1 ? D[i] : 0);
    }
    return 0;
}
```
{% endfold %}