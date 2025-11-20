#ifndef PROTOCOLSTACK_H
#define PROTOCOLSTACK_H

#include <queue>
#include <string>
#include "Packet.h"

class ProtocolStack {
private:
    std::queue<Packet> sendQueue;

public:
    void encapsulate(Packet& pkt);
    void decapsulate(Packet pkt);

    void sendData(const std::string& data);
    void receiveData();
};

#endif
