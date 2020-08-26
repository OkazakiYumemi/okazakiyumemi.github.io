---
title: 「AGC046D」Secret Passage
categories: 题解
urlname: AGC046D
date: 2020-08-26 22:51:15
tags:
- dp
top:
---

## 题意简述

[AGC 046D](https://atcoder.jp/contests/agc046/tasks/agc046_d)

给定一个`01`串 $S$，每次操作把最前面的两个字符删除，并将其中一个插入回串中。求能获得的串的个数，取模。

$|S|\le 300$。

<!-- more -->

## 主要思路

考虑这个过程，不妨设 $(i, c_0, c_1)$ 这个状态表示目前串最长的等于 $S$ 的后缀的子序列长度为 $|S| - i$，除此以外分别多出 $c_0, c_1$ 个`0`,`1`。

我们设 $tr(i, c_0, c_1)$ 为这个状态是否可达，$dp(i, c_0, c_1)$ 为这个状态的方案数。

（初始 $tr(0, 0, 0) = 1$，$dp(n, 0, 0) = 1$，一个要顺着一个要倒着转移。）

然后转移非常多情况要讨论，都扔到注释里了。~~都是力气活……~~

总之最后复杂度就是 $O(|S|^3)$ 了。

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
const int mod = 998244353;
inline void inc(int &x, const int &y) { x += y; if (x >= mod) x -= mod; }
inline void dec(int &x, const int &y) { x -= y; if (x < 0) x += mod; }
} // namespace my_std
using namespace my_std;

#define N 350
char s[N];
int n;
int dp[N][N][N];
bool tr[N][N][N];
//[i][c0][c1]: 剩下 i 个字符没有匹配，多了 c0 个 0 和 c1 个 1

int main() {
    scanf("%s", s + 1);
    n = strlen(s + 1);
    tr[0][0][0] = 1;
    FOR(i, 1, n){
        ROF(c0, n, 0) ROF(c1, n, 0){
            if(i == n && !c0 && !c1) continue;
            tr[i][c0][c1] |= tr[i - 1][c0][c1];
            if(s[i] == 48) tr[i][c0][c1] |= tr[i][c0][c1 + 1];
            if(s[i] == 49) tr[i][c0][c1] |= tr[i][c0 + 1][c1];
            //删掉两个把第二个加回原来位置
            //加回去的可能是多出来的字符
        }
        if(i > 1) FOR(c0, 0, n) FOR(c1, 0, n){
            if((s[i - 1] == 48 || s[i] == 48) && c0){
                tr[i][c0][c1] |= tr[i - 2][c0 - 1][c1];
                //前面两个都不是多余的，把'0'扔到多余的位置
            }
            if(s[i - 1] == 48 && s[i] == 48 && c0){
                tr[i][c0][c1] |= tr[i - 1][c0 - 1][c1 + 1];
                //前面是"10"（'1'是多余的），并把'0'扔到多余的位置
            }
            if((s[i - 1] == 49 || s[i] == 49) && c1){
                tr[i][c0][c1] |= tr[i - 2][c0][c1 - 1];
                //前面两个都不是多余的，把'1'扔到多余的位置
            }
            if(s[i - 1] == 49 && s[i] == 49 && c1){
                tr[i][c0][c1] |= tr[i - 1][c0 + 1][c1 - 1];
                //前面是"01"（'0'是多余的），并把'1'扔到多余的位置
            }
        }
    }
    dp[n][0][0] = 1;
    ROF(i, n, 1){
        FOR(c0, 0, n) FOR(c1, 0, n){
            Rint t(dp[i][c0][c1]);
            inc(dp[i - 1][c0][c1], t);
            if(s[i] == 48) inc(dp[i][c0][c1 + 1], t);
            if(s[i] == 49) inc(dp[i][c0 + 1][c1], t);
        }
    }
    Rint ans(0);
    FOR(i, 0, n){
        FOR(c0, 0, i) FOR(c1, 0, i){
            if(tr[i][c0][c1]) inc(ans, dp[i][c0][c1]);
        }
    }
    printf("%d\n", ans);
    return 0;
}
```
{% endfold %}