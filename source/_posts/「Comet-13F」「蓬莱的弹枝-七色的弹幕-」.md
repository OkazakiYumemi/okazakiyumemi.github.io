---
title: '「Comet#13F」「蓬莱的弹枝 -七色的弹幕-」'
date: 2019-11-12 19:07:35
tags:
- 分块
- 块状链表
categories: 题解
top:
---

一道只有标题和东方有关系的大分块。

[[CometOJ contest#13 F]](https://cometoj.com/contest/72/problem/%EF%BC%A6)

## 题意简述

维护一个长为 $n$ 的序列，要求支持查询距离某个数 $a_x$ **距离最近的相等的数**与这个数的距离，区间加一，区间左移一位（即 $a_{i}\leftarrow a_{i+1} (l\le i\le r), a_r\leftarrow a_l$ ）。

$1\le n,m,a_i\le 10^5$ 。

<!-- more -->

## 主要思路

~~所以平衡树怎么做这个玩意？~~

分块，设每块长度为 $K$ 。每个块内记录：
- $beg$ ：这个块第一个数的下标减一
- $cnt$ ：这个块的长度
- $tag$ ：这个块的加标记
- $\left\langle num\right\rangle$ ：块中元素（存储的值为真实值减去 $tag$ ，存储的数对应原序列中的 $[beg + 1, beg + cnt]$ ）
- $\left\langle fix\right\rangle$ ： $fix_{val}$ 表示块内值为 $val$ （真实值为 $val + tag$ ）的数的数量

### 查询

查询时，寻找这个位置 $x$ 所在的块 $B_x$ ，先在这个块内找有没有相同数；再分别遍历 $B_x$ 后面与前面的块，找到后面与前面分别第一次出现 $a_x$ 的块（由于存储了 $\left\langle fix\right\rangle$ ，可以 $O(1)$ 判断一个块内有没有 $a_x$），在块内暴力寻找 $a_x$ 出现的位置更新答案。

这样得到三组结果，取最优即可。一次时间复杂度 $O(K + \dfrac{n}{K})$ 。

### 区间加

经典操作。散块暴力整块打 $tag$ 即可。一次时间复杂度 $O(K + \dfrac{n}{K})$ 。

### 区间左移

设区间左移 $[l,r]$ 时，最前和最后的要修改的块分别为 $B_l$ 与 $B_r$ 。

[官方题解](http://static.eduzhixin.com/cometoj/solution/contest_72.pdf)中，每个块的大小是定的。这样使用块状数组，每次左移即对于 $[B_l, B_r]$ 中的每个块，将弹出一个数字和插入一个数字，块大小仍然不变，只需处理修改块的 $beg$ 即可。发现散块是在中间插入与弹出，可以暴力处理；整块是在两端插入与弹出，可以 $O(1)$ 搞。一次时间复杂度是 $O(K + \dfrac{n}{K})$ 。

这样写比较方便，代码也比较短。

然而本人将此题当作块状链表的例题来做。即完全按照题意，每次左移将块 $B_l$ 中对应序列中下标为 $l$ 的数弹出，其他块不变，然后插入到块 $B_r$ 中对应序列中下标为 $r$ 的数后面。

由于一个块大小减少 $1$ ，一个块大小增加 $1$ ，所以当一个块大小过大 ( $cnt > 2\times K$ ) 或过小 ( $cnt \le \dfrac{K}{2}$ ) 时，将这个块**分裂成两个块**或**与后一个块合并**（若合并后 $cnt > 2\times K$ ，也不应合并，应将后一个块前端部分移至该块后端使这两个块的大小较为平衡）。

最后也能推出单次时间复杂度为 $O(K + \dfrac{n}{K})$ 。

所以 $K$ 取 $\sqrt{n}$ 时，时间复杂度取到最优为 $O(n\sqrt{n})$ 。这题不卡空间所以 $K$ 开 $\sqrt{n}$ 空间上也不会有问题。


## 参考代码

~~然而我真就把这题当作块状链表的例题来做了，各种不熟练导致调了一下午。~~

一些细节详见代码与注释。

{% fold 大量指针注意。 %}
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
	const int mod = 998244353, N = 100010, BN = 400;
	#ifdef using_mod
	inline void inc(int &x,const int &y){ x+=y; if(x>=mod) x-=mod; }
	inline int ksm(int x,LL y){ int res=1; for(;y;y>>=1,x=1ll*x*x%mod) if(y&1) res=1ll*res*x%mod; return res;}
	#endif
	Templ(_Tp) inline _Tp gcd(_Tp x,_Tp y){ return y?gcd(y,x%y):x; }
	#define FILE(s) freopen(s".in","r",stdin),freopen(s".out","w",stdout)
	#define PBTXDY
}
using namespace my_std;

int n, m, blo, a[N], dtop;

struct Block;
inline Block* new_Block();
inline void del_Block(Block *B);

struct Block{
	Block *pre, *nxt; 
	int beg, fix[N << 1], tag, cnt, num[BN << 1];
	//beg : 前面的块总共有多少个数, cnt : 块大小, tag : 加标记
	//fix : 值域(不加 tag)上每种数的数量, num : 每个数(不加 tag)
	inline void Clear(){
		pre = nxt = nullptr;
		beg = tag = cnt = 0;
		MEM(fix, 0), MEM(num, 0);
	}
	inline Block(){ Clear(); }
	inline void scatter(){//把过大的块拆成两个 
		Rint rc = cnt, *it = nullptr;
		reg Block *to = nxt, *x = new_Block();
		x->pre = this, x->nxt = to;
		nxt = x;//处理各种关系 
		if(to != nullptr) to->pre = x;
		cnt = blo, x->cnt = rc - blo;
		x->beg = beg + blo, x->tag = tag;
		it = x->num;
		FOR(i, blo + 1, rc){//把信息转到分出来的块里去 
			++it;
			*it = num[i], --fix[*it], ++(x->fix[*it]);
		}
		return;
	}
	inline void merge(){//把过小的块和下一个块合并成两个 / 将过小的块后端插入下一个块的一个前缀使该块不至于太小 
		reg Block *x = nxt, *to = nullptr;
		if(x == nullptr) return;
		if(cnt + x->cnt > (blo << 1)){//如果两个块加起来大小过大就不能直接合并(然而这样就不用处理各种关系了, 写起来其实更方便) 
			Rint *it = nullptr;
			FOR(i, 1, cnt) --fix[num[i]], num[i] += tag, ++fix[num[i]];
			tag = 0;
			it = x->num;
			FOR(i, cnt + 1, blo){//把这个块的长度补到 blo 
				++it;
				num[i] = (*it) + x->tag, ++fix[num[i]], --(x->fix[*it]);
			}
			FOR(i, 1, cnt + x->cnt - blo) ++it, x->num[i] = *it;//处理下个块的信息 
			x->cnt += cnt - blo, x->beg += blo - cnt, cnt = blo;
			return;
		}
		to = x->nxt, nxt = to;
		if(to != nullptr) to->pre = this;//处理各种关系 
		Rint rc = cnt + x->cnt, *it = x->num;
		FOR(i, 1, cnt) --fix[num[i]], num[i] += tag, ++fix[num[i]];//把 tag 去掉 
		tag = 0;
		FOR(i, cnt + 1, rc){//把原本的下一个块的信息转过来 
			++it;
			num[i] = *it + x->tag, ++fix[(*it) + x->tag];
		}
		cnt = rc;
		del_Block(x);//把原本的下一个块删了 
		return;
	}
	inline void check(){//检查块是否过大或过小 
		if(cnt > (blo << 1)) return scatter();
		if(cnt <= (blo >> 1)) return merge();
		return;
	}
	inline void print(){
		puts("Block : ");
		printf("beg : %d, cnt : %d, tag : %d\n", beg, cnt, tag);
		FOR(i, 1, cnt) printf("%d ", num[i]);
		puts("");
	}
};
Block *St = nullptr/*, *Ed = nullptr*/, *del[BN << 1];

inline Block* new_Block(){
	return dtop ? del[--dtop] : (new Block);
}
inline void del_Block(Block *B){//由于有拆块和合并块, 所以写了垃圾回收, 回收方式比较奇怪 
	B->Clear();
	del[dtop++] = B;
	return;
}

inline void Debug(){
	reg Block *now = St;
	while(now != nullptr){
		now->print();
		now = now->nxt;
	}
	return;
}

inline void Build(){
	St = new_Block();
	FOR(i, 1, blo) St->num[i] = a[i], ++(St->fix[a[i]]);
	St->beg = 0, St->cnt = blo;//先把头搞出来 
	reg Block *now = St, *to;
	FOR(i, 2, ceil(n / (DB)(blo))){//做出每个块
		to = new_Block();
		now->nxt = to, to->pre = now;
		to->beg = now->beg + now->cnt;
		to->cnt = min(n, to->beg + blo) - to->beg;
		FOR(j, to->beg + 1, to->beg + to->cnt){
			to->num[j - to->beg] = a[j];
			++(to->fix[a[j]]);
		}
		now = to;
	}
//	Ed = now;
	return;//实际上 Ed 根本不需要用... 
}

inline int query(int x){
	reg Block *now = St, *B = nullptr;
	Rint wgh, res = n + 1;
//	find block [x]
	while(now->beg + now->cnt < x) now = now->nxt;
//	query block [x]
	wgh = now->num[x - now->beg];
	if(now->fix[wgh] > 1){//处理自己的块 
		ROF(i, x - now->beg - 1, 1) if(now->num[i] == wgh){
			chkmin(res, x - now->beg - i);
			break;
		}
		FOR(i, x - now->beg + 1, now->cnt) if(now->num[i] == wgh){
			chkmin(res, i - x + now->beg);
			break;
		}
	}
	wgh += now->tag;
//	query next blocks
	B = now->nxt;
	while(B != nullptr){
		if(wgh - B->tag > 0 && B->fix[wgh - B->tag]){//找到 next blocks 中第一个有 wgh 的块, 注意可能 wgh - B->tag 不大于 0, 要判 
			FOR(i, 1, B->cnt) if(B->num[i] == wgh - B->tag){
				chkmin(res, i + B->beg - x);
				break;
			}
			break;
		}
		B = B->nxt;
	}
//	query pre blocks
	B = now->pre;
	while(B != nullptr){
		if(wgh - B->tag > 0 && B->fix[wgh - B->tag]){//找到 pre blocks 中第一个有 wgh 的块, 注意可能 wgh - B->tag 不大于 0, 要判
			ROF(i, B->cnt, 1) if(B->num[i] == wgh - B->tag){
				chkmin(res, x - i - B->beg);
				break;
			}
			break;
		}
		B = B->pre;
	}
	return res != n + 1 ? res : -1;
}

inline void update_add(int l, int r){//区间加没啥难度 
	reg Block *now = St;
	Rint *it = nullptr;
//	find block [l]
	while(now->beg + now->cnt < l) now = now->nxt;
	if(now->beg + now->cnt >= r){//if [l] and [r] in the same block
		it = now->num + l - now->beg - 1;
		FOR(i, l - now->beg, r - now->beg){
			++it;
			--(now->fix[*it]), ++(*it), ++(now->fix[*it]);
		}
		return;
	}
//	update block [l]
	it = now->num + l - now->beg - 1;
	FOR(i, l - now->beg, now->cnt){
		++it;
		--(now->fix[*it]), ++(*it), ++(now->fix[*it]);
	}
//	find block [r]
	now = now->nxt;
	while(now->beg + now->cnt < r){
		++(now->tag);
		now = now->nxt;
	} 
//	update block [r]
	it = now->num;
	FOR(i, 1, r - now->beg){
		++it;
		--(now->fix[*it]), ++(*it), ++(now->fix[*it]);
	}
	return;
}

inline void update_shift(int l, int r){
	reg Block *now = St, *x;
	Rint *it = nullptr, wgh;
//	find block [l]
	while(now->beg + now->cnt < l) now = now->nxt;
	if(now->beg + now->cnt >= r){//if [l] and [r] in the same block
		wgh = now->num[l - now->beg];
		FOR(i, l - now->beg, r - now->beg - 1) now->num[i] = now->num[i + 1];
		now->num[r - now->beg] = wgh;
		return;
	}
//	update block [l]
	wgh = now->num[l - now->beg], --(now->fix[wgh]);
	FOR(i, l - now->beg, now->cnt - 1) now->num[i] = now->num[i + 1];
	wgh += now->tag, --(now->cnt);//把一个点从这个块里抽出来所以 --(now->cnt) , 这个点已经抽出来了所以 wgh += now->tag 
	x = now;
//	find block [r]
	now = now->nxt;
	while(now->beg + now->cnt < r && now != nullptr){
		--(now->beg);//这些中间的块因为前面有块被抽了一个数所以 --(now->beg) 
		now = now->nxt;
	}
	if(now == nullptr) return;
//	update block [r]
	++(now->cnt), --(now->beg);//要插一个数进这个块里所以 ++(now->cnt)
	//由于 wgh - now->tag 可能不大于 0 所以应该把这个块的 tag 归 0 
	it = now->num;
	FOR(i, 1, r - now->beg - 1){
		++it;
		--(now->fix[*it]), *it += now->tag, ++(now->fix[*it]);
	}
	it = now->num + now->cnt;
	ROF(i, now->cnt, r - now->beg + 1){
		*it = *(it - 1);
		--(now->fix[*it]), *it += now->tag, ++(now->fix[*it]);
		--it;
	}//归 0 tag 顺便把 now->num[r - now->beg] 空出来 
	++(now->fix[wgh]), now->num[r - now->beg] = wgh;// tag 归 0 了所以直接插进去 
	now->tag = 0;
	x->check(), now->check();//因为两个块分别减少/增加了一个数所以 check 一下 
	return;
}

int main(){
	n = read(), m = read(), blo = ceil(sqrt(n));
	FOR(i, 1, n) a[i] = read();
	Build();
	Rint opt, l, r;
	while(m--){
		opt = read() - 1, l = read();
		if(opt == 2){
			printf("%d\n", query(l));
			continue;
		}
		r = read();
		opt ? update_add(l, r) : update_shift(l, r);
	}
	return 0;
}
```
{% endfold %}

所以我第一题块状链表是不是还是写个「POJ 2887」Big String 啥的合适啊……

结果为什么写了这题之后马上又拿分块链表过了[「SCOI 2013」多项式的运算](https://www.luogu.org/problem/P3278)呢……