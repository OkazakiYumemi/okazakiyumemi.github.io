---
title: 「CF641F」Little Artem and 2-SAT
categories: 题解
urlname: CF641F
date: 2020-08-26 19:00:48
tags:
- Codeforces
- 2-SAT
- bitset
top:
---

## 题意简述

[CF 641F](https://codeforces.com/contest/641/problem/F)

对于 $n$ 个布尔元素 $a(x), x\in [0, n)$，给定两个 2-SAT 问题，问是否解集相同，否则构造一组解使得其仅为其中之一的解。

两个 2-SAT 均形如 $\land_{i = 0}^{m - 1}(f(x_{i, 0})\lor f(x_{i, 1}))$。

其中 $x_{i, j}\in[0, 2n)$，并有恒等式 $a(x) = \lnot f(2x) = f(2x + 1)$。

$n, m\le 1000$。

<!-- more -->

## 主要思路

先把 2-SAT 图建出来，求一下是否有解。

如果两 2-SAT 均无解，则解集相同。
如果仅一个 2-SAT 有解，则构造其任意一个解即可。
故只需考虑两 2-SAT 均有解的情况。

先来个传递闭包，然后有一些变量的值就是确定的（即可从该变量的某状态到达另一状态）。
如果有一个变量 $f(x)$ 仅在一个 2-SAT 中被确定，则钦定 $f(x)=0$，求出另一 2-SAT 的一个解即可。
如果有一个变量在两个 2-SAT 中被确定的值不同，直接求出某个 2-SAT 的任意一解即可。

现在确定的变量集合已经相同了。

设 $u\implies v$ 意为 $u$ 可达 $v$。
若存在 $u, v$ 使在一个 2-SAT 中有 $u\implies v$ 而另一 2-SAT 中没有，则钦定 $f(u)=1,f(v)=0$，在另一 2-SAT 中求一解即可。
否则两 2-SAT 解集相同。

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
#define MP make_pair
typedef unsigned long long u64;
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

#define N 2048

struct myB{
    u64 a[32];
    inline void reset(){ FOR(i, 0, 31) a[i] = 0; }
    inline void set(){ FOR(i, 0, 31) a[i] = ~0ull; }
    inline myB(){ reset(); }
    inline bool test(int x){ return (a[x >> 6] >> (x & 63)) & 1; }
    inline void set(int x){ a[x >> 6] |= 1ull << (x & 63); }
    inline void reset(int x){ a[x >> 6] &= ~(1ull << (x & 63)); }
    inline myB operator |(const myB &rhs)const{
        reg myB res;
        FOR(i, 0, 31) res.a[i] = a[i] | rhs.a[i];
        return res;
    }
    inline myB operator |=(const myB &rhs){
        FOR(i, 0, 31) a[i] |= rhs.a[i];
        return *this;
    }
    inline bool operator [](const int &x)const{
        return (a[x >> 6] >> (x & 63)) & 1;
    }
};

inline int trans(const int &u){
    return u > 0 ? (u - 1) << 1 | 1 : (-u - 1) << 1;
}

struct SAT2{
    int n, m, fix[N], fail;
    myB lnk[N];
    inline void dfs(int u){
        fix[u] = 1, fix[u ^ 1] = 0;
        FOR(v, 0, n * 2 - 1){
            if(lnk[u][v] && fix[v] == -1) dfs(v);
        }
    }
    inline void build(int _n, int _m){
        n = _n, m = _m;
        FOR(i, 1, m){
            Rint u = trans(read()), v = trans(read());
            lnk[u ^ 1].set(v), lnk[v ^ 1].set(u);
        }
        FOR(i, 0, 2 * n - 1) lnk[i].set(i), fix[i] = -1;
        FOR(v, 0, 2 * n - 1) FOR(u, 0, 2 * n - 1){
            if(lnk[u][v]) lnk[u] |= lnk[v];
        }//传递闭包
        fail = 0;
        FOR(i, 0, n - 1){
            if(lnk[i << 1][i << 1 | 1] && lnk[i << 1 | 1][i << 1])
            fail = 1;
        }//同元素正反相连
        if(fail) return;
        FOR(i, 0, n - 1){
            if(fix[i << 1] != -1 || fix[i << 1 | 1] != -1) continue;
            if(lnk[i << 1][i << 1 | 1]) dfs(i << 1 | 1);
            if(lnk[i << 1 | 1][i << 1]) dfs(i << 1);
        }
    }
    inline void solve(int f0 = -1, int f1 = -1){
        if(f0 >= 0) dfs(f0);
        if(f1 >= 0) dfs(f1);
        //钦定 f0, f1 为真
        FOR(i, 0, 2 * n - 1){
            if(fix[i] == -1) dfs(i);
        }
        FOR(i, 0, n - 1) printf("%d ", fix[i << 1 | 1]);
        putchar(10);
    }
}S0, S1;

int n, m0, m1;

int main() {
    n = read(), m0 = read(), m1 = read();
    S0.build(n, m0), S1.build(n, m1);
    if(S0.fail && S1.fail) return puts("SIMILAR"), 0;
    //都无解
    if(S0.fail) return S1.solve(), 0;
    if(S1.fail) return S0.solve(), 0;
    //仅一个有解

    FOR(i, 0, 2 * n - 1){
        if(S0.fix[i] && !S1.fix[i]) return S0.solve(i), 0;
        if(!S0.fix[i] && S1.fix[i]) return S1.solve(i), 0;
    }//确定的变量不相同或确定变量的值不同
    FOR(i, 0, 2 * n - 1){
        if(S0.fix[i] == -1){
            FOR(j, 0, i - 1){
                if(S0.fix[j] == -1 &&
                    !S0.lnk[i][j] && S1.lnk[i][j])
                    return S0.solve(i, j ^ 1), 0;
            }
        }
        if(S1.fix[i] == -1){
            FOR(j, 0, i - 1){
                if(S1.fix[j] == -1 &&
                    !S1.lnk[i][j] && S0.lnk[i][j])
                    return S1.solve(i, j ^ 1), 0;
            }
        }
    }
    //若一个 2SAT 中有 i -> j 而另一个没有，
    //则可在另一个中钦定 i 取真而 j 取假（即 (j ^ 1) 取真）
    return puts("SIMILAR"), 0;
}
```
{% endfold %}