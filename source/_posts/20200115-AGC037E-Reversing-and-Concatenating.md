---
title: 「AGC037E」Reversing and Concatenating
date: 2020-01-15 10:09:42
tags:
- 字符串
- 猜结论
- AtCoder
categories: 题解
top:
urlname: AGC037E
---

## 题意简述

[[AGC 037E]](https://atcoder.jp/contests/agc037/tasks/agc037_e)

给你一个长为 $n$ 的字符串 $S$ ，你可以进行下面的操作 $k$ 次：

将 $S$ 翻转后接在 $S$ 后面得到一个长为 $2n$ 的字符串 $U$，再将 $S$ 改为 $U$ 的一个长为 $n$ 的子串。

求最后所得字典序最小的字符串。

$1\le n\le 5000, 1\le k\le 10^9$ 。

<!-- more -->

## 主要思路

设 $S$ 中最小的字符为 $c$ ， $U$ 中最长的连续 $c$ 的个数为 $mxl$ 。

若 $2^k\times mxl \ge n$ ，则我们可以每次将最长连续 $c$ 的结尾作为选择的新 $S$ 的结尾，这样最后字符串将变为 $n$ 个 $c$ 。

否则，最后字符串必定开头有 $2^k\times mxl$ 个 $c$ 。而后面的 $n - 2^k\times mxl$ 个字符为第一次选择的新 $S$ 的翻转后的 $[mxl + 1, mxl + n - 2^k\times mxl]$ 字串。

注意到 $n\le 5000$ ，暴力比对取字典序最小即可。

若 $n$ 较大，也可使用各种后缀排序的算法 $O(n\log n)$ 或 $O(n)$ 解决。

~~果然这场的难度是乱序排的……另外这题在[kenkoooo](https://kenkoooo.com/atcoder/)上的难度是 2845 ，在 AGC 的 E 来说是评分很低的。~~

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
	const int mod = 998244353;
	#ifdef using_mod
	inline void inc(int &x, const int &y){ x += y; if(x >= mod) x -= mod; }
	inline void dec(int &x, const int &y){ x -= y; if(x < 0) x += mod; }
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

const int N = 20010;
int n, K, usd[N];
char sc[N], ans[N];

int main(){
	n = read(), K = read() - 1;
	scanf("%s", sc + 1);
	reg char mn = 'z';
	Rint len = 0, mxl = 0;
	FOR(i, 1, n) sc[2 * n + 1 - i] = sc[i], chkmin(mn, sc[i]);
	ROF(i, n * 2, n + 1){
		Rint l = i;
		while(sc[l] == mn && i - l < n) --l;
		chkmax(mxl, i - l);
		i = l;
	}
	if(K >= 13 || n <= (1 << K) * mxl){
		FOR(i, 1, n) putchar(mn);
		return VSC_Local();
	}
	ROF(i, n * 2, n + 1){
		Rint l = i;
		while(sc[l] == mn && i - l < n) --l;
		if(i - l == mxl) usd[i] = 1;
	}
	len = n - (1 << K) * mxl;
	FOR(i, 1, n) ans[i] = 'z';

	function<void(int)> check = [&](int x){
		FOR(i, 1, len){
			if(sc[x - i] > ans[i]) return;
			if(sc[x - i] < ans[i]){
				FOR(j, 1, len) ans[j] = sc[x - j];
				return;
			}
		}
	};

	FOR(i, n + 1, n * 2) if(usd[i]) check(i - mxl + 1);
	FOR(i, len + 1, n) putchar(mn);
	FOR(i, 1, len) putchar(ans[i]);
	return VSC_Local();
}
```
{% endfold %}