---
title: 「CodeChef WALKBT」Walks on the binary tree
categories: 题解
urlname: CodeChef-WALKBT
date: 2020-11-05 19:44:16
tags:
- CodeChef
- 线段树
- log数据结构
top:
---

## 题意简述

[CodeChef WALKBT](https://www.codechef.com/problems/WALKBT)

一棵高度为 $n$ 的满二叉树（节点数为 $2^{n+1}-1$），用一个 $[0,2^n)$ 的数 $X$ 表示一条根到叶子的路径（从最高位开始，为`0`则走左儿子，否则走右儿子）。

初始 $X = 0$，有两种操作共 $q$ 个：

- 将 $X$ 改为 $(X + 2^C) \bmod 2^n$，然后从根出发走到叶子。
- 询问当前有多少个节点至少被访问一次。

$n,q\le 10^5$。

<!-- more -->

## 主要思路

考虑一次新增的节点数，不难发现是 $n$ 减去新的 $X$ 与之前所有数的 lcp 的最大值。
不难发现这个最大值必然在新 $X$ 的前驱或后继中取得。

考虑加上 $2^C$ 的操作，实质上是把前面的一段`1`赋为`0`，把一个`0`赋为`1`。
于是每个修改操作只会改变均摊 $O(1)$ 个位置。
于是可以上棵可持久化线段树维护每个位的值。

那么如何比较大小？
可以在可持久化线段树上二分第一个两数不同的位置，然后 $O(\log n)$ 比较。
那么我们希望能够 $O(1)$ 比较两棵线段树上的一个区间是否完全相等。
这个哈希一下就好了。

于是我们获得了一个 $O(\log n)$ 比较大小的方法，再套个`set`维护前驱后继就 $O(n\log^2n)$ 走了。

~~然后这题被出到模拟赛里有人被卡常了，所以是 CC 太快还是 CC 数据水啊（~~

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
typedef const int CI;
#define fr first
#define sc second
#define MP make_pair
#define Templ(T) template <typename T>
inline int read() {
    Rint ans = 0;
    reg char c = getchar();
    while (!isdigit(c)) c = getchar();
    for (; isdigit(c); c = getchar())
        ans = ans * 10 + c - '0';
    return ans;
}
Templ(_Tp) inline int chkmin(_Tp &x, _Tp y) { return x > y ? x = y, 1 : 0; }
Templ(_Tp) inline int chkmax(_Tp &x, _Tp y) { return x < y ? x = y, 1 : 0; }
#define m1 88888901
#define m2 77777803
} // namespace my_std
using namespace my_std;

#define N 200010
int pw1[N], pw2[N];
const bool init = [&](){
    *pw1 = *pw2 = 1;
    FOR(i, 1, N - 1){
        pw1[i] = (pw1[i - 1] << 1) % m1;
        pw2[i] = (pw2[i - 1] << 1) % m2;
    }
    return true;
}();

int n, q;

int rt[N], lc[N << 5], rc[N << 5], s1[N << 5], s2[N << 5];
int tt, cc;

#define lt lc[t]
#define rt rc[t]
#define ls lc[s]
#define rs rc[s]
#define f1 s1[t]
#define f2 s2[t]
#define g1 s1[s]
#define g2 s2[s]
void upd(int &t, CI &s, CI &l, CI &r, CI &p, CI &v){
    t = ++cc;
    lt = ls, rt = rs;
    f1 = g1, f2 = g2;
    if(l == r) return f1 = f2 = v, void();
    CI mid = (l + r) >> 1;
    if(p <= mid) upd(lt, ls, l, mid, p, v);
    else upd(rt, rs, mid + 1, r, p, v);
    f1 = (s1[lt] + (i64)pw1[mid - l + 1] * s1[rt]) % m1;
    f2 = (s2[lt] + (i64)pw2[mid - l + 1] * s2[rt]) % m2;
}
int is_1(CI &t, CI &l, CI &r, CI &p){
    if(!t) return 0;
    if(l == r) return s1[t];
    CI mid = (l + r) >> 1;
    if(p <= mid) return is_1(lt, l, mid, p);
    else return is_1(rt, mid + 1, r, p);
}
pair<bool, int> cpr(CI &t, CI &s, CI &l, CI &r){
    if(l == r) return MP(f1 <= g1, f1 == g1 ? -1 : l);
    CI mid = (l + r) >> 1;
    if(s1[rt] == s1[rs] && s2[rt] == s2[rs])
        return cpr(lt, ls, l, mid);
    else return cpr(rt, rs, mid + 1, r);
}
#undef lt
#undef rt
#undef ls
#undef rs
#undef f1
#undef f2
#undef g1
#undef g2

#define upd(p, v) \
    ++tt, upd(rt[tt], rt[tt - 1], 0, n - 1, p, v)
#define cpr(t, s) \
    cpr(t, s, 0, n - 1)

struct cmp{
    inline bool operator()(CI &x, CI &y)const{
        return cpr(rt[x], rt[y]).fr;
    }
};

typedef set<int, cmp> SCI;
SCI st;
i64 ans;

inline void work(){
    ans = 0, tt = 0, cc = 0;
    st.clear();
    n = read(), q = read();
    FOR(o, 1, q){
        char c = getchar();
        while(c != '!' && c != '?') c = getchar();
        if(c == '?'){
            printf("%lld\n", ans ? ans : 1);
            continue;
        }
        int x = read();
        while(x < n && is_1(rt[tt], 0, n - 1, x)) upd(x++, 0);
        if(x < n) upd(x, 1);
        SCI::iterator i = st.insert(tt).fr, nxt = next(i), pre = prev(i);
        int res = n;
        if(i != st.begin())
            chkmin(res, cpr(rt[*i], rt[*pre]).sc);
        if(nxt != st.end())
            chkmin(res, cpr(rt[*i], rt[*nxt]).sc);
        ans += res + 1;
        // printf("%lld\n", ans);
    }
}

int main() {
    int esac = read();
    while(esac--) work();
    return 0;
}
```
{% endfold %}