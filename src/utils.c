
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <libubox/ulog.h>
#include <uuid/uuid.h>

#include "utils.h"

// $ date "+%Y-%m-%dT%H:%M:%S%z"
// 2018-03-14T10:41:55+0800
// $ date "+%F %T%z"
// 2018-03-29 08:40:00+0800
char *get_timestamp(char *timestamp)
{
	static char outstr[200]= { 0 };	
	const char *strformat = "%Y-%m-%dT%H:%M:%S";
	char buffer[200]= { 0 };
	struct timespec ts;
	struct tm *tmp;
	
	clock_gettime(CLOCK_REALTIME, &ts);
	
	tmp = localtime(&ts.tv_sec);
	
	memset(outstr, 0, sizeof(outstr));
	if(0 == strftime(buffer, sizeof(buffer), strformat, tmp)) 
	{
		ULOG_ERR("strftime returned 0");
		return NULL;
	}
	snprintf(outstr, sizeof(outstr)-1, "%s.%d", buffer, (int)(ts.tv_nsec/1000));
	
	if(timestamp)
		strcpy(timestamp, outstr);	
	
	return outstr;
}

char *get_random(char *rand)
{
	static char uuid_str[33];
	
	char tmp[3] = { 0 };
	int i = 0;
	unsigned char *ptr;
	uuid_t buf;
	
	uuid_generate(buf);
	
	memset(uuid_str, 0, sizeof(uuid_str));
	ptr = (unsigned char *)buf;
	for(i = 0; i < 16; i++)
	{
		sprintf(tmp, "%2.2x", ptr[i]);
		strcat(uuid_str, tmp);
	}
	
	if(rand)
		memcpy(rand, uuid_str, 10); 
	
	return uuid_str;
}

