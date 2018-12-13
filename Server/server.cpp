#include "winsock2.h"
#include "windows.h"
#pragma comment(lib, "ws2_32.lib")
#include <iostream>
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

SOCKET Connections[512];
int nowcountconnections;
void HandleClient(int index) {
	
	while (true) {

		std::string msg = RecvString(Connections[index]);
		for (int i = 0; i < nowcountconnections; i++) {
			if (i == index) 
				continue;
			if(msg.size()>0)
				SendString(Connections[i], msg);
		}
		
	}
	
}

void main() {
	setlocale(0, "rus");

	WSADATA wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData)) {
		std::cout << "библиотека не загрузилась\n";
		return;
	}
	else {
		std::cout << "ожидание подключения....\n";
	}

	SOCKADDR_IN addr;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);

	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	listen(sListen, SOMAXCONN);
	int sizeofaddr = sizeof(addr);

	for (int c = 0; c < 512; c++) {
		Connections[c] = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);

		if (!Connections){
			std::cout << "клиент не смог пройти проверку";
			return;
		}
		else {
			std::cout << "клиент ip: "<<inet_ntoa(addr.sin_addr) << " подключен\n";
			std::thread t(HandleClient, nowcountconnections++);
			t.detach();
		}
	}
	system("pause");
}