---
title: 「AGC041」Histogram Rooks
categories: 题解
urlname: AGC041
date: 2020-08-29 22:56:14
tags:
- AtCoder
- 计数
- 容斥
top:
---

## 题意简述

给定一个 $n\times n$ 的被破坏的棋盘，其中第 $i$ 列只剩了最底下的 $h_i$ 个。

求有多少种在棋盘中放置「車」的数量使得棋盘被「車」控制，即其他位置均在「車」的攻击范围内。$n\le 400$。

<!-- more -->

## 主要思路

这题官方题解 $O(n^3)$，然后有一群人踩标算……

### $O(n^3)$

考虑 $[l, r]$ 列中除了前 $h$ 行外的格子。这块区域要合法，则每列有三种情况：
1. 此列有「車」。
2. 此列无「車」，但这些格子都被其他「車」控制了。
3. 此列无「車」，且有部分格子没被其它「車」控制，则必须该列前 $h$ 行有「車」。

显然我们一次可以处理一段**极长的**每列都不少于 $h$ 行的列区间。于是状态数 $O(nh)$。

但是有个好性质，就是第 2 类列和第 3 类列不能共存。于是对状态整一个 $0\/1$ 表示是有第 2 类列还是第 3 类列，然后按「$[l, r]$ 列中有无 $h_i=h$」疯狂讨论转移即可。

反正一通讨论后，有的话就是个背包（因为 $h$ 再升高就要分裂成几段了），没有的话就是个递推啥的。

反正最后能推出来复杂度是 $O(n^3)$ 就是了。

### $O(n^2)$

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
inline void qmo(int &x) { x += (x >> 31) & mod; }
} // namespace my_std
using namespace my_std;

#define N 410
#define inf (1145141919)
int n, h[N];
int c[2][N][N], f[N][N][2];
int T;

inline void init(){
    n = read();
    FOR(i, 1, n) h[i] = read();
    f[0][0][0] = -inf;
    FOR(i, 0, n) c[0][i][0] = c[0][0][i] = 1;
    FOR(i, 1, n){
        qmo(c[0][i][1] = -mod + (c[0][i - 1][1] << 1));
        qmo(c[1][i][1] = c[0][i][1] - 1);
        FOR(j, 2, n){
            qmo(c[0][i][j] = (i64)c[0][i][1] * c[0][i][j - 1] % mod);
            qmo(c[1][i][j] = (i64)c[1][i][1] * c[1][i][j - 1] % mod);
        }
    }
}

inline int dp(int l, int r){
    if(l > r) return 0;
    static int tmp[N][2];

    reg const int id(++T), len(r - l + 1);
    reg int (*g)[2] = f[id];
    Rint mn(inf), num(0);
    FOR(i, l, r) chkmin(mn, h[i]);
    g[0][0] = 1;
    for(Rint L(l), R(l), siz(0); R <= r + 1; ++R)
        if(h[R] == mn || R == r + 1){
        reg int (*u)[2] = f[dp(L, R - 1)];
        if(u[0][0] == -inf){
            ++num, L = R + 1;
            continue;
        }
        memset(tmp, 0, sizeof(tmp));
        FOR(i, 0, siz) FOR(j, 0, R - L){
            FOR(x, 0, 1) FOR(y, 0, 1){
                qmo(tmp[i + j][x | y] += (i64)g[i][x] * u[j][y] % mod - mod);
            }
        }
        memcpy(g, tmp, sizeof(tmp));
        ++num, siz += R - L;
        L = R + 1;
    }//处理儿子
    FOR(o, 1, num - 1){
        memcpy(tmp, g, sizeof(tmp));
        FOR(j, 0, len - 1){
            qmo(tmp[j + 1][0] -= g[j][0]);
            qmo(tmp[j + 1][1] += g[j][0] - mod);
        }
        memcpy(g, tmp, sizeof(tmp));
    }//处理 num - 1 个最低的位置
    reg const int hgh(mn - max(h[l - 1], h[r + 1]));
    FOR(i, 0, len){
        qmo(g[i][0] = (i64)c[0][len - i][hgh] * g[i][0] % mod);
        qmo(g[i][1] = (i64)c[1][len - i][hgh] * g[i][1] % mod);
    }//乘容斥系数
    return id;
}

inline void get_ans(reg const int &t){
    Rint ans(0);
    FOR(i, 0, n){
        qmo(ans += f[t][i][0] - mod);
        qmo(ans += f[t][i][1] - mod);
    }
    printf("%d\n", ans);
}

int main() {
    init();
    get_ans(dp(1, n));
    return 0;
}
```
{% endfold %}

### 参考资料

[AGC041F Histogram Rooks | PuFanyi's Blog](https://pufanyi.gitee.io/agc041_f/)
[AtCoder Grand Contest 041 | hl666's Blog](https://www.cnblogs.com/cjjsb/p/12902274.html)
[AGC041F Histogram Rooks | Itst's Blog](https://www.cnblogs.com/Itst/p/12612116.html)