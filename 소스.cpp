#include <winsock2.h>
#include <thread>
#include <iostream>
#include <ws2tcpip.h>

//strtok_s
#include <cstring>

#pragma comment(lib, "ws2_32.lib") //���־󿡼� �������α׷��� �ϱ� ���� ��
using namespace std;

#define PACKET_SIZE 1024

SOCKET soc;

/*�α��� �ϸ� �������� �ο��޴� ��ȣ
	unsigned int login_number;*/

string realid;
string coutid = "�α����� ���ּ���.";
int login_status = 0;
string temp_id;
string temp_password;
string newid;
string newpassword;

//ä�� �ϱ� ���� �ڵ�
void thr_recvs() {
	char buffer[PACKET_SIZE] = { 0 };
	string temp;
	while (!WSAGetLastError()) {
		memset(&buffer, 0, sizeof(buffer));
		recv(soc, buffer, PACKET_SIZE, 0);
		temp = buffer;
		if (temp == "10101") {
			cout << endl;
			cout << "������ ������ ���� �Ͽ����ϴ�. �����÷��� 10101�� �Է����ּ���" << endl;
			break;
		}
		cout << "���� �޼��� : " << buffer << endl;
	}
	cout << "=====================" << endl;
	cout << "===������ ä������===" << endl;
	cout << "=====================" << endl;
	cout << "�����÷��� 10101 �Է�" << endl;
	cout << "=====================" << endl << endl;
}

void join() {
	string sends = "2 ";

	char* sendc = new char[sends.length() + 1];

	cout << "�����Ͻ� ���̵� �Է��� �ּ��� : ";
	cin >> newid;

	cout << "�����Ͻ� ��й�ȣ�� �Է��� �ּ��� : ";
	cin >> newpassword; cout << endl;

	sends += newid + " ";
	sends += newpassword;
	sendc[sends.length()] = '\n';
	sends.copy(sendc, sends.length());

	send(soc, sendc, PACKET_SIZE, 0);
}

void login() {
	for (int i = 0; i < 5; i++) {

		cout << "5�� ���н� ���� ȭ������ ���ư��ϴ�. Ʋ��Ƚ�� : " << i << "��" << endl;
		string sends = "1 ";
		char buffer[PACKET_SIZE] = { 0 };
		memset(&buffer, 0, sizeof(buffer));

		cout << "���̵� �Է����ּ��� : " ;
		cin >> temp_id;
		cout << "��й�ȣ�� �Է����ּ��� : ";
		cin >> temp_password;
		cout << endl;
		//�������� istringstream���� �����ؼ� �ް� �ϱ� ���� ���� �߰�

		sends += (temp_id + " ");
		sends += temp_password;

		char* sendc = new char[sends.length() + 1];
		sendc[sends.length()] = '\n';

		//string char �迭�� �ٲ㼭 ������
		sends.copy(sendc, sends.length());

		send(soc, sendc, strlen(sendc), 0);
		recv(soc, buffer, PACKET_SIZE, 0);
		string login_num = buffer;
		
		if (login_num == "2") {
			coutid = (temp_id + "�� �ȳ��ϼ���");
			realid = temp_id;
			login_status = 1;
			break;
		}

		else if(login_num == "0"){
			cout << "���� ���̵� �Դϴ�. �ٽ� �Է� ���ּ���" << endl;
		}

		else if (login_num == "1") {
			cout << "��й�ȣ�� Ʋ���ϴ�. �ٽ� �Է� ���ּ���" << endl;
		}
	}
}

