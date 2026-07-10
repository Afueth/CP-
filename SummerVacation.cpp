#include <bits/stdc++.h>
using namespace std;

static const int MAXNODES = 20'000'000;
static int Lc[MAXNODES], Rc[MAXNODES], SZ[MAXNODES], VA[MAXNODES];
static int cnt = 0;

static inline int clone(int t) {
    ++cnt;
    Lc[cnt] = Lc[t]; Rc[cnt] = Rc[t];
    SZ[cnt] = SZ[t]; VA[cnt] = VA[t];
    return cnt;
}
static inline void pull(int t) { SZ[t] = SZ[Lc[t]] + SZ[Rc[t]] + 1; }

static uint64_t rngState = 0x9E3779B97F4A7C15ULL;
static inline uint64_t rnd() {
    rngState ^= rngState << 13;
    rngState ^= rngState >> 7;
    rngState ^= rngState << 17;
    return rngState;
}

// build perfectly balanced tree with values lo..hi-1
int build(int lo, int hi) {
    if (lo >= hi) return 0;
    int mid = (lo + hi) >> 1;
    int t = ++cnt;
    VA[t] = mid;
    Lc[t] = build(lo, mid);
    Rc[t] = build(mid + 1, hi);
    pull(t);
    return t;
}

// persistent split: first k elements go left
pair<int,int> split(int t, int k) {
    if (!t) return {0, 0};
    if (k <= SZ[Lc[t]]) {
        auto [a, b] = split(Lc[t], k);
        int c = clone(t);
        Lc[c] = b; pull(c);
        return {a, c};
    } else {
        auto [a, b] = split(Rc[t], k - SZ[Lc[t]] - 1);
        int c = clone(t);
        Rc[c] = a; pull(c);
        return {c, b};
    }
}

// persistent merge, randomized by subtree size (keeps expected depth O(log n))
int merge(int a, int b) {
    if (!a || !b) return a ? a : b;
    if (rnd() % (uint64_t)(SZ[a] + SZ[b]) < (uint64_t)SZ[a]) {
        int c = clone(a);
        Rc[c] = merge(Rc[a], b); pull(c);
        return c;
    } else {
        int c = clone(b);
        Lc[c] = merge(a, Lc[b]); pull(c);
        return c;
    }
}

// value at index k (0-based)
static inline int kth(int t, int k) {
    while (true) {
        int s = SZ[Lc[t]];
        if (k < s) t = Lc[t];
        else if (k == s) return VA[t];
        else { k -= s + 1; t = Rc[t]; }
    }
}

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    vector<int> a(n);
    for (auto &x : a) scanf("%d", &x);

    int D = 2 * n;
    int root = build(0, D);   // identity: P[x] = x

    vector<int> ans(n);
    for (int i = n - 1; i >= 0; --i) {
        int A = a[i];
        // answer for arriving on day i+1 (k = n - i last days)
        ans[n - 1 - i] = kth(root, A);

        // new P = P[A : 2A] ++ P[0 : D - A]
        auto [t1, _r1] = split(root, 2 * A);   // t1 = P[0 : 2A]
        auto [_l2, mid] = split(t1, A);        // mid = P[A : 2A]
        auto [pre, _r3] = split(root, D - A);  // pre = P[0 : D - A]
        root = merge(mid, pre);
    }

    for (int k = 0; k < n; ++k)
        printf("%d ", ans[k]);
    printf("\n");
    return 0;
}