---
title: 「CF1266F」Almost Same Distance
date: 2019-12-19 14:01:23
tags:
- dfs类
- 树论
- Codeforces
categories: 题解
top:
urlname: CF1266F
---

<a href="https://codeforces.com/profile/nantf" style="color:#FF8C00; font-weight:bold; text-decoration:none">nantf</a> ~~由于生病~~窝在家里打 Global Round 6 然后 rating 超过 <a href="https://codeforces.com/profile/p_b_p_b" style="color:#FF8C00; font-weight:bold; text-decoration:none">p\_b\_p\_b</a> 了……

{% asset_img ntfnb.png "ntfnb!" %}

然后他场上做了 A \- E。

## 题意简述

[[CF 1266F]](https://codeforces.com/problemset/problem/1266/F)

给定一棵树大小为 $n$ 。定义一个点集 $S$ 是 $k$ 均匀的，当且仅当对于任意 $u,v\in S, u\ne v$  ，有 $\operatorname{dist}(u, v) = k$ 或 $\operatorname{dist}(u, v) = k + 1$ ，其中 $\operatorname{dist}(u, v)$ 表示 $u$ 与 $v$ 在树上的距离，即它们的简单路径上的边数。

现在希望你对于 $1\le k\le n$ ，求出 $k$ 均匀点集点数的最大值。$2 \le n \le 5\times 10^5$ 。

<!-- more -->

## 主要思路

题解讲得很迷，然后看了<a href="https://codeforces.com/profile/300iq" style="color:#FF0000; font-weight:bold; text-decoration:none"><span style="color:#000000">3</span>00iq</a>的代码才明白的。

设 $ans[k]$ 为 $k$ 均匀点集点数的最大值。

显然我们可以 dfs 求出最长链的长度 $mxl$ ， $k > mxl$ 则 $ans[k] = 1$ ， $k \le mxl$ 则 $ans[k] \ge 2$ 。

称一棵有根树的最深深度为根节点到所有叶子节点的距离最大值。

- 对于 $k = 2l + 1$ ($l > 1$) ，最大 $k$ 均匀集必定以一个点 $r$ 为“中心”（该点不在均匀集中），而所有均匀集中的点 $x$ 距离 $r$ 为 $l + 1$ 或 $l$ ，且距离为 $l$ 的点的数量不超过 $1$ 个。
- 对于 $k = 2l$ ($l > 1$) ，也类似，不过较为复杂。可能是以一个点 $r$ 为“中心”，此时所有均匀集中的点 $x$ 距离 $r$ 为 $l$ ；也可能是以一条边 $(s, t)$ 为“中心”，此时所有均匀集中的点 $x$ 有 $\min(\operatorname{dist}(x, s), \operatorname{dist}(x, t)) = l$ 。

由上，可得 $ans[k] \ge ans[k + 2]$ 。

对于 $k$ 为奇数，或 $k$ 为偶数的第一种情况，我们可以对每个点 $u$ ，求出若这棵树以这个点为根，它每个儿子的子树最深深度，并降序排序存入数组 $ret$ 中。

这样，我们就可以 $O(n\log_2 n)$ 地获得上述情况的答案。

这部分非常好写，但是对于 $k$ 为偶数且以一条边为“中心”的情况无法处理。

对于这种情况，我们对每个点 $u$ ，枚举其儿子 $v$ 。然后对于 $v$ 也求出它每个儿子的子树最深深度（不包含 $u$ ），并降序存入数组 $tmp$ 中。

然后对于每个 $ret[i], tmp[i]$ ，用类似上述的方法来更新。

这里每个节点会被访问两次，所以总的时间复杂度还是 $O(n\log_2 n)$ 。

## 参考代码

上面的东西都可以两次 dfs 搞定。

{% fold 写得丑了点不要介意。 %}
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
	const int mod = 998244353, N = 500010;
	#ifdef using_mod
	inline void inc(int &x,const int &y){ x+=y; if(x>=mod) x-=mod; }
	inline int ksm(int x,LL y){ int res=1; for(;y;y>>=1,x=1ll*x*x%mod) if(y&1) res=1ll*res*x%mod; return res;}
	#endif
	Templ(_Tp) inline _Tp gcd(_Tp x,_Tp y){ return y?gcd(y,x%y):x; }
	inline int VSC_Local(){
		#ifdef VSC_Compile
		while(getchar() != '\n');
		#endif
		return 0;
	}
	#define FILE(s) freopen(s ".in", "r", stdin), freopen(s ".out", "w", stdout)
	#define PBTXDY
}
using namespace my_std;
 
#define swap(a,b) (a ^= b ^= a ^= b)
 
int n, dep[N], mxl, ans[N << 2], anss[N << 2];
vector<int> g[N];
 
struct BIT{
	int a[N];
	inline void update(int x, int v){ for(; x >= 0; x = (x & (x + 1)) - 1) a[x] += v; }
	inline int query(int x){ Rint res = 0; for(; x <= n; x = (x | (x + 1))) res += a[x]; return res; }
}BT;
 
void dfs(int u, int fth){
	dep[u] = 0;
	for(int v : g[u]){
		if(v != fth){
			dfs(v, u);
			chkmax(dep[u], dep[v] + 1);
		}
	}
	return;
}
 
