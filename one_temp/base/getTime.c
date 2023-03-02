/*********************************************************************************
 *      Copyright:  (C) 2023 iot<iot@email.com>
 *                  All rights reserved.
 *
 *       Filename:  getTime.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(02/28/23)
 *         Author:  iot <iot@email.com>
 *      ChangeLog:  1, Release initial version on "02/28/23 13:56:56"
 *                 
 ********************************************************************************/

#include <time.h>
#include <stdio.h>

int main()
{
	time_t t;
    struct tm *tmp,*tmp2;
    char buf[64], buf2[64];

    time(&t);
					      
	tmp = localtime(&t);
	strftime(buf, 64, "time and date: %F %T", tmp);
	printf("%s\n", buf);

	tmp2 = gmtime(&t);
	strftime(buf2, 64, "time and date: %F %T", tmp2);
	printf("%s\n", buf2);
}
