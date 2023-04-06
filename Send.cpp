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
	// 创建了一个名为 ws 的 WSADATA 结构体变量。这个结构体是 Winsock2 的初始化函数 WSAStartup() 所需的参数之一。WSADATA 结构体包含了 Winsock2 环境的信息，包括 Winsock2 版本等。
	WSADATA ws;

	// 调用 WSAStartup() 函数初始化 Winsock2 环境。WSAStartup() 函数的第一个参数是用来指定 Winsock2 的版本号的。MAKEWORD(2, 2) 宏将 2 和 2 合并成一个无符号短整型整数，表示使用 Winsock2.2 版本。第二个参数是一个指向 WSADATA 结构体的指针，用于返回 Winsock2 环境的信息。
	WSAStartup(MAKEWORD(2, 2), &ws);

	// socket() 函数创建一个 UDP 套接字。socket() 函数的第一个参数是协议族，这里使用 AF_INET 表示 IPv4 协议族。第二个参数是套接字类型，这里使用 SOCK_DGRAM 表示创建一个数据报套接字，即 UDP 套接字。第三个参数是协议编号，通常设置为 0，表示使用默认协议。
	int client = socket(AF_INET, SOCK_DGRAM, 0);

	// 将char类型的buf转换成string类型 方法一：
	//   1.引入 #include<string>
	//   2.定义一个string类型的变量 ip_addrs
	//   3.定义一个char类型的指针 ip_addr，并初始化 (注："\0"代表空字符串，此行意思为初始化为空字符串)
	//   4.通过 &ip_addrs[0] 的方式获取变量 ip_addrs 所代表的字符串的首字符地址，并将其赋值给 ip_addr 指针变量。这样做是为了让指针变量 ip_addr 指向用户输入的字符串

	// 将char类型的buf转换成string类型 方法二：
	// c++11开始 std::string 类型提供了c_str() 方法，可以直接获取其内部存储的字符串的首字符地址
	//   1.创建一个string类型的对象 ip_addrs
	//   2.const char *ip_addr = ip_addrs.c_str();
	//   注：c_str()方法返回的是一个指向常量字符的指针，因此不能通过此指针修改原始字符串，所以注意野指针问题
	string ip_addrs;
	int ip_port;
	// char *ip_addr = "\0";
	cout << "请输入目标主机IP地址：";

	cin >> ip_addrs;
	const char *ip_addr = ip_addrs.c_str();
	// ip_addr = &ip_addrs[0];
	cout << "请输入目标主机端口号：";
	cin >> ip_port;

	sockaddr_in addr;
	addr.sin_family = AF_INET;						// addr.sin_family指定地址族，在本例中为 IPv4
	addr.sin_addr.S_un.S_addr = inet_addr(ip_addr); // addr.sin_addr.S_un.S_addr指定套接字的 IP 地址。该函数用于将 IP 地址从字符串格式转换为套接字可以使用的二进制格式
	addr.sin_port = htons(ip_port);					// addr.sin_port指定套接字的端口号

	while (true)
	{
		// 发送时间
		time_t now = time(NULL);
		string date = ctime(&now);
		// 解决ctime()函数中字符串最后一位是换行符问题：C++11 引入的函数 pop_back() ，擦除字符串的最后一个字符
		date.pop_back();

		string str;
		char *buf = "\0";
		// getline()方法是解决cin不能输入含空格的问题--接收一个字符串可以包含空格并输出但需引入 #include<string>
		getline(cin, str);
		buf = &str[0]; // 将字符串类型的变量 str 所代表的字符串的首地址赋给变量buf

		cout << "\033[36m┌┅「\033[0m \033[32mSend To: " << inet_ntoa(addr.sin_addr) << ":" << ntohs(addr.sin_port) << "\033[0m \033[36m」┅「\033[0m \033[33m" << date << "\033[0m \033[36m」" << endl
			 << "└┅\033[0m ";

		//cout << endl;
		int bufSize = strlen(buf);

		int sendRet = sendto(client, buf, bufSize, 0, (sockaddr *)&addr, sizeof(addr));
		if (sendRet == -1)
		{
			cout << "发送失败:" << errno << endl;
			exit(0);
		}
	}
	// 关闭WSAStart()函数，需调用WSACleanup()函数--这将释放Winsock库使用的资源并关闭该库。注： 在调用WSACleanup()函数之前，应确保所有套接字都已关闭。
	WSACleanup();

	// 关闭socket套接字的函数closesocket()--用于关闭套接字符描述，该描述符释放关联的资源并终止任何未完成的网络操作。注：此函数采用单个参数，一经调用套接字就不能继续用于通信
	closesocket(client);
	return 0;
}