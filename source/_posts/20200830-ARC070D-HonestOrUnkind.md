---
title: 「ARC070D」HonestOrUnkind
categories: 题解
urlname: ARC070D
date: 2020-08-30 11:57:29
tags:
- AtCoder
- 交互题
- 构造
top:
---

## 题意简述

交互题。
$A$ 个诚实的人和 $B$ 个不诚实的人互相知道各自是否诚实。这些人编号 $[0, A+B)$，你希望判断每个编号的人是否诚实。
你可进行不超过 $2n$ 次提问，每次提问可给出两数 $(a,b)$，表示询问 $a$ 号人 $b$ 号人是否诚实。若 $a$ 诚实则他会如实回答，否则他有可能**说假话**。
有无法判断的情况。$1\le A,B\le 2000$。

<!-- more -->

## 主要思路

若 $A\le B$ 显然无解。那 $B$ 个人总有办法串通起来让你无法判断某人是否诚实。

于是只用考虑 $A>B$ 的情况。

发现若询问 $(a, b)$ 回答不诚实，那么这二人至少有一人不诚实。
考虑用一个栈维护，每次新加一个人，若栈不为空则询问栈顶的人新来的人是否诚实。若回答不诚实，则弹出栈顶且扔掉新来的；否则把新来的加入栈中。

考察最后栈的情况，必定是栈底若干个不诚实的人（可能无），栈顶若干个诚实的人（必定有）。
为啥？一次删除最多会删掉一个诚实的和一个不诚实的，由于诚实的人多，所以最后必定能留下诚实的人；并且诚实的人一定会披露不诚实的，故栈顶必为诚实的人。

最后再问栈顶的人每个人是否诚实即可。

## 参考代码

```cpp
#include <bits/stdc++.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a), ed_##i = (b); i <= ed_##i; ++i)

#define N 4010
int A, B, n;
int d[N], t;
char f[N], g[5];

int main() {
    scanf("%d%d", &A, &B);
    n = A + B;
    if(A <= B) return puts("Impossible"), 0;
    FOR(i, 0, n - 1){
        if(!t) d[++t] = i;
        else{
            printf("? %d %d\n", d[t], i), fflush(stdout);
            scanf("%s", g);
            *g == 'Y' ? d[++t] = i : --t;
        }
    }
    const int u = d[t];
    f[u] = 49;
    FOR(i, 0, n - 1) if(i != u){
        printf("? %d %d\n", u, i), fflush(stdout);
        scanf("%s", g);
        f[i] = (*g == 'Y') + 48;
    }
    printf("! %s\n", f), fflush(stdout);
    return 0;
}
```