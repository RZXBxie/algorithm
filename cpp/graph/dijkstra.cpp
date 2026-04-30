#include <iostream>
#include <vector>
#include <limits>

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

    return 0;
}