void solve(int u, int fth, int upx){
	vector<int> ret{upx, 0};
	for(int v : g[u]){
		if(v != fth) ret.push_back(dep[v] + 1);
	}
	sort(ret.rbegin(), ret.rend());
 
	Rint fr = ret[0], sc = ret[1], siz = (int)ret.size();
	chkmax(mxl, fr + sc);
	FOR(i, 1, siz - 1){
		chkmax(ans[2 * ret[i] + (ret[i] < ret[i - 1])], i + 1);
	}
	
	for(int v : g[u]){
		if(v == fth) continue;
		Rint tmp = (dep[v] + 1 == fr ? sc : fr);
		solve(v, u, tmp + 1);
	}
	vector<int> vec(siz);
 
	for(int v : g[u]){
		if(v == fth) continue;
		vector<int> tmp;
 
		for(int x : g[v]){
			if(x != u) tmp.push_back(dep[x] + 1);
		}
		sort(tmp.rbegin(), tmp.rend());
 
		FOR(i, 0, (int)tmp.size() - 1){
			Rint l = 0, r = siz, mid;
			while(l < r - 1){
				mid = (l + r) >> 1;
				if(ret[mid] <= tmp[i]) r = mid;
				else l = mid;
			}
			if(r != siz){
				chkmax(vec[r], i + 1);
                chkmax(anss[2 * tmp[i]], i + l + 1);
			}
		}
	}
 
	FOR(i, 1, siz - 1){
		chkmax(vec[i], vec[i - 1]);
	}
	FOR(i, 0, siz - 1){
		chkmax(anss[2 * ret[i]], i + vec[i]);
	}
	return;
}
 
int main(){
	n = read();
	Rint u, v;
	FOR(i, 1, n - 1){
		u = read() - 1, v = read() - 1;
		g[u].push_back(v), g[v].push_back(u);
	}
 
	dfs(0, -1);
	solve(0, -1, 0);
 
	ROF(i, n - 1, 1) chkmax(ans[i], ans[i + 1]);
	ROF(i, n - 2, 1) chkmax(anss[i], anss[i + 2]);
	FOR(i, 1, n){
		chkmax(ans[i], anss[i]);
		chkmax(ans[i], 1 + (i <= mxl));
	}
 
	FOR(i, 1, n) printf("%d ", ans[i]);
	puts("");
	return VSC_Local();
}
			dfs(v, u);
			chkmax(dep[u], dep[v] + 1);
		}
	}
	return;
}

void solve(int u, int fth, int upx){
	vector<int> ret{upx, 0};
	for(int v : g[u]){
		if(v != fth) ret.push_back(dep[v] + 1);
	}
	sort(ret.rbegin(), ret.rend());

	Rint fr = ret[0], sc = ret[1], siz = (int)ret.size();
	chkmax(mxl, fr + sc);
	FOR(i, 1, siz - 1){
		chkmax(ans[2 * ret[i] + (ret[i] < ret[i - 1])], i + 1);
	}
	
	for(int v : g[u]){
		if(v == fth) continue;
		Rint tmp = (dep[v] + 1 == fr ? sc : fr);
		solve(v, u, tmp + 1);
	}
	for(int x : ret) BT.update(x, 1);
	vector<int> vec(siz);

	for(int v : g[u]){
		if(v == fth) continue;
		BT.update(dep[v] + 1, -1);
		vector<int> tmp;

		for(int x : g[v]){
			if(x != u) tmp.push_back(dep[x] + 1);
		}
		sort(tmp.rbegin(), tmp.rend());

		FOR(i, 0, (int)tmp.size() - 1){
			Rint l = 0, r = siz, mid;
			while(l < r - 1){
				mid = (l + r) >> 1;
				if(ret[mid] <= tmp[i]) r = mid;
				else l = mid;
			}
			if(r != siz){
				chkmax(vec[r], i + 1);
			}
		}

		FOR(i, 0, (int)tmp.size() - 1){
			chkmax(anss[2 * tmp[i]], i + 1 + BT.query(tmp[i]));
		}
		BT.update(dep[v] + 1, 1);
	}

	FOR(i, 1, siz - 1){
		chkmax(vec[i], vec[i - 1]);
	}
	FOR(i, 0, siz - 1){
		chkmax(anss[2 * ret[i]], i + vec[i]);
	}

	for(int x : ret) BT.update(x, -1);
	return;
}

int main(){
	n = read();
	Rint u, v;
	FOR(i, 1, n - 1){
		u = read() - 1, v = read() - 1;
		g[u].push_back(v), g[v].push_back(u);
	}

	dfs(0, -1);
	solve(0, -1, 0);

	ROF(i, n - 1, 1) chkmax(ans[i], ans[i + 1]);
	ROF(i, n - 2, 1) chkmax(anss[i], anss[i + 2]);
	FOR(i, 1, n){
		chkmax(ans[i], anss[i]);
		chkmax(ans[i], 1 + (i <= mxl));
	}

	FOR(i, 1, n) printf("%d ", ans[i]);
	puts("");
	return VSC_Local();
}
```
{% endfold %}

顺便从<span style="color:#FF0000; font-weight:bold; text-decoration:none"><span style="color:#000000">3</span>00iq</span>那里学来了一个东西。

```cpp
struct BIT{
	int a[N];
	inline void update(int x, int v){
		for(; x >= 0; x = (x & (x + 1)) - 1) a[x] += v; 
	}
	inline int query(int x){ 
		Rint res = 0;
		for(; x <= n; x = (x | (x + 1))) res += a[x];
		return res;
	}
}BT;
```

这个东西可以支持单点修改和查询后缀和。证明正确性和复杂度的方法好像和普通树状数组差不多。