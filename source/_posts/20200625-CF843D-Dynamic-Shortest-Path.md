---
title: 「CF843D」Dynamic Shortest Path
categories: 题解
date: 2020-06-25 23:27:21
tags:
- Codeforces
- 图论
top:
urlname: CF843D
---

## 题意简述

[[CF 843D]](https://codeforces.com/problemset/problem/843/D)

给定 $n$ 个点 $m$ 条边的有向图，边带权。$q$ 个操作，可能是求目前 $1$ 到 $u$ 的最短路或是将 $c$ 条边的权加 $1$。

$n, m\le 10^5, q\le 2000, \sum c\le 10^6$。

<!-- more -->

## 主要思路

$q$ 这么小，看起来像是 $O(nq)$。

先跑一遍 Dijkstra 来取得 $\langle dis_i\rangle$，表示 $1$ 到 $i$ 的最短路长度。

每次修改边权， $1$ 到任意其他点的距离比原先都不降。

那么求增量 $\langle f_i\rangle$。设 $e(u, v)$ 表示 $u, v$ 之间连边，$w(u, v)$ 表示 $u, v$ 之间边的边权。

显然对于点 $v$ 有 $f_v = \min\limits_{e(u, v)}(dis_u + f_u + w(u, v) - dis_v)$。

增量有上限 $c$，所以可以直接用桶 $O(n + m)$ bfs 做。

总时间复杂度 $O(n\log n + m + (n + m)q)$。

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
#define FORit(templ, arr, i, a, b) \
for (register templ *i = (arr) + (a), *ed_##i = (arr) + (b) + 1; i != ed_##i; ++i)
#define ROFit(templ, arr, i, a, b) \
for (register templ *i = (arr) + (a), *ed_##i = (arr) + (b)-1; i != ed_##i; --i)
#define GO(x, p, e, i, v) for (register int i = p[x].head, v; i; i = e[i].link)
#define MEM(x, v) memset(x, v, sizeof(x))
#define fir first
#define sec second
#define MP make_pair
typedef long long LL;
typedef double DB;
typedef pair<int, int> PII;
typedef pair<LL, int> PLLI;
#define Templ(T) template <typename T>
inline int read() {
    reg int ans = 0;
    reg char c = getchar();
    while (!isdigit(c)) c = getchar();
    for (; isdigit(c); c = getchar()) ans = (ans << 1) + (ans << 3) + (c ^ 48);
    return ans;
}
Templ(_Tp) inline int chkmin(_Tp &x, _Tp y) { return x > y ? x = y, 1 : 0; }
Templ(_Tp) inline int chkmax(_Tp &x, _Tp y) { return x < y ? x = y, 1 : 0; }
#define using_mod
const int mod = 998244353;
#ifdef using_mod
inline void inc(int &x, const int &y) { x += y; if (x >= mod) x -= mod; }
inline void dec(int &x, const int &y) { x -= y; if (x < 0) x += mod; }
inline int ksm(int x, LL y) {
    int res = 1;
    for (; y; y >>= 1, x = 1ll * x * x % mod)
        if (y & 1) res = 1ll * res * x % mod;
    return res;
}
#endif
Templ(_Tp) inline _Tp gcd(_Tp x, _Tp y) { return y ? gcd(y, x % y) : x; }
#define FILE(s) freopen(s ".in", "r", stdin), freopen(s ".out", "w", stdout)
#define PBTXDY
} // namespace my_std
using namespace my_std;

const int N = 100010;
#define inf (0x3f3f3f3f3f3f3f3fll)
struct Vertice{
    int head;
}p[N];
struct Edge{
    int link, to, wgh;
}e[N];
LL dis[N], f[N];
int vis[N];
static int ecnt;
inline void A_E(const int &u, const int &v, const int &w){
    e[++ecnt] = (Edge){p[u].head, v, w};
    p[u].head = ecnt;
}
int n, m, q;

priority_queue<PLLI> pq;
inline void dijkstra(){
    memset(dis + 1, 0x3f, sizeof(LL) * n);
    memset(vis + 1, 0, sizeof(int) * n);
    dis[1] = 0;
    pq.push(MP(0, 1));
    Rint u;
    while(!pq.empty()){
        u = pq.top().sec, pq.pop();
        if(vis[u]) continue;
        vis[u] = 1;
        GO(u, p, e, i, v){
            v = e[i].to;
            if(chkmin(dis[v], dis[u] + e[i].wgh)){
                pq.push(MP(-dis[v], v));
            }
        }
    }
}

queue<int> bar[N];
inline void bfs(const int &cnt){
    memset(f + 1, 0x3f, sizeof(LL) * n);
    f[1] = 0, bar[0].push(1);
    reg LL lim = 0, val = 0;
    while(val <= lim){
        Rint u;
        while(!bar[val].empty()){
            u = bar[val].front(), bar[val].pop();
            if(f[u] < val) continue;
            GO(u, p, e, i, v){
                v = e[i].to;
                if(chkmin(f[v], f[u] + dis[u] - dis[v] + e[i].wgh)){
                    if(f[v] <= cnt){
                        bar[f[v]].push(v);
                        chkmax(lim, f[v]);
                    }
                }
            }
        }
        ++val;
    }
    FOR(i, 2, n) dis[i] = min(dis[i] + f[i], inf);
}

int main(){
    n = read(), m = read(), q = read();
    Rint u, v, w;
    FOR(i, 1, m){
        u = read(), v = read(), w = read();
        A_E(u, v, w);
    }
    dijkstra();
    while(q--){
        u = read();
        if(u == 1){
            v = read();
            printf("%lld\n", dis[v] >= inf ? -1 : dis[v]);
        }
        else{
            v = read();
            FOR(i, 1, v) ++e[read()].wgh;
            bfs(min(v, n - 1));
        }
    }
    return 0;
}
```
{% endfold %}