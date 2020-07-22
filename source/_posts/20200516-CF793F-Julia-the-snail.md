---
title: 「CF793F」Julia the snail
date: 2020-05-16 16:12:35
categories: 题解
tags:
- 数据结构
- 线段树
- 莫队
- Codeforces
top:
urlname: CF793F
---

《关于本弱智为了减小常数而手动讨论导致写错调到死这件事》

[[CF 793F]](https://codeforces.com/problemset/problem/793/F)

## 题意简述

有一只蜗牛在树干上爬，有两种移动方式，沿着某根绳子向上爬，或者顺着树干往下溜。

树干高度为 $n$，有 $m$ 根绳子，第 $i$ 条连接了高度 $l_i$ 至 $r_i$，保证 $r_i$ 互不相同。

有 $q$ 次询问，每次给出两个数 $L, R$ ，问蜗牛从高度 $L$ 开始爬，只考虑被包含在 $[L, R]$ 间的绳子（即若一条绳子的区间超出询问范围即不能使用），蜗牛能够爬到的最大高度。

$1 \le n, m, q \le 10^5$ ，输入均为正整数。

<!-- more -->

## 主要思路

### $O(n\sqrt{n})$ 做法

假设我们要处理右端点为 $R_0$ 的所有询问，则可以将右端点不超过 $R_0$ 的绳子按左端点降序排序，依次插入，用单调栈维护当前被绳子覆盖的区间，在 $O(n)$ 时间内解决。

则考虑回滚莫队，块长为 $\sqrt{n}$ 。我们现在试图一次处理完右端点在某个块 $b$ 中的所有询问。定义该块前最右点为 $R_0$，右端点落在该块中的绳子称为「该块中的绳子」，落在该块前的称为「已处理的绳子」。

显然对于一个从 $x$ 出发的询问，必定先沿着已处理的绳子走到最右端后，再试图沿着该块中的绳子走。所以将该块中的绳子按左端点升序排序后，能走就走即可。 

### $O(n\log{n})$ 做法

考虑按右端点从左到右地处理每个询问。设当前处理到的右端点为 $R_0$ ，当前从 $x$ 出发的询问的答案为 $a_x$ 。

假设右端点右移一格，加入一条绳子 $[L_0, R_0 + 1]$。对于 $x > L_0$，这条绳子因不在询问区间内而不会产生任何影响。对于 $x \le L_0$，若 $a_x < L_0$ ，则因本来就无法碰到该线段所在位置而不会产生任何影响；而对于 $a_x \ge L_0$，有 $a_x\gets R_0 + 1$。

形式化地，相当于维护序列 $\langle a\rangle$，有两个操作：
- 将区间中不小于 $x$ 的数设为 $y$；
- 单点求值。

这不是 jiry_2 老师的线段树么。

{% fold %}
大概就是对每个节点记录最大值与**严格**次大值 $fir, sec$，和一个二元的 tag $(x, y)$，即将不小于 $x$ 的数改为 $y$。

下传标记操作的时候就将儿子的 $y$ 改成自己的 $y$（由于绳子按右端点升序排序，这样显然更优），而在 $x$ 已有值时应改为较小值。

~~然后本人因为上传节点操作试图卡常写挂了~~
{% endfold %}

于是 $O(n\log n)$ 了。（复杂度证明详见 jls 原课件）

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
	// #define MP make_pair
	typedef long long LL;
	typedef double DB;
	// typedef pair<int,int> PII;
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
	#define FILE(s) freopen(s ".in", "r", stdin), freopen(s ".out", "w", stdout)
	#define PBTXDY
}
using namespace my_std;

struct my_pair{
	int first, second;
	inline my_pair(int f = 0, int s = 0){ first = f, second = s; }
};
#define MP(a, b) my_pair(a, b)

const int N = 100010, inf = 1145141919;
int n, m, q;
int lft[N], ans[N];
vector<my_pair> Q[N];
my_pair a[N << 2], tag[N << 2];

inline void merge_node(int x, int y){
	reg my_pair res = a[y];
	if(res.fir > a[x].fir){
		a[x].sec = a[x].fir, a[x].fir = res.fir;
		chkmax(a[x].sec, res.sec);
	}
	else if(res.fir < a[x].fir){
		chkmax(a[x].sec, res.fir);
	}
	else{
		chkmax(a[x].sec, res.sec);
	}
}
inline void merge_tag(int x, int y){
	reg my_pair res = tag[y];
	if(a[x].fir >= res.sec){
		a[x].fir = res.fir;
		if(!tag[x].sec) tag[x].sec = res.sec;
		else chkmin(tag[x].sec, res.sec);
		tag[x].fir = res.fir;
	}
}
inline void push_up(int t){
	a[t] = a[t << 1], merge_node(t, t << 1 | 1);
}
inline void push_down(int t){
	if(tag[t].fir){
		merge_tag(t << 1, t), merge_tag(t << 1 | 1, t);
		tag[t] = MP(0, 0);
	}
	return;
}

void build(int t, int l, int r){
	if(l == r) return a[t] = my_pair(l, -inf), void();
	Rint mid = (l + r) >> 1;
	build(t << 1, l, mid), build(t << 1 | 1, mid + 1, r);
	return push_up(t);
}

void update(int t, int l, int r, int L, int R, my_pair x){
	if(a[t].fir < x.sec) return;
	if(L <= l && r <= R && a[t].sec < x.sec) return *tag = x, merge_tag(t, 0);
	push_down(t);
	Rint mid = (l + r) >> 1;
	if(L <= mid) update(t << 1, l, mid, L, R, x);
	if(R > mid) update(t << 1 | 1, mid + 1, r, L, R, x);
	return push_up(t);
}
int query(int t, int l, int r, int x){
	if(l == r) return a[t].fir;
	push_down(t);
	Rint mid = (l + r) >> 1;
	return x <= mid ? query(t << 1, l, mid, x) : query(t << 1 | 1, mid + 1, r, x);
}

int main(){
	n = read();
	m = read();
	Rint l, r;
	FOR(i, 1, m){
		l = read(), r = read();
		lft[r] = l;
	}
	q = read();
	FOR(i, 1, q){
		l = read(), r = read();
		Q[r].push_back(MP(l, i));
	}
	build(1, 1, n);
	FOR(i, 1, n){
		if(lft[i]) update(1, 1, n, 1, lft[i], MP(i, lft[i]));
		for(my_pair &x : Q[i]){
			ans[x.sec] = query(1, 1, n, x.fir);
		}
	}
	FOR(i, 1, q) printf("%d\n", ans[i]);
	return 0;
}
```
{% endfold %}

## 参考资料

Segment Tree Beats!

[ZigZagK 的博客](https://zigzagk.top/2018/10/03/CF793F)