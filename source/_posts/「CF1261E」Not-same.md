---
title: 「CF1261E」Not same
date: 2019-11-25 18:12:38
tags: 
- 构造
- Codeforces
categories: 题解
top:
---

这构造题怎么随便猜个结论就对了啊……另外这个题做法好多……

所以 **Technocup** 都是升分好场？~~等等 div 2 没这题~~

~~总有一天我不会再咕掉升分好场的！~~

[[CF 1261E]](https://codeforces.com/problemset/problem/1261/E)

## 题意简述

给你一个整数序列 $\left\langle a_n\right\rangle$ ，保证每个数都在 $[1,n]$ 之内。

你需要找出至多 $n + 1$ 个互不相同的 $1, 2, 3, \dots, n$ 的子集，使得对于任意整数 $i$ ( $1\le i\le n$ ) 在这些集合中的出现次数为 $a_i$ 。

输出任意一个答案， $1\le n\le 10^3$ 。

<!-- more -->

## 主要思路

### 本人做法

首先肯定还是先排序。下文中基于 $\left\langle a_n\right\rangle$ 为不降的序列讨论。

根据样例，先考虑 $\forall i, a_i = n$ 的情况。

在 $n = 5$ 时的一个答案：

```plain
6
11111
01111
10111
11011
11101
11110
```

这给我们一些微小的启发。

#### 初始化

现在我们规定 $g(i,j)$ 表示集合 $i$ 内是否有数 $j$ 。

考虑刚刚的情况。如果 $a_i \le i$ ，令 $g(k, i) = 1$ ( $1\le k\le a_i$ ) ；如果 $a_i > i$ ，令 $g(k, i) = 1$ ( $k\in [1,i] \cup [i+2,a_i+1]$ ) 。

例如样例二：

**Input:**
```plain
5
1 3 4 4 5
```

**g:**
```plain
11111
01111
00111
01011
00101
```

发现此时每个集合都是不同的，即已经找到了一个答案。

显然，这样的方法并不能保证最后每个集合都不同。例如样例三：

**Input:**
```plain
5
1 1 1 1 5
```

**g:**
```plain
11111
00001
00001
00001
00001
```

此时，有 $4$ 个相同的集合，**g**不是合法的答案。

#### 解决冲突

考虑如何将这些相同的集合变为不同的集合。

发现第一个集合必定是全集。下称`第一个集合`为`元集合`。

我们考虑从元集合中取出一些数放到其他集合中使得所有集合不同。

比如上面的样例三就可以变成这样：

**g(new):**
```plain
00011
00001
00101
01001
10001
```

我们将元集合中 $1, 2, 3$ 这三个数分别移出并放入第 $5, 4, 3$ 这 $3$ 个集合中。

定义 $h(i)$ 为处理前 $i$ 个数时，原本的 $g$ 中序号最大的不为空的集合的序号。

例如样例四：

**Input:**
```plain
5
2 2 3 5 5
```

**g:**
```plain
11111
01111
10111
00011
00001
00010
```

| $i$ | $1$ | $2$ | $3$ | $4$ | $5$ |
| :--: | :--: | :--: | :--: | :--: | :--: |
| $h(i)$ | $3$ | $3$ | $3$ | $6$ | $6$ |

可以看出，因为 $g(i, i + 1) = 0$ ，几个冲突的集合必定为前缀一段`0`，后缀一段`1`的形式。

而仅当 $a_i > a_{i - 1}$ 时，才有可能发生冲突。此时有相同的集合个数为 $\min(a_i, i) - h(i - 1)$ 。

相同的 $c$ 个集合可以对除第一个以外的 $c-1$ 个中加入一个数使得其互不相同。

一个显然的想法是维护一个 $p$ ，表示当前元集合中最小的数。每当出现冲突时，将该数从元集合中取出，并插入到冲突的最后一个集合中，然后将 $p$ 设为元集合现在最小的数。

这样，就得到了上文中样例三的一个答案。

然而我们发现这样会有一些问题。

例如样例五：

**Input:**
```plain
7
1 1 1 4 4 4 7
```

**g:**
```plain
1111111
0001111
0001111
0001111
0000001
0000001
0000001
```

处理第 $4$ 个数出现的冲突时没有问题，此时 **g** 变为：
```plain
0011111
0001111
0101111
1001111
0000001
0000001
0000001
```

而当处理第 $7$ 个数出现的冲突时，如果将元集合中的 $3$ 移出放入第 $7$ 个集合，则会导致第 $1$ 与第 $2$ 个集合相同。

所以，为了保证元集合不和其他集合相同，如果 $a_p < a_{p + 1}$ ，那么 $p$ 这个数就不应该被从元集合中移出。

那么还有什么元集合中的数应该保留？

例如样例五：

**Input:**
```plain
7
1 3 3 3 3 3 7
```

**g:**
```plain
1111111
0111111
0011111
0100001
0000001
0000001
0000001
```

处理第 $7$ 个数的冲突时，会导致 **g** 变为：
```plain
0011111
0111111
0011111
0100001
0000001
0100001
1000001
```

此时，第 $1$ 与 第 $3$ 个集合相同了。

所以，如果 $a_i > i$ ，数 $i$ 也不应从元集合中取出。

加上这两个处理后开码，一交过了。

#### 正确性证明

为什么这是对的？

首先我们先将初始的 **g** 表示成几个点与一条直线。

![「CF1261E」01](「CF1261E」01.png)

例如上图对应的 **g** :
```plain
1111111
0001111
0001111
0001111
0000111
0000011
0000101
0000110
```

设这条直线为 $\text{base}$ ，向右为 $x$ 轴正方向，向下为 $y$ 轴正方向。将所有在 $\text{base}$ 下方的点都向上移动一个单位，这样，第 $i$ 个点的位置即为 $(i, a_i)$ 。

![「CF1261E」02](「CF1261E」02.png)

然后把相邻两个点之间的先向右再向下两条线段连起来。这样得到一条折线，从 $(1, a_1)$ 开始，到 $(n, a_n)$ 结束。

![「CF1261E」03](「CF1261E」03.png)

考虑这条折线中所有竖直的线段。设所有跨过直线 $\text{base}$ 的线段的在 $\text{base}$ 下方的长度和为 $nS$ ，所有线段的长度和为 $S$ ，线段条数为 $cS$ （将跨过 $\text{base}$ 的线段算作两条）。例如上图中， $nS = 1, S = 6, cS = 3$ 。

则需要将 $S - cS - nS$ 个元集合中的数取出。

考虑这条折线中所有水平的线段。设所有跨过直线 $\text{base}$ 的线段的在 $\text{base}$ 下方的长度和为 $nT$ ，所有线段的长度和为 $T$ ，线段条数为 $cT$ 。例如上图中， $nT = 1, T = 6, cT = 3$ 。

则有 $T - cT - nT$ 个元集合中的数可以被取出。

现在我们要证明 $S - cS - nS \le T - cT - nT$ 。

因为折线的结束点 $(n, a_n)$ 必定不在直线 $\text{base}$ 下方，所以可以得到 $nS = nT$ 。并且易得 $T = n - 1, S = a_n - 1$ ， 所以 $S \le T$ 。

由于从 $(1, a_1)$ 出发后第一条线段必定向右，所以 $cS \le cT$ 。

最后就可以得到 $S - cS - nS \le T - cT - nT$ 。

所以这个做法是对的！

### 神仙 HZB 做法

还是先不降排序。

#### 构造方法

仍然规定 $g(i,j)$ 表示集合 $i$ 内是否有数 $j$ 。

当 $a_i \le i$ 时，令 $g(k, i) = 1$ ( $i - a_i + 1 \le k \le i$ ) ；否则令 $g(k, i) = 1$ ( $k\in [1, i] \cup [i + 2, a_i + 1]$ ) 。

例如样例二：

**Input:**
```plain
5
1 3 4 4 5
```

**g:**
```plain
11111
01111
00111
01011
00101
```

~~哎这怎么和本人的方法得到的集合一模一样啊~~

样例三：

**Input:**
```plain
5
1 1 1 1 5
```

**g:**
```plain
10001
01001
00101
00011
00001
```

不像上面本人的做法，这个答案直接就对了。

于是直接写一发，然后过了。

<span style="font-size:24px; font-weight:bold">？？？？？？</span>

#### 正确性证明

好，证明。

观察到一个鲜明的特点：集合 $i$ ( $1 \le i \le n$ ) 有 $g(i, i) = 1$ 。

首先证明第 $n + 1$ 个集合不会和任何其他集合相同。

如果 $a_{n - 1} < n$ 显然这个集合为空，而其他集合均为非空。

如此，我们设 $t$ 为最小满足 $a_t = n$ 的整数。

此时 $g(n + 1, i) = 1$ ( $t \le i \le n - 1$ ) 。

易得集合 $[t, n]$ 均不与其相同（因为后面都挖掉了一个数而且每个集合挖掉的数是不同的）；且集合 $[1, t - 1]$ 均不与其相同（因为 $g(j, i) = 1$ ( $1 \le j < t, t \le i \le n$ ) 而 $g(n + 1, n) = 0$ ）。

假设集合 $x$ 与集合 $y$ ( $x < y \le n$ )相同。

因为 $g(x, x) = 1, g(y, y) = 1$ ，且这两个集合相同，故有 $g(y, x) = 1, g(x, y) = 1$ 。

所以 $a_x \ge y - 1$ ，即 $a_{y - 1} \ge y - 1$ ，即 $g(x, y - 1) = 1$ 。但显然 $g(y, y - 1) = 0$ ，与我们的假设矛盾。

故不存在两个集合相同。

故该构造方法正确。

**BGNB!**

### 神 tourist 做法

~~没看懂，咕了。~~

### 原社论

先不升排序。我们现在依次在这至多 $n + 1$ 个集合中加入数 $1$ 至数 $n$ 。

我们用一可重集合 $S$ 来描述已经加入 $[1, x - 1]$ 后的状态。

假设 $S = \{c_1, c_2, c_3, \dots, c_{|S|}\}$ ，则目前至多有 $|S|$ 种互相不同的集合，第 $i$ 种集合的数量为 $c_i$ 。

现在我们研究加入 $x$ 时会发生什么。

首先假设 $S$ 中最大的数为 $A$ 。

如果 $a_x < A$ ，我们可以将 $x$ 加入到这 $A$ 个集合中的前 $a_x$ 个中去。这样，原本 $A$ 个相同的集合就变成了 $a_x$ 个新的相同的集合和 $A - a_x$ 个旧的相同的集合。由于其他集合中必定没有 $x$ ，所以这 $a_x$ 个新的相同的集合必定和其他集合不相同。

如果 $a_x \ge A$ ，我们可以类似地将 $A$ 个相同的集合变为 $A - 1$ 个新的相同的集合和 $1$ 个旧的集合。多出来的 $a_x - A + 1$ 个 $x$ 只用随机放到剩下的集合中就可以了。

开始时令 $S=\{n + 1\}$ 即可。由于最后要将 $S$ 变为一个有 $n + 1$ 个 $1$ 的集合，且每放入一个数都会导致其中的一个元素分裂成两个而集合内元素总和不变，所以只需要 $n$ 次，一定可以满足要求。

## 参考代码

{% fold 本人做法： %}
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

struct NODE{
	int num, id;
	inline int operator <(const NODE &X)const{ return num < X.num; }
}a[N];
int n, g[N][N], cbu[N], cnt, ot[N];

inline int get_ans(){
	ROF(i, n + 1, 1) FOR(j, 1, n) if(g[i][j]) return i;
	return 0;
}

int main(){
	n = read();
	FOR(i, 1, n) a[i].num = read(), a[i].id = i;
	sort(a + 1, a + n + 1);
	FOR(i, 1, n) ot[a[i].id] = i;
	FOR(i, 1, n){
		FOR(j, 1, min(a[i].num, i)) g[j][i] = 1;
		FOR(j, i + 1, a[i].num) g[j + 1][i] = 1;
	}
	FOR(i, 1, n - 1) if(a[i].num == a[i + 1].num && a[i].num <= i) ++cnt, cbu[i] = 1;
	Rint tmp = 1, h = a[1].num + (a[1].num > 1);
	FOR(i, 2, n){
		if(a[i - 1].num < a[i].num){
			Rint res = min(a[i].num, i);
			for(Rint t = 1; t < res - h;){
				if(cbu[tmp]){
					g[1][tmp] = 0;
					g[res - t + 1][tmp] = 1;
					++t;
				}
				++tmp;
			}
		}
		chkmax(h, a[i].num + (a[i].num > i));
	}
	Rint ans = get_ans();
	printf("%d\n", ans);
	FOR(i, 1, ans){ FOR(j, 1, n) putchar(g[i][ot[j]] ^ 48); putchar('\n'); }
	return 0;
}
```
{% endfold %}

~~看，多短（然而在所有做法中好像也是最繁琐的……）~~