#pragma once
#include <thread>
#include <mutex>
#include <deque>

#include "Packet.h"
#include "Define.h"

class PacketManager {
public:
	
	void EnqueuePacketData(char* recvPakcet_);

	USER_STATE login(SOCKET soc_, USER_STATE UserState_);

private:

	std::unordered_map<UINT32, FriendInfo*> Friends;
	std::mutex dLock;
	std::deque<char*> RecvPacket;
};