int main() {

	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	soc = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	cout << "���� ���� �����..." << endl << endl;

	SOCKADDR_IN addr = {};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(9000);
	//addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr.s_addr);

	while (1) {
		if (!connect(soc, (SOCKADDR*)&addr, sizeof(addr))) {
			cout << "������ ����Ϸ�" << endl << endl;
			break;
		}
	}

	while (1) {
		int select;
		cout << "======================" << endl;
		cout << coutid << endl;
		cout << "======================" << endl;
		cout << "===   1. ȸ������  ===" << endl;
		cout << "===   2.  �α���   ===" << endl;
		cout << "===   3. ä���ϱ�  ===" << endl;
		cout << "===   4. �κ��丮  ===" << endl;
		cout << "=5. ģ�� Ȯ�� �� �߰�=" << endl;
		cout << "===   6. �α׾ƿ�  ===" << endl;
		cout << "===   7.  ������   ===" << endl;
		cout << "======================" << endl;
		cout << "======================" << endl;
		//���� �������� ģ�� �� Ȯ��
		if (login_status) {

			char buffer[PACKET_SIZE];
			memset(buffer, 0, PACKET_SIZE);
			string sends;
			char* sendc = new char[sends.length() + 1];
			sends += ("98 " + realid);
			sendc[sends.length()] = '\n';
			sends.copy(sendc, sends.length());

			send(soc,sendc , PACKET_SIZE, 0);
			recv(soc,buffer,PACKET_SIZE,0);
			cout << "���� �������� ģ�� : " <<buffer<<endl;
		}
		else {
			cout << "======================"<<endl;
		}
		cout << "======================" << endl;
		cout << "======================" << endl;
		cin >> select;

		cout << endl;

		switch (select) {

			//ȸ������
		case 1: {
			if (login_status) {
				cout << "�̹� �α��� �Ǿ� �־�� ! ���� ȭ������ ���ư��ϴ�." << endl << endl;
				break;
			}
			join();
			break;
		}
			//�α���
		case 2: {
			//�α��� ���� �ȵ� ����
			if (!login_status) {
				login();
				if (coutid == "�α����� ���ּ���.") cout << "�α��ο� �����Ͽ����ϴ�. ���� ȭ������ ���ư��ϴ�" << endl << endl;
				else cout << "�α��� ���� ! ���� ȭ������ ���ư��ϴ�." << endl << endl;
			}
			//�α��� �� ����
			else {
				cout << "�̹� �α��� �Ǿ� �־�� ! ���� ȭ������ ���ư��ϴ�." << endl << endl;
			}
		}
			  break;

			  //ä���ϱ�
		case 3: {

			if (!login_status) {
				cout << "�α����� ���� ���ּ���" << endl << endl;
				break;
			}

			while (1) {
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

				cout << "���� �������� ģ�����" << endl << endl;
				while (friends != NULL) {
					cout << cnt++ << ". " << friends << endl;
					friends = strtok_s(NULL, " ", &nulltext);
				}

				cout << endl;

				cout << "ä���� ���ϴ� ģ���� ���̵� �Է��� �ּ��� (����ȭ������ ���ư��÷��� 10101�� �Է����ּ���.) : ";
				string want_chat;
				cin >> want_chat;
				cout <<endl;

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

				// ģ�� ��Ͽ� �� ���̵� �����Ҷ�
				if (chat_friend == "1") {
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
					cout << endl;
					cout << "=======================" << endl;
					cout << want_chat << "�԰� ä�ý���" << endl;
					cout << "=======================" << endl;
					cout << "=�����Ͻ÷���10101�Է�=" << endl;
					cout << "=======================" << endl << endl;

					thread proc1(thr_recvs);

					while (!WSAGetLastError()) {

						// ���⵵ �ޱ� ���ؼ� cin���� cin.getline ���
						cin.getline(buffer, PACKET_SIZE, '\n');
						string finish = buffer;
						if (finish == "10101") {
							send(soc, "10101", strlen(buffer), 0);
							cout << "������ ������ֽ��ϴ�. ���� ���� ���� �����.." << endl;
							break;
						}
						if (buffer) {
							cout << "���� ������ �޽��� : " << buffer << endl;
						}
						send(soc, buffer, strlen(buffer), 0);
					}
					proc1.join();
					cout << want_chat <<"�԰� ä�ÿ����� ���� �Ǿ����ϴ�." << endl << endl;
					continue;
				}

				//ģ�� ��Ͽ� �� ���̵� �������� ���� ��
				else {
					cout << "ģ����Ͽ� �����ϴ�. �ٽ� �Է����ּ���." << endl<<endl;
					continue;
				}
			}
			break;
		}

		// �κ��丮
		case 4 : {
			cout << "�κ��丮�� ���� ������" << endl << endl;
			break;
		}

		// ģ�� �߰� �� Ȯ��
		case 5: {
			if (!login_status) {
				cout << "�α����� ���� ���ּ���" << endl << endl;
				break;
			}
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

			//buffer2���� ���� ��� ��� (1, �ѹ��� ,�� �����ؼ� �޾Ƽ� split, 2. �ϴ� ģ�� �� k�� k��ŭ �ݺ��� ���� �׸�ŭ ����ؼ� recv�ޱ�)
			char* nulltext = NULL;
			char* friends = strtok_s(buffer2, " ", &nulltext);
			int cnt = 1;
			string want_chat;

			while (friends != NULL) {
				string tempchar;
				cout << "���� �������� ģ�����" << endl << endl;
				for (int i = 0; i < strlen(friends) - 1; i++) {
					tempchar += friends[i];
				}
				if (friends[strlen(friends) - 1] == '1') {
					cout << cnt++ << ". " << tempchar << "�� �������Դϴ�." << endl;
				}
				else {
					cout << cnt++ << ". " << tempchar << "�� �������� �ƴմϴ�." << endl;
				}
				friends = strtok_s(NULL, " ", &nulltext);
			}
			string add_friends;
			cout << endl;
			cout << "ģ�� �߰� �ϰ� ���� ���̵� �Է� �� �ּ���.(�ڷ� ���÷��� 10101�� �����ּ���) : ";
			cin >> add_friends;

			if (add_friends == "10101") break;
			
			else {
				
				string sends = "96 ";
				sends += (realid+" ");
				sends += add_friends;
				memset(&buffer2, 0, sizeof(buffer2));
				char* sendc2 = new char[sends.length() + 1];
				sendc2[sends.length()] = '\n';
				sends.copy(sendc2, sends.length());
				send(soc, sendc2, PACKET_SIZE, 0);
				recv(soc, buffer2, PACKET_SIZE, 0);
				
				//��û ģ�� ����
				if (buffer2 == "1") {
					string sends = "96 ";
					sends += (realid + " ");
					sends += add_friends;
					char* sendc2 = new char[sends.length() + 1];
					sendc2[sends.length()] = '\n';
					sends.copy(sendc2, sends.length());
					send(soc, sendc2, PACKET_SIZE, 0);

					cout << add_friends <<"�Կ��� ģ���߰� ��û�� ���½��ϴ�." << endl;
					
				}
				//��û ģ�� ����
				else {
					cout << add_friends << "���� ���� �����Դϴ�. �ٽ� �Է����ּ���" << endl;
				}
			}

			break;
		}
		
			  // �α׾ƿ�
		case 6: {

			if (!login_status) {
				cout << "�α��� ���°� �ƴմϴ�." << endl << endl;
				break;
			}

			string sends="101 ";
			sends += realid;
			char* sendc = new char[sends.length() + 1];
			sendc[sends.length()] = '\n';
			sends.copy(sendc, sends.length());

			login_status = 0;
			send(soc, sendc, PACKET_SIZE, 0);

			coutid = "�α����� ���ּ���.";
			break;
		}
			  // ������
		case 7: {
			cout << "������ �� ������" << endl;

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
			cout << "������ ����Ǿ����ϴ�.";
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