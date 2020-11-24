---
title: 「CF493E」Vasya and Polynomial
categories: 题解
urlname: CF493E
date: 2020-11-24 15:10:52
tags:
- Codeforces
- 数学
- 猜结论
top:
---

## 题意简述

[CF493E](https://codeforces.com/contest/493/problem/E)

给三个正整数 $A, B, C$，求有多少个**非负**整数系数多项式 $f(x)$，满足 $f(A)=B,f(B)=C$。
无穷个则输出`-1`。
有限个，则求出次数最高，并且系数从高到低字典序最小的一个。
$A, B, C\le 10^{18},T\le 100$。

<!-- more -->

## 主要思路

这个是打某个神奇模拟赛的时候做到的，~~看起来比原题加强了一点（好像并没有）~~。

首先特判掉特殊情况，只考虑 $A<B<C$。
记 $f(A)=B,f(B)=C$ 为「基本条件」。

发现非负整数系数不知道怎么弄，就先来个整数系数。

那么显然可以构造一次函数 $L(x) = t_1x + t_0$，其中 $t_1 = {C-B\over B-A}$。
如果 $t_1$ 不是整数，则不可能有满足「基本条件」的整系数多项式，直接无解。

考虑另一个满足「基本条件」的整系数多项式 $F(x)$，以及 $F(x)-L(x)=G(x)$。
不难发现有 $G(A)=0,G(B)=0$，故 $G(x)$ 必含有因式 $(x-A)(x-B)$。

于是一个满足「基本条件」的整系数多项式可以被写成 $F(x)=L(x)+(x-A)(x-B)P(x)$ 的形式，其中 $P(x)$ 也是整系数多项式。

我们希望在 $L(x)$ 的基础上将 $F(x)$ 修改为**非负**整系数多项式。
考虑到次数 $lim\le{\ln C\over\ln B}$ 不大，直接暴力求出 $P(x)$ 的系数即可。
具体来说就是对于次数 $i\le lim - 2$，假设 $F(x)$ 当前次数的系数 $a_i<0$，可以直接给 $P(x)$ 加一个 $\lceil{-a_i \over AB}\rceil x^i$ 来让其系数非负。

最后如果 $a_{lim-1}<0$ 就无解了，否则获得了一个唯一的解。

至于这个解为什么唯一，通过本做法比较难证明，建议阅读[原题解](https://codeforces.com/blog/entry/14985)。
~~当然也可以通过感性理解，比如发现自己没法构造出一种超过两个解的情况，就认为没有多解。~~

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
} // namespace my_std
using namespace my_std;

#define MMR 70
i64 A, B, C;
i64 T1, T0;
i64 K1, K0;
i64 a[MMR];
int a_l;

#define dvc(x, y) (((x) - 1) / (y) + 1)

void work(){
    if(A == B){
        if(A == C){
            if(A == 1) return puts("-1"), void();
            puts("2\n1 1 0");
            return void();
        }
        return puts("0"), void();
    }
    if(B == C){
        printf("1\n0 %lld\n", B);
        return void();
    }
    if(A > B || B > C){
        return puts("0"), void();
    }
    if((C - B) % (B - A) != 0){
        return puts("0"), void();
    }

    T1 = (C - B) / (B - A);
    T0 = B - T1 * A;

    const int lim = log2(C) / log2(B);
    if(lim == 1){
        if(T0 < 0) return puts("0"), void();
        printf("1\n1 %lld %lld\n", T1, T0);
        return;
    }

    K1 = (A + B), K0 = A * B;
    FOR(i, 0, lim) a[i] = 0;

    a[0] = T0, a[1] = T1;
    FOR(i, 0, lim - 2){
        if(a[i] < 0){
            i64 W = dvc(-a[i], K0);
            a[i] += K0 * W;
            a[i + 1] -= K1 * W;
            a[i + 2] += W;
        }
    }
    if(a[lim - 1] < 0){
        return puts("0"), void();
    }
    ROF(i, lim, 0) if(a[i] > 0){
        a_l = i;
        break;
    }
    printf("1\n%d ", a_l);
    ROF(i, a_l, 0) printf("%lld ", a[i]);
    puts("");
}

int main() {
    int esac;
    scanf("%d", &esac);
    while(esac --){
        scanf("%lld%lld%lld", &A, &B, &C);
        work();
    }
    return 0;
}
```
{% endfold %}