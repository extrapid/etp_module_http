#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "ThreadInfo.h"
#include "cJSON.h"
#include "addone.h"
#include "etp_log/log.h"
#include "etp_sdk/modulesdk.h"

LinkLib *Linklib;
cJSON *cjsonfile;
int Port = 8001;
char LogPath[256];
cJSON *UserAgentIndexPage;

SOCKET initBindSocket(const int);
ConnectInfo conneectClient(const SOCKET);
void handClient(int sock, struct sockaddr);
void *getcommand(LPVOID);
int initserver();
extern "C"
{
	ModuleType_t _init_(int proco)
	{
		switch (initserver())
		{
		case -1:
			extrapidLog(LOG_ERROR, "HTTP", "初始化失败");
			exit(1);
		case 1:
			initserver();
			break;
		default:
			break;
		}
		ModuleType_t mod = SDK_CreateModule("HTTP module", "This is a simple http server", 1);
		SDK_AddBindPort(&mod, Port, handClient);
		return mod;
	}
}

void *getcommand(LPVOID a)
{
	char cmd[256];
	while (1)
	{
		fgets(cmd, 256, stdin);
		cmd[strlen(cmd) - 1] = 0;

		if (!strcmp(cmd, "stop"))
		{
			printf("退出服务器\n");
			exit(0);
		}
		else if (!strcmp(cmd, "help"))
		{
			printf("服务器控制台帮助文档:\n");
			printf("输入stop退出服务器\n输入memuse查看进程内存使用信息\n输入bash进入系统bash控制台\n输入help查看帮助\n");
		}
		else if (!strcmp(cmd, "listapp"))
		{
			printf(">>开始列出application<<\n");
			LinkLib *temp = Linklib;
			while (temp)
			{
				printf("找到addone%s\n", temp->libname);
				Func *f = temp->funcslist;
				while (f)
				{
					printf("	存在函数:%s\n", f->funcname);
					f = f->next;
				}
				temp = temp->next;
			}
		}
		else if (!strcmp(cmd, "memuse"))
		{
			printf(">>进程内存使用信息<<\n");
			int info = open("/proc/self/status", O_RDONLY);
			char text[1024];
			read(info, text, 1024);
			// printf("%s\n",text);
			//打印内存峰值
			char *p = strstr(text, "VmPeak");
			printf("内存使用峰值:");
			for (int i = 0; p[i] != '\n' && p != NULL; i++)
			{
				putc(p[i], stdout);
			}
			putc('\n', stdout);
			//打印当前内存使用量
			p = strstr(text, "VmSize:");
			printf("当前内存使用量:");
			for (int i = 0; p[i] != '\n' && p != NULL; i++)
			{
				putc(p[i], stdout);
			}
			putc('\n', stdout);

			//打印当前物理内存使用量
			p = strstr(text, "VmRSS:");
			printf("当前内存使用量:");
			for (int i = 0; p[i] != '\n' && p != NULL; i++)
			{
				putc(p[i], stdout);
			}
			putc('\n', stdout);

			close(info);
			printf(">>信息打印完成<<\n");
		}
		else if (!strcmp(cmd, "bash"))
		{
			printf(">>进入bash,输入exit返回服务器控制台<<\n");
			system("bash");
			printf(">>返回服务器控制台<<\n");
		}
		else if (strstr(cmd, "addone"))
		{
			addone_ConsoleControl(cmd, &Linklib);
		}
		else if (strstr(cmd, "_app."))
		{
			char addonename[256];
			char funcname[256];
			char *p = strstr(cmd, "_app.");
			(*p) = 0; //将下划线暂时改为字符串结束
			strcpy(addonename, cmd);
			(*p) = '_'; //改回来
			// p+=5;//移动到函数名
			char *t = strstr(p + 5, " ");
			if (t != NULL)
			{
				*t = 0;
			}

			printf("调试:addonename:%s funcname:%s p:%s\n", addonename, funcname, p);
			if (addone_RunAddoneAsCommand(Linklib, addonename, funcname, cmd) != 0)
			{
				printf("UNKNOWN COMMAND!\n");
			}
		}
		else
		{
			printf("UNKNOWN COMMAND!\n");
		}
	}
}
