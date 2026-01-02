#include <iostream>
#include <vector>
#include <cstdlib> // 用于rand() and srand()
#include <ctime>   // 用于time()

using namespace std;

// 定义二叉树节点结构
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;

    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

// 插入节点到二叉排序树
TreeNode* insert(TreeNode* root, int val) {
    if (root == NULL) {
        return new TreeNode(val);
    }
    if (val < root->val) {
        root->left = insert(root->left, val);
    }
    else if (val >= root->val) { // 允许重复值，重复值放右边
        root->right = insert(root->right, val);
    }
    return root;
}

// 查找最小值节点（用于删除操作中寻找右子树的最小节点）
TreeNode* minValueNode(TreeNode* node) {
    TreeNode* current = node;
    while (current && current->left != NULL)
        current = current->left;
    return current;
}

// 删除节点
TreeNode* deleteNode(TreeNode* root, int key) {
    if (root == NULL) return root;

    // 1. 寻找要删除的节点
    if (key < root->val) {
        root->left = deleteNode(root->left, key);
    }
    else if (key > root->val) {
        root->right = deleteNode(root->right, key);
    }
    else {
        // 2. 找到节点，进行删除

        // 情况A: 只有一个子节点 或 没有子节点
        if (root->left == NULL) {
            TreeNode* temp = root->right;
            delete root;
            return temp;
        }
        else if (root->right == NULL) {
            TreeNode* temp = root->left;
            delete root;
            return temp;
        }

        // 情况B: 有两个子节点
        // 获取右子树中的最小节点（中序后继）
        TreeNode* temp = minValueNode(root->right);

        // 将后继节点的值复制到当前节点
        root->val = temp->val;

        // 删除右子树中的那个后继节点
        root->right = deleteNode(root->right, temp->val);
    }
    return root;
}

// 前序遍历 (根 -> 左 -> 右)
void preOrder(TreeNode* root) {
    if (root != NULL) {
        cout << root->val << " ";
        preOrder(root->left);
        preOrder(root->right);
    }
}

// 中序遍历 (左 -> 根 -> 右)
void inOrder(TreeNode* root) {
    if (root != NULL) {
        inOrder(root->left);
        cout << root->val << " ";
        inOrder(root->right);
    }
}

// 后序遍历 (左 -> 右 -> 根)
void postOrder(TreeNode* root) {
    if (root != NULL) {
        postOrder(root->left);
        postOrder(root->right);
        cout << root->val << " ";
    }
}

// 辅助函数：根据用户选择执行遍历
void selectChoice(TreeNode* root) {
    int choice;
    cout << "\n请选择遍历方式:\n";
    cout << "1. 前序遍历 (Pre-order)\n";
    cout << "2. 中序遍历 (In-order)\n";
    cout << "3. 后序遍历 (Post-order)\n";
    cout << "请输入选项 (1-3): ";
    cin >> choice;

    cout << "输出结果: ";
    switch (choice) {
    case 1:
        preOrder(root);
        break;
    case 2:
        inOrder(root);
        break;
    case 3:
        postOrder(root);
        break;
    default:
        cout << "无效选项，默认执行中序遍历: ";
        inOrder(root);
    }
    cout << endl;
}

// 查找节点是否存在（用于验证输入）
bool search(TreeNode* root, int key) {
    if (root == NULL) return false;
    if (root->val == key) return true;
    if (key < root->val) return search(root->left, key);
    return search(root->right, key);
}

int main() {
    srand((unsigned)time(0)); // 随机数种子

    int N;
    while (true) {
        cout << "请输入要生成的随机整数个数 N (N > 20): ";
        cin >> N;
        if (N > 20) break;
        cout << "输入无效，N 必须大于 20。" << endl;
    }

    TreeNode* root = NULL;
    vector<int> rawData;

    cout << "\n正在生成 " << N << " 个随机整数 (范围 1-100)...\n";
    cout << "原始数据: ";
    for (int i = 0; i < N; ++i) {
        int num = rand() % 100 + 1; // 生成1到100之间的随机数
        root = insert(root, num);
        rawData.push_back(num);
        cout << num << " ";
    }
    cout << "\n\n=== 二叉排序树建立完成 ===" << endl;

    // 第一次遍历选择
    selectChoice(root);

    // 删除操作
    int deleteKey;
    while (true) {
        cout << "\n请输入要删除的数据: ";
        cin >> deleteKey;
        if (search(root, deleteKey)) {
            break;
        }
        else {
            cout << "树中不存在该数据，请重新输入存在的数值。\n";
        }
    }

    root = deleteNode(root, deleteKey);
    cout << "已删除节点: " << deleteKey << endl;

    // 第二次遍历选择
    cout << "\n=== 删除后的二叉树 ===";
    selectChoice(root);

    cout << "\n程序结束。\n";
    system("pause"); // 防止窗口直接关闭
    return 0;
}