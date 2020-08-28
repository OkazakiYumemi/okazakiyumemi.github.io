---
title: 「AGC044E」Random Pawn
categories: 题解
urlname: AGC044E
date: 2020-08-28 15:04:58
tags:
- AtCoder
- dp
top:
---

## 题意简述

[AGC 044E](https://atcoder.jp/contests/agc044/tasks/agc044_e)

给出一个长为 $n$ 的环，每个点有 $a_i, b_i$。随机一个点 $i$ 开始，每次你可以选择停止，则获得当前位置的价值 $a_i$；或者耗费 $b_i$ 的价值，等概率随机移动到左右两点中的一个。

求期望获得的最大价值。$2\le n\le 200000, 0\le a_i\le 10^{12}, 0\le b_i\le 100$。

<!-- more -->

## 主要思路

由于花费非负，若移动到了价值最大的点上显然不会再移动。故可以断环为以最大价值的点开头和结尾的长 $n+1$ 的序列。

显然，若设 $g_i$ 为从点 $i$ 开始的期望最大价值，则 $g_i = \max(a_i, \dfrac{g_{i - 1} + g_{i + 1}}{2} - b_i)$。

但是这式子里有个 $b_i$ 不好搞，给每个点设一个修正值 $c_i$，然后两边同时减去这个值。

$$
g_i - c_i = \max(a_i - c_i, \dfrac{g_{i - 1} - c_{i - 1} + g_{i + 1} - c_{i + 1}}{2} - b_i - c_i + \dfrac{c_{i - 1} + c_{i + 1}}{2})
$$

为了消去 $b_i$，应有 $b_i + c_i = \dfrac{c_{i - 1} + c_{i + 1}}{2}$。

钦定 $c_1 = 0$，即可求出所有 $c_i$。
设 $f_i = g_i - c_i, v_i = a_i - c_i$，此时的转移式即为 $f_i = \max(v_i, \dfrac{f_{i-1}+f_{i+1}}{2})$。

这是个经典问题。类似于价值最大的点，应当有一些「终止点」，移动到则直接停止最优。

假设相邻的两个终止点 $l, r$，考虑求 $\sum\limits_{i=l}^{r} f_i$。
显然 $f_l = v_l, f_r = v_r$。
而对于 $i\in(l, r)$，应有 $f_i = \dfrac{f_{i-1}+f_{i+1}}{2}$，即 $f_i, i\in[l, r]$ 为等差数列。

于是贡献就是相邻终止点向 $x$ 轴作垂线形成梯形的面积。不妨求出答案的两倍。
即贡献为 $(v_l + v_r) \times (r - l)$。

显然当选取终止点形成上凸壳时，总贡献取最大值。
于是直接 $O(n)$ 做就完事了。

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
Templ(T) inline T read() {
    reg T ans = 0;
    reg char c = getchar();
    while (!isdigit(c)) c = getchar();
    for (; isdigit(c); c = getchar()) ans = (ans << 1) + (ans << 3) + (c ^ 48);
    return ans;
}
Templ(_Tp) inline int chkmin(_Tp &x, _Tp y) { return x > y ? x = y, 1 : 0; }
Templ(_Tp) inline int chkmax(_Tp &x, _Tp y) { return x < y ? x = y, 1 : 0; }
} // namespace my_std
using namespace my_std;

#define N 200010

int n;
i64 a[N], c[N];
int b[N];

struct Point{
    i64 x, y;
    inline Point(i64 _x = 0, i64 _y = 0){ x = _x, y = _y; }
    inline Point operator -(const Point &t)const{
        return Point(x - t.x, y - t.y);
    }
}d[N];
inline i64 cross(const Point &l, const Point &r){
    return l.x * r.y - r.x * l.y;
}

void init(){
    i64 *aa = new i64[N], mx(-1);
    int *bb = new int[N], pos(0);
    n = read<int>();
    FOR(i, 1, n){
        if(chkmax(mx, aa[i] = read<i64>()))
            pos = i;
    }
    FOR(i, 1, n) bb[i] = read<int>();
    memcpy(a + 1, aa + pos, sizeof(i64) * (n - pos + 1));
    memcpy(a + n - pos + 2, aa + 1, sizeof(i64) * pos);
    memcpy(b + 1, bb + pos, sizeof(int) * (n - pos + 1));
    memcpy(b + n - pos + 2, bb + 1, sizeof(int) * pos);
    delete aa;
    delete bb;
}

int main() {
    init();
    c[1] = 0, ++n;
    FOR(i, 2, n) c[i] = 2 * (c[i - 1] + b[i - 1]) - c[i - 2];
    reg Point t;
    Rint h(0);
    FOR(i, 1, n){
        t = Point(i, a[i] - c[i]);
        while(h > 1 && cross(d[h] - d[h - 1], t - d[h]) > 0) --h;
        d[++h] = t;
    }
    reg i64 ans(0);
    FOR(i, 2, h){
        ans += (d[i].x - d[i - 1].x) * (d[i].y + d[i - 1].y);
    }
    ans += d[1].y + d[h].y;
    ans -= 2 * a[1];
    FOR(i, 1, n) ans += 2 * c[i];
    printf("%.12lf", 0.5 * ans / (n - 1.0));
    return 0;
}
```
{% endfold %}