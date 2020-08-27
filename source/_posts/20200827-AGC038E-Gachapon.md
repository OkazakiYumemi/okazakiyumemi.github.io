---
title: 「AGC038E」Gachapon
categories: 题解
urlname: AGC038E
date: 2020-08-27 21:57:47
tags:
- AtCoder
- dp
- min-max容斥
top:
---



## 题意简述

[AGC 038E](https://atcoder.jp/contests/agc038/tasks/agc038_e)

有 $n$ 个物品，第 $i$ 个物品给定 $a_i$ 和 $b_i$。

每一个单位时间你会获得一个物品，设 $\sum_{i=1}^n a_i = \sum a, \sum_{i=1}^n b_i = \sum b$，则第 $i$ 个物品有 $\dfrac{a_i}{\sum a}$ 的概率获得。

计算第一次对于每个 $i$ 都有第 $i$ 个物品获得至少 $b_i$ 个的期望时间。

$n, \sum a, \sum b\le 400$。

<!-- more -->

## 主要思路

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
inline int ksm(int x, i64 y) {
    int res = 1;
    for (; y; y >>= 1, x = (i64)x * x % mod)
        if (y & 1) res = (i64)res * x % mod;
    return res;
}
} // namespace my_std
using namespace my_std;

#define N 410
int fac[N], ifac[N];
int n;
inline void init(int lim){
    *fac = *ifac = 1;
    FOR(i, 1, lim) fac[i] = (i64)i * fac[i - 1] % mod;
    ifac[lim] = ksm(fac[lim], mod - 2);
    ROF(i, lim - 1, 1) ifac[i] = (i64)(i + 1) * ifac[i + 1] % mod;
}

int a[N], b[N], Sa, Sb;
int f[N][N];

int main() {
    n = read();
    FOR(i, 1, n) a[i] = read(), Sb += (b[i] = read()) - 1;
    init(Sb), Sb = 0;
    f[0][0] = mod - 1;
    FOR(i, 1, n){
        Sa += a[i], Sb += b[i] - 1;
        ROF(j, Sa, a[i]) ROF(k, Sb, 0){
            reg i64 t(1);
            FOR(x, 0, min(b[i] - 1, k)){
                qmo(f[j][k] -= t * ifac[x] % mod * f[j - a[i]][k - x] % mod);
                (t *= a[i]) %= mod;
            }
        }
    }
    Rint ans(0);
    FOR(j, 1, Sa){
        reg const int inv(ksm(j, mod - 2));
        reg i64 t(Sa * (i64)inv % mod);
        FOR(k, 0, Sb){
            qmo(ans += (t * fac[k] % mod * f[j][k] % mod) - mod);
            (t *= inv) %= mod;
        }
    }
    printf("%d\n", ans);
    return 0;
}
```
{% endfold %}