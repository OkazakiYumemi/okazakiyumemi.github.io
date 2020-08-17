---
title: 「CF506E」Mr. Kitayuta's Gift
categories: 题解
urlname: CF506E
date: 2020-08-15 23:18:17
tags:
- dp
- 构造
top:
---

队爷的 pdf 题解根本看不懂，，，

## 题意简述

给定一个小写字符串 $s$ 与正整数 $n$。$|s|\le 200, n\le 10^9$。

要求在 $s$ 中插入恰好 $n$ 个小写字符使其为回文串的方案数，对 $10^4 + 7$ 取模。

<!-- more -->

## 主要思路

在这里只考虑 $n + |s|$ 为偶数的情况（因为实际上两者差不多）。

考虑 dp。设 $f(i, l, r)$ 为只考虑最终回文串的前 $i$ 与后 $i$ 个字符，它们与 $s$ **尽可能**匹配后还剩下 $[l, r]$ 这段区间的方案数。
然后顺便设 $g(i)$ 为只考虑最终回文串的前 $i$ 与后 $i$ 个字符已经将 $s$ 完全匹配的方案数。

显然转移有：
1. $s_l = s_r, r - l\le 1$:
$$\begin{aligned}
    g(i + 1)&\leftarrow f(i, l, r)\\\\
    f(i + 1, l, r)&\leftarrow 25\cdot f(i, l, r)
\end{aligned}$$
2. $s_l = s_r, r - l > 1$:
$$\begin{aligned}
    f(i + 1, l + 1, r - 1)&\leftarrow f(i, l, r)\\\\
    f(i + 1, l, r)&\leftarrow 25\cdot f(i, l, r)
\end{aligned}$$
3. $s_l \ne s_r$:
$$\begin{aligned}
    f(i + 1, l + 1, r)&\leftarrow f(i, l, r)\\\\
    f(i + 1, l, r - 1)&\leftarrow f(i, l, r)\\\\
    f(i + 1, l, r)&\leftarrow 24\cdot f(i, l, r)
\end{aligned}$$
4. $g$:
$$g(i + 1)\leftarrow 26\cdot g(i)$$

把这个状态数 $O(|s|^2)$ 的 dp 强行矩乘优化可得到 $O(|s|^6\log n)$ 的复杂度，但显然无法通过此题。

观察，可以发现这个 dp 可以表示为在一个**有限状态自动机**上匹配的过程。我们不妨将终点设为 $T$，转移过程的点设为 $(l, r)$。

{% asset_img g.png %}

令对于所有 $(l, r)$ 满足 $s_l = s_r$，设其为**一类点**（上图粉色），否则设为**二类点**。

由于除了 $l = r$ 以外，经过一个一类点会使得未匹配长度减少 $2$，故若一条链上经过了 $x$ 个二类点，则应经过 $\lceil\frac{|s| - i}{2}\rceil$ 个一类点。

## 参考代码

难写，，，