#include "shuffle.h"
#include <bits/stdc++.h>
using namespace std;
const int N = 1003;
typedef vector<int> VI;
namespace {
static int n, b, k, q, ST, qcnt;
static VI ans;
static bool vis[N];
} // namespace
void GG(int x) {
    printf("%d\n", x);
    exit(0);
}
vector<VI> shuffle(vector<VI> boxes) {
    if (++qcnt > q) GG(-1);
    if ((int)boxes.size() != b) GG(-2);
    memset(vis + 1, 0, n);
    for (VI &i: boxes) {
        if ((int)i.size() != k) GG(-2);
        for (int &j: i) {
            if(vis[j]) GG(-3);
            vis[j] = true;
            j = ans[j - 1];
        }
        random_shuffle(i.begin(), i.end());
    }
    if (ST != 3)
        random_shuffle(boxes.begin(), boxes.end());
    return boxes;
}
int main() {
    srand(time(0));
    scanf("%d%d%d%d%d", &n, &b, &k, &q, &ST);
    ans.resize(n);
    for (int i = 0; i < n; ++i)
        scanf("%d", &ans[i]);
    if (solve(n, b, k, q, ST) != ans) GG(0);
    else if (qcnt <= 9 || ST != 6) GG(100);
    else if (qcnt <= 50)
        GG((22 + 30 * (50.0 - qcnt) * (50.0 - qcnt) / 1681) * 13 / 25 + 1 - 1e-8);
    else GG(qcnt <= 500 ? 34 : 16);
}
