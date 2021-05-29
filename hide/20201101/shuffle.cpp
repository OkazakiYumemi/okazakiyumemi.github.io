#include "shuffle.h"
#include <bits/stdc++.h>
namespace my_std {
using namespace std;
#define reg register
#define Rint register int
#define FOR(i, a, b) for (register int i = (a), ed_##i = (b); i <= ed_##i; ++i)
#define ROF(i, a, b) for (register int i = (a), ed_##i = (b); i >= ed_##i; --i)
typedef pair<int, int> PII;
typedef vector<int> VI;
#define pb push_back
#define fr first
#define sc second
#define pq priority_queue
#define MP make_pair
#define SZ(x) ((int)(x).size())
#define Templ(T) template <typename T>
Templ(_Tp) inline int chkmin(_Tp &x, _Tp y) { return x > y ? x = y, 1 : 0; }
Templ(_Tp) inline int chkmax(_Tp &x, _Tp y) { return x < y ? x = y, 1 : 0; }
} // namespace my_std
using namespace my_std;

#define N 1024
#define inf 1008
inline void print(VI x){
    for(int i: x) printf("%d ", i);
    puts("");
}
int n, B, K;
int ans[N];
VI return_ans(){
    VI res;
    FOR(i, 1, n) res.pb(ans[i]);
    return res;
}

namespace Set{
static int b[N];
VI Mul(VI x, VI y){
    VI res;
    for(int i: y) b[i] = 1;
    for(int i: x) if(b[i]) res.pb(i);
    for(int i: y) b[i] = 0;
    return res;
}
VI Add(VI x, VI y){
    VI res = y;
    for(int i: y) b[i] = 1;
    for(int i: x) if(!b[i]) res.pb(i);
    for(int i: y) b[i] = 0;
    return res;
}
VI Del(VI x, VI y){
    VI res;
    for(int i: y) b[i] = 1;
    for(int i: x) if(!b[i]) res.pb(i);
    for(int i: y) b[i] = 0;
    return res;
}
bool isIn(VI x, int y){
    for(int i: x) if(i == y) return true;
    return false;
}
}
using namespace Set;

