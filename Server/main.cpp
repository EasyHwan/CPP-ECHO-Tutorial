#include <iostream>
#include <string>
#include <winsock2.h>

using namespace std;

void ShowErrorMessage(string message)  //���� �ڵ鸵 ���� �Լ�
{
	cout << "[���� �߻�]" << message << '\n';
	system("pause");
	exit(1); // ���α׷� �ٷ� ����
}
int main()
{
	WSADATA wsaData;
	SOCKET serverSocket, clientSocket;
	SOCKADDR_IN serverAddress, clientAddress;  //�� �Լ��� ��� winsock�� ����

	int serverPort=9876; // ������Ʈ 9876���� ����
	char received[256];//���� �� ���ڿ� ������ ũ�� 256

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) //winsock�� �ʱ�ȭ(2.2 ������ ����ϰڴ�.), ���������� ��ȯ �� 0 ��ȯ. ���� ���������� ��ȯ ���н� �����޼���
		ShowErrorMessage("WSAStartup()");
	
	serverSocket = socket(PF_INET, SOCK_STREAM, 0); // ������ �ʱ�ȭ (TCP ���� ����)

	if (serverSocket == INVALID_SOCKET) //����ó��
		ShowErrorMessage("socket()");

	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET; //���� �ּ�ü�踦 IPv4�� ����ϰڴ�.
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); //h to nl ���⼭ l�� long�� �ǹ�, 4 ����Ʈ ������  ��Ʈ��ũ ����Ʈ ��������
	serverAddress.sin_port = htons(serverPort); // s�� short�� �ǹ��ϱ� ������ 2 ����Ʈ ���� ��Ʈ��ũ ����Ʈ ��������, severPort�� 9876���� ���� ����
	if (bind(serverSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		ShowErrorMessage("bind()");
	cout << "[���� ����] bind()\n";

	if (listen(serverSocket, 5) == SOCKET_ERROR) //���ÿ� ���� ��û�ϴ� Ŭ���̾�Ʈ ���ڰ� �� 5
		ShowErrorMessage("listen()");
	cout << "[���� ����] listen()\n";

	int sizeClientAddress = sizeof(clientAddress);
	clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddress, &sizeClientAddress);
	cout << "[���� ����] accept()\n";

	if (clientSocket == INVALID_SOCKET)
		ShowErrorMessage("accept()");
	while (1) {//Ŭ���̾�Ʈ�� �޼����� �޾Ƽ� �״�� ����
		int length = recv(clientSocket, received, sizeof(received), 0);
		received[length] = NULL; //����ġ ���� ������ �� ����������
		cout << "[Ŭ���ξ�Ʈ �޼���]:" << received << '\n';
		cout << "[�޼��� ����]:" << received << '\n';
		if (strcmp(received, "[exit]") == 0) { //���� ���ڰ� exit �̸� ���� ����
			send(clientSocket, received, sizeof(received) - 1, 0);// ���ͷ� �Է��ϱ� ������ ������ ���鹮�ڴ� -1�� ������
			cout << "[���� ����]\n";
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

