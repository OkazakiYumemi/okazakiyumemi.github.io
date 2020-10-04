---
title: 「GYM101741H」Compressed Spanning Subtrees
categories: 题解
urlname: GYM101741H
date: 2020-09-30 17:24:29
tags:
- GYM
- 交互题
- 构造
top:
---

## 题意简述

[GYM101741H](https://codeforces.com/gym/101741/problem/H)

交互题。有一棵 $n$ 个点的无根树，你至多询问 $2550$ 次点集 $X$，交互器会告诉你 $X$ 的虚树大小。求这棵树。
$n\le 100$，虚树是也是无根的，没有二度点。

<!-- more -->

## 主要思路

询问 $\\{[n]\backslash x\\}$ 可以知道 $x$ 是不是叶子。

然后随便取一个叶子为根 $rt$。
对每个非叶子 $i$ 和叶子 $j$ 询问 $\\{rt, i, j\\}$ 可以知道 $i$ 是否是 $j$ 的祖先。不妨设 $S(x)$ 为点 $x$ 子树内的叶子集合。

注意到树上没有二度点，所以对于点 $x$ 与其父亲 $f$ 有 $S(x)\varsubsetneq S(f)$（$x,f \neq rt$）。
所以我们可以对于每个任意两点之间找出他们是否有祖先关系了。

然后对于点 $x$，求一个祖先 $p$ 使得 $\operatorname{siz}(p)$ 最小，就是其父亲。

## 参考代码

这 cf 怎么不支持`__uint128_t`啊（半恼）

{% fold %}
```cpp
#include <bits/stdc++.h>
namespace my_std {
using namespace std;
#define reg register
#define Rint register int
#define FOR(i, a, b) for (register int i = (a), ed_##i = (b); i <= ed_##i; ++i)
#define ROF(i, a, b) for (register int i = (a), ed_##i = (b); i >= ed_##i; --i)
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
} // namespace my_std
using namespace my_std;

typedef vector<int> V;
typedef unsigned long long u64;
struct u128{
    u64 a[2];
    inline u128(){ a[0] = a[1] = 0; }
    inline u128(const u64 b_0, const u64 b_1){ a[0] = b_0, a[1] = b_1; }
    inline popcount(){
        return __builtin_popcountll(a[0]) + 
            __builtin_popcountll(a[1]);
    }
    inline void set(const int &x){
        a[x >> 6] |= 1ull << (x & 63);
    }
    inline bool test(const int &x){
        return (a[x >> 6] >> (x & 63)) & 1;
    }
    inline u128 operator &(const u128 &x)const{
        return u128(a[0] & x.a[0], a[1] & x.a[1]);
    }
    inline bool operator ==(const u128 &x)const{
        return a[0] == x.a[0] && a[1] == x.a[1];
    }
};

#define pb push_back
#define N 128
inline int query(const V &a){
    printf("? %d ", (int)a.size());
    for(Rint v: a) printf("%d ", v);
    puts("");
    fflush(stdout);
    return read();
}

int n;
int root;
int siz[N], fth[N];
bool leaf[N];
u128 s[N], pr[N];
V e[N];

void dfs(const int &u, const int &f){
    fth[u] = f;
    for(Rint v: e[u]) if(v != f) dfs(v, u);
}

int main() {
    n = read();
    FOR(i, 1, n){
        V a;
        FOR(j, 1, i - 1) a.pb(j);
        FOR(j, i + 1, n) a.pb(j);
        (leaf[i] = query(a) == n - 1) && (root = i);
    }
    FOR(i, 1, n){
        if(leaf[i]){
            s[i].set(i);
            s[root].set(i);
        }else{
            FOR(j, 1, n) if(leaf[j] && j != root){
                V a;
                a.pb(root), a.pb(i), a.pb(j);
                query(a) == 3 && (s[i].set(j), 1);
            }
        }
    }
    FOR(i, 1, n) FOR(j, 1, n){
        (s[i] & s[j]) == s[j] && (pr[i].set(j), ++siz[i]);
    }
    // FOR(i, 1, n) siz[i] = pr[i].popcount();
    *siz = 128;
    FOR(i, 1, n){
        Rint x(0);
        FOR(j, 1, n) if(i != j && pr[j].test(i))
            siz[j] < siz[x] && (x = j);
        if(x) e[i].pb(x), e[x].pb(i);
    }
    dfs(1, 0);
    printf("! ");
    FOR(i, 2, n) printf("%d ", fth[i]);
    puts("");
    fflush(stdout);
    return 0;
}
```
{% endfold %}