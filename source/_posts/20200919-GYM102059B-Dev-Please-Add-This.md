---
title: 「GYM102059B」Dev, Please Add This!
categories: 题解
urlname: GYM102059B
date: 2020-09-19 23:01:26
tags:
- GYM
- 图论
- 2-SAT
top:
---

由于一些不明原因，俺的同学们都开始搞杂题选蒋，所以我也来胡几个题（

## 题意简述

[GYM 102059B](https://codeforces.com/gym/102059/problem/B)

$n\times m$ 的网格，网格上有一些障碍和星星，以及一个球。
每次移动可以令球上下左右任意方向滚动直到撞到边界或障碍停下。
球在移动过程中会收集经过的星星。求是否能收集所有星星。
$n, m\le 50$。

<!-- more -->

## 主要思路

每个格子向其四个方向能到达的格子连边。缩点。

注意到每个格子向左右走到的格子必定属于一个强连通分量，上下也是。且一个星星能被收集当且仅当路径经过这两个强连通分量的至少一个。

这是一个 2-SAT 问题：

- 每个星星对应的两个强连通分量至少有一个被选择。
- 每一对选择的强连通分量都可达。
- 每个选择的强连通分量都从起点可达。

时间复杂度 $O(n^2m^2)$，但显然不满，感觉开 200 都绰绰有余。

## 参考代码

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

#define N 55
#define M 2505

typedef unsigned long long u64;
struct MyB{
    #define W 40
    u64 a[W];
    inline void reset(){ FOR(i, 0, W - 1) a[i] = 0; }
    inline void set(int x){ a[x >> 6] |= 1ull << (x & 63); }
    MyB(){ reset(); }
    inline MyB operator |(const MyB &rhs)const{
        reg MyB res;
        FOR(i, 0, W - 1) res.a[i] = a[i] | rhs.a[i];
        return res;
    }
    inline MyB operator |=(const MyB &rhs){
        FOR(i, 0, W - 1) a[i] |= rhs.a[i];
        return *this;
    }
    inline bool operator [](const int &x)const{
        return (a[x >> 6] >> (x & 63)) & 1;
    }
    #undef W
};

int n, m;
char str[N][N];
int Lx[N][N], Ux[N][N], id[N][N];

vector<int> E[M];

int bx, by;
inline void Init(){
    scanf("%d%d", &n, &m);
    FOR(i, 1, n) scanf("%s", str[i] + 1);
    FOR(i, 1, n) str[i][0] = str[i][m + 1] = '#';
    FOR(i, 1, m) str[0][i] = str[n + 1][i] = '#';
    // FOR(i, 0, n + 1) FOR(j, 0, m + 1) printf("%c%c", str[i][j], " \n"[j == m + 1]);
    
    FOR(i, 1, n) FOR(j, 1, m){
        id[i][j] = (j == 1 ? id[i - 1][m] : id[i][j - 1]) + 1;
        if(str[i][j] == 'O') bx = i, by = j;
    }
    ROF(i, n, 1) ROF(j, m, 1){
        if(str[i][j + 1] == '#') Lx[i][j] = j;
        else E[id[i][j]].push_back(id[i][Lx[i][j] = Lx[i][j + 1]]);
        if(str[i + 1][j] == '#') Ux[i][j] = i;
        else E[id[i][j]].push_back(id[Ux[i][j] = Ux[i + 1][j]][j]);
    }
    // FOR(i, 1, n) FOR(j, 1, m) printf("%d%c", Lx[i][j], " \n"[j == m]);
    // FOR(i, 1, n) FOR(j, 1, m) printf("%d%c", Ux[i][j], " \n"[j == m]);
    memset(Lx, 0, sizeof(Lx)), memset(Ux, 0, sizeof(Ux));
    FOR(i, 1, n) FOR(j, 1, m){
        if(str[i][j - 1] == '#') Lx[i][j] = j;
        else E[id[i][j]].push_back(id[i][Lx[i][j] = Lx[i][j - 1]]);
        if(str[i - 1][j] == '#') Ux[i][j] = i;
        else E[id[i][j]].push_back(id[Ux[i][j] = Ux[i - 1][j]][j]);
    }
    // FOR(i, 1, n) FOR(j, 1, m) printf("%d%c", Lx[i][j], " \n"[j == m]);
    // FOR(i, 1, n) FOR(j, 1, m) printf("%d%c", Ux[i][j], " \n"[j == m]);
}

namespace TAR{
int dfn[M], low[M], dcnt, scc[M], scnt;
int st[M], *tp(st);
bool ins[M], vis[M];
void tarjan(const int &u){
    dfn[u] = low[u] = ++dcnt;
    *tp++ = u, ins[u] = 1;
    for(Rint v: E[u]){
        if(!dfn[v]) tarjan(v), chkmin(low[u], low[v]);
        else if(ins[v]) chkmin(low[u], dfn[v]);
    }
    if(low[u] == dfn[u]){
        ++scnt;
        while(*--tp != u){
            scc[*tp] = scnt, ins[*tp] = 0;
        }
        scc[*tp] = scnt, ins[*tp] = 0;
    }
}
vector<int> obj[M];
MyB clo[M];
void trans(const int &U){
    // clo[U].set(U);
    #define V scc[v]
    vis[U] = 1;
    for(Rint u: obj[U]) for(Rint v: E[u]) if(U != V){
        if(!vis[V]) trans(V);
        clo[U] |= clo[V];
    }
    #undef V
}
inline int Tr_Cl(){//Transitive closure
    tarjan(id[bx][by]);
    FOR(i, 1, n) FOR(j, 1, m) obj[scc[id[i][j]]].push_back(id[i][j]);
    FOR(i, 1, scnt) clo[i].set(i);
    trans(scc[id[bx][by]]);
    return scnt;
}
}
MyB *con(TAR::clo);

namespace Ni_SAT{
int sc;
MyB clo[M];
inline bool build(){
    FOR(i, 2, sc << 1 | 1) clo[i].set(i);
    FOR(i, 1, sc) FOR(j, i + 1, sc) if(!con[i][j] && !con[j][i]){
        clo[i << 1].set(j << 1 | 1), clo[j << 1].set(i << 1 | 1);
    }
    FOR(i, 1, n) FOR(j, 1, m) if(str[i][j] == '*'){
        Rint L(TAR::scc[id[i][Lx[i][j]]]), U(TAR::scc[id[Ux[i][j]][j]]);
        if(!L && !U) return 0;
        if(L == U) clo[L << 1 | 1].set(L << 1);
        else if(!U) clo[L << 1 | 1].set(L << 1);
        else if(!L) clo[U << 1 | 1].set(U << 1);
        else clo[L << 1 | 1].set(U << 1), clo[U << 1 | 1].set(L << 1);
    }
    return 1;
}
inline void Tr_Cl(){
    FOR(i, 2, sc << 1 | 1) FOR(j, 2, sc << 1 | 1){
        if(clo[j][i]) clo[j] |= clo[i];
    }
}
}
MyB *B(Ni_SAT::clo);

int main() {
    Init();
    Ni_SAT::sc = TAR::Tr_Cl();
    // printf("sc: %d\n", Ni_SAT::sc);
    // FOR(i, 1, n) FOR(j, 1, m) printf("%d%c", TAR::scc[id[i][j]], " \n"[j == m]);
    if(!Ni_SAT::build()) return puts("NO"), 0;
    Ni_SAT::Tr_Cl();
    FOR(i, 1, Ni_SAT::sc){
        if(B[i << 1][i << 1 | 1] && B[i << 1 | 1][i << 1]){
            return puts("NO"), 0;
        }
    }
    return puts("YES"), 0;
}
```
{% endfold %}