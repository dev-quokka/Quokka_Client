#include <winsock2.h>
#include <thread>
#include <iostream>
#include <ws2tcpip.h>
#include <sstream>
#include <string>

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

//���� ���̵�,��й�ȣ
string newid;
string newpassword;

int login_status = 0;

// ���ο� ģ�� �߰� ��û �ִ��� Ȯ��
bool new_friends_req = false;

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

		//�α��ο� �ӽ� 
		string temp_id;
		string temp_password;

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
		cout << "===   5. ģ����û  ===" << endl;
		cout << "===   6. ģ���߰�  ===" << endl;
		cout << "===   7. �α׾ƿ�  ===" << endl;
		cout << "===   8.  ������   ===" << endl;
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
			cout << "======================" << endl;
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
			cout << "ģ���߰� ��û " << new_friends_num <<"���� �ֽ��ϴ�." << endl;
			cout << "======================" << endl;
			new_friends_req = true;
		}

		// 0���̸� ģ�� ��û �Ⱥ����� 
		else {
			new_friends_req = false;
		}

		cout << endl;
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

		//ģ����û
		case 5: {
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

					cout << "���ο� ģ���߰� ��û" << endl << endl;
					cout << "=======================" << endl;
					//string���� ���ڿ� �ڸ���
					//buffer3���� ���� ��� ��� (1, �ѹ��� ,�� �����ؼ� �޾Ƽ� split, 2. �ϴ� ģ�� �� k�� k��ŭ �ݺ��� ���� �׸�ŭ ����ؼ� recv�ޱ�)
					string temp_rcv_friends_s = buffer2;
					string temp_string;
					char temp_rcv_friends_c = ' ';
					stringstream ss(temp_rcv_friends_s);
					
					while (getline(ss, temp_string, ',')) {
						cout << cnt++ << ". " << temp_string << "���� ģ���߰� ��û�� �Ͽ����ϴ�." << endl;
					}

					cout << "=======================" << endl << endl;
					cout << "ģ�� �߰� �Ͻ� ��ȣ�� �Է� �� ���͸� �����ּ���. (ģ�� �߰��� �ѹ��� �ѹ��� ����, �����÷��� 10101�� �����ּ���))" << endl;
					cout << "ģ�� �߰� �� ��ȣ : ";
				}
				else {
					cout << "���ο� ģ�� ��û�� �����ϴ�. �ڷ� ���÷��� 10101�� �����ּ���." << endl;;
				}

				string temp_friend_req;
				cin >> temp_friend_req;

				if (temp_friend_req == "10101")break;
				else {
					string sends = "94 ";
					sends += (realid+" "+ temp_friend_req);
					char* sendc = new char[sends.length() + 1];
					sendc[sends.length()] = '\n';
					sends.copy(sendc, sends.length());
					send(soc, sendc, strlen(sendc), 0);
				}
			}
			break;
		}
		// ģ�� �߰�
		case 6: {
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

			string want_chat;

			//char�� ���ڿ� �ڸ���
			//buffer2���� ���� ��� ��� (1, �ѹ��� ,�� �����ؼ� �޾Ƽ� split, 2. �ϴ� ģ�� �� k�� k��ŭ �ݺ��� ���� �׸�ŭ ����ؼ� recv�ޱ�)
			char* nulltext = NULL;
			char* friends = strtok_s(buffer2, " ", &nulltext);
			int cnt = 1;

			cout << "���� �������� ģ�����" << endl<< endl;
			cout << "=======================" << endl;
			while (friends != NULL) {
				string tempchar;

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
			cout << "=======================" << endl << endl;

			while (1) {
				string add_friends;
				cout << endl;
				cout << "���Ӱ� ģ�� �߰� �ϰ� ���� ���̵� �Է� �� �ּ���.(�ڷ� ���÷��� 10101�� �����ּ���) : ";
				cin >> add_friends;

				if (add_friends == "10101") break;

				else {
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
						string sends = "96 2 ";
						sends += (realid + " ");
						sends += add_friends;
						char* sendc2 = new char[sends.length() + 1];
						sendc2[sends.length()] = '\n';
						sends.copy(sendc2, sends.length());
						send(soc, sendc2, PACKET_SIZE, 0);

						cout << add_friends << "�Կ��� ģ���߰� ��û�� ���½��ϴ�." << endl;

					}
					//��û ģ�� ����
					else {
						cout << add_friends << "���� ���� �����Դϴ�. �ٽ� �Է����ּ���" << endl;
					}
				}
			}
			cout << endl;
			break;
		}
		
			  // �α׾ƿ�
		case 7: {

			if (!login_status) {
				cout << "�α��� ���°� �ƴմϴ�." << endl << endl;
				break;
			}
			new_friends_req = false;

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
		case 8: {
			cout << "������ �� ������" << endl;
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