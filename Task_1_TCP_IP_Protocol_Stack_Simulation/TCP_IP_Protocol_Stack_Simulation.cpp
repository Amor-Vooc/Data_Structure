#include <iostream>
#include <cstdlib>
#include "ProtocolStack.h"

using namespace std;

void CleanDOS() {
    system("pause");
    system("cls"); // 清空控制台屏幕
}

int main() {
    ProtocolStack stack;

    cout << "=== 模拟 TCP/IP 协议栈封装与解封装 ===\n\n";

    // 示例演示
	cout << "--- 示例演示 ---\n";
	cout << "发送内容 Hello, TCP/IP!\n";
    stack.sendData("Hello, TCP/IP!");
	cout << "发送内容 Second Packet Example\n";
    stack.sendData("Second Packet Example");

    stack.receiveData();
    stack.receiveData();
    stack.receiveData();  // 队列空

    CleanDOS();

    // --- 在示例后通过菜单调用 ---
    while (true) {
        cout << "----- 菜单 -----\n";
        cout << "1. 发送数据\n";
        cout << "2. 接收数据\n";
        cout << "0. 退出\n";
        cout << "\n请选择操作: ";

        string choice;
        if (!getline(cin, choice)) break;

        if (choice == "1") {
            cout << "请输入要发送的数据: ";
            string data;
            if (!getline(cin, data)) break;
            stack.sendData(data);
            CleanDOS();
        } else if (choice == "2") {
            stack.receiveData();
			CleanDOS();
        } else if (choice == "0") {
            cout << "退出程序。\n";
            break;
        } else {
            cout << "无效选择，请重试。\n";
			CleanDOS();
        }
    }

    return 0;
}
