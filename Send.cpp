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

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1"); // TODO：域名解析 + IP 端口可设定
	addr.sin_port = htons(8080);

	while (true)
	{
		// 发送时间
		time_t now = time(NULL);
		char *date = ctime(&now);

		// 从这里开始
		string str;
		char *buf = "\0";

		cout << "┌┅ Send To: " << inet_ntoa(addr.sin_addr) << ":" << ntohs(addr.sin_port) << " ┅ "
			 << date << "└┅ ";
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