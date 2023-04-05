#include <iostream>
#include <time.h>
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

	int client = socket(AF_INET, SOCK_DGRAM, 0);

	string ip_addrs;
	int ip_port;
	char *ip_addr = "\0";
	cout << "请输入目标主机IP地址：";
	cin >> ip_addrs;
	ip_addr=&ip_addrs[0];
	cout << "请输入目标主机端口号：";
	cin >> ip_port;

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = inet_addr(ip_addr); 
	addr.sin_port = htons(ip_port);

	while (true)
	{
		// 发送时间
		time_t now = time(NULL);
		string date = ctime(&now);
		date.pop_back();

		// 从这里开始
		string str;
		char *buf = "\0";

		cout << "\033[36m┌┅「\033[0m \033[32mSend To: " << inet_ntoa(addr.sin_addr) << ":" << ntohs(addr.sin_port) << "\033[0m \033[36m」┅「\033[0m \033[33m" << date << "\033[0m \033[36m」" << endl
			 << "└┅\033[0m ";
		getline(cin, str);
		buf = &str[0]; //******
		cout << endl;

		int bufSize = strlen(buf);
		// 到这里结束 重点学一学

		int sendRet = sendto(client, buf, bufSize, 0, (sockaddr *)&addr, sizeof(addr));
		if (sendRet == -1)
		{
			cout << "发送失败:" << errno << endl;
			exit(0);
		}
	}
}

// TODO：美化终端显示
// TODO：把他写的垃圾代码改成优秀代码，就是自己的了
// TODOEND：都做好之后，移植Linux