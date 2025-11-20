#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <windows.h>
#include <string>
#include <limits>
#include <cstdlib>
#include "CallLog.h"

using namespace std;

void CleanDOS() {
    system("pause");
    system("cls"); // 清空控制台屏幕
}

int main() {
    CallLog log;

    // 一开始填充几条示例记录
    log.insertRecord(new CallRecord("13800008888", INCOMING, 35));
    Sleep(1000);
    log.insertRecord(new CallRecord("19912345678", MISSED, 0));
    Sleep(1000);
    log.insertRecord(new CallRecord("18019582857", MISSED, 0));
    Sleep(1000);
    log.insertRecord(new CallRecord("15566667777", OUTGOING, 120));

    while (true) {
        cout << "===== 通话记录管理系统 =====\n";
        cout << "1. 显示所有通话记录（按时间）\n";
        cout << "2. 插入记录\n";
        cout << "3. 删除指定记录（按索引）\n";
        cout << "4. 向前遍历（旧 -> 新）\n";
        cout << "5. 向后遍历（新 -> 旧）\n";
        cout << "6. 分类型查找所有记录\n";
        cout << "0. 退出\n";
        cout << "请选择: ";

        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            string _tmp;
            getline(cin, _tmp);
            continue;
        }

        if (choice == 0) break;

        switch (choice) {
        case 1:
            log.printAllWithIndex();
            CleanDOS();
            break;
        case 2: {
            string number;
            int typ;
            int dur;
            cout << "输入电话号码: ";
            cin >> number;
            cout << "选择类型 (0 来电, 1 去电, 2 未接): ";
            cin >> typ;
            cout << "输入时长(秒): ";
            cin >> dur;
            if (typ < 0 || typ > 2) {
                cout << "类型无效，操作取消。\n";
                CleanDOS();
                break;
            }
            CallType ct = static_cast<CallType>(typ);
            log.insertRecord(new CallRecord(number, ct, dur));
            cout << "插入成功。\n";
            CleanDOS();
            break;
        }
        case 3: {
            log.printAllWithIndex();
            cout << "输入要删除的记录索引: ";
            int idx; cin >> idx;
            if (log.deleteRecordByIndex(idx)) cout << "删除成功。\n";
            else cout << "删除失败：索引无效。\n";
            CleanDOS();
            break;
        }
        case 4:
            log.traverseForward();
            CleanDOS();
            break;
        case 5:
            log.traverseBackward();
            CleanDOS();
            break;
        case 6: {
            cout << "选择类型列出所有记录 (0 来电, 1 去电, 2 未接): ";
            int typ; cin >> typ;
            if (typ < 0 || typ > 2) { cout << "类型无效。\n"; CleanDOS();  break; }
            log.printAllByType(static_cast<CallType>(typ));
            CleanDOS();
            break;
        }
        default:
            cout << "无效选择。\n";
            CleanDOS();
            break;
        }
    }

    cout << "退出程序。\n";
    return 0;
}
