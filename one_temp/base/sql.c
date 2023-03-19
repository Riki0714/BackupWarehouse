/*********************************************************************************
 *      Copyright:  (C) 2023 iot<iot@email.com>
 *                  All rights reserved.
 *
 *       Filename:  sql.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(03/03/23)
 *         Author:  iot <iot@email.com>
 *      ChangeLog:  1, Release initial version on "03/03/23 07:31:36"
 *                 
 ********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"
 
int main(int argc, char* argv[])
{
	 sqlite3 *db;
	 char *zErrMsg = 0;
	 int rc;
			  
     rc = sqlite3_open("test.db", &db);
				 
     if( rc )
	 {
			fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
			exit(0);
	 }
	 else
	 {
		    fprintf(stderr, "Opened database successfully\n");
	 }
	 sqlite3_close(db);

	 return 0;
}
