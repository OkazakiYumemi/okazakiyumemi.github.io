---
title: 「AGC047E」Product Simulation
categories: 题解
urlname: AGC047E
date: 2020-10-07 19:28:56
tags:
- AtCoder
- 构造题
top:
---

最近感觉所有要做的题都是在 CF 上的。
然而今天 CF 挂了。
随便点开最近的 AGC。一看，怎么是造计算机题啊。

## 题意简述

[AGC 047E](https://atcoder.jp/contests/agc047/tasks/agc047_e)

给一个大小为 $N=2\times 10^5$ 的内存池 $a_i$，有两种操作：
- `+ i j k`，$a_k\gets a_i + a_j$。
- `< i j k`，$a_k\gets [a_i < a_j]$。

注意操作时不需要满足`i, j, k`互不相同。

输出一个操作序列使得按序列操作后 $a_2 = a_0\times a_1$。除了 $a_0, a_1$ 外其他元素初始均为 $0$。
记你的操作序列长度为 $Q$，则应保证 $Q\le 2\times 10^5$。
保证非负整数 $a_0,a_1\le 10^9$，所有元素在使用时应保证不大于 $10^{19}$。

<!-- more -->

## 主要思路

首先我们会把一个位置 $i$ 设为 $0$：将 $i$ 和某个未使用的位置比较。
其次我们也会把一个位置 $i$ 设为 $1$：将某个未使用的位置和 $a_0 + a_1$ 作比较。

注意到若 $a_0 = a_1 = 0$ 造不出 $1$，但是此时显然无论怎么操作都是 $0$，所以答案也是对的。

其次我们会把一个数左移 $k$ 位：自己加自己 $k$ 次就好了。

那么考虑如何取出一个数 $x$ 的所有二进制位。

先造出所有 $2^k$。
然后从高到低确定每一位即可：

设已经确定的位置的和为 $C$，现在在确定第 $k$ 位。
那么将 $C + 2^k$ 与 $x + 1$ 比较得到 $tmp$ 就是这位是否是 $1$。
最后把 $C\gets C + tmp\times 2^k$ 即可。

于是把 $a_0, a_1$ 的二进制位都取出来，模拟二进制乘法即可。

最终取得了`3903`次的答案。感觉还没卡到最优但懒得卡了（

## 参考代码

{% fold %}
```cpp
#include <bits/stdc++.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a), ed_##i = (b); i <= ed_##i; ++i)
#define ROF(i, a, b) for (int i = (a), ed_##i = (b); i >= ed_##i; --i)
typedef long long i64;
struct ASK{
    int O, I, J, K;
    inline ASK(): O(0), I(0), J(0), K(0){}
    inline ASK(int _O, int _I, int _J, int _K): O(_O), I(_I), J(_J), K(_K){}
};
vector<ASK> ans;
#define ask(o, i, j, k) ans.push_back(ASK(o, i, j, k))
inline void write(){
    printf("%d\n", (int)ans.size());
    for(ASK q: ans)
        printf("%c %d %d %d\n", "+<"[q.O], q.I, q.J, q.K);
}
//`+ i j k`: a[k] = a[i] + a[j]
//`< i j k`: a[k] = bool(a[i] < a[j])

#define N 300
//a[N] = 0
//a[N - 1] = a[0] + a[1]
//a[N - 2] = 1, a[N - 2 - i] = 1 << i

inline void Reset(const int &x){
    ask(1, x, N, x);
}
inline void Set(const int &x){
    ask(1, N, N - 1, x);
}
inline void Set(const int &x, const int &y){
    ask(0, N, y, x);
}
inline void Double(const int &x){
    ask(0, x, x, x);
}
inline void Shift(const int &x, const int &t){
    FOR(i, 1, t) Double(x);
}
// inline void Make(const int &x, const i64 &v){
//     int m = 64 - __builtin_clzll(v);
//     while(m--){
//         if((v >> m) & 1) ask(0, x, N - 2, x);
//         if(m) Double(x);
//     }
// }
// //use: 190001
// inline void Add(const int &x, const i64 &v){
//     #define tmp 190001
//     if(!v) return;
//     int m = 64 - __builtin_clzll(v);
//     if(1ll << (m - 1) == v){
//         ask(0, x, N - 1 - m, x);
//         return;
//     }
//     Reset(tmp), Make(tmp, v);
//     ask(0, x, tmp, x);
//     #undef tmp
// }

//use: [200, 202)
inline void Split(const int &x, int t){
    #define P 200
    #define C 201
    #define tmp 202
    ask(0, x, N - 2, P);
    Reset(C);
    ROF(z, 29, 0){
        ask(0, C, N - 2 - z, tmp);
        ask(1, tmp, P, tmp);
        Set(t + z, tmp);
        Shift(tmp, z);
        ask(0, C, tmp, C);
    }
    // return t + 30;
    #undef P
    #undef C
    #undef tmp
}
//use: [100, 200)
inline void Mul(const int &x, const int &a, const int &b){
    #define A 100
    #define B 150
    #define tmp 199
    Split(a, A), Split(b, B);
    ROF(k, 58, 0){
        int i, j;
        for(i = 0; i <= k; ++i){
            j = k - i;
            if(i <= 29 && j <= 29){
                ask(0, A + i, B + j, tmp);
                ask(1, N - 2, tmp, tmp);
                ask(0, x, tmp, x);
            }
        }
        if(k) Double(x);
    }
    #undef A
    #undef B
    #undef tmp
}

inline void init(){
    ask(0, 0, 1, N - 1);
    Set(N - 2);
    FOR(i, 1, 29) ask(0, N - 1 - i, N - 1 - i, N - 2 - i);
}

int main() {
    init();
    Mul(2, 0, 1);
    write();
    return 0;
}
```
{% endfold %}