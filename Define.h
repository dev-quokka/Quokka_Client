#pragma once
#define PACKET_SIZE 1024
#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>
#include <unordered_map>
#include <thread>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#define PACKET_SIZE 1024

enum class USER_STATE
{
	NONE = 0,
	LOGIN = 1,
	PARYTY = 2,
	PLAY = 3
};

struct FriendInfo {
	UINT32 userPkNum;
	std::string id;
	UINT8 userLevel;
	UINT16 partyIdx = 0;
	INT8 Check = 0; // 내가 검색할 땐 로그인 요청 확인, 친구 검색시에는 로그인 상태 확인
};

