---
title: 「CF559E」Gerald and Path
date: 2020-02-13 20:58:18
tags:
- dp
- Codeforces
categories: 题解
top:
urlname: CF559E
---

细节好多，写死人了。

## 题意简述

[[CF 559E]](https://codeforces.com/problemset/problem/559/E)

有 $n$ 个二元组 $(a_i, l_i)$ ，对于每个二元组有两个选择：将数轴上的 $[a_i - l_i, a_i]$ 染色或将 $[a_i, a_i + l_i]$ 染色。求最大化的最终被染色总长度。输入均为整数， $1\le n\le 100, 0\le a_i\le 10^8, 1\le l_i\le 10^8$ ，保证 $a_i$ 互不相同。

<!-- more -->

## 主要思路

先把二元组按 $a_i$ 排序，考虑动态规划，设 $f(x, s), x\in[1, 2n], s\in[0, 1]$ 表示「第 $x$ 个二元组向左（$s=1$）或右（$s=0$）贡献，最终染色总长度与 $(-\infty, a_i - s\times l_i]$ 的交的最大值」。

先考虑第 x 个二元组向左贡献。则 $[a_x - l_x, a_x]$ 这段区间内的所有二元组，也都会向左贡献，这会产生连锁反应。连锁反应过后，我们可以得到最右边的不被连锁反应影响的二元组 $y$ 。此时便转化为一个子问题，然而多了一个右端点的限制，但是总体区别不大。
然后考虑这个二元组往右贡献。发现其实更简单，因为不会产生连锁反应，所以不被影响的最右的二元组即为 $x - 1$ 。所以无论左右的转移代价都是常数。

有 $O(n)$ 中状态，而每种状态的转移方式不会超过 $O(n)$ 种，转移代价为常数，总复杂度 $O(n^2)$ 。

## 参考代码

{% fold "感觉可能是我有史以来写过注释比例最高的代码……" %}
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
	const int mod = 998244353;
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

const int N = 210, inf = 1145141919;

struct bulb{
	int pos, len;
	inline bool operator <(const bulb &t)const{ return pos < t.pos; }
}p[N];
int n, f[N], pre[N], rpre[N], md[N][N];
//pre[i] : 点 i 向左这段区域内最左边的点的编号; rpre[i] : 点 i 一直向左扩展最后最左的编号（相当于 pre[i] 的迭代）
//md[i][j] : [i, j] 这段区间中向左的端点最左的点

int dp(int tr){
	if(f[tr] != -1) return f[tr];
	Rint id, R;
	//id : 当前最右可贡献点; R : 当前最右允许贡献位置
	if(tr > n * 2) id = n, R = inf;
	else if(tr > n) id = pre[tr - n] - 1, R = p[tr - n].pos - p[tr - n].len;
	//「当前位置」向左，显然在定义下「当前位置」无法造成贡献，故实际当前位置为 pre[tr - n] - 1
	else id = tr - 1, R = p[tr].pos;
	//「当前位置」向右，同理「当前位置」无法造成贡献，故实际当前位置为 tr - 1
	if(!id) return f[tr] = 0;
	f[tr] = p[id].pos - (p[rpre[id]].pos - p[rpre[id]].len) + dp(rpre[id] + n);
	//考虑当前最左可贡献点 (rpre[id]) 向左
	chkmax(f[tr], min(p[id].len, R - p[id].pos) + dp(id));
	//考虑当前最右可贡献点 (id) 向右
	ROF(i, id - 1, 1){
		if(p[i].pos + p[i].len <= p[id].pos) continue;
		Rint u = md[i + 1][id];
		//令 i 右边的点向左贡献，能贡献到最左的 u
		if(p[u].pos - p[u].len > p[i].pos){
			chkmax(f[tr], min(p[i].len, R - p[i].pos) + dp(i));
			//假如点 i 右边没有点可以贡献到它的左边，直接把这个点贡献到右边
		}
		else{
			Rint v;
			if(pre[u] < i){
				v = rpre[md[pre[u]][i - 1]];
				if(p[v].pos - p[v].len > p[u].pos - p[u].len) v = u;
				//找出的点 v 比原来的点 u 还劣
			}
			else v = u;
			//找出在 i 向右时的最左可贡献点 v 并将其向左贡献
			chkmax(f[tr], min(R, p[i].pos + p[i].len) - (p[v].pos - p[v].len) + dp(v + n));
			//i 向右贡献， v 向左贡献
		}
	}
	return f[tr];
}

int main(){
	n = read();
	FOR(i, 1, n) p[i].pos = read(), p[i].len = read();
	sort(p + 1, p + n + 1);
	p[0].pos = -inf, p[n + 1].pos = inf;
	FOR(i, 1, n){
		const int L = p[i].pos - p[i].len;
		pre[i] = i;
		while(p[pre[i] - 1].pos >= L) --pre[i];
		//求 pre[i]
	}
	FOR(i, 1, n){
		Rint L = p[i].pos - p[i].len, x = i;
		FOR(j, i, n){
			if(chkmin(L, p[j].pos - p[j].len)) x = j;
			md[i][j] = x;
		}//暴力求 md[i][j] 
	}
	FOR(i, 1, n){
		Rint x = md[pre[i]][i];
		rpre[i] = (i == x) ? i : rpre[x];
		//求 rpre[i] 
	}
	MEM(f, -1);
	printf("%d\n", dp(n * 2 + 1));
	return VSC_Local();
}
```
{% endfold %}