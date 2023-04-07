#include <iostream>
#include <string>
#include <thread>
#include <winsock2.h>
#include <cstring>
#pragma comment(lib, "ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1
using namespace std;

int main()
{
	WSADATA ws;
	WSAStartup(MAKEWORD(2, 2), &ws);
	int server = socket(AF_INET, SOCK_DGRAM, 0);

	int ip_port;
	cout << "请输入目标主机端口号：";
	cin >> ip_port;

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY; // 将套接字绑定到任意可用的IP地址上
	char *ip_addr = inet_ntoa(addr.sin_addr);
	addr.sin_port = htons(ip_port);

	// 将套接字绑定到IP地址和端口号
	int bindRet = bind(server, (sockaddr *)&addr, sizeof(addr));
	if (bindRet == -1)
	{
		cout << "绑定端口失败，或许端口已经被占用" << errno << endl;
		closesocket(server);
		WSACleanup();
		return 1;
	}

	while (true)
	{
		// 来者是谁
		sockaddr_in fromAddr;
		int fromAddrLen = sizeof(fromAddr);

		// 接收数据
		char buf[1024] = {0};
		int recvRet = recvfrom(server, buf, sizeof(buf), 0, (sockaddr *)&fromAddr, &fromAddrLen);
		if (recvRet == -1)
		{
			cout << "接收失败！" << errno << endl;
			break;
		}

		// 接收时间
		time_t now = time(NULL);
		string date = ctime(&now);
		date.pop_back();

		// IP端口号表示:127.0.0.1:8080
		cout << "\033[36m┌┅「\033[0m \033[32mReceive from: " << inet_ntoa(fromAddr.sin_addr) << ":" << ntohs(fromAddr.sin_port) << "\033[0m \033[36m」┅「\033[0m \033[33m" << date << "\033[0m \033[36m」" << endl
			 << "└┅\033[0m " << buf << endl
			 << endl;
	}

	closesocket(server);
	WSACleanup();

	return 0;
}
