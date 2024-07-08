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

//채팅 하기 위한 코드
void thr_recvs() {
	char buffer[PACKET_SIZE] = { 0 };
	string cmd;
	while (!WSAGetLastError()) {
		memset(&buffer, 0, sizeof(buffer));
		recv(soc, buffer, PACKET_SIZE, 0);
		cmd = buffer;
		if (cmd == "hi") break;
		cout << "받은 메세지 : " << buffer << endl << endl;
	}
}

void login() {
	for (int i = 0; i < 5; i++) {

		/*cout << "5번 실패시 메인 화면으로 돌아갑니다. "<<i+1<<"번" << endl;*/
		string sends;
		char buffer[PACKET_SIZE] = { 0 };
		memset(&buffer, 0, sizeof(buffer));

		cout << "아이디를 입력해주세요" << endl;
		cin >> temp_id;
		cout << "비밀번호를 입력해주세요" << endl;
		cin >> temp_password;

		//서버에서 istringstream으로 구분해서 받게 하기 위한 공백 추가

		sends += (2 + " ");
		sends += (temp_id + " ");
		sends += temp_password;

		char* sendc = new char[sends.length() + 1];
		sendc[sends.length()] = '\n';

		//string char 배열로 바꿔서 보내기
		sends.copy(sendc, sends.length());

		send(soc, sendc, strlen(sendc), 0);
		recv(soc, buffer, PACKET_SIZE, 0);
		if (buffer[0] != '0') {
			coutid = (temp_id + "님 안녕하세요");
			realid = temp_id;
			login_status = 1;
			break;
		}
		else continue;
	}
}

int main() {

	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	soc = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

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
		cout << "현재 접속중인 친구 : "<< endl;
		cout << "======================" << endl;
		cin >> select;

		switch (select) {
		
		//회원가입
		case 1: 
			if (login_status) {
				cout << "이미 로그인 되어 있어요 ! 메인 화면으로 돌아갑니다." << endl << endl;
				break;
			}
			
			break;

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

			char buffer[PACKET_SIZE] = { 0 };
			char msg[PACKET_SIZE] = {};
			cout << endl;
			cout << "======================" << endl;
			cout << "=======채팅시작=======" << endl;
			cout << "======================" << endl << endl;

			thread proc1(thr_recvs);

			while (!WSAGetLastError()) {
				// 띄어쓰기도 받기 위해서 cin말고 cin.getline 사용
				cin.getline(buffer, PACKET_SIZE, '\n');
				if (buffer)
					cout << "내가 전달한 메시지 : " << buffer << endl << endl;
				send(soc, buffer, strlen(buffer), 0);
			}

			proc1.join();

			cout << "======================" << endl;
			cout << "=======채팅종료=======" << endl;
			cout << "======================" << endl << endl;

			break;
		}
		// 로그아웃
		case 4 : {

			if (!login_status) {
				cout << "로그인 상태가 아닙니다." << endl << endl;
				break;
			}

			string sends;
			sends += (101 + " ");
			sends += (realid);
			char* sendc = new char[sends.length() + 1];
			sendc[sends.length()] = '\n';
			sends.copy(sendc, sends.length());

			login_status = 0;
			send(soc, sendc, PACKET_SIZE, 0);
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
			return;

			//위에 로그아웃 질문
			
			//recv로 확실하게 너 연결 종료되었다 확인 받고 끝마치는게 맞는건가요 ?
			//아님 서버가 바쁘면 종료 길어지니까 일단 보내고 종료되게 하는게 맞나요 ?


		}

		//다시 메인 화면으로 돌아감
		continue;
	}
}