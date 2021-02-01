#include <iostream>
#include <string>
#include <winsock2.h>

using namespace std;

void ShowErrorMessage(string message)  //에러 핸들링 관련 함수
{
	cout << "[오류 발생]" << message << '\n';
	system("pause");
	exit(1); // 프로그램 바로 종료
}
int main()
{
	WSADATA wsaData;
	SOCKET serverSocket, clientSocket;
	SOCKADDR_IN serverAddress, clientAddress;  //이 함수들 모두 winsock에 포함

	int serverPort=9876; // 서버포트 9876으로 설정
	char received[256];//받을 때 문자열 버퍼의 크기 256

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) //winsock을 초기화(2.2 버전을 사용하겠다.), 성공적으로 반환 시 0 반환. 따라서 성공적으로 반환 실패시 에러메세지
		ShowErrorMessage("WSAStartup()");
	
	serverSocket = socket(PF_INET, SOCK_STREAM, 0); // 소켓을 초기화 (TCP 소켓 생성)

	if (serverSocket == INVALID_SOCKET) //예외처리
		ShowErrorMessage("socket()");

	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET; //소켓 주소체계를 IPv4를 사용하겠다.
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); //h to nl 여기서 l은 long을 의미, 4 바이트 정수를  네트워크 바이트 형식으로
	serverAddress.sin_port = htons(serverPort); // s는 short을 의미하기 때문에 2 바이트 정수 네트워크 바이트 형식으로, severPort는 9876으로 위에 설정
	if (bind(serverSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		ShowErrorMessage("bind()");
	cout << "[현재 상태] bind()\n";

	if (listen(serverSocket, 5) == SOCKET_ERROR) //동시에 연결 요청하는 클라이언트 숫자가 총 5
		ShowErrorMessage("listen()");
	cout << "[현재 상태] listen()\n";

	int sizeClientAddress = sizeof(clientAddress);
	clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddress, &sizeClientAddress);
	cout << "[현재 상태] accept()\n";

	if (clientSocket == INVALID_SOCKET)
		ShowErrorMessage("accept()");
	while (1) {//클라이언트의 메세지를 받아서 그대로 전달
		int length = recv(clientSocket, received, sizeof(received), 0);
		received[length] = NULL; //예기치 문자 들어왔을 때 날려보내줌
		cout << "[클라인언트 메세지]:" << received << '\n';
		cout << "[메세지 전송]:" << received << '\n';
		if (strcmp(received, "[exit]") == 0) { //보낸 문자가 exit 이면 서버 종료
			send(clientSocket, received, sizeof(received) - 1, 0);// 엔터로 입력하기 때문에 마지막 공백문자는 -1로 날려줌
			cout << "[서버 종료]\n";
			break;
		}
		send(clientSocket, received, sizeof(received) - 1, 0);
	}
	closesocket(clientSocket);
	closesocket(serverSocket);
	WSACleanup();
	system("pause");
	return 0;
}

