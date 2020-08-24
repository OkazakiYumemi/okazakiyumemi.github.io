---
title: 「HDU5451」Best Solver
categories: 题解
urlname: HDU5451
date: 2020-08-24 18:42:49
tags:
- 数学
top:
---

~~这都啥年代的套路了~~

## 题意简述

For a given integer $x$ and a given prime number $p$, print
$$[(5 + 2\sqrt{6})^{1 + 2^x}]\%p$$
$[A]$ means the integer part of $A$.
$T$ test cases, $T\le 1000, 1\le x< 2^32, p\le 46337$.

<!-- more -->

## 主要思路

不难发现 $0 < 5 - 2\sqrt{6}< 1$，所以 $[(5 + 2\sqrt{6})^K] + 1 = (5 + 2\sqrt{6})^K + (5 - 2\sqrt{6})^K$。
然后根据共轭根式的性质，若 $(5 + 2\sqrt{6})^K = A + B\sqrt{6}$，则 $(5 - 2\sqrt{6})^K = A - B\sqrt{6}$。这东西显然可以维护根式然后快速幂。
还有个问题是 $1 + 2^x$ 太大了无法直接快速幂。但注意到我们实际上是在有限域上做计算，然后除了加法单位元（即 $0 + 0\sqrt{6}$）外元素个数是 $p^2 - 1$，所以循环节必然是 $p^2 - 1$ 的因数。
于是做完了。

## 参考代码

{% fold %}
```cpp
#include<iostream>
namespace my_std {
using namespace std;
#define reg register
#define Rint register int
#define FOR(i, a, b) for (register int i = (a), ed_##i = (b); i <= ed_##i; ++i)
typedef long long LL;
typedef pair<int, int> PII;
#define Templ(T) template <typename T>
Templ(T) inline T read() {
    reg T ans = 0;
    reg char c = getchar();
    while (!isdigit(c)) c = getchar();
    for (; isdigit(c); c = getchar()) ans = ans * 10 + c - '0';
    return ans;
}
} // namespace my_std
using namespace my_std;

#define MP make_pair
#define fr first
#define sc second

int ksm(Rint x, reg LL y, reg const int p){
    Rint res(1);
    for(; y; y >>= 1, x = (LL)x * x % p)
        if(y & 1) res = (LL)res * x % p;
    return res;
}
#define mul(x, y)                                 \
    MP((6ll * x.sc * y.sc + (LL)x.fr * y.fr) % p, \
       ((LL)x.fr * y.sc + (LL)x.sc * y.fr) % p)
PII ksm(reg PII x, reg LL y, reg const int p){
    reg PII res(1, 0);
    for(; y; y >>= 1, x = mul(x, x))
        if(y & 1) res = mul(res, x);
    return res;
}

inline void work(){
    LL n(read<LL>());
    int p(read<int>());
    int res(ksm(MP(5, 2), ksm(2, n, p * p - 1) + 1, p).fr);
    printf("%d\n", (res * 2 + p - 1) % p);
}

int main() {
    Rint esac = read<int>();
    FOR(o, 1, esac){
        printf("Case #%d: ", o);
        work();
    }
    return 0;
}
```
{% endfold %}