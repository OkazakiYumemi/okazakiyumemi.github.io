---
title: 「CF356E」Xenia and String Problem
categories: 题解
urlname: CF356E
date: 2020-08-21 22:31:00
tags:
- Codeforces
- 字符串
- 后缀数组
top:
---

大型分类讨论。

## 题意简述

称字符串 $s$ 为`Gray`的串，仅当满足以下所有条件：
- $|s|$ 为奇数。
- 串正中间的字符仅在 $s$ 中出现一次。
- 要么 $|s| = 1$，要么删去中间的字符，左右是相等的`Gray`。

给一个字符串 $|S|$，一个字串 $s$ 若为`Gray`，贡献为 $|s|^2$，否则贡献为 $0$。求更改最多 $1$ 个字符后整个串的最大总贡献。

$|S|\le 10^5$。

[[CF 356E]](https://codeforces.com/problemset/problem/356/E)

<!-- more -->

## 主要思路

设 $n = |S|$。不难发现`Gray`的长度均为 $2^k - 1$，故总共只有 $O(n\log n)$ 个可能成为`Gray`的字串。先考虑不修改求总贡献。

考虑 $[s_l, e_r]$ 是否为`Gray`，假设 $[s_l, e_r] = [s_r, e_r]$ 且均为`Gray`，则只需判断 $s_{s_r - 1}$ 是否字串 $[s_l, e_r]$ 中只出现一次即可。

求两个字串是否相等，后缀数组求 lcp 即可。

考虑将 $s_i$ 改为 $v$ 会发生什么：减掉先前所有包含 $i$ 的`Gray`的贡献，再加上所有更改为 $v$ 后新的贡献。

包含 $i$ 的贡献 $c(i)$ 好求，维护差分，每次判断到一个`Gray`更新即可。

而 $s_i$ 改为 $v$ 后的新贡献 $f(i, v)$ 不好直接求，考虑枚举每个可能为`Gray`的字串是否更改 $1$ 个字符可变为`Gray`。

- 对于一个`Gray`，仅可能更改中间的位置。
- 对于左半边右半边均非`Gray`的串，仅更改一个位置不可能。
- 对于左右均为`Gray`且相等，可能可更改中间位置。
- 对于左右至少有一个为`Gray`且左右字串仅相差 1 个字符的，可能可以更改该位置，注意同时保证中间位置的字符仅出现一次。

就结束了。使用后缀数组 $O(1)$ lcp，复杂度 $O(\Sigma n\log n)$。

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
typedef long long LL;
#define Templ(T) template <typename T>
Templ(_Tp) inline int chkmin(_Tp &x, _Tp y) { return x > y ? x = y, 1 : 0; }
Templ(_Tp) inline int chkmax(_Tp &x, _Tp y) { return x < y ? x = y, 1 : 0; }
} // namespace my_std
using namespace my_std;

const int N = 100010;
int n;
char str[N];
int l2g[N];
int g[N][26], s[N][26];
LL f[N][26], c[N], ans;
int a[N];

namespace SA{
int sa[N], rk[N], oldrk[N << 1], cnt[N], px[N], id[N], ht[N];
int st[20][N];
inline int cmp(const int &x, const int &y, const int &w){
    return oldrk[x] == oldrk[y] && oldrk[x + w] == oldrk[y + w];
}
inline void build(){
    Rint m = 127, p = 0, w = 0;
    FOR(i, 1, n) ++cnt[rk[i] = str[i]];
    FOR(i, 1, m) cnt[i] += cnt[i - 1];
    ROF(i, n, 1) sa[cnt[rk[i]]--] = i;
    
    for(w = 1; p < n; w <<= 1, m = p){
        p = 0;
        ROF(i, n, n - w + 1) id[++p] = i;
        FOR(i, 1, n) if(sa[i] > w) id[++p] = sa[i] - w;
        
        memset(cnt, 0, sizeof(cnt));
        FOR(i, 1, n) ++cnt[px[i] = rk[id[i]]];
        FOR(i, 1, m) cnt[i] += cnt[i - 1];
        ROF(i, n, 1) sa[cnt[px[i]]--] = id[i];
        
        p = 0;
        memcpy(oldrk, rk, sizeof(rk));
        FOR(i, 1, n){
            rk[sa[i]] = cmp(sa[i], sa[i - 1], w) ? p : ++p;
        }
    }
    //build sa[], rk[]
    p = 0;
    FOR(i, 1, n){
        if(p) --p;
        while(str[i + p] == str[sa[rk[i] - 1] + p]) ++p;
        ht[rk[i]] = p;
    }
    //build ht[]
    //ht[i] = lcp(sa[i], sa[i - 1])
    FOR(i, 1, n) st[0][i] = ht[i];
    FOR(i, 2, n + 1) l2g[i] = l2g[i >> 1] + 1;
    FOR(i, 1, l2g[n]){
        FOR(j, 1, n - (1 << i) + 1){
            st[i][j] = min(st[i - 1][j],
                st[i - 1][j + (1 << (i - 1))]);
        }
    }
	// FOR(i, 1, n){
	// 	FOR(j, sa[i], n) putchar(str[j]);
	// 	putchar('\n');
	// }
}
inline int st_q(Rint x, Rint y){
    Rint k = l2g[y - x + 1];
    y -= (1 << k) - 1;
    return min(st[k][x], st[k][y]);
}
inline int lcp(Rint x, Rint y){
    if(x == y) return n - x + 1;
    x = rk[x], y = rk[y];
    if(x > y) swap(x, y);
    // if(!x || !y) return 0;
    return st_q(x + 1, y);
}
}
#define lsp(x, y) SA::lcp(x, y)
#define len ((1 << (t - 1)) - 1)
#define ls (i)
#define rs (i + (1 << (t - 1)))
#define le (i + (1 << (t - 1)) - 2)
#define re (i + (1 << t) - 2)
#define LEN ((1 << t) - 1)
inline int S(int l, int r, int o){
    return s[r][o] - s[l - 1][o];
}
inline LL sqr(int x){
    return (LL)x * x;
}