namespace Sub5{
vector<VI> A[3], X[3];
VI solve(){
    FOR(o, 0, 2) A[o].resize(2);
    FOR(i, 1, n / 2 - 1) FOR(o, 0, 2) A[o][0].pb(i);
    FOR(i, n / 2 + 1, n - 2) FOR(o, 0, 2) A[o][1].pb(i);
    A[0][0].pb(n / 2), A[1][0].pb(n - 1), A[2][0].pb(n);
    A[0][1].pb(n - 1), A[1][1].pb(n), A[2][1].pb(n / 2);
    A[0][1].pb(n), A[1][1].pb(n / 2), A[2][1].pb(n - 1);
    // FOR(o, 0, 2){
    //     for(VI i: A[o]) print(i);
    //     puts("-----");
    // }
    FOR(o, 0, 2) X[o] = shuffle(A[o]);

    function<bool(int)> LEFT = [&](const int &T){
        VI x;
        FOR(i, 1, n) x.pb(i);
        FOR(o, 0, 2) x = Mul(x, X[o][(T >> o) & 1]);
        return SZ(x) == n / 2 - 1;
    };


    FOR(i, 0, 7) if(LEFT(i)){
        FOR(o, 0, 2) if((i >> o) & 1)
            swap(X[o][0], X[o][1]);
        break;
    }

    // FOR(o, 0, 2){
    //     for(VI i: X[o]) print(i);
    //     puts("-----");
    // }

    VI res = Mul(Mul(X[0][0], X[1][0]), X[2][0]);
    ans[n / 2] = Del(X[0][0], res)[0];
    ans[n - 1] = Del(X[1][0], res)[0];
    ans[n] = Del(X[2][0], res)[0];
    // print(res);
    // FOR(i, 1, n) printf("%d%c", ans[i], " \n"[i == ed_i]);

    vector<pair<VI, VI> > sol(2);
    FOR(i, 1, n / 2 - 1) sol[0].fr.pb(i);
    sol[0].sc = res;
    FOR(i, n / 2 + 1, n - 2) sol[1].fr.pb(i);
    sol[1].sc = Mul(Mul(X[0][1], X[1][1]), X[2][1]);
    // for(pair<VI, VI> i: sol) print(i.fr), print(i.sc);
    // puts("-------");

    while([&](){
        vector<pair<VI, VI> > ss;
        for(pair<VI, VI> x: sol){
            if(SZ(x.fr) == 1) ans[x.fr[0]] = x.sc[0];
            else ss.pb(x);
        }
        sol = ss;
        // puts("-------");
        // for(pair<VI, VI> x: sol){
        //     print(x.fr), print(x.sc);
        // }
        // puts("--sol--"), fflush(stdout);
        return;
    }(), !sol.empty()){
        vector<pair<VI, VI> > ss;
        vector<VI> Q(2), A;
        Q[0].pb(n - 1), Q[1].pb(n);
        for(pair<VI, VI> &x: sol){
            int t = SZ(Q[0]) < SZ(Q[1]);
            FOR(i, 0, SZ(x.fr) / 2 - 1) Q[t].pb(x.fr[i]);
            FOR(i, SZ(x.fr) / 2, SZ(x.fr) - 1) Q[t ^ 1].pb(x.fr[i]);
            x.sc.pb(inf + t);
        }
        FOR(i, 1, n - 2) if(ans[i]) Q[SZ(Q[0]) == n / 2].pb(i);

        A = shuffle(Q);
        // puts("---");
        // for(VI i: Q) print(i);
        // for(VI i: A) print(i);
        // puts("---");

        if(isIn(A[0], ans[n])) swap(A[0], A[1]);
        for(pair<VI, VI> &x: sol){
            int t = x.sc.back() - inf;
            x.sc.pop_back();
            // print(x.fr);
            // printf("t: %d\n", t);
            pair<VI, VI> y;

            FOR(i, 0, SZ(x.fr) / 2 - 1) y.fr.pb(x.fr[i]);
            y.sc = Mul(x.sc, A[t]);
            ss.pb(y);

            y.fr.clear(), y.sc.clear();
            FOR(i, SZ(x.fr) / 2, SZ(x.fr) - 1) y.fr.pb(x.fr[i]);
            y.sc = Mul(x.sc, A[t ^ 1]);
            ss.pb(y);
        }
        sol = ss;
    }

    // FOR(i, 1, n) printf("%d%c", ans[i], " \n"[i == ed_i]);
    return return_ans();
}
}

namespace Sub4{
int flag;
vector<VI> A[4], X[4];
int p[4][N];
VI solve(){
    FOR(i, 1, n / 2 - 1) A[0].pb({i * 2 - 1, i * 2}), A[1].pb({i * 2, i * 2 + 1});
    A[0].pb({n - 1, n}), A[1].pb({n, 1});
    A[2].pb({1, 3}), A[3].pb({1, 3});
    A[2].pb({2, 4});
    FOR(i, 3, n / 2) A[2].pb({i * 2 - 1, i * 2}), A[3].pb({i * 2 - 2, i * 2 - 1});
    A[3].pb({n, 2});
    FOR(o, 0, 3){
        X[o] = shuffle(A[o]);
        for(VI x: X[o]) p[o][x[0]] = x[1], p[o][x[1]] = x[0];
    }
    for(VI i: X[3]){
        int x = i[0], y = i[1];
        if(p[0][x] == p[1][y] && p[2][x] == y && p[2][y] == x){ ans[2] = p[0][x]; break; }
        if(p[1][x] == p[0][y] && p[2][x] == y && p[2][y] == x){ ans[2] = p[1][x]; break; }
    }
    ans[1] = p[0][ans[2]];
    FOR(i, 3, n) ans[i] = p[i & 1][ans[i - 1]];
    return return_ans();
}
}

