#include "PacketManager.h"

void PacketManager::EnqueuePacketData(char* recvData_) {
	std::lock_guard<std::mutex> guard(dLock);
	RecvPacket.push_back(recvData_);
}

USER_STATE PacketManager::login(SOCKET soc_, USER_STATE UserState_) {
	for (int i = 0; i < 5; i++) {

		std::cout << "5�� ���н� ���� ȭ������ ���ư��ϴ�. Ʋ��Ƚ�� : " << i << "��" << std::endl;
		char rcvBuffer[PACKET_SIZE];
		memset(rcvBuffer,0, PACKET_SIZE);

		LOGIN_REQUEST_PACKET lrp;
		lrp.PacketId = (UINT32)PACKET_ID::LOGIN_REQUEST;
		lrp.PacketLength = sizeof(LOGIN_REQUEST_PACKET);

		char id[MAX_USER_ID_LEN + 1];
		char pass[MAX_USER_ID_LEN + 1];

		std::cout << "���̵� �Է����ּ��� : ";
		std::cin >> id;
		std::cout << "��й�ȣ�� �Է����ּ��� : ";
		std::cin >> pass;
		std::cout << std::endl;

		strcpy_s(lrp.UserID, MAX_USER_ID_LEN + 1,id);
		strcpy_s(lrp.UserPW,MAX_USER_ID_LEN + 1,pass);

		std::cout << lrp.UserID << std::endl;
		std::cout << lrp.UserPW << std::endl;

		send(soc_, (char*)&lrp, sizeof(LOGIN_REQUEST_PACKET), 0);
		recv(soc_, rcvBuffer, PACKET_SIZE, 0);

		auto LoginDBResult = reinterpret_cast<LOGIN_RESPONSE_PACKET*>(rcvBuffer);
		auto result = LoginDBResult->LoginResult;

		if (result == (UINT16)0) {
			std::cout << "�α��� ����" << std::endl;
			UserState_ = USER_STATE::LOGIN;
			memset(rcvBuffer, 0, PACKET_SIZE);
			recv(soc_, rcvBuffer, PACKET_SIZE, 0);

			auto MyFriendDBResult = reinterpret_cast<FIND_FRIENDS_RESPONSE*>(rcvBuffer);
			INT8 &cnt = MyFriendDBResult->friendInfo->Check;

			Friends[MyFriendDBResult->friendInfo->userPkNum] = MyFriendDBResult->friendInfo;

			for (INT8 i = 0; i < cnt - 1; i++) {
				memset(rcvBuffer, 0, PACKET_SIZE);
				recv(soc_, rcvBuffer, PACKET_SIZE, 0);
				auto MyFriendsDBResult = reinterpret_cast<FIND_FRIENDS_RESPONSE*>(rcvBuffer);
				Friends[MyFriendsDBResult->friendInfo->userPkNum] = MyFriendsDBResult->friendInfo;
			}

			return USER_STATE::LOGIN;
		}

		else if (result == (UINT16)31) {
			std::cout << "���� ���� ������" << std::endl;
			continue;
		}

		else if(result == (UINT16)32) {
			std::cout << "���̵� �ٸ�" << std::endl;
			continue;
		}

		else if (result == (UINT16)33) {
			std::cout << "��� �ٸ�" << std::endl;
			continue;
		}
	}
	return USER_STATE::NONE;
}