#include<stdio.h>
#include "messagequeue.h"
#include "pm.h"
#include <minix/callnr.h>
#include <minix/com.h>
#include <minix/type.h>
#include <minix/endpoint.h>
#include <minix/type.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include "param.h"
#include <lib.h>
#define MAX_MESG_LEN 100
int ret1 = 0;


/*-----MY System Calls Start Here.....*/

int do_mq_open()
{
	//printf("\n SYSCALL: MINIT");
	queue_creat();
	return 1;
}

int do_queue_search(void) //58
{
	int ret;

	//printf("\n SYSCALL: MSEARCH...");

	char nam[100];
	//copy the message from PM-->MESG
	sys_datacopy(m_in.m_source, (vir_bytes) m_in.m7_p1, PM_PROC_NR,
			(vir_bytes)nam, 100);

	strcat(nam,"\0");

	ret = que_list(nam);

	return ret;
}


int do_search_sender(void) //108
{

	int sid, regnum;
	int ret;

	//printf("SYSCALL: MSENDER");

	sid = m_in.m7_i1;
	regnum = m_in.m7_i2;
	ret=add_sender_list(sid, regnum);

	return ret;

}

int do_search_reciver(void) //109
{

	int rid, regnum;
	int ret;

	//printf("SYSCALL: Mreciver");

	rid = m_in.m7_i2;
	regnum= m_in.m7_i1;

	ret=add_recv_list(rid, regnum);

	return ret;

}

int do_mq_send(void) //69
{
	int ret,check;
	int  SID, destrec[5], no_of_recv;
	//printf("\n SYSCALL: mq_send");

	char mesg[100];
	char que[100];
	//copy the message from PM-->MESG
	sys_datacopy(m_in.m_source, (vir_bytes) m_in.m7_p1, PM_PROC_NR,
			(vir_bytes)mesg, 100);
	sys_datacopy(m_in.m_source, (vir_bytes) m_in.m7_p2, PM_PROC_NR,
				(vir_bytes)que, 100);


	strcat(mesg,"\0");
	strcat(que,"\0");
	check = que_list(que);
	if (check == 1)
	{

	//copy sendID & no. of receivers...
	SID=m_in.m7_i1;
	no_of_recv=m_in.m7_i2;

	destrec[0]=m_in.m7_i3;
	destrec[1]=m_in.m7_i4;
	destrec[2]=m_in.m7_i5;
	printf("\n MESSAGE=%s & SENDER=%d & RECEIVERS=%d", mesg,SID,no_of_recv);

	if(strlen(mesg) > MAX_MESG_LEN)
			return MSG_LEN_OVERFLOW_ERROR;

	ret=message_send(mesg, SID, destrec, no_of_recv);  //calling to mess.c

	return ret;
	}
	else{
		printf("Dude the queue was not there \n");
		return -1;
	}
	return 0;
}


int do_mq_receiver()  //70
{
	//printf("\n SYSCALL: mq_receiver");
	int i;
	char *mesg1[100], temp[100],que[100];
	int rd, ret,check;
	sys_datacopy(m_in.m_source, (vir_bytes) m_in.m7_p2, PM_PROC_NR,
					(vir_bytes)que, 100);

	bzero(temp,100);
	strcat(que,"\0");
	check = que_list(que);

	if(check ==1)
	{

	rd=m_in.m7_i2;

	ret=message_receive(mesg1, rd);

    if(ret > 0)
	{
		//printf("\n message received in PM=%s ", mesg1[0]);
		i=0;
    	while(i < ret)
    	{
    		strcat(temp, mesg1[i]);
    		strcat(temp, "->");
    		i++;
    	}

		sys_datacopy(PM_PROC_NR, (vir_bytes) temp,
		    		m_in.m_source,(vir_bytes) m_in.m7_p1, MAX_MESG_LEN);
		return ret;
	}
	else
		printf("\n No messages brother \n");

	return 0;
	}
	else
	{
		printf("Queue was not there ");
		return 0;
	}
}


int do_mq_close()  //56
{

	int val = m_in.m7_i1;
	//printf("\nSYSCALL: Clean and Close");
	if(val == 3)
	{
		printf("\n Completely delete the message queue");
		memset( recv_list,0, sizeof(recv_list) );
		memset( sender_list,0, sizeof(sender_list) );
		memset( message1, 0, sizeof(message1));
		no_of_recv = 0;
		no_of_sender = 0;
		mess_cnt =0;
		return val;
	}
	if(val == 4)
	{
		printf("Clearing the queue list for the new operation");

			for(int i=0; i<10; i++)
			{
				strcpy(queue_name[i],"");
				que_array[i]=0;
			}
	}
	if(val == 1)
	{
		printf("\n Garbage collecting Sender Lists...");
		memset( sender_list,0, sizeof(sender_list) );
		no_of_sender=0;
		return val;
	}
	else if(val == 2)
	{
		printf("\n Garbage collecting Receiver Lists...");
		memset( recv_list,0, sizeof(recv_list) );
		no_of_recv = 0;
		return val;
	}
	else if(val == -1)
	{
		printf("\n Garbage collecting message Lists...");
		memset( message1, 0, sizeof(message1));
		mess_cnt=0;
		return val;
	}

	return val;
}

int do_mq_set()
{
	printf("Message GET attribute ");
	int val = m_in.m7_i1;
	return val;

}

int do_mq_get()
{
	printf("Message GET attribute from MESS FIle");
	int val = m_in.m7_i1;
	printf("HI from MEssfun file");
	if (val == 2)
	{
		printf("Size of the messages is %d",sizeof(que_array));
		return 0;
	}
	else if(val==1)
	{
		printf("The message queue was implemented without blocking feature \n");
		return 0;
	}
	else{
		return -1;
	}



}

int do_mq_reqnotify()
{
	int check;
	printf("Message Notify attribute ");
	char que[100];
	sys_datacopy(m_in.m_source, (vir_bytes) m_in.m7_p2, PM_PROC_NR,
						(vir_bytes)que, 100);
	strcat(que,"\0");
		check = que_list(que);
		if (check==1)
		{
			reqnotify();

		}
		return -1;

}