namespace Sub6{
struct Node{
    VI a, p;
    int f;
    inline void clear(){
        a.clear();
        p.clear();
    }
};
vector<VI> A[3], X[3];
inline Node find(const int &u, const int &v){
    VI x;
    Node e;
    for(VI i: X[2]) if(isIn(i, ans[u])){ x = i; break; }
    for(VI i: X[0]){
        VI j = Mul(i, x);
        if(SZ(j) == K - 1){
            ans[v] = Del(i, x)[0];
            x = j;
            break;
        }
    }
    FOR(i, 1, K - 1) e.a.pb(v + i);
    e.p = x;
    return e;
}
VI solve(){
    A[0].resize(B);
    FOR(i, 1, n) A[0][(i - 1) / K].pb(i);
    A[1] = A[2] = A[0];
    swap(A[1][0][0], A[1][1][0]);
    A[2][0][0] = K * 2 + 1, A[2][1][0] = K + 1;
    FOR(i, 2, B - 2) A[2][i][0] = K * (i + 1) + 1;
    A[2][B - 1][0] = 1;
    FOR(o, 0, 2) X[o] = shuffle(A[o]);
    // FOR(o, 0, 2){
    //     for(VI x: A[o]) print(x);
    //     puts("-----");
    // }

    vector<Node> sol;
    {
        VI u;
        vector<VI> res;
        for(VI x: X[0]) for(VI y: X[1]) if(SZ(Mul(x, y)) == K - 1) res.pb(x);
        for(VI x: res) for(VI y: X[2]) if(SZ(Mul(x, y)) == K - 1){
            u = x;
            ans[1] = Del(x, y)[0];
            break;
        }
        sol.pb(find(1, K * (B - 1) + 1));
        ROF(i, B - 1, 3) sol.pb(find(K * i + 1, K * (i - 1) + 1));
        sol.pb([&](){
            Node e;
            for(VI i: X[1]){
                VI x = Mul(u, i);
                if(SZ(x) == K - 1){
                    ans[K + 1] = Del(i, u)[0];
                    break;
                }
            }
            for(VI i: X[0]) if(isIn(i, ans[K + 1])){
                e.p = Del(i, {ans[K + 1]});
                break;
            }
            FOR(i, K + 2, K * 2) e.a.pb(i);
            return e;
        }()), sol.pb([&](){
            Node e;
            for(VI i: X[0]) if(isIn(i, ans[1])){
                e.p = Del(i, {ans[1]});
                break;
            }
            FOR(i, 2, K) e.a.pb(i);
            return e;
        }());
        // FOR(i, 0, B - 1) printf("%d%c", ans[K * i + 1], " \n"[i == ed_i]);
        // for(Node e: sol){
        //     print(e.a), print(e.p);
        //     puts("--0--");
        // }
    }

    while([&](){
        vector<Node> ss;
        for(Node e: sol){
            if(SZ(e.a) == 1) ans[e.a[0]] = e.p[0];
            else ss.pb(e);
        }
        sol = ss;
    }(), !sol.empty()){
        vector<Node> ss;
        vector<VI> A(B), X;
        FOR(i, 0, B - 1) A[i].pb(K * i + 1);
        for(Node e: sol){
            sort(A.begin(), A.end(), [&](const VI &x, const VI &y){
                return SZ(x) < SZ(y);
            });
            const int siz = SZ(e.a), t = siz / B;
            int l = 0, r;
            FOR(i, 0, B - 1){
                r = l + t + (t * B + i < siz);
                if(l == r) continue;
                Node g;
                FOR(j, l, r - 1) A[i].pb(e.a[j]), g.a.pb(e.a[j]);
                l = r;
                g.f = ans[A[i][0]];
                g.p = e.p;
                ss.pb(g);
            }
        }
        FOR(i, 1, n) if(ans[i] && (i - 1) % K != 0){
            for(VI &x: A) if(SZ(x) < K){
                x.pb(i);
                break;
            }
        }
        // for(Node e: ss) print(e.a), print(e.p), puts("--=--");
        // for(VI x: A) print(x);
        // puts("--000--");
        X = shuffle(A);

        for(Node &e: ss){
            for(VI x: X) if(isIn(x, e.f)){
                e.p = Mul(e.p, x);
                break;
            }
        }
        sol = ss;
    }
    return return_ans();
}
}

VI solve(int n, int b, int k, int q, int st){
    ::n = n, ::B = b, ::K = k;
    if(b == 2) return Sub5::solve();
    if(k == 2) return Sub4::solve();
    return Sub6::solve();
}