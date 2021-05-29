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
#define Templ(T) template <typename T>
Templ(_Tp) inline int chkmin(_Tp &x, _Tp y) { return x > y ? x = y, 1 : 0; }
Templ(_Tp) inline int chkmax(_Tp &x, _Tp y) { return x < y ? x = y, 1 : 0; }
} // namespace my_std
using namespace my_std;

#define N 100010
#ifdef LOCAL
const char wci[] = "012DEFGHIJKLMNOPQRSTUVWXYZ";
const char wic[] = "ABC";
int _s[2][N];
#else
const char wci[] = "A2CDEFGHIJKLMNOPQ0STUVWX1Z";
const char wic[] = "RYB";
#endif
int n;
int s[2][N];
vector<PII> ans[2];

void init(){
    scanf("%d", &n);
    static char c[N];
    scanf("%s", c + 1);
    FOR(i, 1, n) s[1][i] = wci[c[i] - 'A'] - '0';
    scanf("%s", c + 1);
    FOR(i, 1, n) s[0][i] = wci[c[i] - 'A'] - '0';
#ifdef LOCAL
    FOR(o, 0, 1) memcpy(_s[o] + 1, s[o] + 1, sizeof(int) * n);
#endif
}
inline void print(const int *a){
    FOR(i, 1, n) printf("%d%c", a[i], " \n"[i == ed_i]);
}

#define np(i) ((i) < n ? (i) + 1 : 1)
#define pp(i) ((i) > 1 ? (i) - 1 : n)
#define GO(i, a, b) for(int i = (a), ed_##i = (b); i != ed_##i; i = np(i))

void finish(){
    FOR(i, 1, n) assert(s[0][i] == s[1][i]);
    reverse(ans[0].begin(), ans[0].end());
    for(PII x: ans[0]){
        ans[1].push_back(x);
    }
    printf("%d\n", (int)ans[1].size());
    for(PII x: ans[1]){
        printf("%d %c\n", x.fr, wic[x.sc]);
    }
#ifdef LOCAL
    for(PII x: ans[1]){
        int i = x.fr;
        assert(_s[1][np(i)] != _s[1][pp(i)]);
        _s[1][i] = x.sc;
    }
    FOR(i, 1, n) assert(_s[1][i] == _s[0][i]);
#endif
    exit(0);
}

#define nc(i) ((i) < n ? a[(i) + 1] : a[1])
#define pc(i) ((i) > 1 ? a[(i) - 1] : a[n])
void work(const int &z, const int &x){
    int *a = s[z];
    function<void(int, int)> upd = [&](const int &i, const int &j){
        if(z) a[i] = j, ans[1].push_back(MP(i, j));
        else ans[0].push_back(MP(i, a[i])), a[i] = j;
        return;
    };
    const int f = pc(x);
    GO(i, x, pp(x)){
        int c = [&](){
            FOR(o, 0, 2) if(o != nc(np(i)) && o != f) return o;
            return -1;
        }();
        assert(c != -1);
        if(c != a[i]){
            assert(pc(i) != nc(i));
            upd(i, c);
        }
    }
    // print(a);
    GO(i, x, pp(pp(x))){
        if(f != a[i]){
            assert(pc(i) != nc(i));
            upd(i, f);
        }
    }
    // for(PII x: ans[z]){
    //     printf("%d %c\n", x.fr, wic[x.sc]);
    // }
    return;
}
int check(const int *a){
    FOR(i, 1, n) if(pc(i) != nc(i)) return i;
    return 0;
}
int getp(const int *a){
    FOR(i, 1, n) if(pc(i) != nc(i) && pc(i) != a[i]) return i;
    return 0;
}
#undef nc
#undef pc

int main() {
    init();
    int f[2] = {check(s[0]), check(s[1])};
    if(!f[0] || !f[1]){
        if(f[0] || f[1]) return puts("-1"), 0;
        FOR(i, 1, n) if(s[0][i] != s[1][i]) return puts("-1"), 0;
        return puts("0"), 0;
    }
    // FOR(o, 0, 1) print(s[o]);
    FOR(o, 0, 1){
        f[o] = getp(s[o]);
        assert(f[o] > 0);
        if([&](){
            const int x = f[o];
            const int c = s[o][x];
            GO(i, x, pp(x)) if(c != s[o][i]) return false;
            return true;
        }()){
            f[o] = pp(f[o]);
        }else{
            work(o, f[o]);
            f[o] = pp(pp(f[o]));
        }
    }
    // puts("-----");
#define nc(i, j) s[j][np(i)]
#define pc(i, j) s[j][pp(i)]
#define upd(i, j) s[1][i] = (j), ans[1].push_back(MP(i, (j)))
    if(f[0] != f[1]){
        if(f[0] == pp(f[1])){
            int c = s[1][f[0]];
            upd(f[0], s[1][f[1]]);
            upd(f[1], c);
        }else{
            int c = s[1][f[1]], d = s[1][f[0]];
            GO(i, np(f[1]), np(f[0])){
                upd(i, c);
            }
            // print(s[1]);
            GO(i, f[1], f[0]){
                upd(i, d);
            }
            // print(s[1]);
        }
    }
    // FOR(o, 0, 1) print(s[o]);
    const int p = f[0];
    const int c[2][2] = {{s[0][p], nc(p, 0)}, {s[1][p], nc(p, 1)}};
    // puts("-----");
    if(c[1][0] == c[0][1]){
        const int x = c[0][0], y = c[0][1];
        upd(np(p), y);
        upd(p, x);
        GO(i, np(np(p)), p){
            upd(i, y);
        }
    }else{
        const int x = c[0][1], y = c[0][0];
        if(c[0][1] != c[1][1]){
            GO(i, np(p), p){
                upd(i, x);
            }
        }
        if(c[0][0] != c[1][0]){
            upd(np(p), y);
            upd(p, y);
            upd(np(p), x);
        }
    }
    // print(s[1]);
    finish();
#undef nc
#undef pc
#undef upd
    return 0;
}
/*
3
RYB
YBR

3
ABC
BCA

10
RBRBRYRYYY
BBYBRYYBYY

10
ACACABABBB
CCBCABBCBB
*/