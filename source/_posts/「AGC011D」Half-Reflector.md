---
title: 「AGC011D」Half Reflector
date: 2020-01-13 12:48:13
tags: 
- 猜结论
- 找规律
- AtCoder
categories: 题解
top:
---

## 题意简述

[[AGC 011D]](https://atcoder.jp/contests/agc011/tasks/agc011_d)

给你一个长为 $n$ 的 `AB` 字符串。假设一个球从某一个方向（左或右）到达了一个 `A` ，该位置将变为 `B` 且该球会反弹；到达了一个 `B` ，该位置将变为 `A` 且该球会穿过该位置。

现在从字符串的最左边扔进去 $k$ 个球，每个球都在上一个球已经弹出字符串之后再放入（可以证明，任何字符串都不能把球永远留在字符串中）。

求最后的字符串。

$1\le n\le 200000, 1\le k\le 10^9$ 。

<!-- more -->

## 主要思路

$k$ 这么大，说明最后的复杂度和 $k$ 没大关系。

发现如果扔进去球的时候第一个位置是 `A` ，则球从左边弹出，只改变第一个位置的值；否则，必然从右边弹出。

考虑球从某个位置的右边弹出时，原先在的位置和弹到的位置的值（注意到弹出时该位置必定为 `A`）。

- `AA` 变为 `BA` ；
- `AB` 变为 `AA` ；

发现第二个位置的值取反即为最后第一个位置的值。

加以分析，即可得若球从字符串右边弹出，新字符串为原字符串左移一位每位取反后，最后一位补上 `A` 。

这样就得到了一个 $O(n + k)$ 的做法。

由于球从字符串右边弹出后，字符串最后一位必为 `A` ，所以球从字符串右边弹出若干次后，字符串有后缀 `...ABABA` 。扔入足够多的球后，长度为偶数的字符串将变为 `BABA...BABA` 并保持稳定；长度为奇数的字符串将在 `ABAB...BABA` 与 `BBAB...BABA` 中反复。

显然，扔入两个球后至少有一个是从右边弹出的，故若 $k > 2n$ ，可以直接利用上面的结论得到答案。

这样就得到了 $O(n)$ 做法。

## 参考代码

{% fold %}
```cpp
#include<bits/stdc++.h>//AGC011D
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
		while(getchar() != '\n');
		#endif
		return 0;
	}
	#define FILE(s) freopen(s ".in", "r", stdin), freopen(s ".out", "w", stdout)
	#define PBTXDY
}
using namespace my_std;

const int N = 200010;
int n, m, a[N << 2], tag, fst;
char S[N];

int main(){
    n = read(), m = read();
	scanf("%s", S + 1);
	FOR(i, 1, n) a[i] = S[i] == 'A';
	fst = 1;
	function<void(void)> update = [&](){
		if(a[fst] ^ tag){
			a[fst] ^= 1;
			return;
		}
		tag ^= 1;
		a[fst + n] = tag ^ 1;
		fst++;
	};

	FOR(i, 1, min(m, n * 3)) update();
	if(m > n * 3){
		if(n & 1){
			Rint t = m - n * 3;
			if(t & 1) update();
			FOR(i, fst, fst + n - 1) putchar('B' - (tag ^ a[i]));
		}
		else{
			FOR(i, 1, n) putchar('A' + (i & 1));
		}
	}
	else{
		FOR(i, fst, fst + n - 1) putchar('B' - (tag ^ a[i]));
	}
	return VSC_Local();
}
```
{% endfold %}