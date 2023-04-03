#include <iostream>
#include <string>
#include <thread>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1
using namespace std;

int main()
{
	WSADATA ws;
	WSAStartup(MAKEWORD(2, 2), &ws);

	int server = socket(AF_INET, SOCK_DGRAM, 0);

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(8080);

	int bindRet = bind(server, (sockaddr *)&addr, sizeof(addr));
	if (bindRet == -1)
	{
		cout << "绑定端口失败，或许端口已经被占用" << errno << endl;
		exit(0);
	}

	// cout << "udp服务启动，正确8080端口监听" << endl;

	// 来者是谁
	sockaddr_in fromAddr;
	int fromAddrLen = sizeof(fromAddr);

	while (true)
	{
		// TODO：接收时间
		char buf[1024] = {0};
		/*
		  int fd             : 套接字描述符
		  char* buf          : 缓冲区
		  int bufSize        : 缓冲区大小
		  int flags          : 给0就可以了
		  sockaddr* addr     : 用来保存接收到的地址
		  int* addrLen       : 用来保存前一个参数的长度
		  成功返回实际接收的字节数
		  失败返回-1
		*/
		int recvRet = recvfrom(server, buf, sizeof(buf), 0, (sockaddr *)&fromAddr, &fromAddrLen);
		if (recvRet == -1)
		{
			cout << "接收失败:请重新输入" << errno << endl;
			break;
		}

		// 接收时间
		time_t now = time(NULL);
		char *date = ctime(&now);

		// IP端口号表示:127.0.0.1:8080
		cout << "┌┅ Receive from: " << inet_ntoa(addr.sin_addr) << ":" << ntohs(addr.sin_port) << " ┅ "
			 << date << "└┅ " << buf << endl
			 << endl;
	}

	cout << "服务运行结束:" << errno << endl;
}
