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
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>

#include "get_temp.h"
#include "mySocket.h"
#include "myDns.h"
#include "myTimer.h"
#include "mySql.h"
#include "sqlite3.h"

#define STR_LEN		256
#define NAME_LEN	64
#define TIME 		5

#define INPUT_PARA_ERROR  	-2
#define GET_TEMP_ERROR 	  	-3
#define BASENAME			"cliData.db"
#define TABELNAME			"TEMP"

//#define CONFIG_DEBUG
#include "myDebug.h"

void print_usage(char *proname); //dns man
void timer(int sig);  	//timer
void sig_timer(int sig);

int alrm_flag=1; //timer flag

int main(int argc, char *argv[])
{
	sock_infor	cli_infor_t;
	int		file_fd = -1;
	int		time = TIME; //sample interval
	int		ch = -1; //Parameter resolution return value
	int 	rv = -1; //return value
	char 	serialNum[STR_LEN] = {0}; //The obtained temperature string
	int		errorFlag = 0;
	int		link_flag = 0;
	char    buf[STR_LEN]={0};
	char    ReceBuf[STR_LEN]={0};
	char	bufToDb[NAME_LEN]={0};

	sqlite3		 *db=NULL;
	char		 *errmsg = NULL;
	int			  dataIndex = 0;
	//char		  dbName[NAME_LEN] = BASENAME;
	char		 *dbName = BASENAME;
	//char		  tbName[NAME_LEN] = TABELNAME;
	char		 *tbName = TABELNAME;

	memset(&cli_infor_t, 0, sizeof(cli_infor_t));
	cli_infor_t.ip = (char *)&rv;
	cli_infor_t.fd = -1;

	//-------------- Command line argument parsing  --------------
	struct option	opts[] = {
		{"ipaddr", 	 required_argument,  NULL, 'i'},
		{"port",   	 required_argument,  NULL, 'p'},
		{"time",  	 optional_argument,  NULL, 't'},
		{"dbName",   optional_argument,  NULL, 'a'},
		{"tbName",   optional_argument,  NULL, 'b'},
		{"help",  	 no_argument,        NULL, 'h'},
		{0, 0, 0, 0}
	};

	while( (ch=getopt_long(argc, argv, "i:p:t::a::b::h", opts, NULL)) != -1 )
	{
		switch(ch)
		{
			case 'i':
			{
				int 	getLen = strlen(optarg);
				char 	getStr[100] = {0};

				strncpy(getStr, optarg, 100);
				if( (getStr[getLen-1]>'a' && getStr[getLen-1]<'z') ||  (getStr[getLen-1]>'A' && getStr[getLen-1]<'Z') )
				{
					cli_infor_t.ip = get_ip_dns(optarg ,100); //dns
				}
				else
					cli_infor_t.ip = optarg;

				break;
			}

			case 'p':
				cli_infor_t.port = atoi(optarg);
				break;

			case 'a':
				dbName = optarg;
				break;

			case 'b':
				tbName = optarg;
				break;

			case 't':
				time = atoi(optarg);
				break;

			case 'h':
				print_usage(argv[0]);
				return 0;
		}
	}
	//----------------------------------------
  
	if( !cli_infor_t.port || !cli_infor_t.ip )
	{
		printf("The entered ip and port are incorrect!\n");
		print_usage(argv[0]);
		return INPUT_PARA_ERROR;
	}
	printf("The entered ip and port are correct!\n");
	dbg_print("%s %d %d\n", cli_infor_t.ip, cli_infor_t.port, time);

	//---------------------
	rv = -1;
	rv = sqlite3_open(dbName, &db);
	if(rv)
	{
		printf("open database %s failure: %s\n", dbName, sqlite3_errmsg(db));
		sqlite3_close(db);
		return -12;
	}
	if( sql_op(db, tbName, FIND, NULL) )
	{
		sql_op(db, tbName, CREATE, "id int, content char");
	}
		//sql_op(db, "table1", CREATE, "id int, content char");
	//sqlite3_exec(db, "INSERT INTO TEMP VALUES(0, '------ new data ------')", NULL, NULL, &errmsg);
	dbg_print("Open database successfully! %d\n", rv);

	//------------ connect to server -------------
	printf("Now the client tries the server...\n");
	if( (rv = client_init(&cli_infor_t) < 0) )
	{
		printf("Failed to connect to the server\n");
		link_flag = 0;
	}
	else  link_flag = 1;
	
	while(1)
	{
		//----------- Acquired temperature -------------
		if ( !get_temp_str(serialNum, STR_LEN) )
		{
			printf("get temperature error!\n");
			errorFlag++;

			if(errorFlag==5)
			{
				printf("Sorry, please check your hardware device and try again\n");
				return -1; //If you can't get it, an error exit is reported
			}
			else
			{
				printf("Unable to obtain temperature, trying again...\n");
				sleep(5);
				continue;
			}
		}
		errorFlag = 0;
		dbg_print("%s\n", serialNum);
		printf("%s\n", serialNum);
			
		//------------ Send data to the server -------------
		if( link_flag )
		{
			dbg_print("hei %d\n", link_flag);
			if( write(cli_infor_t.fd, serialNum, strlen(serialNum)) < 0 )
			{
				printf("Write %d bytes data back to client[%d] failure: %s\n", rv, cli_infor_t.fd, strerror(errno));
				close(cli_infor_t.fd);
				link_flag = 0;

				continue;
				//put into database
				//goto EXIT1;
			}

			memset(ReceBuf, 0, sizeof(ReceBuf));
			if( ( rv=read(cli_infor_t.fd, ReceBuf, sizeof(buf)) )  < 0 )
			{
				printf("read data from server[%d] failure: %s\n", cli_infor_t.fd, strerror(errno));
				link_flag = 0;
				continue;
			}
			else if( rv==0 )
			{
				printf("The connection to the server[%d] is disconnected\n", cli_infor_t.fd);
				link_flag = 0;
				continue;
			}
			printf("read %d bytes data from client[%d] and echo it back: '%s\n'", rv, cli_infor_t.fd, ReceBuf);
		}
		
		if( !link_flag ) //Failed to connect to the server
		{
			//------------ Put into database ---------------
			if(dataIndex<10000) dataIndex++;
			else dataIndex=1;
			
			dbg_print("%s\n", serialNum);
			//memset(bufToDb, 0, sizeof(bufToDb));
			snprintf(bufToDb, sizeof(bufToDb), "%d, '%s'", dataIndex, serialNum);
			dbg_print("%s\n", bufToDb);

			sql_op(db, tbName, INSERT, bufToDb);
			printf("Successfully put data into the database [%s - %s]\n", dbName, tbName);

			//------------ reconnection --------------------
			if( (rv = client_init(&cli_infor_t) < 0) )
			{
				printf("Failed to connect to the server\n");
				link_flag = 0;
			}
			else
			{
				link_flag = 1;
				dataIndex = 0;
			}

			//Send everything in the database to the server
		}

		dbg_print("hei22 %d\n", link_flag);
		//close(cli_infor_t.fd);
		timer_s(time, 0);
		
	}

//EXIT1:
	sqlite3_close(db);

	return 0;
}

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


void sig_timer(int sig)
{
	alrm_flag = 1;
	//alarm(5);
}


