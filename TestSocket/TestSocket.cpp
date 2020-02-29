// TestSocket.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#ifdef WIN32
#include <Windows.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#define closesocket close
#endif

#include <stdio.h>
#include <iostream>

int main(int argc, char* argv[])
{
	// linux 中不需要初始化
#ifdef WIN32
	// 加载动态库
	WSADATA ws;
	// 动态库引用加一 windows 中动态库只加载一遍， 所有程序退出之后，才会卸载掉
	// 先调用
	WSAStartup(MAKEWORD(2, 2), &ws);
#endif // WIN32
	// TCP/IP 协议， 第二个参数选择TCP， 函数的返回值为int
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	// 失败处理
	// 实际上是创建一个文件， 在windows上比较难失败， 但是在linux上给每一个文件上分配的句柄数是有限的
	if (sock == -1)
	{
		std::cout << "create socket failed" << std::endl;
		return -1;
	}
	printf("[%d] ", sock);
	// 在所有处理完成之后关闭连接
	// 关闭后再打开可以利用上一次的资源

	// 如果传了参数，指定port
	unsigned short port = 8080;
	if (argc > 1)
	{
		if(argv[1])
		port = atoi(argv[1]);
	}
	sockaddr_in saddr;
	// 指定协议
	saddr.sin_family = AF_INET;
	// 本地字节序转换为网络字节序
	saddr.sin_port = htons(port);
	// bind ip addr
	// 1. 可以绑定本地 ip， 只能本地访问
	// 2. 绑定外网ip， 只有外网能访问
	// 可能存在多个网卡
	// 绑定0， 接受任意ip地址发来的数据
	// 实际上是使用int来表示地址， 四个字节， 每个字节0-255， 刚好表示ip addr
	saddr.sin_addr.s_addr = htonl(0);

	// bind, 传入大小是为了 重载使用
	// 返回值不为零则失败
	// 绑定端口很容易失败
	// 对于bind函数， 告知绑定的端口号
	if (bind(sock, (sockaddr*)& saddr, sizeof(saddr)) != 0)
	{
		std::cout << "bind port: " << port <<  " failed" << std::endl;
		return -2;
	}
	std::cout << "bind port: " << port << " successfully" << std::endl;
	// 10-> 第二个参数为 套接字使用队列的长度， 能存的连接的信息的数量， 可以理解为一个缓冲大小
	listen(sock, 10);

	// 接受用户的连接， 读取连接信息
	// 第二个参数和第三个参数，可以直接设为0， 表示不关心对方的信息， 也会读取缓冲
	// 会创建一个新的socket， 并返回
	// 传入的sock只用来建立连接
	// 新返回的sock是用来发送数据和客户端进行独立通信的

	int client = accept(sock, 0, 0);
	std::cout << "accept client: " << client << std::endl;
	closesocket(client);


	// linux 中关闭为 close
	// closesocket(sock);
	// 为了在 win中暂停窗口
	getchar();

}


// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
