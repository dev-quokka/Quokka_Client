#include <winsock2.h>
#include <thread>
#include <iostream>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib") //비주얼에서 소켓프로그래밍 하기 위한 것
using namespace std;

#define PACKET_SIZE 1024

SOCKET soc;

/*로그인 하면 서버에서 부여받는 번호
	unsigned int login_number;*/

string realid;
string coutid = "로그인을 해주세요.";
int login_status = 0;
string temp_id;
string temp_password;
string newid;
string newpassword;

//채팅 하기 위한 코드
void thr_recvs() {
	char buffer[PACKET_SIZE] = { 0 };
	string temp;
	while (!WSAGetLastError()) {
		memset(&buffer, 0, sizeof(buffer));
		recv(soc, buffer, PACKET_SIZE, 0);
		temp = buffer;
		if (temp == "10101") {
			cout << endl;
			cout << "서버가 연결을 종료 하였습니다. 나가시려면 10101을 입력해주세요" << endl;
			break;
		}
		cout << "받은 메세지 : " << buffer << endl;
	}
	cout << "=====================" << endl;
	cout << "===서버와 채팅종료===" << endl;
	cout << "=====================" << endl;
	cout << "나가시려면 10101 입력" << endl;
	cout << "=====================" << endl << endl;
}

void login() {
	for (int i = 0; i < 5; i++) {

		cout << "5번 실패시 메인 화면으로 돌아갑니다. 틀린횟수 : " << i << "번" << endl;
		string sends = "2 ";
		char buffer[PACKET_SIZE] = { 0 };
		memset(&buffer, 0, sizeof(buffer));

		cout << "아이디를 입력해주세요 : " ;
		cin >> temp_id;
		cout << "비밀번호를 입력해주세요 : ";
		cin >> temp_password;
		cout << endl;
		//서버에서 istringstream으로 구분해서 받게 하기 위한 공백 추가

		sends += (temp_id + " ");
		sends += temp_password;

		char* sendc = new char[sends.length() + 1];
		sendc[sends.length()] = '\n';

		//string char 배열로 바꿔서 보내기
		sends.copy(sendc, sends.length());

		send(soc, sendc, strlen(sendc), 0);
		recv(soc, buffer, PACKET_SIZE, 0);
		string login_num = buffer;
		
		if (login_num == "2") {
			coutid = (temp_id + "님 안녕하세요");
			realid = temp_id;
			login_status = 1;
			break;
		}

		else if(login_num == "0"){
			cout << "없는 아이디 입니다. 다시 입력 해주세요" << endl;
		}

		else if (login_num == "1") {
			cout << "비밀번호가 틀립니다. 다시 입력 해주세요" << endl;
		}
	}
}

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
		cout << "======================" << endl;
		cout << coutid << endl;
		cout << "======================" << endl;
		cout << "===   1. 회원가입  ===" << endl;
		cout << "===   2.  로그인   ===" << endl;
		cout << "===   3. 채팅하기  ===" << endl;
		cout << "===   4. 로그아웃  ===" << endl;
		cout << "===   5.  나가기   ===" << endl;
		cout << "======================" << endl;
		cout << "======================" << endl;
		cout << "현재 접속중인 친구 : " << endl;
		cout << "======================" << endl;
		cout << "======================" << endl;
		cin >> select;

		cout << endl;

		switch (select) {

			//회원가입
		case 1: {
			if (login_status) {
				cout << "이미 로그인 되어 있어요 ! 메인 화면으로 돌아갑니다." << endl << endl;
				break;
			}
			string sends = "1 ";

			char* sendc = new char[sends.length() + 1];

			cout << "가입하실 아이디를 입력해 주세요 : ";
			cin >> newid; 

			cout << "가입하실 비밀번호를 입력해 주세요 : ";
			cin >> newpassword; cout << endl;

			sends += newid + " ";
			sends += newpassword;
			sendc[sends.length()] = '\n';
			sends.copy(sendc, sends.length());

			send(soc, sendc, PACKET_SIZE, 0);
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
			cout << "========채팅시작=======" << endl;
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
					break;
				}
				if (buffer){
					cout << "내가 전달한 메시지 : " << buffer << endl;
				}
				send(soc, buffer, strlen(buffer), 0);
			}
			proc1.join();

			cout <<endl;

			break;
		}
			  // 로그아웃
		case 4: {

			if (!login_status) {
				cout << "로그인 상태가 아닙니다." << endl << endl;
				break;
			}

			string sends="101 ";
			sends += realid;
			char* sendc = new char[sends.length() + 1];
			sendc[sends.length()] = '\n';
			sends.copy(sendc, sends.length());

			login_status = 0;
			send(soc, sendc, PACKET_SIZE, 0);

			coutid = "로그인을 해주세요.";
			break;
		}
			  // 나가기
		case 5: {
			cout << "다음에 또 만나요" << endl;

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