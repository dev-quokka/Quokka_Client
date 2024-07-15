
#include <winsock2.h>
#include <thread>
#include <iostream>
#include <ws2tcpip.h>
#include <sstream>
#include <string>

#include "헤더.h"

using namespace std;

 SOCKET soc;

/*로그인 하면 서버에서 부여받는 번호
	unsigned int login_number;*/

 string realid;
 string coutid = "로그인을 해주세요.";

//현재 아이디,비밀번호
 string newid;
 string newpassword;

 int login_status = 0;

// 새로운 친구 추가 요청 있는지 확인
 bool new_friends_req = false;

int main() {

	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	soc = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	cout << "서버 연결 대기중..." << endl << endl;

	SOCKADDR_IN addr = {};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(9000);
	//addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr.s_addr);

	while (1) {
		if (!connect(soc, (SOCKADDR*)&addr, sizeof(addr))) {
			cout << "서버와 연결완료" << endl << endl;
			break;
		}
	}

	while (1) {

		int select;
		cout << "========================" << endl;
		cout << coutid << endl;
		cout << "========================" << endl;
		cout << "===    1. 회원가입   ===" << endl;
		cout << "===    2.  로그인    ===" << endl;
		cout << "===    3. 채팅하기   ===" << endl;
		cout << "===    4. 인벤토리   ===" << endl;
		cout << "===    5. 친구요청   ===" << endl;
		cout << "= 6. 친구 추가 및 삭제 =" << endl;
		cout << "===    7. 로그아웃   ===" << endl;
		cout << "===    8.  나가기    ===" << endl;
		cout << "========================" << endl;

		//현재 접속중인 친구 수 확인
		if (login_status) {

			char buffer[PACKET_SIZE];
			memset(buffer, 0, PACKET_SIZE);
			string sends;
			char* sendc = new char[sends.length() + 1];
			sends += ("98 " + realid);
			sendc[sends.length()] = '\n';
			sends.copy(sendc, sends.length());

			send(soc, sendc, PACKET_SIZE, 0);
			recv(soc, buffer, PACKET_SIZE, 0);
			cout << "현재 접속중인 친구 : " << buffer << endl;
			cout << "========================" << endl;
		}

		char buffer[PACKET_SIZE];
		memset(buffer, 0, PACKET_SIZE);
		string sends;
		char* sendc = new char[sends.length() + 1];
		sends += ("95 1 " + realid);
		sendc[sends.length()] = '\n';
		sends.copy(sendc, sends.length());

		send(soc, sendc, PACKET_SIZE, 0);
		recv(soc, buffer, PACKET_SIZE, 0);

		string new_friend;
		new_friend = buffer;
		int new_friends_num = stoi(new_friend);

		if (new_friends_num && login_status) {
			cout << "친구추가 요청 " << new_friends_num << "건이 있습니다." << endl;
			cout << "========================" << endl;
			new_friends_req = true;
		}

		// 0건이면 친추 요청 안보여줌 
		else {
			new_friends_req = false;
		}

		cout << endl;
		cin >> select;
		cout << endl;

		switch (select) {

			//회원가입
		case 1: {

			if (login_status) 
				cout << "이미 로그인 되어 있어요 ! 메인 화면으로 돌아갑니다." << endl << endl;
			
			else 
				join();

			break;
		}
			  //로그인
		case 2: {
			//로그인 아직 안된 상태
			if (!login_status) {
				login();
				if (coutid == "로그인을 해주세요.") cout << "로그인에 실패하였습니다. 메인 화면으로 돌아갑니다" << endl << endl;
				else cout << "로그인 성공 ! 메인 화면으로 돌아갑니다." << endl << endl;
			}
			//로그인 된 상태
			else {
				cout << "이미 로그인 되어 있어요 ! 메인 화면으로 돌아갑니다." << endl << endl;
			}
		}
			  break;

			  //채팅하기
		case 3: {

			if (!login_status) {
				cout << "로그인을 먼저 해주세요" << endl << endl;
			}

			// 접속중인 친구 목록 요청
			else {
				while (1) {

					std::cout << "========================" << endl;
					std::cout <<realid << "님이 채팅중인 방 목록" << endl;
					
					// 현재 채팅중인 방 목록 요청 보냄
					string sends = "99 ";
					sends += (realid);
					char* sendc = new char[sends.length() + 1];
					sendc[sends.length()] = '\n';
					sends.copy(sendc, sends.length());
					char buffer2[PACKET_SIZE] = { 0 };
					memset(&buffer2, 0, sizeof(buffer2));
					send(soc, sendc, strlen(sendc), 0);
					recv(soc, buffer2, PACKET_SIZE, 0);

					if () {

					}
					else {

					}

					std::cout << "========================" << endl;
					std::cout << endl;

					//접속중인 친구 목록 요청 보냄
					string sends = "99 ";
					sends += (realid);
					char* sendc = new char[sends.length() + 1];
					sendc[sends.length()] = '\n';
					sends.copy(sendc, sends.length());
					char buffer2[PACKET_SIZE] = { 0 };
					memset(&buffer2, 0, sizeof(buffer2));
					send(soc, sendc, strlen(sendc), 0);
					recv(soc, buffer2, PACKET_SIZE, 0);

					//buffer2에서 받은 목록 출력 (1, 한번에 ,로 구분해서 받아서 split, 2. 일단 친구 수 k면 k만큼 반복문 만들어서 그만큼 계속해서 recv받기)
					char* nulltext = NULL;
					char* friends = strtok_s(buffer2, " ", &nulltext);
					int cnt = 1;

					cout << "현재 접속중인 친구목록" << endl << endl;
					while (friends != NULL) {
						cout << cnt++ << ". " << friends << endl;
						friends = strtok_s(NULL, " ", &nulltext);
					}

					cout << endl;
					int chat_room_check_num;
					std::cout << "1. 방 들어가기" << endl;
					std::cout << "2. 방 생성하기" << endl;
					std::cout << "3. 친구랑 채팅하기" << endl;
					std::cout << "4. 코드로 방 찾기" << endl;
					cin >> chat_room_check_num;

					switch (chat_room_check_num) {
					case 1: {} break;
					case 2: {} break;
					case 3: {} break;
					}
					cout << "채팅을 원하는 친구의 아이디를 입력해 주세요 (메인화면으로 돌아가시려면 10101을 입력해주세요.) : ";
					string want_chat;
					cin >> want_chat;
					cout << endl;

					if (want_chat == "10101") break;

					sends = "97 ";
					sends += (realid + " " + want_chat);
					char* sendc3 = new char[sends.length() + 1];
					sendc3[sends.length()] = '\n';
					sends.copy(sendc3, sends.length());
					char buffer3[PACKET_SIZE] = { 0 };
					memset(&buffer3, 0, sizeof(buffer3));
					send(soc, sendc3, PACKET_SIZE, 0);
					recv(soc, buffer3, PACKET_SIZE, 0);

					string chat_friend = buffer3;

					// 친구 목록에 그 아이디가 존재할때
					if (chat_friend == "1") {
						//채팅 요청 보냄
						sends = "3 ";
						sends += (realid)+" ";
						sends += want_chat;
						char* sendc2 = new char[sends.length() + 1];
						sendc2[sends.length()] = '\n';
						sends.copy(sendc2, sends.length());
						send(soc, sendc2, strlen(sendc2), 0);

						char buffer[PACKET_SIZE] = { 0 };
						char msg[PACKET_SIZE] = {};
						cout << endl;
						cout << "=======================" << endl;
						cout << want_chat << "님과 채팅시작" << endl;
						cout << "=======================" << endl;
						cout << "=종료하시려면10101입력=" << endl;
						cout << "=======================" << endl << endl;

						thread proc1(thr_recvs);



						while (!WSAGetLastError()) {

							// 띄어쓰기도 받기 위해서 cin말고 cin.getline 사용
							cin.getline(buffer, PACKET_SIZE, '\n');
							string finish = buffer;
							if (finish == "10101") {
								send(soc, "10101", strlen(buffer), 0);
								cout << "연결이 종료되있습니다. 서버 연결 종료 대기중.." << endl;

								proc1.join();

								break;
							}
							if (buffer) {
								cout << "내가 전달한 메시지 : " << buffer << endl;
							}

							send(soc, buffer, strlen(buffer), 0);
						}

						cout << want_chat << "님과 채팅연결이 종료 되었습니다." << endl << endl;
						continue;
					}



					//친구 목록에 그 아이디가 존재하지 않을 때
					else {
						cout << "친구목록에 없습니다. 다시 입력해주세요." << endl << endl;
						continue;
					}

				}
			}

			break;
		}

			  // 인벤토리
		case 4: {
			cout << "인벤토리는 현재 구현중" << endl << endl;
			break;
		}

			  //친구요청
		case 5: {

			if (!login_status) {
				cout << "로그인을 먼저 해주세요" << endl << endl;
			}

			else {
				while (1) {
					int cnt = 1;

					if (new_friends_req) {
						string sends = "95 2 ";
						sends += (realid);
						char* sendc = new char[sends.length() + 1];
						sendc[sends.length()] = '\n';
						sends.copy(sendc, sends.length());
						char buffer2[PACKET_SIZE] = { 0 };
						memset(&buffer2, 0, sizeof(buffer2));
						send(soc, sendc, strlen(sendc), 0);
						recv(soc, buffer2, PACKET_SIZE, 0);

						cout << "새로운 친구추가 요청" << endl << endl;
						cout << "=======================" << endl;
						//string으로 문자열 자르기
						//buffer3에서 받은 목록 출력 (1, 한번에 ,로 구분해서 받아서 split, 2. 일단 친구 수 k면 k만큼 반복문 만들어서 그만큼 계속해서 recv받기)
						string temp_rcv_friends_s = buffer2;
						string temp_string;
						char temp_rcv_friends_c = ' ';
						stringstream ss(temp_rcv_friends_s);

						while (getline(ss, temp_string, ',')) {
							cout << cnt++ << ". " << temp_string << "님이 친구추가 요청을 하였습니다." << endl;
						}

						cout << "=======================" << endl << endl;
						cout << "친구 추가 하실 번호를 입력 후 엔터를 눌러주세요. (친구 추가는 한번에 한번씩 가능, 나가시려면 10101을 눌러주세요))" << endl;
						cout << "친구 추가 할 번호 : ";
					}

					else {
						cout << "새로운 친구 요청이 없습니다. 뒤로 가시려면 10101을 눌러주세요." << endl;
					}

					string temp_friend_req;
					cin >> temp_friend_req;

					int temp_friend_req_s = stoi(temp_friend_req);
					//입력한 수가 위 cnt 범위 안에 있을 때
					if (temp_friend_req == "10101") break;

					if (temp_friend_req_s <= cnt && temp_friend_req_s > 0) {

						string sends = "94 ";
						sends += (realid + " " + temp_friend_req);
						char* sendc = new char[sends.length() + 1];
						sendc[sends.length()] = '\n';
						sends.copy(sendc, sends.length());
						send(soc, sendc, strlen(sendc), 0);

					}

					//입력한 수가 위에 적힌 수보다 많거나 적을 때
					else {
						cout << "위에 적힌 번호 중에서 입력해 주세요" << endl << endl;
					}

				}
			}

			break;
		}

			  // 친구 추가 및 삭제
		case 6: {
			if (!login_status) {
				cout << "로그인을 먼저 해주세요" << endl << endl;
			}

			else {
				while (1) {

					//내 친구 목록 요청 보냄
					string sends = "100 ";
					sends += (realid);
					char* sendc = new char[sends.length() + 1];
					sendc[sends.length()] = '\n';
					sends.copy(sendc, sends.length());
					char buffer2[PACKET_SIZE] = { 0 };
					memset(&buffer2, 0, sizeof(buffer2));
					send(soc, sendc, strlen(sendc), 0);
					recv(soc, buffer2, PACKET_SIZE, 0);

					string want_chat;

					//char로 문자열 자르기
					//buffer2에서 받은 목록 출력 (1, 한번에 ,로 구분해서 받아서 split, 2. 일단 친구 수 k면 k만큼 반복문 만들어서 그만큼 계속해서 recv받기)
					char* nulltext = NULL;
					char* friends = strtok_s(buffer2, " ", &nulltext);
					int cnt = 1;

					cout << "현재 접속중인 친구목록" << endl << endl;
					cout << "=======================" << endl;
					while (friends != NULL) {
						string tempchar;

						for (int i = 0; i < strlen(friends) - 1; i++) {
							tempchar += friends[i];
						}
						if (friends[strlen(friends) - 1] == '1') {
							cout << cnt++ << ". " << tempchar << "님 접속중입니다." << endl;
						}
						else {
							cout << cnt++ << ". " << tempchar << "님 접속중이 아닙니다." << endl;
						}
						friends = strtok_s(NULL, " ", &nulltext);
					}
					cout << "=======================" << endl << endl;



					string add_friends;
					string addordelete;
					cout << endl;
					cout << "친구추가 하시려면 1을 입력하고 삭제 하시려면 2를 입력하고 엔터 후 아이디를 입력 주세요.(뒤로 가시려면 10101을 눌러주세요) : ";
					cin >> addordelete;
					cout << endl;

					if (addordelete == "10101") break;

					else if (addordelete == "1") {
						cin >> add_friends;
						string sends = "96 1 ";
						sends += (realid + " ");
						sends += add_friends;
						memset(&buffer2, 0, sizeof(buffer2));
						char* sendc2 = new char[sends.length() + 1];
						sendc2[sends.length()] = '\n';
						sends.copy(sendc2, sends.length());
						send(soc, sendc2, PACKET_SIZE, 0);
						recv(soc, buffer2, PACKET_SIZE, 0);
						string rcv_temp = buffer2;
						//요청 친구 있음
						if (rcv_temp == "1") {

							//친구신청
							string sends = "96 3 ";
							sends += (realid + " ");
							sends += add_friends;
							char* sendc2 = new char[sends.length() + 1];
							sendc2[sends.length()] = '\n';
							sends.copy(sendc2, sends.length());
							send(soc, sendc2, PACKET_SIZE, 0);

							cout << add_friends << "님에게 친구추가 요청을 보냈습니다." << endl << endl;

						}
						//요청 친구 없음
						else {
							cout << add_friends << "님은 없는 유저입니다. 다시 입력해주세요" << endl << endl;
						}
					}

					else if (addordelete == "2") {
						cin >> add_friends;
						string sends = "96 2 ";
						sends += (realid + " ");
						sends += add_friends;
						memset(&buffer2, 0, sizeof(buffer2));
						char* sendc2 = new char[sends.length() + 1];
						sendc2[sends.length()] = '\n';
						sends.copy(sendc2, sends.length());
						send(soc, sendc2, PACKET_SIZE, 0);
						recv(soc, buffer2, PACKET_SIZE, 0);
						string rcv_temp = buffer2;
						// 요청 친구 있음
						if (rcv_temp == "1") {

							// 친구 삭제
							string sends = "96 4 ";
							sends += (realid + " ");
							sends += add_friends;
							char* sendc2 = new char[sends.length() + 1];
							sendc2[sends.length()] = '\n';
							sends.copy(sendc2, sends.length());
							send(soc, sendc2, PACKET_SIZE, 0);

							cout << add_friends << "님을 친구에서 삭제하였습니다" << endl << endl;

						}
						//요청 친구 없음
						else {
							cout << add_friends << "님은 없는 유저입니다. 다시 입력해주세요" << endl << endl;
						}
					}

					else cout << "1,2,10101 세개의 번호 중 한가지를 입력해주세요" << endl << endl;

				}
				cout << endl;

			}
			break;
		}

			  // 로그아웃
		case 7: {

			if (!login_status) {
				cout << "로그인 상태가 아닙니다." << endl << endl;
			}
			else {
				new_friends_req = false;

				string sends = "101 ";
				sends += realid;
				char* sendc = new char[sends.length() + 1];
				sendc[sends.length()] = '\n';
				sends.copy(sendc, sends.length());

				login_status = 0;
				send(soc, sendc, PACKET_SIZE, 0);

				coutid = "로그인을 해주세요.";
			}
			break;
		}
			  // 나가기
		case 8: {
			cout << "다음에 또 만나요" << endl;
			new_friends_req = false;

			//로그인 상태면 로그아웃메시지 보내주기
			if (login_status) {
				string sends;
				sends += (101 + " ");
				sends += (realid);
				char* sendc = new char[sends.length() + 1];
				sendc[sends.length()] = '\n';
				sends.copy(sendc, sends.length());

				login_status = 0;
				send(soc, sendc, PACKET_SIZE, 0);
			}

			closesocket(soc);
			cout << "연결이 종료되었습니다.";
			WSACleanup();
			return 0;
			//위에 로그아웃 질문

			//recv로 확실하게 너 연결 종료되었다 확인 받고 끝마치는게 맞는건가요 ?
			//아님 서버가 바쁘면 종료 길어지니까 일단 보내고 종료되게 하는게 맞나요 ?


		}

			  //다시 메인 화면으로 돌아감
			  continue;
		}
	}
}