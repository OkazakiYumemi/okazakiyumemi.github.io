---
title: 「CodeChef LUCKYDAY」Lucky Days
categories: 题解
urlname: CC-LUCKYDAY
date: 2021-01-14 21:09:43
tags:
- CodeChef
- 数学
- BSGS
top:
---

## 题意简述

有序列 $S$，其中 $S_1=A,S_2=B;S_i=X\cdot S_{i-1}+Y\cdot S_{i-2}+Z(i>2)\\\% P$。
$Q$ 个询问求 $\sum\limits_{i=L}^{R}[S_i=C]$。
$A,B,X,Y,C,P,Q$ 均给定，$P$ 为不超过 $10007$ 的质数，$0\le A,B,X,Y,C<P$，$Q\le 2\times 10^4$，$1\le L\le R\le 10^{18}$。

<!-- more -->

## 主要思路

先把 $X=0$ 或 $Y=0$ 等情况判了。此时循环节长度 $O(P)$，暴力找出即可。
**注意到此时可能循环节从 $3$ 开始。**

-----

设 $F_i=\begin{bmatrix}S_i\\\\S_{i+1}\\\\1\end{bmatrix},M=\begin{bmatrix}0&1&0\\\\Y&X&Z\\\\0&0&1\end{bmatrix}$，则有 $F_i = M\times F_{i-1}$。

不同的 $F_i$ 仅有 $P^2$ 个，由[HDU 5451](../HDU5451)的经验，循环节一定是 $P^2-1$ 的约数，并且 $X>0,Y>0$ 时因为转移矩阵可逆，故一定是纯循环。

那么就先来找最小循环节 $R$。枚举约数矩阵快速幂即可。

然后考虑如何找到循环节中所有 $S_i=C$。由于循环节内相同 $F_i$ 只会出现一次，我们试图枚举 $m$，找到所有 $t=\begin{bmatrix}C\\\\m\\\\1\end{bmatrix}$ 的位置。
即，找到 $0\le k<R$ 使 $M^k F_1=t$ 成立。

设一阈值 $L$，则 $M^{dL-e}F_1=t$。由于 $M$ 可逆，变成 $M^{dL}F_1=M^et$，且 $d$ 是 $O({R\over L})$ 级别，$e$ 是 $O(L)$ 级别。

不难想到可以 $O({R\over L})$ 预处理出所有左边的可能值，扔到 Hash Table 里，就支持 $O(1)$ 查询。
那么每次查询时，枚举 $e$，即可做到 $O(L)$ 单次查询。
~~喂这不就是 BSGS……~~

由于有 $O(P)$ 次查询（枚举 $m$），所以时间复杂度是 $O(LP)+O({R\over L})$，由于 $R$ 是 $O(P^2)$ 级别，故 $L$ 取 $\sqrt{P}$ 时复杂度最优为 $O(P\sqrt{P})$。

之后，每个询问就 $O(\log P)$ 了。

总时间复杂度 $O(P\sqrt{P} + Q\log P)$。

## 参考代码

