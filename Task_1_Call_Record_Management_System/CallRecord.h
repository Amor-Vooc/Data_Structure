#pragma once
#include <iostream>
#include <string>
#include <ctime>

using namespace std;

enum CallType {
    INCOMING,     // 来电
    OUTGOING,     // 去电
    MISSED        // 未接
};

string TypeToString(CallType t) {
    switch (t) {
    case INCOMING: return "来电";
    case OUTGOING: return "去电";
    case MISSED: return "未接";
    default: return "未知";
    }
}

struct CallRecord {
    string number;      // 电话号码
    time_t timestamp;   // 时间戳
    CallType type;      // 通话类型
    int duration;       // 时长（秒）

    CallRecord* prev;
    CallRecord* next;

    CallRecord(string n, CallType t, int d)
        : number(n), type(t), duration(d), prev(nullptr), next(nullptr)
    {
        timestamp = time(nullptr); // 自动记录当前系统时间
    }
};
