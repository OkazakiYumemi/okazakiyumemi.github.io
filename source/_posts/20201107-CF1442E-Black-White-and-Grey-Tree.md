---
title: "「CF1442E」Black, White and Grey Tree"
categories: 题解
urlname: CF1442E
date: 2020-11-07 23:07:47
tags:
- Codeforces
- 构造
- 找规律
top:
---

## 题意简述

给一棵树，树上每个节点都为灰色、白色或黑色。
每次可以选择**一个连通块**中的一个节点集合，要求集合中不能同时存在黑色和白色的节点，然后删去集合中所有节点。
问最少多少次可以把整棵树删完。

$n\le 200000$。

<!-- more -->

## 主要思路


首先玩仅有黑色和白色。
显然相同颜色的连通块可以缩成一个点，先钦定任意一条边两端颜色不同。
不难发现答案为 $\lfloor{k\over 2}\rfloor+1$，其中 $k$ 为直径的长度（点的数量）。

那么有灰色的时候，就是要把每个灰色的点赋一种颜色，使得直径最短。
不妨思考一下直径的求法：随便取一个点 $S_0$ 为根，dfs 找到树上深度最大的点 $S_1$；再以 $S_1$ 为根 dfs 找到深度最大的点 $S_2$；$S_1,S_2$ 即为直径的两个端点。

那么再次考虑仅有黑色和白色，不难发现直接套用上述算法，更改深度定义为「到根路径上极长颜色相同段个数」即可。

再考虑有灰色。
如果 dfs 到一个灰色的点，我们钦定其为其**最近的非灰色祖先**的颜色。
显然，（由于钦定了灰色点的颜色）产生贡献的地方仍然只有黑白间。
但是在选深度最深点的时候，灰色点应该优先级低于黑白色的点的。
就没了。

## 参考代码

{% fold %}
```cpp
#include<bits/stdc++.h>
#define FOR(i, a, b) for (int i = (a), ed_##i = (b); i <= ed_##i; ++i)
inline int read() {
	int x = 0;
	char c = getchar();
	while (!isdigit(c)) c = getchar();
	for (; isdigit(c); c = getchar())
		x = x * 10 + c - '0';
	return x;
}
#define Templ(T) template <typename T>
Templ(_Tp) inline int chkmax(_Tp &x, _Tp y) { return x < y ? x = y, 1 : 0; }

#define N 200010
int n, a[N], ans, rt, col;
std::vector<int> E[N];

void dfs(int u, int fa, int now, int dis){
	if(!col || chkmax(ans, dis)) rt = u, col = now;
	for(int v: E[u]) if(v != fa){
		dfs(v, u, a[v] ? a[v] : now, dis + ((now ^ a[v]) == 3));
	}
}

void work(){
	n = read();
	ans = -1, col = 0;
	FOR(i, 1, n) a[i] = read(), E[i].clear();
	FOR(i, 1, n - 1){
		int u = read(), v = read();
		E[u].emplace_back(v), E[v].emplace_back(u);
	}
	dfs(1, 0, a[1], 0);
	dfs(rt, 0, col, 0);
	printf("%d\n", (ans + 3) >> 1);
}

int main(){
	FOR(o, 1, read()) work();
}
```
{% endfold %}