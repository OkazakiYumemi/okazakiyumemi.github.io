---
title: 'Old Driver Tree学习笔记'
date: 2019-11-14 14:43:04
tags: ODT
categories: 算法
top:
urlname: OldDriverTree学习笔记
---

Old Driver Tree (**ODT**)，又称珂朵莉树，是源于 lxl 出的[「CF 896C」](https://codeforces.com/contest/896/problem/C)的，对一类以推平区间作为关键操作的维护数列方法。

原理很简单：使用一棵平衡树（大多数情况下为`std::set`），每个节点存储一段权值相等的极长区间。

<!-- more -->

# 一些操作

下述时间复杂度时， $n$ 表示平衡树中的节点数量（即序列中权值相等的极长区间数量）。

## split

将某一个权值相等的区间分裂成两个，要求 $pos$ 为后一个的左端点。

在平衡树中找到跨过 $pos$ 的区间，删除并添加新区间即可。复杂度稳定 $O(\log_2{n})$ 。

## assgin

将区间 $[l,r]$ 推平。

先把 $l$ 和 $r + 1$ 进行 **split** 操作，再把 $[l,r]$ 合成一个节点。

注意要先 **split** $r+1$ 再 **split** $l$ 。

单次复杂度最坏 $O(n)$ 。

## 其他操作

在平衡树上遍历节点即可。

# 复杂度的证明

假设在数据真随机的情况下，开始时有 $n$ 个长为 $1$ 的区间，接下来有 $k$ 个区间推平操作。

考虑一次推平操作后，点 $i$ 作为一个权值相等的极长区间的结尾的可能性为:
$$1 - \dfrac{i + \binom{i - 1}{2} + \binom{n - i}{2}}{\binom{n}{2}} = 1 - \dfrac{2i(n - i - 1)}{n(n - 1)} \approx \left(\dfrac{i}{n}\right)^2 + \left(1 - \dfrac{i}{n}\right)^2$$

假设这 $k$ 次区间推平是独立的，那么 $k$ 次推平后点 $i$ 作为一个权值相等的极长区间的结尾的可能性约为 $((\frac{i}{n})^2 + (1 - \frac{i}{n})^2)^k$ 。

所以总的权值相等的极长区间的总数即约为 $\sum\limits_{i=1}^{n}((\frac{i}{n})^2 + (1 - \frac{i}{n})^2)^k\approx n\int_0^1 (x^2 + (1 - x)^2)^k \mathrm{d}x$ 。

$$\begin{aligned}
  & n\int_0^1 (x^2 + (1 - x)^2)^k \mathrm{d}x \\\\
=\ & n \int_{-1/2}^{1/2} ((\frac{1}{2} + x)^2 + (\frac{1}{2} - x)^2)^k \mathrm{d}x \\\\
=\ & n \int_{-1/2}^{1/2} (\frac{1}{2} + 2x^2)^k \mathrm{d}x \\\\
=\ & n\cdot 2^{-k} \int_{-1/2}^{1/2} (1 + 4x^2)^k \mathrm{d}x \\\\
=\ & n\cdot 2^{-k} \sum\limits_{i=0}^{k} \binom{k}{i}\cdot 4^i \int_{-1/2}^{1/2} x^{2i} \mathrm{d}x \\\\
=\ & n\cdot 2^{-k} \sum\limits_{i=0}^{k} \binom{k}{i}\cdot 4^i\cdot \frac{(1/2)^{2i+1} - (-1/2)^{2i+1}}{2i+1} \\\\
=\ & n\cdot 2^{-k} \sum\limits_{i=0}^{k} \binom{k}{i} \frac{1}{2i + 1} \\\\
\le\ & n\cdot 2^{-k} \sum\limits_{i=0}^{k} \binom{k}{i} \frac{1}{i + 1} \\\\
=\ & n\cdot 2^{-k} \cdot \frac{2^{k+1} - 1}{k + 1} \\\\
=\ & O(\frac{n}{k})
\end{aligned}$$

所以，最坏情况下 $k$ 次区间推平的复杂度是 $O(n\log_2k)$ 。

[lxl的题解](https://codeforces.com/blog/entry/56135)，[Akababa的原证明](https://codeforces.com/blog/entry/56135?#comment-398940)。另外，[Blaze](https://www.luogu.org/blog/blaze/solution-cf896c)也给出了一种证明方法。

# 代码实现及例题

## 「CF 896C」Willem, Chtholly and Seniorious

[例题](https://codeforces.com/contest/896/problem/C)。

维护长为 $n$ 的序列， $m$ 个操作，可能为区间加、区间推平、区间查询第 $k$ 小，区间查询 $(\sum_{i=l}^{r} a_i^x)\mod y$ 。

$1\le n,m\le 10^5$ ，数据随机生成。

### 思路

直接上 **ODT** 即可。

由于要区间加，记得节点开 $mutable$ 。

{% fold 代码： %}
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
//	#define using_mod
	const LL mod = 1000000007;
	#ifdef using_mod
	inline void inc(int &x,const int &y){ x+=y; if(x>=mod) x-=mod; }
	Templ(_Tp) inline _Tp ksm(_Tp x,LL y){ reg _Tp res=1; for(;y;y>>=1,x=1ll*x*x%mod) if(y&1) res=1ll*res*x%mod; return res;}
	#endif
	Templ(_Tp) inline _Tp gcd(_Tp x,_Tp y){ return y?gcd(y,x%y):x; }
	#define FILE(s) freopen(s".in","r",stdin),freopen(s".out","w",stdout)
	#define PBTXDY
}
using namespace my_std;

inline LL ksm(LL x, LL y, const LL &p){
	x %= p;
	reg LL res = 1;
	for(; y; y >>= 1, x = x * x % p) if(y & 1) res = res * x % p;
	return res;
}

int n, m;
LL seed, vmess;

inline LL rnd(){
	reg LL ret = seed;
	seed = (seed * 7 + 13) % mod;
	return ret;
}

namespace ODT{
	struct node{
		int l, r;
		mutable LL val;
		inline node(int rl = 0, int rr = -1, LL rv = 0){ l = rl, r = rr, val = rv; }
		inline int operator <(const node &t)const{ return l < t.l; }
	};
	set<node> s;
	typedef set<node>::iterator iter;
	inline iter split(int pos){
		iter it = s.lower_bound(node(pos));
		if(it != s.end() && it->l == pos) return it;
		--it;
		Rint l = it->l, r = it->r;
		reg LL v = it->val;
		s.erase(it);
		s.insert(node(l, pos - 1, v));
		return s.insert(node(pos, r, v)).first;
	}
	inline void assign(int l, int r, LL val){
		iter itr = split(r + 1), itl = split(l);
		s.erase(itl, itr);
		s.insert(node(l, r, val));
	}
	inline void add(int l, int r, LL val){
		iter itr = split(r + 1), itl = split(l);
		while(itl != itr) (itl++)->val += val;
		return;
	}
	inline LL k_th(int l, int r, int k){
		iter itr = split(r + 1), itl = split(l);
		vector< pair<LL, int> > vec;
		vec.clear();
		while(itl != itr){
			vec.push_back(MP(itl->val, itl->r - itl->l + 1));
			itl++;
		}
		sort(vec.begin(), vec.end());
		for(pair<LL, int> &i : vec){
			k -= i.sec;
			if(k <= 0) return i.fir;
		}
		return -1ll;
	}
	inline LL sum(int l, int r, LL x, LL y){
		iter itr = split(r + 1), itl = split(l);
		reg LL res = 0;
		while(itl != itr){
			res =(res + ksm(itl->val, x, y) * (itl->r - itl->l + 1)) % y;
			itl++;
		}
		return res;
	}
}
using namespace ODT;


int main(){
	n = read(), m = read(), seed = read(), vmess = read();
	FOR(i, 1, n) s.insert(node(i, i, rnd() % vmess + 1));
	Rint opt, l, r;
	reg LL x, y;
	FOR(i, 1, m){
		opt = rnd() % 4 + 1;
		l = rnd() % n + 1;
		r = rnd() % n + 1;
		if(l > r) swap(l, r);
		x = rnd() % ((opt == 3) ? (r - l + 1) : vmess) + 1;
		switch(opt){
			case 1:{
				add(l, r, x);
				break;
			}
			case 2:{
				assign(l, r, x);
				break;
			}
			case 3:{
				printf("%lld\n", k_th(l, r, x));
				break;
			}
			case 4:{
				y = rnd() % vmess + 1, printf("%lld\n", sum(l, r, x, y));
				break;
			}
			
		}
	}
	return 0;
}
```
{% endfold %}

## 「CometOJ contest#14 D」转转的数据结构题

[[CometOJ contest#14 D]](https://cometoj.com/contest/73/problem/D)。

一个长为 $m$ 的整数序列 $c$ ，初值全为 $0$ ；一个长为 $n$ 的操作序列，第 $i$ 个用三元组 $(l_i, r_i, v_i)$ 描述，代表将 $c_{l_i}, c_{l_i + 1}, c_{l_i + 2}, \cdots, c_{r_i}$ 赋值为 $v_i$ 。

$q$ 个询问，第 $i$ 个有两个参数 $x_i$ , $y_i$ ( $x_i\le y_i$ )，回答对初值全为 $0$ 的空序列 $c$ ，按顺序执行序号位于 $[x_i,y_i]$ 中的操作后， $c$ 中所有整数的和。

$1\le n,m,q\le 5\times 10^5,0\le v_i\le 2\times 10^9$ 。

### 思路

将询问按照 $y_i$ 排序。使用类似 ODT 的方法来维护 $c$ 序列。

对平衡树上的每个节点存储一个时间戳 $t$ ，即这个节点是由哪次操作产生的。发现询问 $x_i, y_i$ 即为做完前 $y_i$ 个操作后，时间戳不小于 $x_i$ 的节点的和。这可以很容易地使用树状数组维护。

将时间后移（即进行下一个修改）时修改树状数组即可。

考虑这个方法的时间复杂度。每个节点最多只会被删除 $1$ 次。而一个修改在插入 $1$ 个节点的同时最多只会分裂 $2$ 个节点，所以节点的变化次数是 $O(n)$ 的。

所以，虽然这个题的数据不随机，然而时间复杂度是稳定 $O(n\log_2{n})$ 的。

{% fold 代码写得特别丑。 %}
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
	const int mod = 998244353, N = 500010;
	#ifdef using_mod
	inline void inc(int &x,const int &y){ x+=y; if(x>=mod) x-=mod; }
	inline int ksm(int x,LL y){ int res=1; for(;y;y>>=1,x=1ll*x*x%mod) if(y&1) res=1ll*res*x%mod; return res;}
	#endif
	Templ(_Tp) inline _Tp gcd(_Tp x,_Tp y){ return y?gcd(y,x%y):x; }
	#define FILE(s) freopen(s".in","r",stdin),freopen(s".out","w",stdout)
	#define PBTXDY
}
using namespace my_std;

int n, m, q;
struct Query_Node{
	int tl, tr, id;
	inline int operator <(const Query_Node &t)const{ return tr < t.tr; }
}qry[N];
struct Cover_Node{
	int l, r, val;
}cvr[N];
struct ODT_Node{
	int l, r, val, tsp;
	inline ODT_Node(int ll = 0, int rr = 0, int vall = 0, int tspp = 0){ l = ll, r = rr, val = vall, tsp = tspp; }
	inline int operator <(const ODT_Node &t)const{ return l < t.l; }
};
struct BIT{
	LL a[N];
	inline void update(int x, LL val){ if(!x) return; for(; x <= m; x += x & (-x)) a[x] += val; return; }
	inline LL query(int x){ reg LL res = 0; for(; x; x -= x & (-x)) res += a[x]; return res; }
	inline LL queryS(int l, int r){ return query(r) - query(l - 1); }
}BT;

set<ODT_Node> odt;
typedef set<ODT_Node>::iterator ODTit;
LL ans[N];

inline ODTit split(int pos){
	ODTit it = odt.lower_bound(ODT_Node(pos, 0, 0, 0));
	if(it != odt.end() && it->l == pos) return it;
	--it;
	Rint l = it->l, r = it->r, val = it->val, tsp = it->tsp;
	odt.erase(it);
	odt.insert(ODT_Node(l, pos - 1, val, tsp));
	return odt.insert(ODT_Node(pos, r, val, tsp)).fir;
}

inline void Assign(int l, int r, int val, int tsp){
	ODTit R = split(r + 1), L = split(l);
	for(ODTit i = L; i != R; ++i) BT.update(i->tsp, -1ll * (i->val) * (i->r - i->l + 1));
	odt.erase(L, R);
	odt.insert(ODT_Node(l, r, val, tsp));
	BT.update(tsp, 1ll * val * (r - l + 1));
}

int main(){
	m = read(), n = read(), q = read();
	FORit(Cover_Node, cvr, i, 1, m) i->l = read(), i->r = read(), i->val = read();
	FORit(Query_Node, qry, i, 1, q) i->tl = read(), i->tr = read(), i->id = i - qry;
	sort(qry + 1, qry + q + 1);
	odt.insert(ODT_Node(1, n + 1, 0, 0));
	FORit(Query_Node, qry, i, 1, q){
		FOR(tsp, (i - 1)->tr + 1, i->tr) Assign(cvr[tsp].l, cvr[tsp].r, cvr[tsp].val, tsp);
		ans[i->id] = BT.queryS(i->tl, m);
	}
	FOR(i, 1, q) printf("%lld\n", ans[i]);
	return 0;
}
```
{% endfold %}