---
title: 「AGC022F」Checkers
date: 2020-02-09 18:35:03
tags:
- dp
- 计数
- AtCoder
categories: 题解
top:
urlname: AGC022F
---

这题太神了，对着题解看了好久还没完全搞懂……

## 题意简述

[[AGC 022F]](https://atcoder.jp/contests/agc022/tasks/agc022_f)

数轴上有 $n$ 个点，第 $i$ 个点的坐标是 $10^{100i}$ 。每次操作选择两个棋子 $a, b$ ，之后 $a$ 跳到关于 $b$ 的对称点上，然后删掉 $b$ 。求 $n - 1$ 次操作后，最后剩下的棋子可能的坐标个数模 $10^9 + 7$ 。$1 \le n \le 50$ 。

<!-- more -->

## 主要思路

显然，两种方案不同仅当某个棋子对最后答案贡献的系数不同。

定义一次操作 $a \rightarrow b$ （称「$a$ 合并到 $b$」）表示选择第 $b$ 个棋子与第 $a$ 个棋子进行操作（删除 $a$ 上的棋子）。我们对于每一次操作 $a\rightarrow b$ ，将 $b$ 向 $a$ 连一条边，则会形成一棵以最后剩下的点为根的树。将深度从 $0$ 开始计，则深度为 $d$ 的点贡献的系数为 $+2^d$ 或 $-2^d$ 。

设点 $x$ 最后给答案贡献的正负性（称「点权」）为 $p_x\in \\{-1, 1\\}$ 。设点 $x$ 的「奇偶性」为 $q_x\in \\{-1, 1\\}$，有奇数个儿子的点为奇点（$q_x = -1$），有偶数个儿子的点为偶点（$q_x = 1$）。

由题面定义，若某一棋子 $y$ 合并到棋子 $x$ ，则棋子 $x$ 贡献的正负性将改变。

从上向下考虑可以得到，对于点 $rt$ 的总共 $k$ 个儿子，其中有 $\lfloor\frac{k}{2}\rfloor$ 个儿子 $ch$ 有 $p_{rt} = p_{ch}\times q_{ch}$ ，$\lceil\frac{k}{2}\rceil$ 个儿子 $ch$ 有 $-p_{rt} = p_{ch}\times q_{ch}$ 。（$q_{ch}$ 可以理解为操作完 $ch$ 子树后 $ch$ 目前的贡献正负性）

我们并不关心整棵树的形态，而只关心每一层中两种点权分别的个数。按深度从小到大挂点，设 $f_{i, j}$ 表示已经挂了 $i$ 个点，上一层有 $j$ 个奇点的答案。

钦定上一层的所有奇点的点权均为 $1$ （若原本应为 $-1$，则相当于对整棵子树的点权取反，这样的方案数与原来是相等的）。枚举这一层的点数 $x$ 与点权为 $-1$ （在钦定该层奇点点权均为 $1$ 的情况下）的点的个数 $y$ 。根据上面的结论，可以得到这一层有 $\frac{x + j}{2}$ 个点 $v$ 使得 $p_v\times q_v = -1$ 。因此，这一层的奇点数应该是 $\|y - \frac{x + j}{2}\|$ ，乘上组合数转移即可。

注意 $x \ge j$ （使得 $p_v\times q_v = -1$ 成立的点 $v$ 的个数比不成立的点个数多 $j$ 个）且 $x + j$ 为偶数。

时间复杂度 $O(n ^ 4)$ 。

## 参考代码

{% fold "更多细节见代码。" %}
```cpp
#include<bits/stdc++.h>
using namespace std;
#define reg register
#define Rint register int
#define FOR(i,a,b) for(register int i=(a),ed_##i=(b);i<=ed_##i;++i)
#define ROF(i,a,b) for(register int i=(a),ed_##i=(b);i>=ed_##i;--i)
inline int read(){
	reg int ans=0,f=1; reg char c=getchar();
	while(!isdigit(c)) f^=(c=='-'), c=getchar();
	for(;isdigit(c);c=getchar()) ans=(ans<<1)+(ans<<3)+(c^48);
	return f?ans:-ans;
}
const int mod = 1000000007;
inline void inc(int &x,const int &y){ x+=y; if(x>=mod) x-=mod; }
inline int _abs(const int &x){ return x >> 31 ? -x : x; }
#define abs(x) _abs(x)
const int N = 55;
int n;
int f[N][N], C[N][N];

int main(){
	n = read();
	FOR(i, 0, n){
		C[i][0] = C[i][i] = 1;
		FOR(j, 1, i - 1) inc(C[i][j], C[i - 1][j - 1] + C[i - 1][j]);
	}
	f[1][0] = f[1][1] = n;
	FOR(i, 1, n - 1) FOR(j, 0, i){
		FOR(x, 1, n - i){
			if(x < j || (x ^ j) & 1) continue;
			FOR(y, 0, x){
				Rint tr = abs(y - ((x + j) >> 1));
				inc(f[i + x][tr], 1ll * f[i][j] * C[n - i][x] % mod * C[x][y] % mod);
			}
		}
	}
	printf("%d\n", f[n][0]);
	return 0;
}
```
{% endfold %}