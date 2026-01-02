#include <iostream>
#include <vector>
#include <queue>
#include <iomanip> // 用于格式化输出
#include <stack>

using namespace std;

// 定义边结构体
struct Edge {
    int to;             // 目标节点
    double reliability; // 链路可靠性 (0.0 - 1.0)
};

// 定义图的存储结构：邻接表
// graph[u] 包含所有从 u 出发的边
using Graph = vector<vector<Edge>>;

// 用于优先队列的状态结构
struct State {
    int node;
    double prob;

    // 优先队列默认是最大堆，需要重载 < 运算符
    // 这里的逻辑是：如果 p1 < p2，则 s1 < s2。
    // priority_queue 会把 "大" 的放在上面，所以我们需要 prob 大的在堆顶
    bool operator<(const State& other) const {
        return prob < other.prob;
    }
};

/**
 * 寻找最大可靠性路径 (Dijkstra 变体)
 * @param graph 图结构
 * @param startNode 源节点
 * @param endNode 目的节点
 * @param maxProb 引用传参，用于返回计算出的最大概率
 * @return 构成最优路径的节点列表
 */
vector<int> findMaxReliabilityPath(const Graph& graph, int startNode, int endNode, double& maxProb) {
    int n = graph.size();

    // dist[i] 存储从源点到节点 i 的当前最大可靠性
    vector<double> dist(n, -1.0);

    // parent[i] 存储节点 i 的前驱节点，用于路径还原
    vector<int> parent(n, -1);

    // 优先队列 (最大堆)，存储 {节点, 当前累积概率}
    priority_queue<State> pq;

    // 初始化源点
    dist[startNode] = 1.0;
    pq.push({ startNode, 1.0 });

    while (!pq.empty()) {
        State current = pq.top();
        pq.pop();

        int u = current.node;
        double p = current.prob;

        // 如果到了目的节点，虽然可以提前退出，但为了确保是全局最优（在某些特殊负权变种需注意，但在概率积场景下通常可以直接continue，
        // 这里为了完整遍历所有可能更新，也可以不break，取决于图的规模。标准Dijkstra在出堆时即为最短/最优）
        if (u == endNode) {
            // 注意：这里不能直接 return，因为可能还有其他路径没处理完
            // 实际上对于Dijkstra，第一次从堆中取出终点时，就是最优解。
            // 我们可以选择在这里 break，或者让队列跑完。为了逻辑简单，让它跑完。
        }

        // 如果当前取出的概率小于已记录的最大概率（过期数据），则跳过
        if (p < dist[u]) continue;

        // 遍历邻居
        for (const auto& edge : graph[u]) {
            int v = edge.to;
            double weight = edge.reliability;

            // 核心松弛逻辑：乘法
            if (dist[u] * weight > dist[v]) {
                dist[v] = dist[u] * weight;
                parent[v] = u;
                pq.push({ v, dist[v] });
            }
        }
    }

    maxProb = dist[endNode];

    // 路径还原
    vector<int> path;
    if (maxProb == -1.0) {
        return path; // 无法到达
    }

    int curr = endNode;
    while (curr != -1) {
        path.push_back(curr);
        curr = parent[curr];
    }

    // 反转路径（因为是从终点回溯到起点的）
    // 这里也可以用 std::reverse，或者手动交换
    for (size_t i = 0; i < path.size() / 2; ++i) {
        swap(path[i], path[path.size() - 1 - i]);
    }

    return path;
}

// 辅助函数：添加边
void addEdge(Graph& g, int u, int v, double reliability) {
    g[u].push_back({ v, reliability });
}

int main() {
    // 设计网络结构
    int numNodes = 5;
    Graph net(numNodes);

    // 添加边 (源, 目的, 可靠性)
    addEdge(net, 0, 1, 0.99);
    addEdge(net, 0, 2, 0.98); 
    addEdge(net, 1, 2, 0.97);
    addEdge(net, 1, 3, 0.99);
    addEdge(net, 2, 0, 0.97);
    addEdge(net, 2, 3, 0.95);
    addEdge(net, 2, 4, 0.98); 
    addEdge(net, 3, 4, 0.99); 
    addEdge(net, 4, 3, 0.99);

    int src = 0;
    int dst = 4;
    double maxRel = 0.0;

    cout << "计算从节点 " << src << " 到节点 " << dst << " 的最大可靠性路径..." << endl;

    vector<int> path = findMaxReliabilityPath(net, src, dst, maxRel);

    if (path.empty()) {
        cout << "无法从源节点到达目的节点！" << endl;
    }
    else {
        cout << "--------------------------------" << endl;
        cout << "最优路径: ";
        for (size_t i = 0; i < path.size(); ++i) {
            cout << path[i];
            if (i < path.size() - 1) cout << " -> ";
        }
        cout << endl;

        cout << fixed << setprecision(4); // 设置输出精度
        cout << "最大可靠性 (概率): " << maxRel << " (" << maxRel * 100 << "%)" << endl;
        cout << "--------------------------------" << endl;
    }

    return 0;
}