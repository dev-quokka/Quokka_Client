
#include <winsock2.h>
#include <thread>
#include <iostream>
#include <ws2tcpip.h>
#include <sstream>
#include <string>

#include "���.h"

using namespace std;

 SOCKET soc;

/*�α��� �ϸ� �������� �ο��޴� ��ȣ
	unsigned int login_number;*/

 string realid;
 string coutid = "�α����� ���ּ���.";

//���� ���̵�,��й�ȣ
 string newid;
 string newpassword;

 int login_status = 0;

// ���ο� ģ�� �߰� ��û �ִ��� Ȯ��
 bool new_friends_req = false;

int main() {

	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	soc = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	std::cout << "���� ���� �����..." << endl << endl;

	SOCKADDR_IN addr = {};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(9000);
	//addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr.s_addr);

	while (1) {
		if (!connect(soc, (SOCKADDR*)&addr, sizeof(addr))) {
			std::cout << "������ ����Ϸ�" << endl << endl;
			break;
		}
	}

	while (1) {

		int select;
		std::cout << "========================" << endl;
		std::cout << coutid << endl;
		std::cout << "========================" << endl;
		std::cout << "===    1. ȸ������   ===" << endl;
		std::cout << "===    2.  �α���    ===" << endl;
		std::cout << "===    3. ä���ϱ�   ===" << endl;
		std::cout << "===    4. ��ƼȮ��   ===" << endl;
		std::cout << "===    5. �κ��丮   ===" << endl;
		std::cout << "===    6. ģ����û   ===" << endl;
		std::cout << "= 7. ģ�� �߰� �� ���� =" << endl;
		std::cout << "===    8. �α׾ƿ�   ===" << endl;
		std::cout << "===    9.  ������    ===" << endl;
		std::cout << "========================" << endl;

		// ���⿡ ���� ��Ƽ �� �� �ְ�
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
				std::cout << "���� ��Ƽ���� �����ϴ�." << endl;
				std::cout << "========================" << endl;
			}
			else {
				std::cout << "���� ��Ƽ�� �� : " << string(buffer) << endl;
				std::cout << "========================" << endl;
			}

		}

		// �̰� ��Ƽ ��û�� �������� �������� ����. (������� ������)
		std::cout << "���� ��Ƽ �ʴ� ��û" << endl;


		std::cout << "========================" << endl;

		//���� �������� ģ�� �� Ȯ��
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
			std::cout << "���� �������� ģ�� : " << buffer << endl;
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
			std::cout << "ģ���߰� ��û " << new_friends_num << "���� �ֽ��ϴ�." << endl;
			std::cout << "========================" << endl;
			new_friends_req = true;
		}

		// 0���̸� ģ�� ��û �Ⱥ����� 
		else {
			new_friends_req = false;
		}

		std::cout << endl;
		std::cin >> select;
		std::cout << endl;

		switch (select) {

			//ȸ������
		case 1: {

			if (login_status) 
				std::cout << "�̹� �α��� �Ǿ� �־�� ! ���� ȭ������ ���ư��ϴ�." << endl << endl;
			
			else 
				join();

			break;
		}
			  //�α���
		case 2: {
			//�α��� ���� �ȵ� ����
			if (!login_status) {
				login();
				if (coutid == "�α����� ���ּ���.") std::cout << "�α��ο� �����Ͽ����ϴ�. ���� ȭ������ ���ư��ϴ�" << endl << endl;
				else std::cout << "�α��� ���� ! ���� ȭ������ ���ư��ϴ�." << endl << endl;
			}
			//�α��� �� ����
			else {
				std::cout << "�̹� �α��� �Ǿ� �־�� ! ���� ȭ������ ���ư��ϴ�." << endl << endl;
			}
		}
			  break;

			  //ä���ϱ�
		case 3: {

			if (!login_status) {
				std::cout << "�α����� ���� ���ּ���" << endl << endl;
			}

			// �������� ģ�� ��� ��û
			else {
				std::cout << "�ӼӸ��� ���ϸ� 10100�� �Է� �� ģ���� ���̵� �Է��� �ּ��� (����ȭ������ ���ư��÷��� 10101�� �Է����ּ���.) : ";
				while (1) {
					
					// ��ü ä�� ������ ����
					string sends = "3 1 ";
					sends += realid;
					char* sendc = new char[sends.length() + 1];
					sendc[sends.length()] = '\n';
					sends.copy(sendc,sends.length());
					memset(buffer,0,sizeof(buffer));
					send(soc, sendc, PACKET_SIZE, 0);
					recv(soc,buffer,PACKET_SIZE,0);

					// ���� ��üä�� ����
					if (string(buffer) == "1") {
						string want_chat;
						std::cin >> want_chat;
						std::cout << endl;

						// ��Ƽ�� ��üä�� ���� �̰� ��Ƽ�� ���� ���� �� ���� �Ȳ���
						thread proc1(thr_recvs_party);

						while (!WSAGetLastError()) {

							// ���⵵ �ޱ� ���ؼ� cin���� cin.getline ���
							std::cin.getline(buffer, PACKET_SIZE, '\n');
							string finish = buffer;

							if (finish == "10101") {
								send(soc, "10101 ", strlen(buffer), 0);
								break;
							}

							// �ӼӸ� ����
							else if (want_chat == "10100") {

								//�������� ģ�� ��� ��û ����
								string sends = "99 ";
								sends += (realid);
								char* sendc = new char[sends.length() + 1];
								sendc[sends.length()] = '\n';
								sends.copy(sendc, sends.length());
								char buffer2[PACKET_SIZE] = { 0 };
								memset(&buffer2, 0, sizeof(buffer2));
								send(soc, sendc, strlen(sendc), 0);
								recv(soc, buffer2, PACKET_SIZE, 0);

								//buffer2���� ���� ��� ��� (1, �ѹ��� ,�� �����ؼ� �޾Ƽ� split, 2. �ϴ� ģ�� �� k�� k��ŭ �ݺ��� ���� �׸�ŭ ����ؼ� recv�ޱ�)
								char* nulltext = NULL;
								char* friends = strtok_s(buffer2, " ", &nulltext);
								int cnt = 1;

								std::cout << "���� �������� ģ�����" << endl << endl;
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

								// ģ�� ��Ͽ� �� ���̵� �����Ҷ�
								if (string(buffer3) == "1") {
									//ä�� ��û ����
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
									std::cout << "�ӼӸ��� �����ϰ� ��ü ä���� �Ͻ÷��� 10101�Է�" << endl;

									thread proc1(thr_recvs_whisper);
									while (!WSAGetLastError()) {

										// ���⵵ �ޱ� ���ؼ� cin���� cin.getline ���
										std::cin.getline(buffer, PACKET_SIZE, '\n');
										string finish = buffer;
										if (finish == "10101") {
											send(soc, "10101 ", strlen(buffer), 0);
											std::cout << "������ ������ֽ��ϴ�. ���� ���� ���� �����.." << endl;

											proc1.join();

											break;
										}
										if (buffer) {
											std::cout << "[ " << want_chat << " ]�Կ��� : " << buffer << endl;
										}

										send(soc, buffer, strlen(buffer), 0);
									}

									std::cout << want_chat << "�԰� ä�ÿ����� ���� �Ǿ����ϴ�." << endl << endl;
									continue;
								}

								//ģ�� ��Ͽ� �� ���̵� �������� ���� ��
								else {
									std::cout << "ģ����Ͽ� �����ϴ�. �ٽ� �Է����ּ���." << endl << endl;
									continue;
								}

							}

							else {
								std::cout << realid <<": " << buffer << endl;
								send(soc, buffer, strlen(buffer), 0);
							}

						}
					}

					// � ������ ��üä�� ���� �Ұ��� 
					else {

					}

				}
			}
			break;
		}

		// ��ƼȮ��
		case 4: {
			if (!login_status) {
				std::cout << "�α����� ���� ���ּ���" << endl << endl;
			}
			else {

				while (1) {

					// �̰� ��Ƽ ��û�� �������� �������� ����.
					std::cout << "���� ��Ƽ ��û" << endl;

					//��Ƽ�� ������ (������ ���� ���ְ� �����)
					std::cout << "=======================" << endl;
					std::cout << "���� ��Ƽ��" << endl;



					std::cout << "=======================" << endl << endl;

					//��Ƽ���� �θ� �̻��϶� �߰� �ϱ�
					std::cout << "��Ƽ ������" << endl;
					std::cout << "��Ƽ�� �߹��ϱ�" << endl;

					// ģ�� ��� �߰� �ؾ� �ڴ�.
					
					std::cout << "��Ƽ �����ϱ�" << endl;

					std::cout << "��Ƽ �ʴ��ϱ�" << endl;

				}

			}

			break;
		}

			  // �κ��丮
		case 5: {
			std::cout << "�κ��丮�� ���� ������" << endl << endl;
			break;
		}

			  //ģ����û
		case 6: {

			if (!login_status) {
				std::cout << "�α����� ���� ���ּ���" << endl << endl;
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

						std::cout << "���ο� ģ���߰� ��û" << endl << endl;
						std::cout << "=======================" << endl;
						//string���� ���ڿ� �ڸ���
						//buffer3���� ���� ��� ��� (1, �ѹ��� ,�� �����ؼ� �޾Ƽ� split, 2. �ϴ� ģ�� �� k�� k��ŭ �ݺ��� ���� �׸�ŭ ����ؼ� recv�ޱ�)
						string temp_rcv_friends_s = buffer2;
						string temp_string;
						char temp_rcv_friends_c = ' ';
						stringstream ss(temp_rcv_friends_s);

						while (getline(ss, temp_string, ',')) {
							std::cout << cnt++ << ". " << temp_string << "���� ģ���߰� ��û�� �Ͽ����ϴ�." << endl;
						}

						std::cout << "=======================" << endl << endl;
						std::cout << "ģ�� �߰� �Ͻ� ��ȣ�� �Է� �� ���͸� �����ּ���. (ģ�� �߰��� �ѹ��� �ѹ��� ����, �����÷��� 10101�� �����ּ���))" << endl;
						std::cout << "ģ�� �߰� �� ��ȣ : ";
					}

					else {
						std::cout << "���ο� ģ�� ��û�� �����ϴ�. �ڷ� ���÷��� 10101�� �����ּ���." << endl;
					}

					string temp_friend_req;
					std::cin >> temp_friend_req;

					int temp_friend_req_s = stoi(temp_friend_req);
					//�Է��� ���� �� cnt ���� �ȿ� ���� ��
					if (temp_friend_req == "10101") break;

					if (temp_friend_req_s <= cnt && temp_friend_req_s > 0) {

						string sends = "94 ";
						sends += (realid + " " + temp_friend_req);
						char* sendc = new char[sends.length() + 1];
						sendc[sends.length()] = '\n';
						sends.copy(sendc, sends.length());
						send(soc, sendc, strlen(sendc), 0);

					}

					//�Է��� ���� ���� ���� ������ ���ų� ���� ��
					else {
						std::cout << "���� ���� ��ȣ �߿��� �Է��� �ּ���" << endl << endl;
					}

				}
			}

			break;
		}

			  // ģ�� �߰� �� ����
		case 7: {
			if (!login_status) {
				std::cout << "�α����� ���� ���ּ���" << endl << endl;
			}

			else {
				while (1) {

					//�� ģ�� ��� ��û ����
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

					//char�� ���ڿ� �ڸ���
					//buffer2���� ���� ��� ��� (1, �ѹ��� ,�� �����ؼ� �޾Ƽ� split, 2. �ϴ� ģ�� �� k�� k��ŭ �ݺ��� ���� �׸�ŭ ����ؼ� recv�ޱ�)
					char* nulltext = NULL;
					char* friends = strtok_s(buffer2, " ", &nulltext);
					int cnt = 1;

					std::cout << "���� �������� ģ�����" << endl << endl;
					std::cout << "=======================" << endl;
					while (friends != NULL) {
						string tempchar;

						for (int i = 0; i < strlen(friends) - 1; i++) {
							tempchar += friends[i];
						}
						if (friends[strlen(friends) - 1] == '1') {
							std::cout << cnt++ << ". " << tempchar << "�� �������Դϴ�." << endl;
						}
						else {
							std::cout << cnt++ << ". " << tempchar << "�� �������� �ƴմϴ�." << endl;
						}
						friends = strtok_s(NULL, " ", &nulltext);
					}
					std::cout << "=======================" << endl << endl;



					string add_friends;
					string addordelete;
					std::cout << endl;
					std::cout << "ģ���߰� �Ͻ÷��� 1�� �Է��ϰ� ���� �Ͻ÷��� 2�� �Է��ϰ� ���� �� ���̵� �Է� �ּ���.(�ڷ� ���÷��� 10101�� �����ּ���) : ";
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
						//��û ģ�� ����
						if (rcv_temp == "1") {

							//ģ����û
							string sends = "96 3 ";
							sends += (realid + " ");
							sends += add_friends;
							char* sendc2 = new char[sends.length() + 1];
							sendc2[sends.length()] = '\n';
							sends.copy(sendc2, sends.length());
							send(soc, sendc2, PACKET_SIZE, 0);

							std::cout << add_friends << "�Կ��� ģ���߰� ��û�� ���½��ϴ�." << endl << endl;

						}
						//��û ģ�� ����
						else {
							std::cout << add_friends << "���� ���� �����Դϴ�. �ٽ� �Է����ּ���" << endl << endl;
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
						// ��û ģ�� ����
						if (rcv_temp == "1") {

							// ģ�� ����
							string sends = "96 4 ";
							sends += (realid + " ");
							sends += add_friends;
							char* sendc2 = new char[sends.length() + 1];
							sendc2[sends.length()] = '\n';
							sends.copy(sendc2, sends.length());
							send(soc, sendc2, PACKET_SIZE, 0);

							std::cout << add_friends << "���� ģ������ �����Ͽ����ϴ�" << endl << endl;

						}
						//��û ģ�� ����
						else {
							std::cout << add_friends << "���� ���� �����Դϴ�. �ٽ� �Է����ּ���" << endl << endl;
						}
					}

					else std::cout << "1,2,10101 ������ ��ȣ �� �Ѱ����� �Է����ּ���" << endl << endl;

				}
				std::cout << endl;

			}
			break;
		}

			  // �α׾ƿ�
		case 8: {

			if (!login_status) {
				std::cout << "�α��� ���°� �ƴմϴ�." << endl << endl;
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

				coutid = "�α����� ���ּ���.";
			}
			break;
		}
			  // ������
		case 9: {
			std::cout << "������ �� ������" << endl;
			new_friends_req = false;

			//�α��� ���¸� �α׾ƿ��޽��� �����ֱ�
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
			std::cout << "������ ����Ǿ����ϴ�.";
			WSACleanup();
			return 0;
			//���� �α׾ƿ� ����

			//recv�� Ȯ���ϰ� �� ���� ����Ǿ��� Ȯ�� �ް� ����ġ�°� �´°ǰ��� ?
			//�ƴ� ������ �ٻڸ� ���� ������ϱ� �ϴ� ������ ����ǰ� �ϴ°� �³��� ?


		}

			  //�ٽ� ���� ȭ������ ���ư�
			  continue;
		}
	}
}