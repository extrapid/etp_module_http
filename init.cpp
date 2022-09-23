#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include "cJSON.h"
#include "ThreadInfo.h"
#include "addone.h"
#include "log.h"
extern int Port;
extern cJSON *cjsonfile;
extern LinkLib *Linklib;
extern cJSON *UserAgentIndexPage;
extern char LogPath[256];

SOCKET initBindSocket(const int port)
{
	//创建套接字并且绑定端口
	//准备工作
	int opt = 1;
	int Fd = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(Fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); //设置端口复用，防止重启程序后无法绑定端口

	printf("[%s] [LOG] 正在创建监听套接字\n", gettime().time);
	if (-1 == Fd)
	{

		return execl("sudo", "./webserver", NULL);
	}
	int n = 1;
	struct sockaddr_in addr = {0};

	addr.sin_family = AF_INET; //和socket第一个参数一致
	addr.sin_port = htons(port);
	//小端转大端
	addr.sin_addr.s_addr = INADDR_ANY; //当前电脑上任意一个网卡都可以
	int sockfd;
	int r = bind(Fd, (struct sockaddr *)&addr, sizeof(addr));
	if (-1 == r)
	{
		printf("[%s] [ERROR] 端口绑定失败，原因:%s\n", gettime().time, strerror(errno));
		exit(0);
	}
	listen(Fd, 10);
	return Fd; //返回绑定完成端口的套接字
}

int initserver()
{
	printf("[%s] [LOG] 正在初始化服务器\n", gettime().time);
	if (access("conf/http_config.json", F_OK))
	{
		printf("[ERROR] 没有找到配置文件，尝试生成配置文件\n");
		system("mkdir conf");
		cJSON *config = cJSON_CreateObject();
		cJSON_AddStringToObject(config, "MimePath", "conf/mime.json");
		cJSON_AddNumberToObject(config, "PORT", 8080);
		cJSON_AddStringToObject(config, "LogPath", "logs/http");
		cJSON *arry = cJSON_CreateArray();
		cJSON_AddItemToObject(config, "UserAgentIndexPage", arry);
		FILE *fp = fopen("conf/http_config.json", "w");
		if (fp == NULL)
		{
			perror("无法创建配置文件，错误原因:");
			exit(1);
		}
		char *p = cJSON_Print(config);
		fprintf(fp, "%s", p);
		free(p);
		cJSON_Delete(config);
		fclose(fp);
		printf("[INFO] 配置文件conf/http_config.json生成成功\n");
		return 1;
	}
	FILE *fp = fopen("conf/http_config.json", "rb");
	if (fp == NULL)
	{
		//没有配置文件
		perror("无法打开配置文件，错误原因:");
		exit(1);
	}
	else
	{

		char temp[1024 * 1024];
		fread(temp, 1024, 1, fp);
		cJSON *json = cJSON_Parse(temp);
		if (json == NULL)
		{
			printf("[%s] [ERROR] 未找到配置文件conf/http_config.json\n", gettime().time);
			exit(1);
		}
		cJSON *p = cJSON_GetObjectItem(json, "MimePath");
		if (p == NULL)
		{
			printf("[%s] [ERROR] MimePath设置错误conf/http_config.json\n", gettime().time);
			exit(1);
		}
		FILE *mimefile = fopen(p->valuestring, "rb");
		if (mimefile == NULL)
		{
			printf("[%s] [ERROR] mime文件不存在\n", gettime().time);
			exit(1);
		}
		fread(temp, 1024 * 1024, 1, mimefile);
		cjsonfile = cJSON_Parse(temp);
		if (cjsonfile == NULL)
		{
			printf("[%s] [ERROR] mime格式错误\n", gettime().time);
			exit(1);
		}
		p = cJSON_GetObjectItem(json, "PORT");
		if (p == NULL)
		{
			printf("[%s] [WORNING] 没有设置port，使用默认8001\n", gettime().time);
			Port = 8001;
		}
		else
		{
			Port = p->valueint; //强制转换成正值
			printf("[%s] [LOG] 设置端口号:%d\n", gettime().time, Port);
		}
		p = cJSON_GetObjectItem(json, "LogPath");
		if (p == NULL)
		{
			printf("[%s] [WARNING] 没有设置日志文件目录，日志记录将无法生效\n", gettime().time);
		}
		else
		{
			strncpy(LogPath, p->valuestring, 256);
		}
		UserAgentIndexPage = cJSON_GetObjectItem(json, "UserAgentIndexPage");

		// cJSON_Delete(json);//因为需要解析UA所以这个JSON数据不释放
		//加载插件
		printf("[%s] [LOG] 开始加载插件\n", gettime().time);
		Linklib = addone_MakeFuncLink(&Linklib, "app");
		printf("[%s] [LOG] 加载完成\n", gettime().time);
		return 0;
	}
}
