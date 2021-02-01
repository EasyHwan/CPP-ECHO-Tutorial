#define _WINSOCK_DEPRECATED_NO_WARNINGS //inet_addr 사용하기 위해서
#include <iostream>
#include <string>
#include <winsock2.h>

using namespace std;

void ShowErrorMessage(string message)  //에러 핸들링 관련 함수
{
	cout << "[오류 발생]" << message << '\n';
	system("pause");
	exit(1);
}

int main()
{
	WSADATA wsaData;
	SOCKET clientSocket;
	SOCKADDR_IN serverAddress;

	int serverPort = 9876; // Port 번호 서버와 똑같이 9876
	char received[256];
	string sent;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) //winsock을 초기화(2.2 버전을 사용하겠다.), 성공적으로 반환 시 0 반환. 따라서 성공적으로 반환 실패시 에러메세지
		ShowErrorMessage("WSAStartup()");

	clientSocket = socket(PF_INET, SOCK_STREAM, 0); // 소켓을 초기화 (TCP 소켓 생성)

	if (clientSocket == INVALID_SOCKET) //예외처리
		ShowErrorMessage("socket()");

	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // 문자열 IP를 네트워크 바이트 형식으로
	serverAddress.sin_port = htons(serverPort);
	if (connect(clientSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) // 클라이언트 소켓을 이용하여 특정 서버를 커넥트한다
		ShowErrorMessage("connect()");
	cout << "[현재 상태]connect()\n";

	while (1) { // 반복적으로 서버에 메세지를 전송하고 에코 메세지를 전달 받습니다
		cout << "[메세지 전송]:";
		getline(cin, sent);
		if (sent == "") continue; //엔터만 치는 경우에는 sent가 공백이니까 무시
		send(clientSocket, sent.c_str(), sent.length(), 0); // sent 문자열 변수 string으로 정의, 씨언어의 문자열 버퍼 형태로 바꾸려면 c_str()
		int length = recv(clientSocket, received, sizeof(received), 0); //에코메세지 바로 전달 받음
		received[length] = '\0';  //마지막에 NULL 값
		if (strcmp(received, "[exit]") == 0) { //exit 전달 받으면 서버 종료
			cout << "[서버종료]\n";
			break;
		}
		cout << "[서버메세지]:" << received << '\n';

	}
	closesocket(clientSocket);
	WSACleanup();
	system("pause");
	return 0;
}