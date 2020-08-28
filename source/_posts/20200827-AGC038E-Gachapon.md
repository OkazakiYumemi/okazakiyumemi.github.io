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

[粉兔题解](https://www.cnblogs.com/PinkRabbit/p/AGC038E.html)，好！

## 题意简述

[AGC 038E](https://atcoder.jp/contests/agc038/tasks/agc038_e)

有 $n$ 个物品，第 $i$ 个物品给定 $a_i$ 和 $b_i$。

每一个单位时间你会获得一个物品，设 $\sum_{i=1}^n a_i = \sum a, \sum_{i=1}^n b_i = \sum b$，则第 $i$ 个物品有 $\dfrac{a_i}{\sum a}$ 的概率获得。

计算第一次对于每个 $i$ 都有第 $i$ 个物品获得至少 $b_i$ 个的期望时间。

$n, \sum a, \sum b\le 400$。

<!-- more -->

## 主要思路

先 min-max 容斥，转化为对于每个非空子集 $S$ 求第一次自己中的任意一个物品获得至少 $b_i$ 个的期望时间，对答案贡献为 $(-1)^{|S|+1}$。
也就是满足子集 $S$ 中的所有物品获得的数量都少于 $b_i$ 个的期望时刻数量，加上 $1$。可以把开始无物品的状态也算作 $1$ 个时刻，即可略去 $+1$。

考虑计算这个期望时刻数量（包括初始时刻）。
即对每种物品获得的状态，计算这种状态在所有可能情况下的期望次数（包括初始时刻），再求和。
不难发现，一旦第一次变成了某种状态，只要再次获得 $S$ 中的物品，状态就会改变。
所以一个状态将期望持续 $\dfrac{\sum a}{\sum\limits_{i\in S}a_i}$ 个时刻。于是只用考虑每种情况的出现概率即可，这时就可以不考虑 $S$ 以外的物品的影响了。

假设第 $i$ 个物品获得了 $0\le x_i< b_i$ 次，$i\in S$，那么达到这种状态就是存在一个获得 $S$ 中的物品前缀满足 $i$ 恰好出现了 $x_i$ 次。
令 $X=\sum\limits_{i\in S}x_i,C=\sum\limits_{i\in S}a_i$，则概率即为 $X!\displaystyle\prod\limits_{i\in S}\left(\dfrac{a_i}{C}\right)^{x_i}\dfrac{1}{x_i!}=\displaystyle\dfrac{X!}{C^X} \prod_{i\in S} \frac{a_i^{x_i}}{(x_i)!}$。阶乘来源于多重组合数。

然后你发现这个状态只关系 $X, C$ 了。不妨设 $f(C, X)$ 为 $(-1)^{|S|+1}\displaystyle\prod_{i\in S} \frac{a_i^{x_i}}{(x_i)!}$ 的期望。
于是转移就非常简单了：加入 $i$ 时，枚举 $0\le x_i< b_i$，$f(C, X) \leftarrow^{-} f(C - a_i, k - x_i)\dfrac{a_i^{x_i}}{x_i!}$。

这样依次加入 $n$ 个物品，每次复杂度为 $O(b_iXC)$ 暴力，总时间复杂度 $O((\sum b)^2\sum a)$。
最后求答案的时候记得对于 $f(C, X)$ 乘上 $\dfrac{X!}{C^{X - 1}\sum a}$ 的系数。
<br><br>
在 EGF 层面考虑，其实这个东西就大概是个卷积形式。

然后也确实有一个暴推生成函数的做法，大概最后能化成 $\sum\limits_{n\ge 0}n![z^n]\sum\limits_{i, j}c_{i, j}z^i e^{jz}$ 之类的东西，然后那个 $j$ 是一堆概率乘一起，所以这玩意就收敛了……

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