#pragma once

//strtok_s
#include <cstring>
#include "Define.h"
#define PACKET_SIZE 1024

#pragma comment(lib, "ws2_32.lib") //비주얼에서 소켓프로그래밍 하기 위한 것
using namespace std;

extern SOCKET soc;

/*로그인 하면 서버에서 부여받는 번호
	unsigned int login_number;*/

extern string realid;
extern string coutid;

//현재 아이디,비밀번호
extern string newid;
extern string newpassword;

extern int login_status;

// 새로운 친구 추가 요청 있는지 확인
extern bool new_friends_req;

// 전체 채팅 수신
void thr_recvs_party() {
	while (!WSAGetLastError()) {
		char buffer[PACKET_SIZE] = { 0 };
		recv(soc, buffer, PACKET_SIZE, 0);
		string temp;
		temp = buffer;
		if (temp == "10101") {
			cout << endl;
			cout << "서버가 연결을 종료 하였습니다. 나가시려면 10101을 입력해주세요" << endl;
			break;
		}
		cout << "받은 메세지 : " << buffer << endl;
	}
	
}




//void join() {
//	string sends = "2 ";
//
//	char* sendc = new char[sends.length() + 1];
//
//	cout << "가입하실 아이디를 입력해 주세요 : ";
//	cin >> newid;
//
//	cout << "가입하실 비밀번호를 입력해 주세요 : ";
//	cin >> newpassword; cout << endl;
//
//	sends += newid + " ";
//	sends += newpassword;
//	sendc[sends.length()] = '\n';
//	sends.copy(sendc, sends.length());
//
//	send(soc, sendc, PACKET_SIZE, 0);
//}


//void login() {
//	for (int i = 0; i < 5; i++) {
//
//		cout << "5번 실패시 메인 화면으로 돌아갑니다. 틀린횟수 : " << i << "번" << endl;
//		char buffer[PACKET_SIZE] = { 0 };
//		memset(&buffer, 0, sizeof(buffer));
//
//		//로그인용 임시 
//		string temp_id;
//		string temp_password;
//
//		cout << "아이디를 입력해주세요 : " ;
//		cin >> temp_id;
//		cout << "비밀번호를 입력해주세요 : ";
//		cin >> temp_password;
//		cout << endl;
//		//서버에서 istringstream으로 구분해서 받게 하기 위한 공백 추가
//
//		sends += (temp_id + " ");
//		sends += temp_password;
//
//		char* sendc = new char[sends.length() + 1];
//		sendc[sends.length()] = '\n';
//
//		//string char 배열로 바꿔서 보내기
//		sends.copy(sendc, sends.length());
//
//		send(soc, sendc, strlen(sendc), 0);
//		recv(soc, buffer, PACKET_SIZE, 0);
//		string login_num = buffer;
//		
//		if (login_num == "2") {
//			coutid = (temp_id + "님 안녕하세요");
//			realid = temp_id;
//			login_status = 1;
//			break;
//		}
//
//		else if(login_num == "0"){
//			cout << "없는 아이디 입니다. 다시 입력 해주세요" << endl;
//		}
//
//		else if (login_num == "1") {
//			cout << "비밀번호가 틀립니다. 다시 입력 해주세요" << endl;
//		}
//	}
//}
//
//string friends_list_all(string realid) {
//	string sends = "100 ";
//	sends += (realid);
//	char* sendc = new char[sends.length() + 1];
//	sendc[sends.length()] = '\n';
//	sends.copy(sendc, sends.length());
//	char buffer2[PACKET_SIZE] = { 0 };
//	memset(&buffer2, 0, sizeof(buffer2));
//	send(soc, sendc, strlen(sendc), 0);
//	recv(soc, buffer2, PACKET_SIZE, 0);
//	string temp = buffer2;
//	return temp;
//}
//
//void print_friends_list(char* friends, char* nulltext) {
//	int cnt = 1;
//	std::cout << "현재 접속중인 친구목록" << endl << endl;
//	std::cout << "=======================" << endl;
//	while (friends != NULL) {
//		string tempchar;
//
//		for (int i = 0; i < strlen(friends) - 1; i++) {
//			tempchar += friends[i];
//		}
//		if (friends[strlen(friends) - 1] == '1') {
//			std::cout << cnt++ << ". " << tempchar << "님 접속중입니다." << endl;
//		}
//		else {
//			std::cout << cnt++ << ". " << tempchar << "님 접속중이 아닙니다." << endl;
//		}
//		friends = strtok_s(NULL, " ", &nulltext);
//	}
//	std::cout << "=======================" << endl << endl;
//}
//
//void friends_find() {
//
//}

