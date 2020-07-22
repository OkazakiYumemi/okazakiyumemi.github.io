---
title: 「Ynoi2019模拟赛」Yuno loves sqrt technology I
date: 2020-02-07 17:02:03
tags:
- 分块
- Ynoi
categories: 题解
top:
urlname: Ynoi2019模拟赛-Yuno-loves-sqrt-technology-I
---

这题好像也是咕咕了很久？

[[Luogu 5046]](https://www.luogu.org/problem/P5046)

## 题意简述

强制在线的区间逆序对， $n$ 长度 $m$ 查询， $1\le n, m\le 10^5$ 。

<!-- more -->

题面还保证是排列，更好写（

## 主要思路

分块，块长为 $O(n)$ ，记录以下几个数组：
- $\langle pre_x\rangle$ ：第 $x$ 个元素到所在块的开头一段的逆序对数。
- $\langle suf_x\rangle$ ：第 $x$ 个元素到所在块的结尾一段的逆序对数。
- $\langle f_{t, x}\rangle$ ：前 $x$ 个元素，每个元素与第 $t$ 块形成的逆序对数总和。注意这里元素若在块中，则钦定可以形成的逆序对数为 $0$ 。

接下来看询问如何解决：

对于跨块的询问，可以拆分成三个区间（两边的散块和中间的一堆整块）。
- 对于两边零散块内部的贡献，处理了 $\langle pre_x \rangle, \langle suf_x \rangle$ ，可以 $O(1)$ 。
- 对于整块内部的贡献，对于每个块求一下与后面的整块形成的逆序对数，这可以通过查 $\langle f_{t, x}\rangle$ 得到；该块内部的贡献和零散块的贡献方法相同。所以单次复杂度 $O(\sqrt{n})$ 。
- 对于零散与中间的整块形成的逆序对，和整块内部贡献类似，查 $\langle f_{t, x}\rangle$ 的表即可 $O(\sqrt{n})$。
- 对于两个零散部分之间形成的逆序对，先对每个块内部排好序，查询时 $O(\sqrt{n})$ 将需要的区间拎出来，然后用双指针可以 $O(\sqrt{n})$ 求出逆序对数。

对于在块内的询问，可以转化为该块的两个前缀的差。当然不能直接 $pre_r - pre_{l - 1}$ ，还要用双指针把这两个段之间产生的贡献减掉。

所以单次查询复杂度就是 $O(\sqrt{n})$ ，可以通过此题。

## 参考代码

{% fold "写了个 fread 结果负优化，所以删了。" %}
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
	inline LL readLL(){
		reg LL ans=0,f=1; reg char c=getchar();
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

const int N = 100010, BN = 320, blo = 317;
struct BIT{
	int b[N];
	inline void upd(Rint x, Rint v){
		for(; x < N; x += x & (-x)) b[x] += v;
	}
	inline int qry(Rint x){
		Rint res = 0;
		for(; x; x -= x & (-x)) res += b[x];
		return res;
	}
}BT;

int n, m, bcnt, a[N], sv[N], sd[N];//array, sorted_val, sorted_id
int bl[N], Blo_L[BN], Blo_R[BN];
int suf[N], pre[N], f[BN][N];

int sl[BN], sr[BN], tl, tr;
inline int merge(){
	Rint res = 0, el = 1, er = 1;
	while(el <= tl && er <= tr){
		sl[el] < sr[er] ? (res += er - 1, ++ el) : (++ er);
	}
	while(el <= tl) res += tr, ++ el;
	tl = tr = 0;
	return res;
}

struct myP{
	int val, id;
	inline myP(int vall = 0, int idd = 0){ val = vall, id = idd; }
	inline int operator < (const myP &d)const{ return val < d.val; }
} ary[N];

inline LL query(const int &l, const int &r){
	if(l > r || r > n || l <= 0) return 0;
	const int lb = bl[l], rb = bl[r];
	if(lb == rb){
		const int L = Blo_L[lb], R = Blo_R[lb];
		if(l == L) return pre[r];
		if(r == R) return suf[l];
		FOR(i, L, R){
			if(l <= sd[i] && sd[i] <= r) sr[++ tr] = sv[i];
			else if(sd[i] < l) sl[++ tl] = sv[i];
		}
		return pre[r] - pre[l - 1] - merge();
	}
	reg LL res = pre[r] + suf[l];
	FOR(i, lb + 1, rb - 1){
		res += (f[i][r] - f[i][Blo_R[i]]) + pre[Blo_R[i]];
		res += (f[i][Blo_R[lb]] - f[i][l - 1]);
	}
	FOR(i, Blo_L[lb], Blo_R[lb]){
		if(sd[i] >= l) sl[++ tl] = sv[i];
	}
	FOR(i, Blo_L[rb], Blo_R[rb]){
		if(sd[i] <= r) sr[++ tr] = sv[i];
	}
	return res + merge();
}

int main(){
	n = read(), m = read();
	FOR(i, 1, n) a[i] = read(), ary[i] = myP(a[i], i);
	bcnt = (n - 1) / blo + 1;
	FOR(i, 1, bcnt){
		Rint L = Blo_L[i] = Blo_R[i - 1] + 1, R = Blo_R[i] = i == bcnt ? n : i * blo, res = 0;
		sort(ary + L, ary + R + 1);
		FOR(j, L, R){
			bl[j] = i, sv[j] = ary[j].val, sd[j] = ary[j].id;
			BT.upd(a[j], 1);
			res += j - L + 1 - BT.qry(a[j]);
			pre[j] = res;
		}
		FOR(j, L, R){
			suf[j] = res;
			BT.upd(a[j], -1);
			res -= BT.qry(a[j] - 1);
		}
	}
	sort(ary + 1, ary + n + 1);
	FOR(i, 1, bcnt){
		const int L = Blo_L[i], R = Blo_R[i];
		Rint it = L, x, v;
		FOR(j, 1, n){
			x = ary[j].id, v = ary[j].val;
			while(it <= R && v > sv[it]) ++it;
			if(x < L) f[i][x] = it - L;
			else if(x > R) f[i][x] = R - it + 1;
		}
		FOR(j, 2, n) f[i][j] += f[i][j - 1];
	}
	//
	reg LL ans = 0, ll, rr;
	while(m --){
		ll = readLL(), rr = readLL();
		printf("%lld\n", ans = query(int(ll ^ ans), int(rr ^ ans)));
	}
	return VSC_Local();
}
```
{% endfold %}