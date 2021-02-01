#define _WINSOCK_DEPRECATED_NO_WARNINGS //inet_addr ����ϱ� ���ؼ�
#include <iostream>
#include <string>
#include <winsock2.h>

using namespace std;

void ShowErrorMessage(string message)  //���� �ڵ鸵 ���� �Լ�
{
	cout << "[���� �߻�]" << message << '\n';
	system("pause");
	exit(1);
}

int main()
{
	WSADATA wsaData;
	SOCKET clientSocket;
	SOCKADDR_IN serverAddress;

	int serverPort = 9876; // Port ��ȣ ������ �Ȱ��� 9876
	char received[256];
	string sent;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) //winsock�� �ʱ�ȭ(2.2 ������ ����ϰڴ�.), ���������� ��ȯ �� 0 ��ȯ. ���� ���������� ��ȯ ���н� �����޼���
		ShowErrorMessage("WSAStartup()");

	clientSocket = socket(PF_INET, SOCK_STREAM, 0); // ������ �ʱ�ȭ (TCP ���� ����)

	if (clientSocket == INVALID_SOCKET) //����ó��
		ShowErrorMessage("socket()");

	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // ���ڿ� IP�� ��Ʈ��ũ ����Ʈ ��������
	serverAddress.sin_port = htons(serverPort);
	if (connect(clientSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) // Ŭ���̾�Ʈ ������ �̿��Ͽ� Ư�� ������ Ŀ��Ʈ�Ѵ�
		ShowErrorMessage("connect()");
	cout << "[���� ����]connect()\n";

	while (1) { // �ݺ������� ������ �޼����� �����ϰ� ���� �޼����� ���� �޽��ϴ�
		cout << "[�޼��� ����]:";
		getline(cin, sent);
		if (sent == "") continue; //���͸� ġ�� ��쿡�� sent�� �����̴ϱ� ����
		send(clientSocket, sent.c_str(), sent.length(), 0); // sent ���ڿ� ���� string���� ����, ������� ���ڿ� ���� ���·� �ٲٷ��� c_str()
		int length = recv(clientSocket, received, sizeof(received), 0); //���ڸ޼��� �ٷ� ���� ����
		received[length] = '\0';  //�������� NULL ��
		if (strcmp(received, "[exit]") == 0) { //exit ���� ������ ���� ����
			cout << "[��������]\n";
			break;
		}
		cout << "[�����޼���]:" << received << '\n';

	}
	closesocket(clientSocket);
	WSACleanup();
	system("pause");
	return 0;
}