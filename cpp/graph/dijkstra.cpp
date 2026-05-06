#include <iostream>
#include <vector>
#include <limits>
#include <queue>

using namespace std;

using ll = long long;
constexpr ll INF = numeric_limits<ll>::max();

/**
 * dijkstra 单源最短路
 * @param w 邻接矩阵
 * @param src 起点
 * @return 从src到各点的最短距离，不可达为INF
 */
vector<ll> dijkstra(const vector<vector<ll> > &w, int src) {
    const int n = w.size();
    vector<ll> dist(n, INF);
    vector<bool> visited(n, false);
    dist[src] = 0;

    for (int i = 0; i < n; ++i) {
        int u = -1;

        // 找到未访问节点中，距离最近的那个节点
        for (int j = 0; j < n; ++j) {
            if (!visited[j] && (u == -1 || dist[j] < dist[u])) u = j;
        }

        // dist[u] == INF 代表节点不可达，可直接结束
        if (u == -1 || dist[u] == INF) break;

        visited[u] = true;

        // 松弛，更新未访问节点的距离
        for (int v = 0; v < n; ++v) {
            if (w[u][v] != INF && (dist[u] + w[u][v] < dist[v])) dist[v] = dist[u] + w[u][v];
        }
    }

    return dist;
}

/**
 * dijkstra 堆优化版本 单源最短路
 * @param g 邻接表，g[u] = {{v, w}, ...} 表示存在边 u -> v，权重为 w
 * @param src 起点
 * @return 从src到各点的最短距离，不可达为INF
 */
vector<ll> dijkstra_heap(const vector<vector<pair<int, ll>>> &g, int src) {
    const int n = g.size();
    vector<ll> dist(n, INF);
    dist[src] = 0;


    // 小顶堆，元素为 {距离, 节点}
    priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<>> pq;
    pq.emplace(0, src);

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        // 已经有更短的距离，跳过该过时记录
        if (d > dist[u]) continue;

        // 松弛，更新邻接节点的距离
        for (auto [v, w]: g[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.emplace(dist[v], v);
            }
        }
    }

    return dist;
}

int main() {
    vector<vector<ll> > w = {
        {0, 4, 2, INF, INF, INF, INF},
        {4, 0, 1, 5, INF, INF, INF},
        {2, 1, 0, 8, 10, INF, INF},
        {INF, 5, 8, 0, 2, 6, INF},
        {INF, INF, 10, 2, 0, 3, INF},
        {INF, INF, INF, 6, 3, 0, INF},
        {INF, INF, INF, INF, INF, INF, 0}
    };
    vector<ll> result = dijkstra(w, 0);
    for (auto v: result) {
        cout << v << " ";
    }
    cout << endl;

    // 邻接矩阵转邻接表
    const int n = w.size();
    vector<vector<pair<int, ll>>> g(n);
    for (int u = 0; u < n; ++u) {
        for (int v = 0; v < n; ++v) {
            if (u != v && w[u][v] != INF) g[u].emplace_back(v, w[u][v]);
        }
    }
    vector<ll> result2 = dijkstra_heap(g, 0);
    for (auto v: result2) {
        cout << v << " ";
    }
    cout << endl;

    return 0;
}