代码真的很烦，~~我的 Hash Table 都是抄 [Sunli Chen](https://codeforces.com/profile/OhWeOnFire) 的。~~

{% fold %}
```cpp
#include <bits/stdc++.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a), ed_##i = (b); i <= ed_##i; ++i)
#define ROF(i, a, b) for (int i = (a), ed_##i = (b); i >= ed_##i; --i)
typedef long long i64;
#define Templ(T) template <typename T>
inline int read() {
    int x = 0; char c = getchar();
    while (!isdigit(c)) c = getchar();
    for (; isdigit(c); c = getchar()) x = x * 10 + c - '0';
    return x;
}
inline i64 read64() {
    i64 x = 0; char c = getchar();
    while (!isdigit(c)) c = getchar();
    for (; isdigit(c); c = getchar()) x = x * 10 + c - '0';
    return x;
}
Templ(_Tp) inline int chkmin(_Tp &x, _Tp y) { return x > y ? x = y, 1 : 0; }
Templ(_Tp) inline int chkmax(_Tp &x, _Tp y) { return x < y ? x = y, 1 : 0; }
int __MOD__;
#define P __MOD__
inline void inc(int &x, const int &y) {
    x += y; if (x >= P) x -= P;
}
inline void dec(int &x, const int &y) {
    x -= y; if(x < 0) x += P;
}
inline int ksm(int x, i64 y) {
    int res = 1;
    for (; y; y >>= 1, x = (i64)x * x % P)
        if (y & 1) res = (i64)res * x % P;
    return res;
}
/*
[ S(i) ]   [0  1  0]   [S(i-1)]
[S(i+1)] = [Y  X  Z] * [ S(i) ]
[  1   ]   [0  0  1]   [  1   ]
*/
#define MR 160
#define SL 150

// Thanks Eric Sunli Chen for his AMAZING hash table!
#define Hash_P 2333333
#define Hash_N 888888
struct Hash_T {
    int head[Hash_P], nxt[Hash_N], a[Hash_N], val[Hash_N], tot;
    void Clear() {
        memset(head, 0, sizeof(head));
        tot = 0;
    }
    Hash_T() { Clear(); }
    inline int &operator[](const int &x) {
        int tx = x % Hash_P;
        for (int i = head[tx]; i; i = nxt[i])
            if (val[i] == x)
                return a[i];
        nxt[++tot] = head[tx];
        head[tx] = tot;
        val[tot] = x;
        a[tot] = 0;
        return a[tot];
    }
    inline int Find(const int &x) const {
        int tx = x % Hash_P;
        for (int i = head[tx]; i; i = nxt[i])
            if (val[i] == x)
                return a[i];
        return -1;
    }
};
Hash_T rem;

struct Matrix{
    int a[3][3];
    inline Matrix(){ memset(a, 0, 36); }
    inline bool Unit(){
        FOR(i, 0, 2) FOR(j, 0, 2) if(a[i][j] != (i == j)) return false;
        return true;
    }
    inline Matrix operator *(const Matrix &y)const{
        Matrix z;
        FOR(i, 0, 2) FOR(k, 0, 2) FOR(j, 0, 2)
            (z.a[i][j] += a[i][k] * y.a[k][j]) %= P;
        return z;
    }
}IE;
struct Vector{
    int a[3];
    inline Vector(){ memset(a, 0, 12); }
    friend inline Vector operator *(const Matrix &x, const Vector &y){
        Vector z;
        z.a[0] = (x.a[0][0] * y.a[0] + x.a[0][1] * y.a[1] + x.a[0][2] * y.a[2]) % P;
        z.a[1] = (x.a[1][0] * y.a[0] + x.a[1][1] * y.a[1] + x.a[1][2] * y.a[2]) % P;
        z.a[2] = 1;
        return z;
    }
};

inline Matrix ksm(Matrix x, int y){
    Matrix z = IE;
    for(; y; y >>= 1, x = x * x)
        if(y & 1) z = z * x;
    return z;
}
inline int Hash(const Vector &x){
    return x.a[0] * P + x.a[1];
}
Matrix M, MM;
Vector S;
int A, B, X, Y, Z, C, Q;
int Per, MxP;
// int SL;
vector<int> W;

inline void work0(){
    i64 L, R, res;
    while(Q--){
        L = read64(), R = read64();
        res = 0;
        if(L == 1) res += R >= 1 && A == C, ++L;
        if(L == 2) res += R >= 2 && B == C, ++L;
        if(Z == C && L <= R) res += R - L + 1;
        printf("%lld\n", res);
    }
}
inline void work1(){
    int *F = (int*)malloc(sizeof(int) * 5 * P), Per = 0;
    F[1] = A, F[2] = B;
    FOR(i, 3, 4 * P){
        F[i] = ((i64)F[i - 1] * X + (i64)F[i - 2] * Y + Z) % P;
    }
    FOR(i, 5, 4 * P){
        if(F[i - 1] == F[3] && F[i] == F[4]){ Per = i - 4; break; }
    }
    *F = 0;
    FOR(i, 1, Per) F[i] = F[i - 1] + (F[i + 2] == C);
    function<i64(i64)> Qry = [F, Per](i64 Lim){
        i64 res = 0;
        if(Lim >= 1) res += A == C, --Lim;
        if(Lim >= 1) res += B == C, --Lim;
        if(Lim <= 0) return res;
        return res + F[Per] * (Lim / Per) + F[Lim % Per];
    };
    i64 L, R;
    while(Q--){
        L = read64(), R = read64();
        printf("%lld\n", Qry(R) - Qry(L - 1));
    }
    free(F);
}
inline void init(){
    M.a[0][1] = 1;
    M.a[1][0] = Y;
    M.a[1][1] = X;
    M.a[1][2] = Z;
    M.a[2][2] = 1;
    S.a[0] = A;
    S.a[1] = B;
    S.a[2] = 1;
    W.clear();
    rem.Clear();
}

inline int Find(Vector v){
    FOR(i, 0, SL - 1){
        int t = rem.Find(Hash(v));
        if(t != -1 && t - i <= Per) return t - i;
        v = M * v;
    }
    return 0;
}

void work(){
    A = read(), B = read();
    X = read(), Y = read(), Z = read();
    P = read(), C = read(), Q = read();
    if(X == 0 && Y == 0) return work0();
    if(X == 0 || Y == 0) return work1();

    init();
    Per = MxP = P * P - 1;
    FOR(i, 1, P) if(MxP % i == 0){
        if(ksm(M, i).Unit()) chkmin(Per, i);
        if(ksm(M, MxP / i).Unit()) chkmin(Per, MxP / i);
    }
    // SL = sqrt(Per);
    MM = ksm(M, SL);
    {
        Vector v = ksm(M, SL - 1) * S;
        for(int i = SL, ed_i = Per + SL; i < ed_i; i += SL){
            int &tmp = rem[Hash(v)];
            if(!tmp) tmp = i;
            v = MM * v;
        }
        v.a[0] = C;
        v.a[2] = 1;
        FOR(i, 0, P - 1){
            v.a[1] = i;
            int t = Find(v);
            if(t) W.emplace_back(t);
        }
        sort(W.begin(), W.end());
    }
    i64 L, R;
    const int t = W.size();
    function<i64(i64)> Qry = [t](const i64 &Lim){
        return t * (Lim / Per) +
            upper_bound(W.begin(), W.end(), Lim % Per) - W.begin() - 1;
    };
    while(Q--){
        L = read64(), R = read64();
        printf("%lld\n", Qry(R) - Qry(L - 1));
    }
}

int main() {
    IE.a[0][0] = 1;
    IE.a[1][1] = 1;
    IE.a[2][2] = 1;
    int T = read();
    while(T--) work();
    return 0;
}
```
{% endfold %}

~~然后喜提最速解（~~