int main() {
    scanf("%s", str + 1);
    n = strlen(str + 1);
    FOR(i, 1, n) g[i][1] = 1, a[i] = str[i] - 'a';
    SA::build();
    FOR(i, 1, n) FOR(o, 0, 25){
        s[i][o] = s[i - 1][o] + (a[i] == o);
    }
    const int lim = l2g[n + 1];
    c[1] = 1;
    ans = n;
    FOR(t, 2, lim) FOR(i, 1, n - (1 << t) + 2){
        if(g[ls][t - 1] && g[rs][t - 1] && lsp(ls, rs) >= len && S(ls, re, a[rs - 1]) == 1){
            g[ls][t] = 1;
            c[ls] += sqr(LEN), c[re + 1] -= sqr(LEN);
            ans += sqr(LEN);
            FOR(o, 0, 25) if(a[rs - 1] != o && S(ls, re, o) == 0) f[rs - 1][o] += sqr(LEN);
        }
    }//get g[][], c[], ans
    // FOR(t, 1, lim) FOR(i, 1, n - (1 << t) + 2){
    //     if(g[i][t]) printf(":%d %d\n", i, t);
    // }
    FOR(t, 2, lim) FOR(i, 1, n - (1 << t) + 2) if(!g[i][t]){
        if(g[ls][t - 1] && g[rs][t - 1]){
            if(lsp(ls, rs) >= len){
                FOR(o, 0, 25) if(a[rs - 1] != o && S(ls, re, o) == 0) f[rs - 1][o] += sqr(LEN);
                // printf("1: %d %d\n", i, t);
            }else{
                int h = lsp(ls, rs);
                if((h == len - 1 || lsp(ls + h + 1, rs + h + 1) + h + 1 >= len) &&
                    S(ls, re, a[rs - 1]) == int(a[rs + h] == a[rs - 1] || a[ls + h] == a[rs - 1]) + 1){
                    // printf("3: %d %d %d %d\n", i, t, ls + h, rs + h);
                    f[ls + h][a[rs + h]] += sqr(LEN), f[rs + h][a[ls + h]] += sqr(LEN);
                    if(a[ls + h] == a[rs - 1]) f[rs + h][a[ls + h]] -= sqr(LEN);
                    if(a[rs + h] == a[rs - 1]) f[ls + h][a[rs + h]] -= sqr(LEN);
                }
            }
        }else if(g[ls][t - 1] + g[rs][t - 1] == 1){
            int h = lsp(ls, rs);
            if((h == len - 1 || (lsp(ls + h + 1, rs + h + 1) + h + 1 >= len)) &&
                S(ls, re, a[rs - 1]) == (((g[ls][t - 1] ? a[rs + h] : a[ls + h]) == a[rs - 1]) ? 2 : 1)){
                // printf("2: %d %d %d\n", i, t, (g[ls][t - 1] ? rs : ls) + h);
                (g[ls][t - 1] ? f[rs + h][a[ls + h]] : f[ls + h][a[rs + h]]) += sqr(LEN);
            }
        }
    }//get f[][]
    FOR(i, 1, n) c[i] += c[i - 1];
    reg LL res = ans;
    // printf("%lld\n", ans);
    FOR(i, 1, n) FOR(o, 0, 25) if(a[i] != o){
        chkmax(res, ans - c[i] + 1 + f[i][o]);
    }
    printf("%lld\n", res);
    return 0;
}
```
{% endfold %}