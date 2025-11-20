#include "ProtocolStack.h"
#include <iostream>
using namespace std;

// ------------------ 封装 ------------------
void ProtocolStack::encapsulate(Packet& pkt) {
    cout << "开始封装数据包..." << endl;

    AppHeader app;
    pkt.headerStack.push("[Application Header] App=" + app.appName);

    TcpHeader tcp;
    pkt.headerStack.push("[TCP Header] SrcPort=" + to_string(tcp.srcPort)
        + " DstPort=" + to_string(tcp.dstPort));

    IpHeader ip;
    pkt.headerStack.push("[IP Header] Src=" + ip.srcIP +
        " Dst=" + ip.dstIP);

    MacHeader mac;
    pkt.headerStack.push("[MAC Header] Src=" + mac.srcMac +
        " Dst=" + mac.dstMac);

    cout << "封装完成，共压入 " << pkt.headerStack.size() << " 层头部\n";
}

// ------------------ 解封装 ------------------
void ProtocolStack::decapsulate(Packet pkt) {
    cout << "\n---- 解封装过程 START ----\n";

    while (!pkt.headerStack.empty()) {
        cout << "弹出头部： " << pkt.headerStack.top() << endl;
        pkt.headerStack.pop();
    }

    cout << "最终得到 Payload： " << pkt.payload << endl;
    cout << "---- 解封装 END ----\n" << endl;
}

// ------------------ 模拟发送 ------------------
void ProtocolStack::sendData(const string& data) {
    Packet pkt;
    pkt.payload = data;

    encapsulate(pkt);

    sendQueue.push(pkt);
    cout << "数据包已加入发送队列（队列长度：" << sendQueue.size() << "）\n\n";
}

// ------------------ 模拟接收 ------------------
void ProtocolStack::receiveData() {
    if (sendQueue.empty()) {
        cout << "发送队列为空，无数据可接收。\n\n";
        return;
    }

    Packet pkt = sendQueue.front();
    sendQueue.pop();

    cout << "数据包从发送队列出队（剩余：" << sendQueue.size() << "）\n";

    decapsulate(pkt);
}
