---
title: 「CF1442F」Differentiating Games
categories: 题解
urlname: CF1442F
date: 2021-01-21 09:38:27
tags:
- Codeforces
- 交互题
- 构造
top:
---

## 题意简述

[CF 1442F](https://codeforces.com/contest/1442/problem/F)

给一个 $n$ 个点 $m$ 条边的 DAG，你可以进行至多 $4242$ 次修改操作，每次删掉一条边或加上一条边（修改之后允许有环、重边、自环）。

在这个图上定义函数 $f(S)$ ，表示如果在多重集合 $S$ 的每一个元素上放一个球，两人轮流操作，每次把一个球向某一条出边移动，不能操作者输，那么是先手是胜还是输，或者永远玩不完（平）。
（双方的策略均为先尽量赢，再尽量平。）

接下来 $T$ 组数据，每组数据中交互器确定一个特殊点 $x$ ，你要通过询问来找到这个特殊点。

一次询问可以问大小不超过 $20$ 的多重集合 $S$ ，返回 $f(S+\{x\})$ 的值。最多问 $20$ 次，集合大小的总和不能超过 $20$ 。

$n\le 1000,m\le 10^5,T\le 2000$

<!-- more -->

## 主要思路

假设图无环，则没有平局情况，故两点的`SG`值必须两两不同，图这必须是一个完全 DAG。

那么，一个显然的想法是给除了那个完全 DAG 里的点都加上一个自环。
称 DAG 里的点是 A 类点，有自环的点是 B 类点。

不难发现，如果至少 2 个球在 B 类点上，一定是`Draw`；有 1 个球在 B 类点上，不可能`Lose`，因为其可以什么都不做。
于是现在考虑仅有 1 个球在 B 类点上的情况，考虑先手什么时候会将球移出去。

显然，先手不会将球移到另一个 B 类点上，因为此时不可能`Win`。故先手仅可能将球移到一个 A 类点上。

不妨假设我们每次仅询问大小为 1 的集合，即，将 A 类点都询问一遍。
A 类点的`SG`值都不相同，所以如果出现了`Lose`，仅可能是询问点和答案相同。

否则，只会有一堆`Win`和`Draw`。类似地，`Win`的情况为答案有连向询问点的出边。
所以只需保证每个 B 类点对 A 类边连边的集合不同即可。

于是钦定 A 类点不超过 $20$ 个，直接冲即可。

最后大概会修改 $3900$ 条边。

## 参考代码

{% fold %}
```cpp
#include <bits/stdc++.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a), ed_##i = (b); i <= ed_##i; ++i)
#define ROF(i, a, b) for (int i = (a), ed_##i = (b); i >= ed_##i; --i)
typedef long long i64;
typedef vector<int> VI;
#define pb emplace_back

#define N 1024
#define M 25
#define BB 20
vector<tuple<char, int, int>> ans;

int n, m, esac, B;
VI E[N];
int ind[N], que[N];
int id[N], rd[M];
int tb[1048599], val[N];

inline void AddEdge(int u, int v){ ans.pb('+', u, v); }
inline void DelEdge(int u, int v){ ans.pb('-', u, v); }
inline int Query(int u){
    printf("? 1 %d\n", u), fflush(stdout);
    static char s[10];
    scanf("%s", s);
    return *s == 'L' ? -1 : (*s == 'W' ? 1 : 0);
}
inline void Report(int u){
    printf("! %d\n", u), fflush(stdout);
    static char s[10];
    scanf("%s", s);
    assert(*s == 'C');
}

inline void Init1(){
    scanf("%d%d%d", &n, &m, &esac);
    FOR(i, 1, m){
        int u, v;
        scanf("%d%d", &u, &v);
        E[u].pb(v), ++ind[v];
    }
    // B = min(n, BB);
    if(n <= BB) B = n;
    else{
        B = 1;
        FOR(i, 1, BB - 1) if(i+(i*(i-1))/2+(i*(i-1)*(i-2)/6)<=n) ++B;
    }
}
inline void Init2(){
    int *ql(que), *qr(que);
    FOR(i, 1, n) if(!ind[i]) *qr++ = i;
    while(ql != qr){
        int u = *ql++;
        for(int v: E[u]){
            if(!(--ind[v])) *qr++ = v;
        }
    }
}
inline void Init3(){
    static int e[M][M];
    memset(id, -1, sizeof(int) * N);
    FOR(i, 1, B){
        id[que[n - i]] = i - 1;
        rd[i - 1] = que[n - i];
    }
    // FOR(i, 0, n - 1) printf("%d%c", que[i], " \n"[i == ed_i]);
    // FOR(i, 0, B - 1) printf("%d%c", rd[i], " \n"[i == ed_i]);
    FOR(i, 0, B - 1){
        int u = rd[i];
        for(int v: E[u]) if(id[v] != -1){
            assert(id[v] < i);
            e[i][id[v]] = 1;
        }
        FOR(j, 0, i - 1) if(!e[i][j]){
            AddEdge(rd[i], rd[j]);
        }
    }
    FOR(i, 1, n) if(id[i] == -1) AddEdge(i, i);
}
inline void Init4(){
    VI s[3];
    FOR(i, 1, (1 << B) - 1){
        int t = __builtin_popcount(i);
        if(t <= 3) s[t - 1].pb(i);
    }

    FOR(i, 1, n) if(id[i] == -1){
        for(int v: E[i]) if(id[v] != -1){
            val[i] |= 1 << id[v];
        }
        if(!tb[val[i]]) tb[val[i]] = i;
    }

    // FOR(i, 1, n) printf("%d%c", val[i], " \n"[i == ed_i]);
    // FOR(i, 0, (1 << B) - 1) printf("%d%c", tb[i], " \n"[i == ed_i]);

    auto UpdNode = [s](int p) -> void{
        FOR(o, 0, 2) for(int j: s[o]) if(!tb[val[p] ^ j]){
            ROF(i, B - 1, 0) if((j >> i) & 1){
                (((val[p] >> i) & 1) ? DelEdge : AddEdge)(p, rd[i]);
            }
            tb[val[p] ^= j] = p;
            return;
        }
    };

    FOR(i, 1, n) if(id[i] == -1 && tb[val[i]] != i){
        UpdNode(i);
    }

    printf("%d\n", (int)ans.size());
    for(auto [x, y, z]: ans){
        printf("%c %d %d\n", x, y, z);
    }
    fflush(stdout);
}

inline void Work(){
    int v = 0, t;
    FOR(i, 0, B - 1){
        t = Query(rd[i]);
        if(t == -1) return Report(rd[i]);
        if(t == 1) v |= 1 << i;
    }
    assert(tb[v] != 0);
    Report(tb[v]);
}

int main() {
    Init1();
    Init2();
    Init3();
    Init4();
    while(esac--) Work();
    return 0;
}
```
{% endfold %}