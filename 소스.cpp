#include <winsock2.h>
#include <thread>
#include <iostream>
#include <ws2tcpip.h>
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

void login() {
	for (int i = 0; i < 5; i++) {

		cout << "5�� ���н� ���� ȭ������ ���ư��ϴ�. Ʋ��Ƚ�� : " << i << "��" << endl;
		string sends = "2 ";
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
		cout << "===   4. �α׾ƿ�  ===" << endl;
		cout << "===   5.  ������   ===" << endl;
		cout << "======================" << endl;
		cout << "======================" << endl;
		cout << "���� �������� ģ�� : " << endl;
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
			string sends = "1 ";

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

			string sends = "3 ";
			char buffer2[PACKET_SIZE] = { 0 };
			memset(&buffer2, 0, sizeof(buffer2));

			sends += (realid);

			char* sendc = new char[sends.length() + 1];
			sendc[sends.length()] = '\n';
			sends.copy(sendc, sends.length());

			send(soc, sendc, strlen(sendc), 0);

			char buffer[PACKET_SIZE] = { 0 };
			char msg[PACKET_SIZE] = {};
			cout << endl;
			cout << "=======================" << endl;
			cout << "========ä�ý���=======" << endl;
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
				if (buffer){
					cout << "���� ������ �޽��� : " << buffer << endl;
				}
				send(soc, buffer, strlen(buffer), 0);
			}
			proc1.join();

			cout <<endl;

			break;
		}
			  // �α׾ƿ�
		case 4: {

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
		case 5: {
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