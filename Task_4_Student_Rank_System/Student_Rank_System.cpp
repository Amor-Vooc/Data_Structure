#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

// 定义学生节点结构体
struct Student {
    string id;      // 学号
    string name;    // 姓名
    double score;   // 成绩
    Student* next;  // 下一个节点指针

    Student(string i, string n, double s) : id(i), name(n), score(s), next(nullptr) {}
};

class StudentManager {
private:
    Student* head; // 链表头指针

public:
    StudentManager() : head(nullptr) {}

    // 析构函数，释放内存
    ~StudentManager() {
        Student* current = head;
        while (current != nullptr) {
            Student* nextNode = current->next;
            delete current;
            current = nextNode;
        }
    }

    // 1. 按学号排序建立学生信息链表 (插入排序逻辑)
    void addStudent(string id, string name, double score) {
        Student* newNode = new Student(id, name, score);

        // 如果链表为空，或新学号小于头节点学号，插入头部
        if (head == nullptr || head->id > id) {
            newNode->next = head;
            head = newNode;
            return;
        }

        // 寻找插入位置
        Student* current = head;
        while (current->next != nullptr && current->next->id < id) {
            current = current->next;
        }

        // 插入节点
        newNode->next = current->next;
        current->next = newNode;
    }

    // 辅助函数：交换两个节点的数据（用于排序）
    void swapData(Student* a, Student* b) {
        swap(a->id, b->id);
        swap(a->name, b->name);
        swap(a->score, b->score);
    }

    // 3. 按成绩降序排序（成绩相同时按姓名升序排序）
    void sortByScore() {
        if (head == nullptr || head->next == nullptr) return;

        bool swapped;
        Student* ptr1;
        Student* lptr = nullptr;

        // 使用冒泡排序交换数据
        do {
            swapped = false;
            ptr1 = head;

            while (ptr1->next != lptr) {
                bool needSwap = false;

                // 成绩降序
                if (ptr1->score < ptr1->next->score) {
                    needSwap = true;
                }
                // 成绩相同，按姓名升序
                else if (ptr1->score == ptr1->next->score) {
                    if (ptr1->name > ptr1->next->name) {
                        needSwap = true;
                    }
                }

                if (needSwap) {
                    swapData(ptr1, ptr1->next);
                    swapped = true;
                }
                ptr1 = ptr1->next;
            }
            lptr = ptr1;
        } while (swapped);

        cout << "链表已按成绩降序(同分按姓名升序)重新排序。" << endl;
    }

    // 2. 查找成绩前N名并输出相关信息（处理并列名次）
    void printTopN(int n) {
        if (head == nullptr) {
            cout << "链表为空。" << endl;
            return;
        }

        // 输出前必须先按照成绩排序
        sortByScore();

        cout << "\n--- 成绩前 " << n << " 名名单 ---" << endl;
        cout << left << setw(10) << "名次"
            << setw(15) << "学号"
            << setw(15) << "姓名"
            << setw(10) << "成绩" << endl;
        cout << string(50, '-') << endl;

        Student* current = head;
        int count = 0;
        int rank = 1;
        int realRank = 1; // 实际序数（1,2,3,4...）
        double prevScore = -1.0;

        while (current != nullptr && count < n) {
            // 处理并列排名逻辑
            if (current->score != prevScore) {
                rank = realRank; // 分数不同，名次等于当前的实际序数
            }
            // 分数相同则 rank 保持不变

            cout << left << setw(10) << rank
                << setw(15) << current->id
                << setw(15) << current->name
                << setw(10) << current->score << endl;

            prevScore = current->score;
            current = current->next;
            realRank++;
            count++;
        }

        if (current != nullptr) {
            cout << "... (后续学生省略)" << endl;
        }
        else if (count < n) {
            cout << "(学生人数少于 " << n << " 人，已显示全部)" << endl;
        }
    }

    // 打印整个链表
    void printAll() {
        if (head == nullptr) {
            cout << "当前没有学生信息。" << endl;
            return;
        }
        cout << "\n--- 所有学生信息 ---" << endl;
        cout << left << setw(15) << "学号"
            << setw(15) << "姓名"
            << setw(10) << "成绩" << endl;
        cout << string(40, '-') << endl;

        Student* current = head;
        while (current != nullptr) {
            cout << left << setw(15) << current->id
                << setw(15) << current->name
                << setw(10) << current->score << endl;
            current = current->next;
        }
        cout << endl;
    }
};

int main() {
    StudentManager sm;
    int choice;

    // 预存一些数据方便测试
    sm.addStudent("2023211283", "ZhangSan", 88);
    sm.addStudent("2023211281", "LiSi", 95);
    sm.addStudent("2023211285", "WangWu", 88); // 与 ZhangSan 同分
    sm.addStudent("2023211282", "ZhaoLiu", 95); // 与 LiSi 同分，且学号在中间
    sm.addStudent("2023211284", "QianQi", 70);

    do {
        cout << "\n============ 学生信息管理系统 ============" << endl;
        cout << "1. 添加学生 (自动按学号排序)" << endl;
        cout << "2. 显示所有学生" << endl;
        cout << "3. 按成绩降序排序 (同分按姓名)" << endl;
        cout << "4. 输出前 N 名学生 (含并列排名)" << endl;
        cout << "0. 退出" << endl;
        cout << "请输入选项: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            string id, name;
            double score;
            cout << "请输入学号: "; cin >> id;
            cout << "请输入姓名: "; cin >> name;
            cout << "请输入成绩: "; cin >> score;
            sm.addStudent(id, name, score);
            cout << "添加成功！" << endl;
            break;
        }
        case 2:
            sm.printAll();
            break;
        case 3:
            sm.sortByScore();
            sm.printAll();
            break;
        case 4: {
            int n;
            cout << "请输入需要查找的前几名 (N): ";
            cin >> n;
            sm.printTopN(n);
            break;
        }
        case 0:
            cout << "退出系统。" << endl;
            break;
        default:
            cout << "无效选项，请重试。" << endl;
        }
    } while (choice != 0);

    return 0;
}