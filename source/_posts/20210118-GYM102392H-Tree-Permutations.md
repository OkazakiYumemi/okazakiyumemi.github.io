---
title: 「GYM102392H」Tree Permutations
categories: 题解
urlname: GYM102392H
date: 2021-01-18 18:29:18
tags:
- GYM
- 树论
- 构造
- 猜结论
top:
---

~~为啥集训队作业只有 NEERC，没有 SEERC~~

## 题意简述

[GYM102392H](https://codeforces.com/gym/102392/problem/H)

神树爷爷建了一棵树，大小为 $n$，根为 $1$，每个节点的父亲 $p_i<i$，连向父亲的边的边权 $w_i$。

神树爷爷把所有 $p_i, w_i$ 混在一起之后乱序扔给了你，记这个序列为 $a$。

显然一个 $a$ 可能对应多棵可能的树。

神树爷爷认为一棵树是`k-good`的，仅当 $1$ 到 $n$ 的路径上有 $k$ 条边；

一棵树是`k-perfect`的，仅当这棵树是`k-good`的树中，$1$ 到 $n$ 路径上的边权和是最大的。

现在神树爷爷希望你对所有 $1\le k\le n-1$，求出`k-perfect`树 $1$ 到 $n$ 路径上的边权和。

如果没有`k-perfect`树，输出`-1`。

$n\le 2\le 10^5, 1\le a_i\le n - 1$。

<!-- more -->

## 主要思路

先排序，下文中出现的序列，若未特殊说明，也默认已按不降排序。

【引理 1】如果对于某些 $i$ 有 $a_i>i$，那么根本没有合法的树。

【证明】假设 $a_i>i$。显然节点 $j\in[2, i+1]$ 的父亲都不大于 $i$，而此时最多仅有 $i-1$ 个不大于 $i$ 的数，故矛盾。

-----

【引理 2】如果对于某些 $i$ 有 $a_i=i$，那么节点 $i$ 在 $1$ 到 $n$ 的路径上。

【证明】注意到节点的编号形成堆结构，即，$1$ 到 $n$ 的路径为每次选择最大的儿子向下走。
假设 $a_i=i$。类似【引理 1】的证明，$i-1$ 个不大于 $i$ 的数分别应是点 $j\in[2, i]$ 的父亲，而对于节点 $j>i$，由于没有更多小于 $i$ 的数，其父亲均不小于 $i$。

-----

我们令 $T$ 为那些 $a_i=i$ 的位置集合，并令 $S$ 为所有未在 $T$ 中出现的 $a_i$ 第一次出现位置集合。
称`k-perfect`树 $1$ 到 $n$ 路径为*路径*，其上的边权和为`k-perfect`树的权值。

【引理 3】对于任意 $|T|\le k\le |T|+|S|$，有`k-good`的树。并且，`k-perfect`树的权值可以用以下方法计算：

标记 $T$ 中所有元素，以及 $S$ 中 $k-|T|$ 个最小元素。`k-perfect`树的权值即为未标记的位置中最大的 $k$ 个值的和。

【证明】显然地，$|T|$ 是下界，$|T|+|S|$ 是上界。我们仅需证明他们都可被成功构造。

记 $k-|T|$ 个 $S$ 中最小的元素为 $s_i(i\in[1,k-|T|])$。那么*路径*上应该包含节点 $a_i(i\in s\cup T)$ 与 $n$。
标记 $s, T$ 中所有位置，同时也标记所有在*路径*上的节点（这些节点除了 $1$ 都已有了父亲）。

未标记的 $2n-2-k$ 个位置记为 $\langle x\rangle$，未标记的 $n-k-1$ 个节点记为 $\langle v\rangle$。钦定 $a_{x_i}$ 为 $v_i$ 的父亲 $(i\in[1,n-k-1])$。
剩下的 $n-1$ 个位置对应的 $a_i$ 均为边权，将最大的 $k$ 个分给*路径*，其他的任意分给其他边。

这样我们得到了上述的答案，下面我们证明通过此方法构造出的确实是一棵树。

由于我们希望分配 $a_{x_i}$ 为 $v_i$ 的父亲，故只需证明 $a_{x_i}<v_i$。

不妨假设对于某些 $i$ 有 $a_{x_i}\ge v_i$。

记*路径*上的点为 $\langle p\rangle$，由于 $p_{k+1}=n$，我们可以找到 $j$ 使 $p_j\le a_{x_i}\le p_{j+1}$。即，有 $j$ 个已标记的节点不大于 $a_{x_i}$。
并且由于 $a_{x_i}\ge v_i$，即有 $i$ 个未标记的节点不大于 $a_{x_i}$。
因此，有 $a_{x_i}\ge i+j$。

现在来考虑 $x_i$，有 $i-1$ 个未标记的位置不大于 $x_i$。
根据 $S,T$ 的定义，$p_j$ 对应的已标记位置应在 $x_i$ 之前。即，有 $j$ 个已标记位置在 $x_i$ 之前。

于是 $i-1+j$ 个位置在 $x_i$ 之前，即 $x_i=i+j$。
即，$a_{x_i}\ge x_i$。

【引理 1】中已经证明 $a_{x_i}\le x_i$，且根据【引理 2】，若 $a_{x_i}=x_i$，则 $x_i$ 必定在*路径*上。Contradiction!

-----

实现可以类似 two pointers，复杂度 $O(n)$。

## 参考代码

{% fold %}
```cpp
#include <bits/stdc++.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a), ed_##i = (b); i <= ed_##i; ++i)
#define ROF(i, a, b) for (int i = (a), ed_##i = (b); i >= ed_##i; --i)
inline int read() {
    int x = 0;
    char c = getchar();
    while (!isdigit(c)) c = getchar();
    for (; isdigit(c); c = getchar())
        x = x * 10 + c - '0';
    return x;
}

#define N 100010
int n;
int c[N], b[N], a[N];
int L, R;
long long ans;

int main() {
    n = read();
    FOR(i, 1, (n - 1) * 2) R += !(c[read()]++);
    FOR(i, 1, n - 1){
        b[i] = b[i - 1] + c[i] - 1;
        if(b[i] < 0){
            FOR(i, 1, n - 1) printf("-1 ");
            return 0;
        }
        a[i] = c[i];
        if(!b[i - 1]) --a[i], ++L;
    }

    int l = 1, r = n - 1, d = 0;
    FOR(i, 1, n - 1){
        if(i < L || i > R){
            printf("-1 ");
            continue;
        }
        if(i > L){
            while(!b[l - 1] || !c[l]) ++l;
            if(a[l]) --a[l];
            else ans -= l, --d;
            ++l;
        }
        while(d != i){
            while(!a[r]) --r;
            --a[r], ++d, ans += r;
        }
        printf("%lld ", ans);
    }
    return 0;
}
```
{% endfold %}