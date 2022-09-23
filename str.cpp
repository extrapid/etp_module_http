#include "str.h" 
char *request302(const char *url, Res res)
{
	char *reshead = (char *)malloc(1024);
	sprintf(reshead, "%s 302 \r\nContent-Length: 0\r\nLocation: %s\r\n", res.version, url);
	return reshead;
}