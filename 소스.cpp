#pragma once

//strtok_s
#include <cstring>
#include "Define.h"
#define PACKET_SIZE 1024

#pragma comment(lib, "ws2_32.lib") //���־󿡼� �������α׷��� �ϱ� ���� ��
using namespace std;

extern SOCKET soc;

/*�α��� �ϸ� �������� �ο��޴� ��ȣ
	unsigned int login_number;*/

extern string realid;
extern string coutid;

//���� ���̵�,��й�ȣ
extern string newid;
extern string newpassword;

extern int login_status;

// ���ο� ģ�� �߰� ��û �ִ��� Ȯ��
extern bool new_friends_req;

// ��ü ä�� ����
void thr_recvs_party() {
	while (!WSAGetLastError()) {
		char buffer[PACKET_SIZE] = { 0 };
		recv(soc, buffer, PACKET_SIZE, 0);
		string temp;
		temp = buffer;
		if (temp == "10101") {
			cout << endl;
			cout << "������ ������ ���� �Ͽ����ϴ�. �����÷��� 10101�� �Է����ּ���" << endl;
			break;
		}
		cout << "���� �޼��� : " << buffer << endl;
	}
	
}




//void join() {
//	string sends = "2 ";
//
//	char* sendc = new char[sends.length() + 1];
//
//	cout << "�����Ͻ� ���̵� �Է��� �ּ��� : ";
//	cin >> newid;
//
//	cout << "�����Ͻ� ��й�ȣ�� �Է��� �ּ��� : ";
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
//		cout << "5�� ���н� ���� ȭ������ ���ư��ϴ�. Ʋ��Ƚ�� : " << i << "��" << endl;
//		char buffer[PACKET_SIZE] = { 0 };
//		memset(&buffer, 0, sizeof(buffer));
//
//		//�α��ο� �ӽ� 
//		string temp_id;
//		string temp_password;
//
//		cout << "���̵� �Է����ּ��� : " ;
//		cin >> temp_id;
//		cout << "��й�ȣ�� �Է����ּ��� : ";
//		cin >> temp_password;
//		cout << endl;
//		//�������� istringstream���� �����ؼ� �ް� �ϱ� ���� ���� �߰�
//
//		sends += (temp_id + " ");
//		sends += temp_password;
//
//		char* sendc = new char[sends.length() + 1];
//		sendc[sends.length()] = '\n';
//
//		//string char �迭�� �ٲ㼭 ������
//		sends.copy(sendc, sends.length());
//
//		send(soc, sendc, strlen(sendc), 0);
//		recv(soc, buffer, PACKET_SIZE, 0);
//		string login_num = buffer;
//		
//		if (login_num == "2") {
//			coutid = (temp_id + "�� �ȳ��ϼ���");
//			realid = temp_id;
//			login_status = 1;
//			break;
//		}
//
//		else if(login_num == "0"){
//			cout << "���� ���̵� �Դϴ�. �ٽ� �Է� ���ּ���" << endl;
//		}
//
//		else if (login_num == "1") {
//			cout << "��й�ȣ�� Ʋ���ϴ�. �ٽ� �Է� ���ּ���" << endl;
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
//	std::cout << "���� �������� ģ�����" << endl << endl;
//	std::cout << "=======================" << endl;
//	while (friends != NULL) {
//		string tempchar;
//
//		for (int i = 0; i < strlen(friends) - 1; i++) {
//			tempchar += friends[i];
//		}
//		if (friends[strlen(friends) - 1] == '1') {
//			std::cout << cnt++ << ". " << tempchar << "�� �������Դϴ�." << endl;
//		}
//		else {
//			std::cout << cnt++ << ". " << tempchar << "�� �������� �ƴմϴ�." << endl;
//		}
//		friends = strtok_s(NULL, " ", &nulltext);
//	}
//	std::cout << "=======================" << endl << endl;
//}
//
//void friends_find() {
//
//}

