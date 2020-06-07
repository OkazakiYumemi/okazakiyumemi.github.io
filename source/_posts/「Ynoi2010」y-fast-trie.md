---
title: 「Ynoi2010」y-fast trie
categories: 题解
date: 2020-06-07 16:45:34
tags:
- Ynoi
top:
---

我谔谔，被卡常力，，，

<!-- more -->

## 题意简述

给定一个数 $C$ ，维护一个集合 $S$，支持 $n$ 次操作，操作是加入或删除一个数 $x$。

每次操作结束后求集合中两个数的和模 $C$ 的最大值，强制在线。

$n\le 5\times 10^5$。

[[Luogu 6105]](https://www.luogu.com.cn/problem/P6105)

## 主要思路

首先把数都模 $C$，然后答案只有可能是两数的和或两数的和减去 $C$。

后者直接把两个最大的数拎出来求一下得了，非常好搞。

考虑前者如何处理。

再把这里的答案分成两个相等数和两个不等数，前者显然也随便做。

对于每个 $x\in S$，容易找到 $y\in S, y\neq x$ 使得对于 $x$ 答案最优（即 $x + y$ 最接近 $C$）。

试图对每个 $x\in S$ 维护一个匹配 $(x, y)$。

此时发现删除 $x$ 不好删，万一有一大堆数的最优匹配都是 $x$ 呢？复杂度退化。

于是考虑令每个数只在一个匹配中，即令匹配为双向的。这样复杂度可以保证，那么正确性呢？

考虑 $(y, z)$ 为匹配，此时加入 $x$，其最优匹配为 $y$ 且 $x > z$。此时若 $x$ 不删去，$z$ 永远不能成为 $y$ 的最优解。

所以每次删除数对其匹配更新一下即可保证正确性。

复杂度 $O(n\log n)$。

## 参考代码

比较卡常，搞了好久才过，，，

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
#define Templ(T) template <typename T>
static char InB[1 << 23], *In_s = InB, *In_t = InB;
static char OutB[1 << 23], *Out_s = OutB;
inline void FGO(){
    fwrite(OutB, 1, Out_s - OutB, stdout), Out_s = OutB;
    return;
}
inline void FGI(){
    In_t = (In_s = InB) + fread(InB, 1, 1 << 23, stdin);
    return;
}
inline char gcr(){
    // if(In_s == In_t){
    //     In_t = (In_s = InB) + fread(InB, 1, 1 << 23, stdin);
    //     if(In_s == In_t) return EOF;
    // }
    return *In_s ++;
}
inline void pcr(const char &c){
    // if(Out_s - OutB == 1 << 23) fwrite(OutB, 1, 1 << 23, stdout), Out_s = OutB;
    *Out_s ++ = c;
}
inline int read() {
    reg int ans = 0;
    reg char c = gcr();
    while (!isdigit(c)) c = gcr();
    for (; isdigit(c); c = gcr()) ans = (ans << 1) + (ans << 3) + (c ^ 48);
    return ans;
}
inline void write(int x){
    static int sta[12];
    if(x < 0) return pcr('-'), write(-x);
    Rint top = 0;
    do{
        sta[top ++] = x % 10, x /= 10;
    }while(x);
    while(top) pcr(sta[-- top] ^ 48);
}
Templ(_Tp) inline int chkmin(_Tp &x, _Tp y) { return x > y ? x = y, 1 : 0; }
Templ(_Tp) inline int chkmax(_Tp &x, _Tp y) { return x < y ? x = y, 1 : 0; }
#define PBTXDY} // namespace my_std
using namespace my_std;

typedef map<int, int>::iterator m_it;
typedef multiset<int>::iterator ms_it;

int n, C, siz;
map<int, int> cnt;
unordered_map<int, int> pr;
multiset<int> df;
set<int> sm;

inline int match(int x){
    reg m_it it = --cnt.lower_bound(C - x);
    if(it->fir == x) --it;
    return it->fir;
}
inline void init(int x){
    Rint y = match(x), z;
    if(y == -1) return;
    if(pr.count(y)){
        z = pr[y];
        if(z < x){
            pr.erase(z), pr[x] = y, pr[y] = x;
            df.erase(df.find(z + y)), df.insert(x + y);
            //把 z 从匹配中删去
        }
    }
    else{
        pr[y] = x, pr[x] = y;
        df.insert(x + y);
    }
    return;
}
inline void add(int x){
    Rint cnt_  = ++ cnt[x];
    if(cnt_ == 1) return init(x);//第一次出现
    if(cnt_ == 2 && 2 * x < C) sm.insert(2 * x);
}
inline void del(int x){
    Rint cnt_ = -- cnt[x];
    if(!cnt_){
        cnt.erase(x);
        Rint y;
        if(pr.count(x)){
            y = pr[x];
            pr.erase(y), pr.erase(x);
            df.erase(df.find(x + y)), init(y);
            //把整个匹配都删了，再把 y 加回去
        }
        return;
    }
    if(cnt_ == 1 && 2 * x < C) sm.erase(2 * x);
}
inline int query(){
    Rint ans = 0;
    reg m_it it = --cnt.end();
    ans = (it->sec > 1) ? 2 * it->fir % C : (it->fir + (--it)->fir) % C;
    if(!sm.empty()) chkmax(ans, *(--sm.end()));
    if(!df.empty()) chkmax(ans, *(--df.end()));
    return ans;
}

int main() {
    FGI();
    cnt[-1] = 1;
    n = read(), C = read();
    Rint opt, x, lastans = 0;
    while(n --){
        opt = read(), x = (read() ^ lastans) % C;
        opt == 1 ? (add(x), ++siz) : (del(x), --siz);
        // if(siz > 1) printf("%d\n", lastans = query());
        // else puts("EE"), lastans = 0;
        if(siz > 1) write(lastans = query());
        else pcr('E'), pcr('E'), lastans = 0;
        pcr('\n');
    }//我谔谔，，，
    return FGO(), 0;
}
```
{% endfold %}