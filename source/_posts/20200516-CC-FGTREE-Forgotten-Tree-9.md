---
title: 「CodeChef FGTREE」Forgotten Tree 9
categories: 题解
date: 2020-05-16 17:55:47
tags:
- CodeChef
- 猜结论
- 交互题
top:
urlname: CC-FGTREE
---

很妙的一道交互。

[[CodeChef FGTREE]](https://www.codechef.com/problems/FGTREE)

## 题意简述

一棵 $n$ 个点的有根二叉树，树中节点编号为中序遍历中的 dfs 序。其中 $n \le 100$ 。

你可以询问不超过 $300$ 次形如 $(i, l, r)$ 的问题，表示询问 $i$ 子树内编号最小值是否为 $l$ **且**最大值是否为 $r$ 。

请回答每个点的父亲的编号。

<!-- more -->

## 主要思路

考虑按照编号从小到大尝试确定父亲。维护一个栈来存储还没有确定父亲的节点。不妨设我们已经加入到点 $x$ 。

先判断其是否在栈顶的子树中，若是则直接压入栈中；否则弹出栈顶并继续直到可压入栈中。

考虑过程中弹出的点按弹出先后为 $\langle v_m\rangle$，则弹出的点子树中的最大编号均为 $x - 1$，故 $\mathrm{fth}(v_i) = v_{i + 1}$（$v_{m + 1} = x$）。

加入所有点后，若栈非空，不难发现这些点子树中最大编号均为 $n$，故也相邻两点有父子关系。

那么在此过程中如何判断 $x$ 是否在某个点 $y$ 的子树内？可以询问 $(y, l_y, x - 1)$，若返回值为假则在子树中。而 $l_y$ 显然可以在过程中更新。

于是做完，查询次数 $O(n)$，据说可以卡满。

## 参考代码

{% fold %}
```cpp
#include<bits/stdc++.h>
using namespace std;

#define F_O fflush(stdout)
const int N = 110;
class Stack{
private:
    int a[N], t;
public:
    inline bool empty(){
        return t == 0;
    }
    inline int size(){
        return t;
    }
    inline int top(){
        return t ? a[t - 1] : -1;
    }
    inline bool pop(){
        return t ? (-- t, 1) : 0;
    }
    inline void push(int x){
        a[t ++] = x;
    }
    inline void clear(){
        t = 0;
    }
    inline void print(){
        puts("Stack:");
        for(int i = 0; i < t; ++i){
            printf("%d, ", a[i]);
        }
        puts("");
    }
};

int n, f[N], l[N];
Stack S, T;

inline bool query(int x, int l, int r){
    printf("Q %d %d %d\n", x, l, r);
    F_O;
    char c[5];
    scanf("%s", c);
    return *c == 'Y';
}

inline void work(){
    scanf("%d", &n);
    S.clear(), T.clear();
    memset(f, 0, sizeof(f));
    int t;
    for(int x = 1; x <= n; ++x) l[x] = x;
    for(int x = 1; x <= n; ++x){
        while(!S.empty()){
            t = S.top();
            S.pop();
            bool g = query(t, l[t], x - 1);
            if(!g){
                S.push(t);
                break;
            }
            T.push(t);
        }
        if(!T.empty()){
            l[x] = l[T.top()];
            T.push(x);
            t = 0;
            while(!T.empty()){
                f[T.top()] = t;
                t = T.top();
                T.pop();
            }
        }
        S.push(x);
    }
    while(!S.empty()){
        t = S.top();
        S.pop();
        f[t] = S.top();
    }
    printf("A ");
    for(int x = 1; x < n; ++x) printf("%d ", f[x]);
    printf("%d\n", f[n]);
    F_O;
    return;
}

int main(){
    int esac;
    scanf("%d", &esac);
    while(esac --) work();
	return 0;
}
```
{% endfold %}