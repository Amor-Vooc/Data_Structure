#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <chrono>   // 用于计时
#include <random>   // 用于生成随机数据
#include <iomanip>  // 格式化输出
#include <fstream>  // 写入文件

using namespace std;

// 定义最大数据量
const int DATA_COUNT = 100000;

// 定义哈希表大小
// 为了体现差异：
// 1. 线性探测法通常需要表大于数据量 (装载因子 < 0.8)，这里选一个略大于10w的质数
const int TABLE_SIZE_PROBING = 1299709; // 稍微大一点以避免死循环，保证效率
// 2. 拉链法可以使用正好约10w的大小
const int TABLE_SIZE_CHAINING = 100003;

// 用户结构体
struct User {
    long long qq;
    string phone;
    bool isEmpty; // 用于开放定址法标记位置是否为空

    User() : qq(0), phone(""), isEmpty(true) {}
    User(long long q, string p) : qq(q), phone(p), isEmpty(false) {}
};

// ==========================================
// 策略 1: 开放定址法 (线性探测)
// ==========================================
class HashTableLinearProbing {
private:
    vector<User> table;
    int size;
    int count;

public:
    HashTableLinearProbing(int s) : size(s), count(0) {
        table.resize(size);
    }

    // 哈希函数
    int hashFunction(long long key) {
        return key % size;
    }

    // 插入
    void insert(long long qq, string phone) {
        if (count >= size) {
            // 实际工程中这里应该扩容 (Rehash)
            return;
        }
        int idx = hashFunction(qq);
        int startIdx = idx;

        // 线性探测：如果当前位置有人，且不是要更新的同一个QQ，则往后找
        while (!table[idx].isEmpty) {
            if (table[idx].qq == qq) { // 也就是更新
                table[idx].phone = phone;
                return;
            }
            idx = (idx + 1) % size; // 循环数组
            if (idx == startIdx) return; // 表满了
        }

        // 找到空位
        table[idx] = User(qq, phone);
        count++;
    }

    // 查找
    int search(long long qq, User& result) {
        int idx = hashFunction(qq);
        int startIdx = idx;
        int comparisons = 0; // 统计比较次数用于分析性能

        while (!table[idx].isEmpty) {
            comparisons++;
            if (table[idx].qq == qq) {
                result = table[idx];
                return comparisons;
            }
            idx = (idx + 1) % size;
            if (idx == startIdx) break; // 绕了一圈没找到
        }
        return -1; // 未找到
    }
};

// ==========================================
// 策略 2: 拉链法 (链地址法)
// ==========================================
class HashTableChaining {
private:
    // 数组中每个元素是一个链表
    vector<list<User>> table;
    int size;

public:
    HashTableChaining(int s) : size(s) {
        table.resize(size);
    }

    int hashFunction(long long key) {
        return key % size;
    }

    void insert(long long qq, string phone) {
        int idx = hashFunction(qq);
        // 检查链表中是否已存在（更新操作）
        for (auto& u : table[idx]) {
            if (u.qq == qq) {
                u.phone = phone;
                return;
            }
        }
        // 插入到链表头部
        table[idx].emplace_back(qq, phone);
    }

    int search(long long qq, User& result) {
        int idx = hashFunction(qq);
        int comparisons = 0;

        for (auto& u : table[idx]) {
            comparisons++;
            if (u.qq == qq) {
                result = u;
                return comparisons;
            }
        }
        return -1; // 未找到
    }
};

// ==========================================
// 辅助工具：生成随机手机号
// ==========================================
string generatePhone() {
    static random_device rd;
    static mt19937 gen(rd());
    static uniform_int_distribution<> dis(0, 9);
    string phone = "1";
    for (int i = 0; i < 10; ++i) phone += to_string(dis(gen));
    return phone;
}

