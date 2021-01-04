---
title: 「AGC015F」Kenus the Ancient Greek
categories: 题解
urlname: AGC015F
date: 2021-01-04 21:32:13
tags:
- AtCoder
- 找规律
top:
---

## 题意简述

给出下列代码：

```python
def F(x, y):
    if x < y: return F(y, x)
    if y == 0: return 0
    return F(y, x % y) + 1
def G(x, y):
    m = 0
    for i in range(1, x + 1):
        for j in range(1, y + 1):
            m = max(m, F(i, j))
    return m
def W(x, y):
    m = G(x, y)
    n = 0
    for i in range(1, x + 1):
        for j in range(1, y + 1):
            if F(i, j) == m: n += 1
    return n % 1000000007
```

有 $Q$ 个询问，每个询问两个整数 $x, y$，求 $G(x, y), W(x, y)$。$Q\le 3\times 10^5, x, y\le 10^{18}$。

<!-- more -->

## 主要思路

设斐波那契数列为 $f$，$f_0 = f_1 = 1$。

第一问简单，因为最小的 $(x, y)$ 满足 $F(x, y) = k$ 即为 $(f_k, f_{k+1})$。

设满足 $F(x, y) = G(x, y) = k$ 的二元组 $(x, y)$ 为 $k$ 阶高对子。
显然 $k$ 阶高对子数量巨大多，无法直接统计。

但是不难发现许多高对子经过一次迭代就会变得一样，于是试图把这些高对子缩在一起，用一个最小的来表示，称为 $k$ 阶妙对子。
显然，$(x, y)$ 是妙对子当且仅当 $(x, y)$ 是高对子且 $y\le 2x$。
那么 $(x, y)$ 这个妙对子实际上就表示了 $(x, y + i\times x), i\in \mathbf{N}$。
$k$ 阶妙对子的数量很少，是 $O(k)$ 个。

给出前面一些妙对子：
```plain
(1, 2)
(2, 3), (3, 4)
(3, 5), (4, 7), (5, 7)
(5, 8), (7, 11), (7, 12), (8, 11)
```

仔细一看，$k$ 阶妙对子就是将所有 $k-1$ 阶妙对子 $(x, y)\to (y, x + y)$，再加一个 $(f_{k+1}, f_{k-1} + f_{k+1})$。

然后就可以 $O(\log^2 lim + Q\log lim)$ 了。

## 参考代码

哦另外要注意的是 $(i, i)$ 均为 $1$ 阶高对子，要特判。

{% fold %}
```cpp
#include <bits/stdc++.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a), ed_##i = (b); i <= ed_##i; ++i)
typedef long long i64;
typedef pair<i64, i64> PLL;
#define pb emplace_back
#define X first
#define Y second
#define MP make_pair
#define mod 10000'00007
#define lim 100'0000'0000'0000'0000
#define N 128
i64 fib[N];
vector<PLL> exc[N];
int K;

const bool __init__ = [ ](){
    fib[0] = fib[1] = 1;
    K = 0;
    while(fib[K] <= lim) ++K, fib[K + 1] = fib[K] + fib[K - 1];

    exc[1].pb(MP(1, 2));
    FOR(i, 2, K){
        for(PLL t: exc[i - 1]) exc[i].pb(MP(t.Y, t.X + t.Y));
        exc[i].pb(MP(fib[i + 1], fib[i - 1] + fib[i + 1]));
    }
    return true;
}();

int main() {
    int Q;
    scanf("%d", &Q);
    while(Q--){
        i64 x, y, z(0);
        scanf("%lld%lld", &x, &y);
        if(x > y) swap(x, y);
        int k = 1;
        while(k < K && fib[k + 1] <= x && fib[k + 2] <= y) ++k;
        for(PLL t: exc[k]){
            if(t.X <= x && t.Y <= y) z += (y - t.Y) / t.X + 1;
            if(t.Y <= x) z += (x - t.Y) / t.X + 1;
            z %= mod;
        }
        if(k == 1) (z += x) %= mod;
        printf("%d %lld\n", k, z);
    }
    return 0;
}
```
{% endfold %}