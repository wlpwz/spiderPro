#include "dso.h"
#include <iostream>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define BUFSIZE 65535
using namespace std;
int handleSavehtml(void* arg)
{

	const char* path = ((HandleData*)arg)->path;
	char* rev_data = ((HandleData*)arg)->data;	
	while(1)
    	{
		//创建一个文件mPage用于保存网页的主要信息
	    	int cofd;
		if(-1 == (cofd = open(path,O_RDWR|O_CREAT|O_APPEND,0766)))
		{
			perror("open or create Page fialed!\n");
			exit(1);
		}

		//将网页的主要内容(text)写入mPage中
		int cf;
		if(-1 == (cf = write(cofd,rev_data,strlen(rev_data))))
		{
			perror("Page写入主页信息失败!\n");
			break;
		}
		else if(cf == 0)
		{
			printf("Page写入主页信息字节数为0!\n");
			break;

		}
		else
		{
			printf("Page写入了主页信息:%d字节\n",cf);
			break;
		}


		//将text清空,方便接受其他网页信息
		memset(rev_data,0,BUFSIZE);

		//关闭文件描述符
		//close(chfd);
		close(cofd);
    	}
	//cout <<"success !!!!!!!!!!!!"<< (int)arg <<endl;
	return 1;
}

Module savehtml = {
    MODULE_MAGOR_VERSION,
    MODULE_SUB_VERSION,
    __FILE__,
    handleSavehtml
};
//g++ main.cpp dso.cpp qstring.cpp -rdynamic -ldl -g  -o main
