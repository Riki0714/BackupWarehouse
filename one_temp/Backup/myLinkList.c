/*********************************************************************************
 *      Copyright:  (C) 2023 iot<iot@email.com>
 *                  All rights reserved.
 *
 *       Filename:  myLinkList.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(03/25/23)
 *         Author:  iot <iot@email.com>
 *      ChangeLog:  1, Release initial version on "03/25/23 12:34:33"
 *                 
 ********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "myLinkList.h"

//-------------- Initializes a linked list so that the header pointer is null
void list_init(Node **pNode)
{
	*pNode = NULL;
	printf("Successful initialization\n");
}

//--------------
int list_insert_head(Node **pNode, elemType insertElem)
{
	Node *pIn;

	pIn = (Node *)malloc(sizeof(Node));
	memset(pIn, 0, sizeof(Node));
	//pIn = calloc(0, sizeof(Node));

	pIn->element = insertElem;
	pIn->next = *pNode;
	*pNode = pIn;

	return 1;
}

//--------------
//
int list_inset_tail(Node **pNode, elemType insertElem)
{
	Node *pIn, *pHead, *pTmp;

	pHead = *pNode;
	pTmp = pHead;
	pIn = calloc(0, sizeof(Node));

	while( NULL != pHead->next )
	{
		pHead = pHead->next;
	}
	pIn->element = insertElem;
	pHead->next = pIn;
	*pNode = pTmp;

	return 1;
}

//--------------
//
elemType * list_find(Node *pHead, elemType x)
{
	if( NULL == pHead )
	{
		printf("The list is empty\n");
		return 0;
	}

	if( x<0 )
	{
		printf("x is not allowed\n ");
		return 0;
	}

	while( (pHead->element != x) && (NULL != pHead->next) )
	{
		pHead = pHead->next;
	}

	if( (pHead->element != x) && (NULL != pHead) )
	{
		printf("can't find\n");
		return 0;
	}
	
	return &(pHead->element);
}

//--------------
//
char * list_get(Node *pHead, int pos)
{
	int		 i=0;
	char	 *buf="NULL";

	if(pos<1)
	{
		printf("pos is not allowed\n");
		return buf;
	}
	if( NULL == pHead )
	{
		printf("The list is empty\n");
		return buf;
	}

	while( NULL != pHead )
	{
		i++;
		if( i==pos ) break;
		pHead = pHead->next;
	}
	if( i<pos )
	{
		printf("can't find\n");
		return buf;
	}

	return pHead->element;
}

//--------------
//
int list_drop_head(Node **pNode)
{
	Node *one, *ptr;
	
	one = *pNode;

	if( NULL == one )
	{
		printf("The list is empty\n");
		return -1;
	}
	else if( one->next == NULL )
	{
		ptr = one;
		one = NULL;
		free(ptr);
		return 1;
	}

	*pNode = one->next;
	free(one);
	
	return 2;
}

//--------------
//
int list_drop_tail(Node **pNode)
{
	Node *pHead, *pTmp, *ptr;

	pHead = *pNode;
	pTmp = pHead;

	if( NULL == pHead )
	{
		printf("The list is empty\n");
		return -1;
	}
	else if( pHead->next == NULL )  //only one node
	{
		ptr = pHead;
		pHead = NULL;
		free(ptr);
		return 1;
	}

	while( NULL!=pHead->next )
	{
		ptr = pHead;
		pHead = pHead->next;
	}

	ptr->next = NULL;	
	free(pHead);
	*pNode = pTmp; //?

	return 2;
}

//--------------
int list_drop_pos(Node *pHead, int pos)
{
	Node 	*pFront, *pLast;
	int 	 i=0;

	if( pos<1 )
	{
		printf("pos is not allowed\n");
		return -1;
	}
	if( NULL == pHead )
	{
		printf("The list is empty\n");
		return -2;
	}

	while( NULL != pHead )
	{
		i++;
		if( i==pos ) break;

		pFront = pHead;
		pHead = pHead->next;
	}

	if( i<pos )
	{
		printf("can't find\n");
		return -3;
	}
	else if( i==pos )
	{
		pFront->next = NULL;
		free(pHead);
	}
	else 
	{
		pFront = pHead->next;
		free(pHead);
	}

	return 1;
}

//--------------
//
int list_clear(Node *pHead)
{
	Node *pNext;

	if( NULL == pHead )
	{
		printf("The list is empty\n");
		return -1;
	}

	while( NULL!=pHead->next )
	{
		pNext = pHead->next;
		free(pHead);
		pHead = pNext;
	}

	return 1;
}

//--------------
//
int list_isEmpty(Node *pHead)
{
	if( NULL == pHead)
	{
		printf("is empty\n");
		return 1;
	}
	printf("isn't empty\n");
	return -1;
}

//-------------- Print the contents of the linked list
//
int list_print(Node *pHead)
{
	if( NULL==pHead )
	{
		printf("The list is empty\n");
		return -1;
	}
	
	while( NULL != pHead )
	{
		printf("%s \n", pHead->element);
		pHead = pHead->next;
	}
	return 1;
}

//-------------- Calculate the length of the list
//
int list_size(Node *pHead)
{
	int len=0;
	
	if( NULL==pHead )
	{
		printf("The list is empty\n");
		return 0;
	}

	while( NULL!=pHead )
	{
		len++;
		pHead = pHead->next;
	}
	return len;
}


int main()
{
	Node *pList;
	char buf[32]="heiha";
	char buf1[32]="wuwu";
	char buf2[32]="33";

	list_insert_head(&pList, buf);
	list_insert_head(&pList, buf1);
	list_insert_head(&pList, buf2);

	list_drop_head(&pList);

	list_print(pList);
}




