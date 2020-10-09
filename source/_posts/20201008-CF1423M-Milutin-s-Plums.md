---
title: 「CF1423M」Milutin's Plums
categories: 题解
urlname: CF1423M
date: 2020-10-08 17:52:03
tags:
- Codeforces
- 交互题
top:
---

## 题意简述

交互题。
矩阵 $n\times m$，有性质：
对于任意子矩阵（即任意行的子集和列的子集的交），设子矩阵中第 $i$ 行最小值最先出现的位置是 $L(i)$，则 $L(i)$ 单调不降。

通过不超过 $4(n + m)$ 次单点询问确定该矩阵的最小值。

<!-- more -->

## 主要思路

转化问题为对每行 $i$ 求 $ans_i$ 使得 $(i, ans_i)$ 为该行最小最前的位置。

那么根据给定的性质，一个自然的想法是：

假如现在需要求答案的行集合为 $S$（按大小排序），已求出 $T = \\{S_{2i}\\}$ 的答案。
则此时 $S$ 中未被求出答案的所有元素 $x$ 的答案都被限定在上下两元素的答案范围内，可以暴力询问求解。
边界情况显然为 $|S| = 1$，此时使用 $m$ 次询问暴力求出答案即可。

那么对于集合 $S$，记其最多需要的询问次数为 $f(|S|) = f(\frac{|S|}{2}) + |S| + m$。
则总共需要 $2n + m\log n$ 次。

尽管 $n$ 的系数足够小，但 $m$ 的系数太大。

## 参考代码

{% fold %}
```cpp
#include <bits/stdc++.h>
using namespace std;
#define reg register
#define Rint register int
#define FOR(i, a, b) for (register int i = (a), ed_##i = (b); i <= ed_##i; ++i)
#define ROF(i, a, b) for (register int i = (a), ed_##i = (b); i >= ed_##i; --i)
#define MP make_pair
#define pb push_back
typedef pair<int, int> PII;
typedef vector<int> VI;
#define SZ(x) ((int)(x).size())
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

#ifdef LOCAL
#define N 1010
int qry[N][N];
int qry_cnt;
#endif

map<PII, int> mem;

inline int ask(int x, int y){
    #ifdef LOCAL
    return ++qry_cnt, qry[x][y];
    #endif
    if(mem.count(MP(x, y))) return mem[MP(x, y)];
    printf("? %d %d\n", x, y), fflush(stdout);
    return mem[MP(x, y)] = read();
}

//由于列太多，需要去除一些不可能在 ans 里的列
inline VI Reduce(VI rows, VI cols){
    VI ans;
    const int n = SZ(rows);

    int i = 0, r;
    for(int c: cols){
        if(ans.empty()){
            ans.pb(c);
            continue;
        }
        r = rows[i];
        int &y = ans.back();
        if(ask(r, y) <= ask(r, c)){
            if(SZ(ans) == n){
                //所有行都已经找到了最前的可能的列
                continue;
            }else{
                //显然此列不可能为这行的答案，于是将其配给下一行
                ans.pb(c);
                ++i;
            }
        }else{
            //修改该行匹配的列
            y = c;
        }
    }
    //注意这里仅仅是简单地去掉了一些多余的列
    //并不具有正确性
    return ans;
}

//(rows[i], ans[i]) 为该行最小且最前的
VI Solve(VI rows, VI cols){
    if(SZ(rows) < SZ(cols)){
        cols = Reduce(rows, cols);
    }

    const int n = SZ(rows), m = SZ(cols);

    VI nrows;
    for(int i = 1; i < n; i += 2){
        nrows.pb(rows[i]);
    }

    VI nans;
    if(!nrows.empty()){
        nans = Solve(nrows, cols);
    }
    //先做奇数行

    VI ans;
    int lc = 0, rc;
    int x, y;
    FOR(i, 0, n - 1){
        if(i & 1){
            //对于奇数行，已经做过了
            ans.pb(nans[i >> 1]);
            continue;
        }
        rc = m - 1;
        if(i + 1 < n){
            rc = lc;
            while(cols[rc] < nans[i >> 1]){
                ++rc;
            }
        }else{
            rc = m - 1;
        }
        //根据题目性质，偶数行的答案应该在上下两行之间
        x = rows[i];
        y = cols[lc];
        int mn = ask(x, y);
        FOR(j, lc + 1, rc){
            if(chkmin(mn, ask(x, cols[j]))){
                y = cols[j];
            }
        }
        //暴力找出来即可
        ans.pb(y);
        lc = rc;
    }
    return ans;
}

int main() {
    int n = read(), m = read();

    #ifdef LOCAL
    FOR(i, 1, n) FOR(j, 1, m) qry[i][j] = read();
    #endif

    VI rows, cols;
    FOR(i, 1, n) rows.pb(i);
    FOR(i, 1, m) cols.pb(i);
    VI ans = Solve(rows, cols);

    int mn = 0x3f3f3f3f;
    FOR(i, 0, n - 1){
        chkmin(mn, ask(rows[i], ans[i]));
    }

    printf("! %d\n", mn), fflush(stdout);

    #ifdef LOCAL
    printf("ask times: %d\n", qry_cnt);
    int rmn = 0x3f3f3f3f;
    FOR(i, 1, n) FOR(j, 1, m) chkmin(rmn, qry[i][j]);
    printf("real min: %d\n", rmn);
    puts(mn == rmn ? (qry_cnt <= 4 * (n + m) ? 
        "AC" : "AC but too many times") : "WA");
    #endif

    return 0;
}
```
{% endfold %}