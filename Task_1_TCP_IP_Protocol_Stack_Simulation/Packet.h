#ifndef PACKET_H
#define PACKET_H

#include <string>
#include <stack>
using namespace std;

// 协议层类型
enum LayerType {
    APPLICATION,
    TRANSPORT,
    NETWORK,
    DATALINK
};

// 各层头部结构
struct AppHeader {
    string appName = "MyApp";
};

struct TcpHeader {
    int srcPort = 12345;
    int dstPort = 80;
};

struct IpHeader {
    string srcIP = "192.168.1.10";
    string dstIP = "192.168.1.100";
};

struct MacHeader {
    string srcMac = "AA:BB:CC:DD:EE:FF";
    string dstMac = "11:22:33:44:55:66";
};

// Packet 数据包
struct Packet {
    string payload;               // 原始数据
    stack<string> headerStack;    // 模拟协议栈
};

#endif