int main() {
    // 1. 准备测试数据
    cout << ">>> 正在生成 " << DATA_COUNT << " 条随机测试数据..." << endl;
    vector<pair<long long, string>> dataset;
    dataset.reserve(DATA_COUNT);

    // 随机数生成器 (QQ范围 10000 ~ 999999999)
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<long long> qqDis(10000, 999999999);

    for (int i = 0; i < DATA_COUNT; ++i) {
        dataset.push_back({ qqDis(gen), generatePhone() });
    }

    // 选取最后一个生成的QQ作为测试查找目标，保证一定能找到
    long long searchTargetQQ = dataset.back().first;
    // 选取一个不存在的QQ测试失败查找
    long long failTargetQQ = 123;

    cout << ">>> 数据生成完毕.\n" << endl;

    // 将生成的数据写入当前目录下的文件
    {
        const string outFileName = "qq_dataset.txt";
        ofstream out(outFileName);
        if (out) {
            for (const auto& item : dataset) {
                out << item.first << ' ' << item.second << '\n';
            }
            out.close();
            cout << "已将测试数据写入文件: " << outFileName << endl;
        }
        else {
            cerr << "写文件失败: 无法打开 " << outFileName << " 进行写入。" << endl;
        }
    }

    // ==========================================
    // 测试 1: 线性探测法
    // ==========================================
    {
        cout << "=== 测试 1: 开放定址法 (线性探测) ===" << endl;
        HashTableLinearProbing ht(TABLE_SIZE_PROBING); // 注意：线性探测表大小通常要大于数据量

        // 计时插入
        auto start = chrono::high_resolution_clock::now();
        for (const auto& item : dataset) {
            ht.insert(item.first, item.second);
        }
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> elapsed = end - start;
        cout << "插入耗时: " << elapsed.count() << " ms" << endl;

        // 查找测试
        User res;
        int comps = ht.search(searchTargetQQ, res);
        if (comps != -1) {
            cout << "查找成功 -> QQ: " << res.qq << " | Phone: " << res.phone << endl;
            cout << "查找比较次数: " << comps << endl;
        }
        else {
            cout << "查找失败" << endl;
        }
    }

    cout << endl;

    // ==========================================
    // 测试 2: 拉链法
    // ==========================================
    {
        cout << "=== 测试 2: 拉链法 (链地址法) ===" << endl;
        HashTableChaining ht(TABLE_SIZE_CHAINING);

        // 计时插入
        auto start = chrono::high_resolution_clock::now();
        for (const auto& item : dataset) {
            ht.insert(item.first, item.second);
        }
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> elapsed = end - start;
        cout << "插入耗时: " << elapsed.count() << " ms" << endl;

        // 查找测试
        User res;
        int comps = ht.search(searchTargetQQ, res);
        if (comps != -1) {
            cout << "查找成功 -> QQ: " << res.qq << " | Phone: " << res.phone << endl;
            cout << "查找比较次数: " << comps << endl;
        }
        else {
            cout << "查找失败" << endl;
        }
    }

    // ==========================================
    // 用户交互测试
    // ==========================================
    cout << "\n>>> 请输入一个要查找的QQ号 (输入0退出): ";
    long long inputQQ;
    // 为演示，重新构建两个哈希表用于交互：一个线性探测，一个拉链法
    HashTableLinearProbing demoLp(TABLE_SIZE_PROBING);
    HashTableChaining demoHt(TABLE_SIZE_CHAINING);
    for (const auto& item : dataset) {
        demoLp.insert(item.first, item.second);
        demoHt.insert(item.first, item.second);
    }

    while (cin >> inputQQ && inputQQ != 0) {
        // 线性探测法查找
        User u1;
        int comps1 = demoLp.search(inputQQ, u1);
        cout << "=== 开放定址法 (线性探测) ===" << endl;
        if (comps1 != -1) {
            cout << "查找成功 -> QQ: " << u1.qq << " | Phone: " << u1.phone << endl;
            cout << "查找比较次数: " << comps1 << endl;
        }
        else {
            cout << "查找失败" << endl;
        }

        // 拉链法查找
        User u2;
        int comps2 = demoHt.search(inputQQ, u2);
        cout << "=== 拉链法 (链地址法) ===" << endl;
        if (comps2 != -1) {
            cout << "查找成功 -> QQ: " << u2.qq << " | Phone: " << u2.phone << endl;
            cout << "查找比较次数: " << comps2 << endl << endl;
        }
        else {
            cout << "查找失败\n" << endl;
        }

        cout << ">>> 继续输入 (0退出): ";
    }

    return 0;
}