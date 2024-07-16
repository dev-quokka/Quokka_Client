
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

	std::cout << "서버 연결 대기중..." << endl << endl;

	SOCKADDR_IN addr = {};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(9000);
	//addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr.s_addr);

	while (1) {
		if (!connect(soc, (SOCKADDR*)&addr, sizeof(addr))) {
			std::cout << "서버와 연결완료" << endl << endl;
			break;
		}
	}

	while (1) {

		int select;
		std::cout << "========================" << endl;
		std::cout << coutid << endl;
		std::cout << "========================" << endl;
		std::cout << "===    1. 회원가입   ===" << endl;
		std::cout << "===    2.  로그인    ===" << endl;
		std::cout << "===    3. 채팅하기   ===" << endl;
		std::cout << "===    4. 파티확인   ===" << endl;
		std::cout << "===    5. 인벤토리   ===" << endl;
		std::cout << "===    6. 친구요청   ===" << endl;
		std::cout << "= 7. 친구 추가 및 삭제 =" << endl;
		std::cout << "===    8. 로그아웃   ===" << endl;
		std::cout << "===    9.  나가기    ===" << endl;
		std::cout << "========================" << endl;

		// 여기에 현재 파티 뜰 수 있게
		if (login_status) {
			char buffer[PACKET_SIZE];
			memset(buffer, 0, PACKET_SIZE);
			string sends;
			char* sendc = new char[sends.length() + 1];
			sends += ("4 1 " + realid);
			sendc[sends.length()] = '\n';
			sends.copy(sendc, sends.length());
			send(soc, sendc, PACKET_SIZE, 0);
			recv(soc, buffer, PACKET_SIZE, 0);

			if (string(buffer) == "1") {
				std::cout << "현재 파티원이 없습니다." << endl;
				std::cout << "========================" << endl;
			}
			else {
				std::cout << "현재 파티원 수 : " << string(buffer) << endl;
				std::cout << "========================" << endl;
			}

		}

		// 이건 파티 요청이 있을때만 보여지게 하자. (쓰레드로 만들어보자)
		std::cout << "들어온 파티 초대 요청" << endl;


		std::cout << "========================" << endl;

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
			std::cout << "현재 접속중인 친구 : " << buffer << endl;
			std::cout << "========================" << endl;
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
			std::cout << "친구추가 요청 " << new_friends_num << "건이 있습니다." << endl;
			std::cout << "========================" << endl;
			new_friends_req = true;
		}

		// 0건이면 친추 요청 안보여줌 
		else {
			new_friends_req = false;
		}

		std::cout << endl;
		std::cin >> select;
		std::cout << endl;

		switch (select) {

			//회원가입
		case 1: {

			if (login_status) 
				std::cout << "이미 로그인 되어 있어요 ! 메인 화면으로 돌아갑니다." << endl << endl;
			
			else 
				join();

			break;
		}
			  //로그인
		case 2: {
			//로그인 아직 안된 상태
			if (!login_status) {
				login();
				if (coutid == "로그인을 해주세요.") std::cout << "로그인에 실패하였습니다. 메인 화면으로 돌아갑니다" << endl << endl;
				else std::cout << "로그인 성공 ! 메인 화면으로 돌아갑니다." << endl << endl;
			}
			//로그인 된 상태
			else {
				std::cout << "이미 로그인 되어 있어요 ! 메인 화면으로 돌아갑니다." << endl << endl;
			}
		}
			  break;

			  //채팅하기
		case 3: {

			if (!login_status) {
				std::cout << "로그인을 먼저 해주세요" << endl << endl;
			}

			// 접속중인 친구 목록 요청
			else {
				std::cout << "귓속말을 원하면 10100을 입력 후 친구의 아이디를 입력해 주세요 (메인화면으로 돌아가시려면 10101을 입력해주세요.) : ";
				while (1) {
					
					// 전체 채팅 쓰레드 실행
					string sends = "3 1 ";
					sends += realid;
					char* sendc = new char[sends.length() + 1];
					sendc[sends.length()] = '\n';
					sends.copy(sendc,sends.length());
					memset(buffer,0,sizeof(buffer));
					send(soc, sendc, PACKET_SIZE, 0);
					recv(soc,buffer,PACKET_SIZE,0);

					// 이제 전체채팅 가능
					if (string(buffer) == "1") {
						string want_chat;
						std::cin >> want_chat;
						std::cout << endl;

						// 파티원 전체채팅 받음 이건 파티원 전부 나갈 때 까지 안꺼짐
						thread proc1(thr_recvs_party);

						while (!WSAGetLastError()) {

							// 띄어쓰기도 받기 위해서 cin말고 cin.getline 사용
							std::cin.getline(buffer, PACKET_SIZE, '\n');
							string finish = buffer;

							if (finish == "10101") {
								send(soc, "10101 ", strlen(buffer), 0);
								break;
							}

							// 귓속말 진행
							else if (want_chat == "10100") {

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

								std::cout << "현재 접속중인 친구목록" << endl << endl;
								while (friends != NULL) {
									std::cout << cnt++ << ". " << friends << endl;
									friends = strtok_s(NULL, " ", &nulltext);
								}

								sends = "97 ";
								sends += (realid + " " + want_chat);
								char* sendc3 = new char[sends.length() + 1];
								sendc3[sends.length()] = '\n';
								sends.copy(sendc3, sends.length());
								char buffer3[PACKET_SIZE] = { 0 };
								memset(&buffer3, 0, sizeof(buffer3));
								send(soc, sendc3, PACKET_SIZE, 0);
								recv(soc, buffer3, PACKET_SIZE, 0);

								// 친구 목록에 그 아이디가 존재할때
								if (string(buffer3) == "1") {
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
									std::cout << endl;
									std::cout << "귓속말을 종료하고 전체 채팅을 하시려면 10101입력" << endl;

									thread proc1(thr_recvs_whisper);
									while (!WSAGetLastError()) {

										// 띄어쓰기도 받기 위해서 cin말고 cin.getline 사용
										std::cin.getline(buffer, PACKET_SIZE, '\n');
										string finish = buffer;
										if (finish == "10101") {
											send(soc, "10101 ", strlen(buffer), 0);
											std::cout << "연결이 종료되있습니다. 서버 연결 종료 대기중.." << endl;

											proc1.join();

											break;
										}
										if (buffer) {
											std::cout << "[ " << want_chat << " ]님에게 : " << buffer << endl;
										}

										send(soc, buffer, strlen(buffer), 0);
									}

									std::cout << want_chat << "님과 채팅연결이 종료 되었습니다." << endl << endl;
									continue;
								}

								//친구 목록에 그 아이디가 존재하지 않을 때
								else {
									std::cout << "친구목록에 없습니다. 다시 입력해주세요." << endl << endl;
									continue;
								}

							}

							else {
								std::cout << realid <<": " << buffer << endl;
								send(soc, buffer, strlen(buffer), 0);
							}

						}
					}

					// 어떤 이유로 전체채팅 아직 불가능 
					else {

					}

				}
			}
			break;
		}

		// 파티확인
		case 4: {
			if (!login_status) {
				std::cout << "로그인을 먼저 해주세요" << endl << endl;
			}
			else {

				while (1) {

					// 이건 파티 요청이 있을때만 보여지게 하자.
					std::cout << "들어온 파티 요청" << endl;

					//파티가 있을때 (없으면 나만 떠있게 만들기)
					std::cout << "=======================" << endl;
					std::cout << "현재 파티원" << endl;



					std::cout << "=======================" << endl << endl;

					//파티원이 두명 이상일때 뜨게 하기
					std::cout << "파티 나가기" << endl;
					std::cout << "파티원 추방하기" << endl;

					// 친구 목록 뜨게 해야 겠다.
					
					std::cout << "파티 참가하기" << endl;

					std::cout << "파티 초대하기" << endl;

				}

			}

			break;
		}

			  // 인벤토리
		case 5: {
			std::cout << "인벤토리는 현재 구현중" << endl << endl;
			break;
		}

			  //친구요청
		case 6: {

			if (!login_status) {
				std::cout << "로그인을 먼저 해주세요" << endl << endl;
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

						std::cout << "새로운 친구추가 요청" << endl << endl;
						std::cout << "=======================" << endl;
						//string으로 문자열 자르기
						//buffer3에서 받은 목록 출력 (1, 한번에 ,로 구분해서 받아서 split, 2. 일단 친구 수 k면 k만큼 반복문 만들어서 그만큼 계속해서 recv받기)
						string temp_rcv_friends_s = buffer2;
						string temp_string;
						char temp_rcv_friends_c = ' ';
						stringstream ss(temp_rcv_friends_s);

						while (getline(ss, temp_string, ',')) {
							std::cout << cnt++ << ". " << temp_string << "님이 친구추가 요청을 하였습니다." << endl;
						}

						std::cout << "=======================" << endl << endl;
						std::cout << "친구 추가 하실 번호를 입력 후 엔터를 눌러주세요. (친구 추가는 한번에 한번씩 가능, 나가시려면 10101을 눌러주세요))" << endl;
						std::cout << "친구 추가 할 번호 : ";
					}

					else {
						std::cout << "새로운 친구 요청이 없습니다. 뒤로 가시려면 10101을 눌러주세요." << endl;
					}

					string temp_friend_req;
					std::cin >> temp_friend_req;

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
						std::cout << "위에 적힌 번호 중에서 입력해 주세요" << endl << endl;
					}

				}
			}

			break;
		}

			  // 친구 추가 및 삭제
		case 7: {
			if (!login_status) {
				std::cout << "로그인을 먼저 해주세요" << endl << endl;
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

					std::cout << "현재 접속중인 친구목록" << endl << endl;
					std::cout << "=======================" << endl;
					while (friends != NULL) {
						string tempchar;

						for (int i = 0; i < strlen(friends) - 1; i++) {
							tempchar += friends[i];
						}
						if (friends[strlen(friends) - 1] == '1') {
							std::cout << cnt++ << ". " << tempchar << "님 접속중입니다." << endl;
						}
						else {
							std::cout << cnt++ << ". " << tempchar << "님 접속중이 아닙니다." << endl;
						}
						friends = strtok_s(NULL, " ", &nulltext);
					}
					std::cout << "=======================" << endl << endl;



					string add_friends;
					string addordelete;
					std::cout << endl;
					std::cout << "친구추가 하시려면 1을 입력하고 삭제 하시려면 2를 입력하고 엔터 후 아이디를 입력 주세요.(뒤로 가시려면 10101을 눌러주세요) : ";
					std::cin >> addordelete;
					std::cout << endl;

					if (addordelete == "10101") break;

					else if (addordelete == "1") {
						std::cin >> add_friends;
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

							std::cout << add_friends << "님에게 친구추가 요청을 보냈습니다." << endl << endl;

						}
						//요청 친구 없음
						else {
							std::cout << add_friends << "님은 없는 유저입니다. 다시 입력해주세요" << endl << endl;
						}
					}

					else if (addordelete == "2") {
						std::cin >> add_friends;
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

							std::cout << add_friends << "님을 친구에서 삭제하였습니다" << endl << endl;

						}
						//요청 친구 없음
						else {
							std::cout << add_friends << "님은 없는 유저입니다. 다시 입력해주세요" << endl << endl;
						}
					}

					else std::cout << "1,2,10101 세개의 번호 중 한가지를 입력해주세요" << endl << endl;

				}
				std::cout << endl;

			}
			break;
		}

			  // 로그아웃
		case 8: {

			if (!login_status) {
				std::cout << "로그인 상태가 아닙니다." << endl << endl;
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
		case 9: {
			std::cout << "다음에 또 만나요" << endl;
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
			std::cout << "연결이 종료되었습니다.";
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