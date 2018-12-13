#include "winsock2.h"
#include "windows.h"
#pragma comment(lib, "ws2_32.lib")
#include <iostream>
#include <string>
#include <thread>
#pragma warning(disable:4996)

void SendString(SOCKET& s, std::string& str) {
	int size = str.size();
	send(s, (char*)&size, sizeof(int), 0);
	send(s, str.c_str(), size, 0);
}

std::string RecvString(SOCKET& s) {
	int str_size = 0;
	recv(s, (char*)&str_size, sizeof(int), NULL);//получить размер  строки
	char*msg = new char[str_size + 1];
	msg[str_size] = '\0';
	recv(s, msg, str_size, NULL);//получить строку
	std::string result = msg;
	delete[] msg;
	return result;
}

SOCKET connection;

void ClientHandler() {
	while (true) {
		std::string msg = RecvString(connection);
		if(msg.size()>0)
		std::cout << msg << std::endl;
	}
}

void main() {
	system("chcp 1251");
	system("cls");
	setlocale(0, "rus");
	WSADATA wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData)) {
		std::cout << "библиотека не загрузилась\n";
		return;
	}

	SOCKADDR_IN addr;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(connection, (SOCKADDR*)&addr, sizeof(addr))){
		std::cout << "не удалось соединиться с сервером\n";
		return;
	}
	std::thread t(ClientHandler);
	t.detach();
	std::string msg1;
	while (true) {
		std::getline(std::cin, msg1);
		SendString(connection, msg1);
	}

	

	system("pause");
}