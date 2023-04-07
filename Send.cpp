#include <iostream>
#include <time.h>
#include <string>
#include <thread>
#include <cstdlib>
#include <cstdio>
#include <winsock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1
using namespace std;

string GetDNS(string ip_addrs)
{
	// 将string类型转为char *
	const char *ip_addr = ip_addrs.c_str();

	// 创建WSA和socket
	WSADATA ws;
	int result = WSAStartup(MAKEWORD(2, 2), &ws);
	int client = socket(AF_INET, SOCK_DGRAM, 0);

	// 创建两个ADDRINFO变量
	// result_list - 存储结果列表
	// hints - 用于定义怎样和DNS服务器通讯
	struct addrinfo *result_list = NULL;
	struct addrinfo hints;
	// 初始化 将hints变量的内存，全部初始化为0
	memset(&hints, 0, sizeof(hints));
	// 支持IPv4和IPv6
	hints.ai_family = AF_UNSPEC;
	// 使用TCP协议
	hints.ai_socktype = SOCK_STREAM;
	// 使用TCP协议
	hints.ai_protocol = IPPROTO_TCP;
	// 获取ADDRINFO，并将结果保存在result_list
	client = getaddrinfo(ip_addr, NULL, &hints, &result_list);
	// 判断函数返回值 是否解析成功
	if (client != 0)
	{
		std::cerr << "getaddrinfo failed: " << gai_strerror(client) << std::endl;
		closesocket(client);
		WSACleanup();
		return "解析失败！";
	}

	// 创建一个用于保存IP地址的char数组
	char ipstr[INET_ADDRSTRLEN];
	// 遍历result_list
	for (ADDRINFOA *ptr = result_list; ptr != NULL; ptr = ptr = ptr->ai_next)
	{
		void *addr;
		// 获取IP地址
		// 如果是IPv4的话
		if (ptr->ai_family == AF_INET)
		{
			// 创建一个sockaddr_in指针，将ptr的addr强制转换并赋值
			sockaddr_in *ipv4 = (sockaddr_in *)ptr->ai_addr;
			addr = &(ipv4->sin_addr);
		}
		// 如果是IPv6的话
		else
		{
			sockaddr_in6 *ipv6 = (sockaddr_in6 *)ptr->ai_addr;
			addr = &(ipv6->sin6_addr);
		}
		inet_ntop(ptr->ai_family, addr, ipstr, sizeof(ipstr));
		cout << ip_addrs << "解析为：" << ipstr << endl;
	}
	// 返回IP字符串
	return ipstr;
}

int main()
{

	// 1.创建了一个名为 ws 的 WSADATA 结构体变量。
	WSADATA ws;

	// 调用 WSAStartup() 函数初始化 Winsock2 环境。
	// MAKEWORD(2, 2) - 将 2 和 2 合并成一个无符号短整型整数，表示使用 Winsock2.2 版本
	// &ws - 引用 ws 变量
	// result - 函数返回值
	int result = WSAStartup(MAKEWORD(2, 2), &ws);

	// 创建了一个socket
	// AF_INET - 表示 IPv4 协议
	// SOCK_DGRAM - 表示创建一个 UDP 套接字
	// 0 - 协议编号，通常设置为 0，表示使用默认协议。
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
	cout << "请输入目标主机IP地址：";
	cin >> ip_addrs;
	ip_addrs = GetDNS(ip_addrs);
	const char *ip_addr = ip_addrs.c_str();
	// ip_addr = &ip_addrs[0];
	cout
		<< "请输入目标主机端口号：";
	cin >> ip_port;

	sockaddr_in addr;
	addr.sin_family = AF_INET;						// addr.sin_family指定地址族，在本例中为 IPv4
	addr.sin_addr.S_un.S_addr = inet_addr(ip_addr); // addr.sin_addr.S_un.S_addr指定套接字的 IP 地址。该函数用于将 IP 地址从字符串格式转换为套接字可以使用的二进制格式
	addr.sin_port = htons(ip_port);					// addr.sin_port指定套接字的端口号

	// 这里需要cin.ignore()
	// 原因在于下面的getline会获取到上面cin的最后一个回车，导致执行被跳过（体现为：发送一个换行符）
	// 但如果放在循环内会影响下次循环的字符获取
	cin.ignore();

	while (true)
	{
		// 发送时间
		time_t now = time(NULL); // 该函数是一个标准库函数，通过将参数传递给函数，将从系统时钟中检索当前时间。
		string date = ctime(&now);
		date.pop_back(); // 解决ctime()函数中字符串最后一位是换行符问题：C++11 引入的函数 pop_back() ，擦除字符串的最后一个字符

		// 输出命令提示符
		cout << "\033[36m┌┅「\033[0m \033[32mSend To: " << inet_ntoa(addr.sin_addr) << ":" << ntohs(addr.sin_port) << "\033[0m \033[36m」┅「\033[0m \033[33m" << date << "\033[0m \033[36m」" << endl
			 << "└┅\033[0m ";

		// 输入要发送的数据
		string str;
		char *buf = "\0";
		int bufSize = strlen(buf);
		getline(cin, str); // getline()方法是解决cin不能输入含空格的问题
		buf = &str[0];	   // 将字符串类型的变量 str 所代表的字符串的首地址赋给变量buf

		// 此代码使用 UDP 协议将大小缓冲区中包含的消息发送到指定的远程主机。远程主机由其 IP 地址和端口号标识，这些地址和端口号存储在 指向的结构中。
		int sendRet = sendto(client, buf, bufSize, 0, (sockaddr *)&addr, sizeof(addr));
		if (sendRet == -1)
		{
			cout << "发送失败:" << errno << endl;

			// 关闭socket套接字的函数closesocket()--用于关闭套接字符描述，该描述符释放关联的资源并终止任何未完成的网络操作。注：此函数采用单个参数，一经调用套接字就不能继续用于通信
			closesocket(client);
			// 关闭WSAStart()函数，需调用WSACleanup()函数--这将释放Winsock库使用的资源并关闭该库。注： 在调用WSACleanup()函数之前，应确保所有套接字都已关闭。
			WSACleanup();
			return 1;
		}
	}

	closesocket(client);
	WSACleanup();
	return 0;
}
