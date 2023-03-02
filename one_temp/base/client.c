/*********************************************************************************
 *      Copyright:  (C) 2023 iot<iot@email.com>
 *                  All rights reserved.
 *
 *       Filename:  client.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(02/26/23)
 *         Author:  iot <iot@email.com>
 *      ChangeLog:  1, Release initial version on "02/26/23 08:20:49"
 *                 
 ********************************************************************************/
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include "get_temp.h"


#define STR_LEN		128
#define TIME 		1000

#define INPUT_PARA_ERROR  -2
#define GET_TEMP_ERROR 	  -3

#define CONFIG_DEBUG
#ifdef  CONFIG_DEBUG
#define dbg_print(format, args...) printf(format, args)
#else
#define dbg_print(format, args...) do{} while(0)
#endif

/* 
name: 		print_usage
function:	Description of command line parameters
parameter:	proname--program name
value:		NULL
*/
void print_usage(char *proname)
{
	printf("hello, this is %s:\n", proname);
	printf("-i(--ipaddr): ip address of server\n");
	printf("-p(--port): port of server\n");
	printf("-t(--time): sampling interval\n");
	printf("-h(--help): some help\n");
}

int main(int argc, char *argv[])
{
	int 	cli_fd = -1;
	int		file_fd = -1;
	char    *ipaddress = NULL;
	int		port = 0;
	int		time = TIME;
	int		ch = -1;
	int 	rv = -1; //return value
	char 	serialNum[STR_LEN] = {0};

	struct option	opts[] = {
		{"ipaddr", required_argument,  NULL, 'i'},
		{"port",   required_argument,  NULL, 'p'},
		{"time",   optional_argument, NULL, 't'},
		{"help",   no_argument,        NULL, 'h'},
		{0, 0, 0, 0}
	};

	while( (ch=getopt_long(argc, argv, "i:p:t::h", opts, NULL)) != -1 )
	{
		switch(ch)
		{
			case 'i':
				ipaddress = optarg;
				break;

			case 'p':
				port = atoi(optarg);
				break;

			case 't':
				time = atoi(optarg);
				break;

			case 'h':
				print_usage(argv[0]);
				return 0;
		}
	}
  
	if( !port || !ipaddress || !time )
	{
		printf("ip or port error!\n");
		print_usage(argv[0]);
		return INPUT_PARA_ERROR;
	}

	printf("The entered ip and port are incorrect!\n");
	dbg_print("%s %d %d\n", ipaddress, port, time);

	if ( !get_temp_str(serialNum, STR_LEN) )
	{
		printf("get temperature error!\n");
		return GET_TEMP_ERROR;
	}
	dbg_print("%s", serialNum);

	return 0;
}
