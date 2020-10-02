---
title: 「CF1406E」Deleting Numbers
categories: 题解
urlname: CF1406E
date: 2020-09-23 23:08:50
tags:
- Codeforces
- 交互题
top:
---

## 题意简述

[CF1406E](https://codeforces.com/contest/1406/problem/E)

交互题。给定 $n$, 猜数 $x$ ($1\le x\le n$)。

交互库将维护一个集合 $S$，初始为不大于 $n$ 的所有正整数。交互库提供三个函数：

$A(a)$，返回求出 $S$ 中 $a$ 的倍数的个数 $(1\le a\le n)$。
$B(a)$，返回 $A(a)$，并将所有 $a$ 的倍数从 $S$ 中删去（$x$ 不删，$a > 1$）。
$C(a)$，报告猜得的数 $x = a$。

三个函数总共可以调用不超过 $10^4$ 次。

<!-- more -->

## 主要思路

未特殊提及时下列 $p$ 均为素数。

显然可以通过对每个素数 $B(p)$ 后求 $A(p^k)$ 得到 $x$ 的质因数分解。

发现 $10^5$ 内的素数及其幂次就有约 $10^4$ 个，然后暴毙了。

考虑大小分素数，称不大于 $\sqrt{n}$ 的为小素数，否则为大素数。

对于小素数可以直接通过上面的暴力得到。

如果 $x$ 有小素数因子，则显然最多有一个大素数因子，枚举大素数并求 $B(p)$ 即可得知是哪个大素数。

那么现在剩下的情况只有 $x$ 为大素数的。

发现 $A(1)$ 可以得到目前集合内剩余的数。

不妨直接暴力对每个大素数删除，每 $K$ 个 $B(p)$ 做一次 $A(1)$。
如果和上次差的不是 $K$，证明 $x$ 就在刚刚删去的 $K$ 个中，每个做一次 $A(p)$ 即可。

$K$ 直接取为不大于 $\sqrt{n}$ 的素数个数即可。

## 参考代码

然后还写挂了一会，自己写了一个 grader……

{% fold %}
```cpp
#include <bits/stdc++.h>
using namespace std;
#define reg register
#define Rint register int
#define FOR(i, a, b) for (register int i = (a), ed_##i = (b); i <= ed_##i; ++i)
#define ROF(i, a, b) for (register int i = (a), ed_##i = (b); i >= ed_##i; --i)
#define Templ(T) template <typename T>
Templ(_Tp) inline int chkmin(_Tp &x, _Tp y) { return x > y ? x = y, 1 : 0; }
Templ(_Tp) inline int chkmax(_Tp &x, _Tp y) { return x < y ? x = y, 1 : 0; }
inline int read() {
    Rint ans = 0;
    reg char c = getchar();
    while (!isdigit(c)) c = getchar();
    for (; isdigit(c); c = getchar())
        ans = ans * 10 + c - '0';
    return ans;
}
#define N 100010

int n;
int b[N];
int pr[N], *tp(pr);

#ifdef LOCAL

namespace grader{
int dt[N], X, cnt;
inline void init(int x){
    FOR(i, 1, n) dt[i] = 1;
    X = x, cnt = 0;
    printf("R: %d %d\n", n, X);
}
inline int A(int x){
    ++cnt;
    printf("A: %d\n", x);
    fflush(stdout);
    Rint res(0);
    for(Rint i(x); i <= n; i += x) res += dt[i];
    return res;
}
inline int B(int x){
    ++cnt;
    printf("B: %d\n", x);
    fflush(stdout);
    Rint res(0);
    for(Rint i(x); i <= n; i += x){
        if(dt[i]) ++res, dt[i] = (i == X);
    }
    return res;
}
inline int C(int x){
    ++cnt;
    printf(X == x ? "AC %d: %d\n" : "WA %d: %d\n", X, cnt);
    return 0;
}
}
using grader::A;
using grader::B;
using grader::C;
inline void init(){
    FOR(i, 2, n){
        if(!b[i]) *tp++ = i;
        for(Rint *j(pr); i * *j <= n && j != tp; ++j){
            b[i * *j] = 1;
            if(!(i % *j)) break;
        }
    }
    grader::init(read());
}

#else

inline int A(int x){
    // assert(x <= n);
    return printf("A %d\n", x), fflush(stdout), read();
}
inline int B(int x){
    // assert(x <= n && x > 1);
    return printf("B %d\n", x), fflush(stdout), read();
}
inline int C(int x){
    // assert(x <= n);
    return printf("C %d\n", x), fflush(stdout), 0;
}
inline void init(){
    FOR(i, 2, n){
        if(!b[i]) *tp++ = i;
        for(Rint *j(pr); i * *j <= n && j != tp; ++j){
            b[i * *j] = 1;
            if(!(i % *j)) break;
        }
    }
}

#endif

int ans(1);

int main() {
    n = read();
    init();
    reg const int m(tp - pr);
    reg const int K(sqrt(m));
    {
        for(Rint *i(pr); i != pr + K; ++i){
            B(*i);
            Rint f(1);
            while(f * *i <= n && A(f * *i)){
                f *= *i;
            }
            ans *= f;
        }
    }
    if(ans > 1){
        for(Rint *i(pr + K); (long long)ans * *i <= (long long)n && i != tp; ++i){
            if(B(ans * *i)) ans *= *i;
        }
        return C(ans);
    }else{
        Rint c(0);
        Rint lst(A(1)), now;
        for(Rint *i(pr + K); i != tp; ++i){
            B(*i);
            if(++c != K) continue;
            c = 0;
            now = A(1);
            if(lst - now != K){
                for(Rint *j(i); j != i - K; --j){
                    if(A(*j)) return C(*j);
                }
            }
            lst = now;
        }
        if(c){
            now = A(1);
            if(lst - now != c){
                for(Rint *j(tp - 1); j != tp - 1 - c; --j){
                    if(A(*j)) return C(*j);
                }
            }
        }
    }
    return C(ans);
}
```
{% endfold %}