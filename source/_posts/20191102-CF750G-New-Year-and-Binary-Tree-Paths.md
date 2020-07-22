---
title: 「CF750G」New Year and Binary Tree Paths
date: 2019-11-02 15:28:39
tags:
- 猜结论
- dp
- Codeforces
categories: 题解
top:
urlname: CF750G
---

这题听了几遍还没听懂，结果对着[ nantf 的代码](https://codeforces.com/problemset/submission/750/63967459)爆抄才大概懂了这题在搞什么。

[[CF 750G]](https://codeforces.com/problemset/problem/750/G)

## 题意简述

一棵无限的满二叉树，根节点为 $1$ ，节点 $x$ 的左儿子是 $2x$ ，右儿子是 $2x+1$ 。给定一个正整数 $n$ ，询问这棵满二叉树上有多少条简单链，使得链上所有点之和，即链的权值为 $n$ 。 $1\le n\le 10^{15}$ 。

<!-- more -->

## 主要思路

发现可以将满足条件的链分为 $\text{LCA}$ 是链的端点的（单链）和不是的（双链），分别考虑。

### 单链

考虑从 $x$ 往左儿子走 $h$ 步。此时链权值为 $x(2^h - 1)$ 。

若倒数第 $i$ 步走了右儿子，权值将增加 $2^i - 1$ 。

所以 $x$ 往下形成的长为 $h$ 的链中，设倒数第 $i,i\in T$ 步走向了右儿子，这样一条单链就可以表示成 $(h, T)$ 的形式。

则最后链的权值为：
$$x(2^h - 1) + \sum\limits_{i\in T} 2^i - |T|$$

令 $L=\lfloor\dfrac{n}{2^h - 1}\rfloor$ ，显然有 $|T|< h \le \log_2(n+1) , x \le L$ 。

又 $(L - 1)(2^h - 1) + \sum\limits_{i\in T}2^i - |T| \le n - (2^h - 1) + (2^h - h - 1) = n - h < n$ ，所以 $x > L - 1$ ，即对于给定的 $h$ 有唯一的 $x=L$ ，并且方案数就是关于集合 $T$ 的方程 $L(2^h - 1) + \sum\limits_{i\in T} 2^i - |T| = n$ 的解的个数。发现这个方程最多只有一组解。

### 双链

从 $x$ 的左右儿子出发两条单链，类似上面设这两条单链表示为 $(h_0, T_0), (h_1, T_1)$ 。

此时，这条 $\text{LCA}$ 为 $x$ 的双链的权值和为：
$$\begin{aligned}
&x + 2x(2^{h_0} - 1) + (2x+1)(2^{h_1} - 1) +\sum\limits_{i\in T_0} 2^i +\sum\limits_{i\in T_1}2^i-|T_0|-|T_1|
\\\\&=x(2^{h_0 + 1} + 2^{h_1 + 1} - 3) + 2^{h_1} - 1 + \sum\limits_{i\in T_0} 2^i +\sum\limits_{i\in T_1} 2^i-|T_0|-|T_1|=n
\end{aligned}$$

类似单链时的推论，发现 $h_0,h_1$ 确定时有唯一的 $x=\lfloor\dfrac{n-2^{h_1}+1}{2^{h_0+1}+2^{h_1+1}-3}\rfloor$ 。

此时，问题转化为了在 $2^1,2^2,\dots,2^{h_0-1},2^1,2^2,\dots,2^{h_1-1}$ 中一共选取 $cnt$ 个数，和为 $n - x(2^{h_0 + 1} + 2^{h_1 + 1} - 3) - 2^{h_1} + 1 + cnt$ 的方案数。

考虑枚举 $cnt$ ，设 $f(i,j,0/1)$ 表示枚举完前 $i$ 个指数 ( $2^1,2^2,\dots,2^i$ ) 后，已经选了 $j$ 个数字，二进制下 $i+1$ 是否进位的方案数。 $f(0,0,0) = 1$ 。 dp 过程大概就是枚举两边的单链走左儿子还是右儿子，然后保证上一位和需要凑出的和相同之类的，详见代码。

在枚举了 $h_0,h_1,cnt$ 的情况下，答案就是 $f(max(h_0,h_1),cnt,0)$ 。

可以得出时间复杂度是 $O(\log_2^5n)$ 。

## 参考代码

{% fold 基本爆抄的代码： %}
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
	inline LL read(){
		reg LL ans=0,f=1;reg char c=getchar();
		while(!isdigit(c)){ f^=(c=='-'); c=getchar(); }
		for(;isdigit(c);c=getchar()) ans=ans*10+c-'0'; return f?ans:-ans;
	}
	Templ(_Tp) inline int chkmin(_Tp &x,_Tp y){ return x>y?x=y,1:0; }
	Templ(_Tp) inline int chkmax(_Tp &x,_Tp y){ return x<y?x=y,1:0; }
	#define using_mod
	const int mod = 998244353, MX = 50;
	#ifdef using_mod
	inline void inc(int &x,const int &y){ x+=y; if(x>=mod) x-=mod; }
	inline int ksm(int x,LL y){ int res=1; for(;y;y>>=1,x=1ll*x*x%mod) if(y&1) res=1ll*res*x%mod; return res;}
	#endif
	Templ(_Tp) inline _Tp gcd(_Tp x,_Tp y){ return y?gcd(y,x%y):x; }
	#define FILE(s) freopen(s".in","r",stdin),freopen(s".out","w",stdout)
	#define PBTXDY
}
using namespace my_std;

LL n, f[60][120][2];
//inline int count1(LL x){ return x ? count1(x & (x - 1)) + 1 : 0; }
#define count1(x) (__builtin_popcountll(x))

inline LL work1(){
	reg LL ans = 0, x, res;
	FOR(i, 1, MX){
		x = n / ((1ll << i) - 1);
		if(!x) break;
		res = n - x * ((1ll << i) - 1);
		FOR(j, 0, i - 1) if( !((res + j) & 1) && count1(res + j) == j) ++ans;
	}
	return ans;
}

inline LL work2(){
	reg LL ans = 0, x, res;
	FOR(l, 2, MX) FOR(r, 2, MX){
		x = (n - (1ll << (r - 1)) + 1) / ((1ll << r) + (1ll << l) - 3);
		if(!x) break;
		res = n - (1ll << (r - 1)) + 1 - x * ((1ll << r) + (1ll << l) - 3);
		FOR(cnt, 0, l + r - 4) if(!((res + cnt) & 1)){
			FOR(i, 0, max(l, r)) FOR(j, 0, min(cnt, i << 1)) f[i][j][0] = f[i][j][1] = 0;
			f[0][0][0] = 1;
			FOR(i, 1, max(l, r)) FOR(j, 0, min(cnt, (i - 1) << 1)){
				FOR(k, 0, 1) FOR(a, 0, 1) FOR(b, 0, 1){
					if(i >= l - 1 && a) continue;
					if(i >= r - 1 && b) continue;
					if( ((k + a + b) & 1) == (((res + cnt) >> i) & 1) ){
						f[i][j + a + b][(k + a + b) >> 1] += f[i - 1][j][k];
					}
				}
			}
			ans += f[max(l, r)][cnt][0];
		}
	}
	return ans;
}

int main(){
	n = read();
	return printf("%lld\n", work1() + work2()), 0;
}
```
{% endfold %}

## 参考资料

[nosta 的 blog](https://www.cnblogs.com/nosta/p/10829973.html)

[Kelin 的 blog](https://www.luogu.org/blog/Kelin/solution-cf750g)

[nantf 的代码](https://codeforces.com/problemset/submission/750/63967459)