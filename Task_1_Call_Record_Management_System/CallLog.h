#pragma once
#include "CallRecord.h"
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

class CallLog {
private:
    CallRecord* head;
    CallRecord* tail;

public:
    CallLog() : head(nullptr), tail(nullptr) {}

    // 按时间顺序插入（链表尾部是最新记录）
    void insertRecord(CallRecord* record) {
        if (!head) {  // 空链表
            head = tail = record;
            return;
        }

        // 时间顺序插入（小到大）
        CallRecord* cur = head;
        while (cur && cur->timestamp < record->timestamp) {
            cur = cur->next;
        }

        if (!cur) {
            // 插入到末尾
            record->prev = tail;
            tail->next = record;
            tail = record;
        }
        else if (cur == head) {
            // 插入头部
            record->next = head;
            head->prev = record;
            head = record;
        }
        else {
            // 插入中间
            CallRecord* p = cur->prev;
            p->next = record;
            record->prev = p;

            record->next = cur;
            cur->prev = record;
        }
    }

    // 删除记录（按号码 + 时间）
    bool deleteRecord(const string& number, time_t t) {
        CallRecord* cur = head;

        while (cur) {
            if (cur->number == number && cur->timestamp == t) {

                if (cur == head) head = cur->next;
                if (cur == tail) tail = cur->prev;
                if (cur->prev) cur->prev->next = cur->next;
                if (cur->next) cur->next->prev = cur->prev;

                delete cur;
                return true;
            }
            cur = cur->next;
        }
        return false;
    }

    // 删除指定索引的记录（从0开始）
    bool deleteRecordByIndex(int index) {
        CallRecord* r = getRecordByIndex(index);
        if (!r) return false;
        return deleteRecord(r->number, r->timestamp);
    }

    // 从旧到新遍历
    void traverseForward() {
        cout << "\n【向前遍历：旧 → 新】\n";
        CallRecord* cur = head;
        while (cur) {
            printRecord(cur);
            cur = cur->next;
        }
    }

    // 从新到旧遍历
    void traverseBackward() {
        cout << "\n【向后遍历：新 → 旧】\n";
        CallRecord* cur = tail;
        while (cur) {
            printRecord(cur);
            cur = cur->prev;
        }
    }

    // 按类型查找最近(最新时间)的记录
    CallRecord* findLatestByType(CallType type) {
        CallRecord* cur = tail;  // 从最新开始找
        while (cur) {
            if (cur->type == type) return cur;
            cur = cur->prev;
        }
        return nullptr;
    }

    // 打印单条记录
    void printRecord(CallRecord* r) {
        tm* t = localtime(&r->timestamp);

        cout << "号码: " << r->number
            << " | 时间: " << put_time(t, "%Y-%m-%d %H:%M:%S")
            << " | 类型: " << TypeToString(r->type)
            << " | 时长: " << r->duration << " 秒\n";
    }

    // 打印所有记录并带索引
    void printAllWithIndex() {
        cout << "\n索引 | 记录信息\n";
        int idx = 0;
        CallRecord* cur = head;
        while (cur) {
            cout << setw(4) << idx << " : ";
            printRecord(cur);
            cur = cur->next;
            ++idx;
        }
        if (idx == 0) cout << "(无记录)\n";
    }

    // 根据索引获取记录指针（从0开始）
    CallRecord* getRecordByIndex(int index) {
        if (index < 0) return nullptr;
        int idx = 0;
        CallRecord* cur = head;
        while (cur) {
            if (idx == index) return cur;
            cur = cur->next;
            ++idx;
        }
        return nullptr;
    }

    // 打印指定类型的所有记录
    void printAllByType(CallType type) {
        cout << "\n【按类型列出记录：" << TypeToString(type) << "】\n";
        int cnt = 0;
        CallRecord* cur = head;
        while (cur) {
            if (cur->type == type) {
                printRecord(cur);
                ++cnt;
            }
            cur = cur->next;
        }
        if (cnt == 0) cout << "(未找到该类型的记录)\n";
    }
};
