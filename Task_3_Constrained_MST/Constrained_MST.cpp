#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

// 定义边的结构体
struct Edge {
    int u, v;           // 连接的两个建筑物编号
    double cost;        // 布线成本
    double distance;    // 传输距离
    int id;             // 边的编号（用于追踪）
};

// 并查集结构，用于判断连通性
struct DSU {
    vector<int> parent;

    DSU(int n) {
        parent.resize(n);
        iota(parent.begin(), parent.end(), 0); // 初始化 parent[i] = i
    }

    // 查找根节点（带路径压缩）
    int find(int x) {
        if (x != parent[x])
            parent[x] = find(parent[x]);
        return parent[x];
    }

    // 合并两个集合
    bool unite(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);
        if (rootX != rootY) {
            parent[rootX] = rootY;
            return true; // 合并成功
        }
        return false; // 已经在同一集合，无需合并
    }
};

// 比较函数，按成本从小到大排序
bool compareEdges(const Edge& a, const Edge& b) {
    return a.cost < b.cost;
}

/**
 * 计算网络布线优化方案
 * @param numNodes 建筑物数量
 * @param edges 所有可能的路线
 * @param maxDistLimit 单段路线允许的最大传输距离
 */
void solveCabling(int numNodes, vector<Edge>& edges, double maxDistLimit) {
    // 1. 按成本排序
    sort(edges.begin(), edges.end(), compareEdges);

    DSU dsu(numNodes);
    vector<Edge> resultEdges;
    double totalCost = 0;
    int edgesCount = 0;

    cout << "--- 开始计算 (距离限制: " << maxDistLimit << ") ---" << endl;

    // 2. Kruskal 算法主循环
    for (const auto& edge : edges) {
        // 约束检查：如果距离超过限制，直接跳过
        if (edge.distance > maxDistLimit) {
            continue;
        }

        // 尝试合并
        if (dsu.unite(edge.u, edge.v)) {
            resultEdges.push_back(edge);
            totalCost += edge.cost;
            edgesCount++;
        }
    }

    // 3. 结果输出与验证
    if (edgesCount == numNodes - 1) {
        cout << "布线方案生成成功！" << endl;
        cout << "总布线成本: " << totalCost << endl;
        cout << "选用的路线详情:" << endl;
        cout << "路线ID\t起点\t终点\t成本\t距离" << endl;
        for (const auto& e : resultEdges) {
            cout << e.id << "\t" << e.u << "\t" << e.v << "\t"
                << e.cost << "\t" << e.distance << endl;
        }
    }
    else {
        cout << "错误：无法在限制距离内连通所有建筑物。" << endl;
        cout << "当前连通了 " << edgesCount << " 条边，需要 " << numNodes - 1 << " 条。" << endl;
        // 实际工程中可能需要提示用户增加最大距离限制
    }
    cout << "---------------------------------------" << endl;
}

int main() {
    // === 自行设计的网络结构测试用例 ===
    // 假设有 5 个建筑物 (节点 0-4)
    int numBuildings = 5;

    // 构建图的边 (起点, 终点, 成本, 距离, ID)
    // 场景设计：
    // - 存在一些成本极低但距离很长的线（测试约束过滤）
    // - 存在一些成本较高但距离短的线（测试备选方案）
    vector<Edge> allRoutes = {
        {0, 1, 10.0, 5.0, 1},
        {0, 2, 20.0, 8.0, 2},
        {1, 2, 15.0, 6.0, 3},
        {1, 3, 25.0,  12.0, 4},
        {2, 3, 30.0, 10.0, 5},
        {2, 4, 35.0, 15.0, 6},
        {3, 4, 40.0, 12.0, 7}
    };

    solveCabling(numBuildings, allRoutes, 200.0);

    // - 必须重新初始化边列表（因为sort改变了顺序，虽然这里传引用但不影响再次排序逻辑）
    vector<Edge> routesCopy = allRoutes;
    solveCabling(numBuildings, routesCopy, 12.0);

    routesCopy = allRoutes;
    solveCabling(numBuildings, routesCopy, 5.0);

    return 0;
}