#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
#include <vector>
#include <iomanip>

using namespace std;

// 哈夫曼树节点结构体
struct Node {
    char ch;                // 存储字符
    int freq;               // 存储频率
    Node* left, * right;     // 左右子节点指针

    Node(char character, int frequency) {
        ch = character;
        freq = frequency;
        left = right = nullptr;
    }
};

// 用于优先队列的比较器（构建最小堆）
struct Compare {
    bool operator()(Node* l, Node* r) {
        // 频率小的优先级高（排在队头）
        return l->freq > r->freq;
    }
};

// 递归遍历哈夫曼树，生成编码
void generateCodes(Node* root, string str, unordered_map<char, string>& huffmanCode) {
    if (root == nullptr)
        return;

    // 如果是叶子节点（没有左右孩子），则找到了一个字符
    if (!root->left && !root->right) {
        huffmanCode[root->ch] = str;
    }

    // 递归左子树，编码添加 '0'
    generateCodes(root->left, str + "0", huffmanCode);
    // 递归右子树，编码添加 '1'
    generateCodes(root->right, str + "1", huffmanCode);
}

// 释放内存
void deleteTree(Node* root) {
    if (root == nullptr) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

// 主逻辑函数
void buildHuffmanTree(string text) {
    // 1. 统计字符频率
    unordered_map<char, int> freqMap;
    for (char c : text) {
        freqMap[c]++;
    }

    // 输出字符频率表
    cout << "----------------------------------------" << endl;
    cout << "Step 1: 字符频率统计表" << endl;
    cout << "----------------------------------------" << endl;
    cout << left << setw(10) << "Char" << setw(10) << "Freq" << endl;
    for (auto pair : freqMap) {
        // 为了显示清晰，处理换行符和空格的显示
        string charDisplay;
        if (pair.first == ' ') charDisplay = "' '";
        else if (pair.first == '\n') charDisplay = "\\n";
        else charDisplay = string(1, pair.first);

        cout << left << setw(10) << charDisplay << setw(10) << pair.second << endl;
    }
    cout << "----------------------------------------" << endl << endl;

    // 2. 将所有字符节点放入优先队列
    priority_queue<Node*, vector<Node*>, Compare> pq;
    for (auto pair : freqMap) {
        pq.push(new Node(pair.first, pair.second));
    }

    // 3. 构建哈夫曼树
    // 当队列中节点数多于1个时循环
    while (pq.size() != 1) {
        // 取出频率最小的两个节点
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();

        // 创建一个新的内部节点，频率为两个子节点之和
        // '$' 占位，不存储实际文本字符
        int sum = left->freq + right->freq;
        Node* top = new Node('$', sum);

        top->left = left;
        top->right = right;

        // 将新节点放回队列
        pq.push(top);
    }

    // 剩下的最后一个节点就是根节点
    Node* root = pq.top();

    // 4. 生成哈夫曼编码
    unordered_map<char, string> huffmanCode;
    generateCodes(root, "", huffmanCode);

    // 输出哈夫曼编码表
    cout << "----------------------------------------" << endl;
    cout << "Step 2: 哈夫曼编码表" << endl;
    cout << "----------------------------------------" << endl;
    cout << left << setw(10) << "Char" << setw(15) << "Code" << endl;
    for (auto pair : huffmanCode) {
        string charDisplay;
        if (pair.first == ' ') charDisplay = "' '";
        else if (pair.first == '\n') charDisplay = "\\n";
        else charDisplay = string(1, pair.first);

        cout << left << setw(10) << charDisplay << setw(15) << pair.second << endl;
    }
    cout << "----------------------------------------" << endl << endl;

    // 5. 显示原始字符串的编码结果
    cout << "----------------------------------------" << endl;
    cout << "Step 3: 原始字符串编码结果:" << endl;
    cout << "----------------------------------------" << endl;
    string encodedString = "";
    for (char c : text) {
        encodedString += huffmanCode[c];
    }
    cout << encodedString << endl << endl;

    // 计算压缩比信息
    int originalBits = text.length() * 8;
    int compressedBits = encodedString.length();
    cout << "原始大小: " << originalBits << " bits" << endl;
    cout << "压缩后大小: " << compressedBits << " bits" << endl;
    cout << "压缩率: " << (float)compressedBits / originalBits * 100 << "%" << endl;

    // 清理内存
    deleteTree(root);
}

int main() {
    // 测试数据实例
    string text = "Programmers are perpetual optimists. Most of them think that the way to write a program is to run to the keyboard and start typing. Shortly thereafter the fully debugged program is finished.";
    cout << "测试数据实例: " << endl << "\"" << text << "\"" << endl << endl;
    buildHuffmanTree(text);

	// 用户输入字符串
	cout << "\n请输入一个字符串以构建哈夫曼树: ";
    string input;
    getline(cin, input);
    buildHuffmanTree(input);

    return 0;
}