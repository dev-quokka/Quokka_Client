#include "PacketManager.h"

void PacketManager::EnqueuePacketData(char* recvData_) {
	std::lock_guard<std::mutex> guard(dLock);
	RecvPacket.push_back(recvData_);
}

USER_STATE PacketManager::login(SOCKET soc_, USER_STATE UserState_) {
	for (int i = 0; i < 5; i++) {

		std::cout << "5번 실패시 메인 화면으로 돌아갑니다. 틀린횟수 : " << i << "번" << std::endl;
		char buffer[PACKET_SIZE];

		LOGIN_REQUEST_PACKET lrp;
		lrp.PacketId = (UINT32)PACKET_ID::LOGIN_REQUEST;
		lrp.PacketLength = sizeof(LOGIN_REQUEST_PACKET);

		std::cout << "아이디를 입력해주세요 : ";
		std::cin >> lrp.UserID;
		std::cout << "비밀번호를 입력해주세요 : ";
		std::cin >> lrp.UserPW;
		std::cout << std::endl;

		PacketInfo packetInfo;

		packetInfo.PacketId = (UINT32)PACKET_ID::LOGIN_REQUEST;
		packetInfo.DataSize = sizeof(LOGIN_REQUEST_PACKET);
		packetInfo.pDataPtr = (char*)&lrp;

		send(soc_, (char*)&packetInfo, sizeof(packetInfo), 0);
		recv(soc_, buffer, PACKET_SIZE, 0);

		auto LoginDBResult = reinterpret_cast<LOGIN_RESPONSE_PACKET*>(buffer);

		if (LoginDBResult->LoginResult == (UINT16)0) {
			std::cout << "로그인 성공" << std::endl;
			UserState_ = USER_STATE::LOGIN;
			return USER_STATE::LOGIN;
		}

		else if (LoginDBResult->LoginResult == (UINT16)31) {
			std::cout << "서버 유저 가득참" << std::endl;
			continue;
		}

		else if(LoginDBResult->LoginResult == (UINT16)32) {
			std::cout << "아이디 다름" << std::endl;
			continue;
		}

		else if (LoginDBResult->LoginResult == (UINT16)33) {
			std::cout << "비번 다름" << std::endl;
			continue;
		}
	}
	return USER_STATE::NONE;
}