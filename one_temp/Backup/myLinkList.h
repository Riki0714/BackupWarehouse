/********************************************************************************
 *      Copyright:  (C) 2023 iot<iot@email.com>
 *                  All rights reserved.
 *
 *       Filename:  myLinkList.h
 *    Description:  This file 
 *
 *        Version:  1.0.0(03/25/23)
 *         Author:  iot <iot@email.com>
 *      ChangeLog:  1, Release initial version on "03/25/23 12:36:11"
 *                 
 ********************************************************************************/

#ifndef _MYLINKLIST_H
#define _MYLINKLIST_H

typedef char* 		elemType;

//Defines a single linked list node type
typedef struct	Node{
	elemType 		 element;
	struct Node		*next;
} Node;



#endif

