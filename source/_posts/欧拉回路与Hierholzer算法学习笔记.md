---
title: 欧拉回路学习笔记与Hierholzer算法
date: 2019-11-09 21:59:47
tags: 图论
categories: 算法
top:
---

欧拉回路和欧拉路径的学习笔记。

例题：[[UOJ 117]](http://uoj.ac/problem/117) 、 [[LOJ 10105]](https://loj.ac/problem/10105) 、 [[Luogu 2731]](https://www.luogu.org/problem/P2731) 。

<!-- more -->

# 一些定义

一个图，若能够找到一条路径，使得可以遍历完所有的边且不重复，则这样的图称为欧拉图，这条路径称为欧拉路径，若路径闭合也称为欧拉回路。

# 一些定理

以下大量文字摘自维基百科。

## 定理一

- 连通的无向图有欧拉路径的充要条件是：图中度数是奇数的顶点数量是 $0$ 或 $2$ 。
- 连通的无向图有欧拉回路的充要条件是：图中所有点的度数都是偶数。

### 证明一

- 必要性：如果一个图能够一笔画成，那么对于除了起点和终点的其他顶点，总有路径中连向点的边数与这个点的出边数相同，即这个点的度数为偶数。而对于起点和终点，若起点和终点相同，则这个点的度数也为偶数；否则这两个点的度数为奇数。即图中度数为奇数的点的数量是 $0$ 或 $2$ 。
- 充分性：
  1. 如果图中没有度数为奇数的点，则任选一点出发，连一个环。如果这个环就是原图则结束。若不是，由于图是连通的，该环与原图的其他部分必然有公共顶点，从该点出发，在原图的剩余部分中重复上述步骤。由于原图是连通图，经过若干步后，全图被分为一些环。由于两个相连的环就是一个环，原来的图也就是一条欧拉回路。
  2. 如果图中有两个度数为奇数的顶点 $u$ 和 $v$ ，那么将他们连边后，得到一个无奇数度数的点的连通无向图，由上知这是一个环，因此去掉新加的边后，成为一条路径，起点和终点是 $u$ 与 $v$ 。至此证毕。

## 定理二

如果连通无向图有 $2k$ 个奇数度数的顶点，那么它可以且至少可以用 $k$ 笔画成。

### 证明二

将这 $2k$ 个奇数度数的顶点分成 $k$ 对后分别连起，则得到一个无奇数度数顶点的连通图。由上知这个图是一个环，因此去掉新加的边后至多成为 $k$ 条欧拉路径，因此必然可以用 $k$ 笔画成。但是假设全图可以分为 $q$ 条欧拉路径，则由定理一知，每条链中只有不多于两个奇顶点，于是 $2q\ge 2k$。因此必定至少要 $k$ 笔画成。

## 有向图的欧拉路径

- 一个连通的有向图可以表示为一条欧拉回路的充要条件是：每个顶点的出度和入度都相等。
- 一个连通的有向图可以表示为一条不闭合的欧拉路径的充要条件是：某一个点的出度比入度多 $1$ ，另一个点的出度比入度少 $1$ ，前者为起点而后者为终点。

证明方式类似证明一。

<br><br>

# Hierholzer算法

Hierholzer算法是一种可以在时间复杂度 $O(|E|)$ 内求出无向图或有向图的欧拉路径（如果有的话）的算法。

## 算法流程

首先要判断这个图有没有欧拉路径，判断方法就是前面的定理。

然后，任选择一个顶点 $u$ 开始 **dfs** 。枚举 $u$ 的每条出边（若是无向图则为无向边） $e=(u,v)$ ，将这条边删去并 **dfs** 点 $v$ ，结束后将这条边加入答案序列。

最后再判断答案是否合法（指图是否不连通，图只要连通则必定合法），若合法，该序列倒序即为欧拉路径。

无论是否为欧拉回路均可以找到。

由于选择了一条边之后便将其删去，所以时间复杂度是 $O(|E|)$ 。

## 参考代码

{% fold 细节比较多，详见代码。 %}
```cpp
#include<bits/stdc++.h>
namespace my_std{
	using namespace std;
	#define reg register
	#define Rint register int
	#define FOR(i,a,b) for(register int i=(a),ed_##i=(b);i<=ed_##i;++i)
	#define ROF(i,a,b) for(register int i=(a),ed_##i=(b);i>=ed_##i;--i)
	#define FORit(templ,arr,i,a,b) for(register templ *i=(arr)+(a),*ed_##i=(arr)+(b)+1;i!=ed_##i;++i)
	#define ROFit(templ,arr,i,a,b) for(register templ *i=(arr)+(a),*ed_##i=(arr)+(b)-1;i!=ed_##i;--i)
	#define GO(x,p,e,i,v) for(register int i=p[x].head,v;i;i=e[i].link)
	#define MEM(x,v) memset(x,v,sizeof(x))
	#define fir first
	#define sec second
	#define pq priority_queue
	#define MP make_pair
	typedef long long LL;
	typedef double DB;
	typedef pair<int,int> PII;
	#define Templ(T) template<typename T>
	inline int read(){
		int ans=0,f=1;char c=getchar();
		while(!isdigit(c)){ f^=(c=='-'); c=getchar(); }
		for(;isdigit(c);c=getchar()) ans=(ans<<1)+(ans<<3)+(c^48); return f?ans:-ans;
	}
	Templ(_Tp) inline int chkmin(_Tp &x,_Tp y){ return x>y?x=y,1:0; }
	Templ(_Tp) inline int chkmax(_Tp &x,_Tp y){ return x<y?x=y,1:0; }
	#define using_mod
	const int mod = 998244353, N = 100010;
	#ifdef using_mod
	inline void inc(int &x,const int &y){ x+=y; if(x>=mod) x-=mod; }
	inline int ksm(int x,LL y){ int res=1; for(;y;y>>=1,x=1ll*x*x%mod) if(y&1) res=1ll*res*x%mod; return res;}
	#endif
	Templ(_Tp) inline _Tp gcd(_Tp x,_Tp y){ return y?gcd(y,x%y):x; }
	#define FILE(s) freopen(s".in","r",stdin),freopen(s".out","w",stdout)
	#define PBTXDY
}
using namespace my_std;

int n, m, ecnt, stk[N << 2], stop;

struct Edge{
	int link, to, id, vis;
}e[N << 2];
struct Vertice{
	int head, ind, outd, vis;
}p[N];
inline void add_edge(int u, int v, int i){
	e[++ecnt] = (Edge){ p[u].head, v, i };
	p[u].head = ecnt, ++p[u].outd, ++p[v].ind;
}

inline int Output_path(){
	if(stop < m) return 0;
	puts("YES");
	ROF(i, stop, 1) printf("%d ", stk[i]);
	return 1;
}

inline void dfs_directed_graph(int u){//做有向图
	for(Rint &i = p[u].head, v, real_i; i; i = e[i].link){//&i 是为了保证这条边不会再被遍历到
		real_i = i, v = e[real_i].to;
		if(!e[real_i].vis){
			if(!p[v].vis) p[v].vis = 1;
			e[real_i].vis = 1;
			dfs_directed_graph(v);
			stk[++stop] = e[real_i].id;
		}
	}
	return;
}

inline void dfs_undirected_graph(int u){//做无向图
	for(Rint &i = p[u].head, v, real_i; i; i = e[i].link){
		real_i = i, v = e[real_i].to;
		if(!e[(real_i + 1) >> 1].vis){
			if(!p[v].vis) p[v].vis = 1;
			e[(real_i + 1) >> 1].vis = 1;
			dfs_undirected_graph(v);
			stk[++stop] = e[real_i].id;
		}
	}
	return;
}

int work_directed_graph(){
	n = read(), m = read();
	Rint u, v;
	FOR(i, 1, m) u = read(), v = read(), add_edge(u, v, i);
	FOR(i, 1, n) if(p[i].ind != p[i].outd) return puts("NO"), 0;
	FOR(i, 1, n) if(!p[i].vis){
		p[i].vis = 1, dfs_directed_graph(i);
		if(Output_path()) return 0;
		stop = 0;
	}
	return puts("NO"), 0;
}

int work_undirected_graph(){
	n = read(), m = read();
	Rint u, v;
	FOR(i, 1, m) u = read(), v = read(), add_edge(u, v, i), add_edge(v, u, -i);
	FOR(i, 1, n) if(((p[i].ind + p[i].outd) >> 1) & 1) return puts("NO"), 0;
	FOR(i, 1, n) if(!p[i].vis){
		p[i].vis = 1, dfs_undirected_graph(i);
		if(Output_path()) return 0;
		stop = 0;
	}
	return puts("NO"), 0;
}

int main(){
	int test_type = read() - 1;
	return test_type ? work_directed_graph() : work_undirected_graph();
}
```
{% endfold %}

# 一些套路

哈密顿路径和欧拉路径相对，指的是遍历一个图的所有点且不重复的路径，可以不经过所有的边。

有时一个题目可以转化成哈密顿路径或欧拉路径。

然而图上找哈密顿路径是 NP 问题，所以基本是用欧拉路径做。