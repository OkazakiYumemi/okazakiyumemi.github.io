---
title: 「CF1172D」Nauuo and Portals
date: 2019-12-11 17:57:23
tags:
- 构造
- Codeforces
categories: 题解
top:
urlname: CF1172D
---

`ouuan` 场的有意思构造题。

[[CF 1172D]](https://codeforces.com/problemset/problem/1172/D)

## 题意简述

定义**一对**传送门 $(A,A^\prime)$ 为从 $A$ 门进入会从 $A^\prime$ 门出来，且保持进入 $A$ 门的方向（从 $A^\prime$ 进去同理）。

给出一个 $n\times n$ ($1\le n\le 10^3$) 的矩形和 $2n$ 个要求，其中前 $n$ 个要求的形式为 $(r_i,n)$ ，代表 $\texttt{Nauuo}$ 从 $(i,1)$ 这个格子出发**一直向右走**，经过若干对传送门（或者不经过传送门）后从 $(r_i,n)$ 这个格子**走出**矩形；后 $n$ 个要求的形式为 $(n,c_i)$ ，代表 $\texttt{Nauuo}$ 从 $(1,i)$ 这个格子出发**一直向下走**，经过若干对传送门（或者不经过传送门）后从 $(n,c_i)$ 这个格子**走出**矩形。

请你在这个 $n\times n$ 的矩形中放置若干对传送门，使得这 $2n$ 个请求可以同时满足。输出传送门的**对数**和**每对传送门两个门各自的坐标**。

保证 $r_i, c_i$ 为 $1$ 到 $n$ 的排列。无解时输出 $-1$ 。

**注意：你不需要找到使用传送门最少的方案数，只需要让你给出的方案可以满足题目中给出的要求。**

<!-- more -->

## 主要思路

考虑 $n\times n$ 的问题如何转换成 $(n - 1)\times (n - 1)$ 。

假设从第 $i$ 行进的人要从第 $r_i$ 行出去，第 $i$ 行出去的人要是从第 $idr_i$ 行进的人。

假设从第 $i$ 列进的人要从第 $c_i$ 列出去，第 $i$ 列出去的人要是从第 $idc_i$ 列进的人。

考虑满足第 $i$ 行出去的人和第 $i$ 列出去的人。

我们可以在 $(i, idc_i), (idr_i, i)$ 的位置放置一对传送门（如果 $idc_i$ , $idr_i$ 不都为 $i$ ），并且之后不再在第 $i$ 行或第 $i$ 列放置传送门。

此时，满足了第 $idr_i$ 行进的人与第 $idc_i$ 列进的人。并且第 $i$ 行进的人一定会到达 $(idr_i, i)$ ，第 $i$ 列进的人一定会到达 $(i, idc_i)$ 。

然后更改 $idr_{r_i},r_{idr_i},idc_{c_i},c_{idc_i}$ ，继续做下一行即可。时间复杂度 $O(n)$ 。

## 参考代码

{% fold %}
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
		reg int ans=0,f=1; reg char c=getchar();
		while(!isdigit(c)) f^=(c=='-'), c=getchar();
		for(;isdigit(c);c=getchar()) ans=(ans<<1)+(ans<<3)+(c^48);
		return f?ans:-ans;
	}
	Templ(_Tp) inline int chkmin(_Tp &x,_Tp y){ return x>y?x=y,1:0; }
	Templ(_Tp) inline int chkmax(_Tp &x,_Tp y){ return x<y?x=y,1:0; }
	#define using_mod
	const int mod = 998244353, N = 1010;
	#ifdef using_mod
	inline void inc(int &x,const int &y){ x+=y; if(x>=mod) x-=mod; }
	inline int ksm(int x,LL y){ int res=1; for(;y;y>>=1,x=1ll*x*x%mod) if(y&1) res=1ll*res*x%mod; return res;}
	#endif
	Templ(_Tp) inline _Tp gcd(_Tp x,_Tp y){ return y?gcd(y,x%y):x; }
	#define FILE(s) freopen(s".in","r",stdin),freopen(s".out","w",stdout)
	#define PBTXDY
}
using namespace my_std;

int n, r[N], c[N], idr[N], idc[N];
vector<pair<PII, PII> > ans;

int main(){
	n = read();
	FOR(i, 1, n) idr[r[i] = read()] = i;
	FOR(i, 1, n) idc[c[i] = read()] = i;
	
	FOR(i, 1, n){
		if(idr[i] == i && idc[i] == i) continue;
		ans.push_back(MP(MP(i, idc[i]), MP(idr[i], i)));
		r[idr[i]] = r[i], idr[r[i]] = idr[i];
		idr[i] = i, r[i] = i;
		c[idc[i]] = c[i], idc[c[i]] = idc[i];
		idc[i] = i, c[i] = i;
	}
	
	printf("%d\n", (int)ans.size());
	for(auto &i : ans){
		printf("%d %d %d %d\n", i.fir.fir, i.fir.sec, i.sec.fir, i.sec.sec);
	}
	return 0;
}
```
{% endfold %}