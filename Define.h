#pragma once
#define PACKET_SIZE 1024
#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>

#include <thread>
#include <iostream>
#include <sstream>
#include <string>
#define PACKET_SIZE 1024

enum class USER_STATE
{
	NONE = 0,
	LOGIN = 1,
	PARYTY = 2,
	PLAY = 3
};
