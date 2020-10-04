---
title: 「CF1396E」Distance Matching
categories: 题解
urlname: CF1396E
date: 2020-10-04 10:16:19
tags:
- Codeforces
- 树论
top:
---

## 题意简述

[CF1396E](https://codeforces.com/contest/1396/problem/E)

一棵树大小为 $n$（偶数）。将点两两配对，一对的权值为两点在树上的距离。求方案时所有对的权值和为 $K$。
$n\le 10^5$，需判断无解。

<!-- more -->

## 主要思路

为缩短公式长度，定义 $M_2(x) = x \bmod 2$，$u, v$ 间匹配为 $m(u, v)$，某个方案得到的答案为 $S$。

设一条边 $e$ 将树分成两部分，其中节点数较少的为 $w_e$。
不难发现，至少有 $M_2(w_e)$，至多有 $w_e$ 个匹配经过 $e$。
所以对于一棵树 $T$，答案的下上界就分别是 $L(T) = \sum\limits_e M_2(w_e), R(T) = \sum\limits_e w_e$。
又注意到 $S=\sum\limits_{m(u, v)}(\operatorname{dep}(u) + \operatorname{dep}(v) - 2\operatorname{dep}(\operatorname{lca}(u ,v)))\equiv \sum\limits_{u\in[1, n]}\operatorname{dep}(u)\equiv R(T)\pmod{2}$，故 $K$ 与 $R(T)$ 奇偶性相同也是一个有解的必要条件。
（钦定根节点深度为 $0$。）

然后我们来证明这些就是有解的充分条件。
$n = 2$显然成立。考虑归纳，下证若在 $n = 2n' - 2$ 时命题成立，考虑 $n = 2n'$。
首先我们取出一个重心 $G$，则此时 $\sum\limits_e w_e = \sum\limits_{i\neq G}\operatorname{siz}(i)$。

如果 $K = R(T)$，直接求出 dfs 序 $r_i$，将 $r_i$ 与 $r_{i+n/2}$ 匹配即可。（这里运用了一下[移位技巧](https://www.cnblogs.com/TinyWong/p/13374739.html)。）

否则，取 $G$ 的儿子 $g$ 使 $\operatorname{siz}(g)$ 最大。此时显然有 $\operatorname{siz}(g) \ge 2$（否则为上一种情况）。
取节点 $w$ 为子树 $g$ 中深度最大且 $\operatorname{siz}(w)\ge 2$ 的节点。

此时，如果 $K + 2\operatorname{dep}(w) > R(T)$，必然存在 $g$ 子树中的一个非叶节点 $u$ 使 $K + 2\operatorname{dep}(u) = R(T)$，匹配 $u$ 与 $u$ 的任意一个儿子，对其他点按照上面情况的移位技巧即可。

否则即 $K + 2\operatorname{dep}(w) \le R(T)$。
1. 如果 $w$ 有两个或以上儿子 $u, v$，匹配并在树中删除它们，得到新树 $T'$。
   显然它们都是叶子节点，故 $K' = K - 2, L(T') = L(T) - 2, R(T') = R(T) - 2(\operatorname{dep}(w) + 1)$。
   并且，此时 $T'$ 重心仍为 $G$，节点数减少 $2$，且 $K'\in[L(T'), R(T')]$，故根据归纳假设有解。
2. 如果 $w$ 只有一个儿子 $u$，匹配 $w, u$ 并删除它们，得到新树 $T'$。
   同理易得$K' = K - 1, L(T') = L(T) - 1, R(T') = R(T) - 2\operatorname{dep}(w) - 1$。
   重心不变，节点数减少 $2$，根据归纳假设有解。

由此，我们证明了上述条件为有解的充要条件。

考虑如何构造，发现直接以重心为根，根据上述证明充分性的过程模拟即可。

由于操作的特殊性，复杂度可以做到 $O(n)$，但本人懒得动脑就直接写了个 $O(n\log n)$ 的。

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
typedef vector<int> vec;
typedef pair<int, int> pii;
#define MP(a, b) make_pair(a, b)
#define fr first
#define sc second
#define Templ(T) template <typename T>
Templ(_Tp) inline int chkmin(_Tp &x, _Tp y) { return x > y ? x = y, 1 : 0; }
Templ(_Tp) inline int chkmax(_Tp &x, _Tp y) { return x < y ? x = y, 1 : 0; }
} // namespace my_std
using namespace my_std;

#define MMR (1 << 21)
struct INPUT{
    char buf[MMR], *s;
    INPUT(){ s = buf, fread(buf, 1, MMR, stdin); }
    Templ(_Tp) inline INPUT& operator >>(_Tp &x){
        x = 0;
        while(*s < 48) ++s;
        while(*s > 47) x = x * 10 + *s++ - 48;
        return *this;
    }
}fin;

#define N 100010
int n, n2;
i64 K;
vec E[N];
int siz[N], mxs[N];
int root;
int dep[N], fth[N], vis[N];
int oud[N];
i64 L, R;
set<pii> s[N], Q;

void find_root(int u, int f){
    siz[u] = 1, mxs[u] = 0;
    for(Rint v: E[u]) if(v != f){
        find_root(v, u);
        siz[u] += siz[v], chkmax(mxs[u], siz[v]);
    }
    chkmax(mxs[u], n - siz[u]);
    mxs[u] <= n2 && (root = u);
}

void init(int u, int f, int root){
    siz[u] = 1;
    for(Rint v: E[u]) if(v != f){
        ++oud[u];
        dep[v] = dep[u] + 1;
        fth[v] = u;
        init(v, u, root);
        siz[u] += siz[v];
    }
    if(siz[u] > 1) s[root].insert(MP(dep[u], u));
    L += siz[u] & 1, R += siz[u];
}

inline int get_son(int u){
    Rint v;
    while(E[u].size()){
        v = E[u].back(), E[u].pop_back();
        if(v != fth[u] && !vis[v]) return v;
    }
    return 0;
}
inline void del(int u, int rt){
    Rint f = fth[u];
    if(!--oud[f]) s[rt].erase(MP(dep[f], f));
}

vec ans;
void dfs(int u, int f){
    if(!vis[u]) ans.push_back(u);
    for(Rint v: E[u]) if(v != f) dfs(v, u);
}

int main() {
    fin >> n >> K;
    n2 = n >> 1;
    Rint u, v;
    FOR(i, 1, n - 1){
        fin >> u >> v;
        E[u].push_back(v), E[v].push_back(u);
    }
    find_root(1, 0);
    for(Rint u: E[root]){
        fth[u] = root;
        dep[u] = 1;
        init(u, root, u);
        if(siz[u] > 1) Q.insert(MP(siz[u], u));
    }
    if(K < L || K > R || (K & 1) != (R & 1)) return puts("NO"), 0;
    puts("YES");
    K = (R - K) >> 1;

    while(K){
        Rint rt = Q.rbegin()->sc;
        Q.erase(--Q.end());
        Rint u = s[rt].rbegin()->sc, d = s[rt].rbegin()->fr;
        if(K >= d){
            K -= d;
            Rint v = get_son(u);
            Rint w = get_son(u);
            if(!w) w = u;
            vis[v] = vis[w] = 1;
            del(v, rt), del(w, rt);
            printf("%d %d\n", v, w);
        }else{
            u = s[rt].lower_bound(MP(K, 0))->sc;
            Rint v = get_son(u);
            E[u].push_back(v);
            vis[u] = vis[v] = 1;
            printf("%d %d\n", u, v);
            break;
        }
        siz[rt] -= 2;
        if(siz[rt] > 1) Q.insert(MP(siz[rt], rt));
    }
    dfs(root, 0);
    Rint res = (int)ans.size() >> 1;
    FOR(i, 0, res - 1) printf("%d %d\n", ans[i], ans[i + res]);
    return 0;
}
```
{% endfold %}

## 参考资料

[liouzhou_101's blog | CodeForces 1396E. Distance Matching](https://www.cnblogs.com/TinyWong/p/13605055.html)