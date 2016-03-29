#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "messagequeue.h"
#include<minix/endpoint.h>
#include<minix/com.h>
#include <minix/type.h>
#include "pm.h"
#include <unistd.h>
#include <minix/callnr.h>


int start = 0;
int que_array[10] = {0,0,0,0,0,0,0,0,0,0};

void queue_creat()
{
	memset( message1, 0, sizeof(message1));
	memset( sender_list,0, sizeof(sender_list) );
	memset( recv_list,0, sizeof(recv_list) );

	mess_cnt=0;
	no_of_sender=0;
	no_of_recv=0;

}

int que_list(char q_name[])
{

	int found=0, index;
	int avail_pos = 0;
	int new_pos;
	char q1_name[50];
	strcpy(q1_name,q_name);

	printf("\n Queue_List	\n");
	if(start == 0)
	{
		printf("Inside start\n");
		start = 1;
		for(int i=0; i<10; i++)
			{
			strcpy(queue_name[i],"");

			}

	}
	for(int i = 0; i < 10; i++)
	{
		if (strcmp(q1_name,queue_name[i]) == 0)
		{
			printf("Queue Found its index:%d\n",i);
			found = 1;
			index = i;
			break;
		}
	}
	for(int i=0;i<10;i++)
	{
		if(que_array[i]==0)
		{
			avail_pos = 1;
			new_pos=i;
			break;
		}
	}

	if(found == 1)
	{
		return found;
		}
	else
	{
		//create new queue
		if(avail_pos == 1) {
			//set parameter values
			strcpy(queue_name[new_pos],q1_name);
			que_array[new_pos] = 1;
			queue_creat();
			printf("New queue created at:%d\n",new_pos);
			}
		else
		{
			printf("Queue cannot be created, number of queue exceeded size limit...\n");
			return -1;
			}

		}
	printf("From message queue creation...\n");
	return found;
	}


		/*if(user_list[i] == sid)
		{
			found=1;
			printf("\n Queue already there \n");

		}
	}
	if(regnum == 1 && found == 0 && total_user < MAX_USER)
	{
		// Then register the sender...
		user_list[total_user] = sid;
		printf("\n The New Queue was Registered the id is  \t %d \n", user_list[total_user]);
		total_user++;

	}
	else if( total_user>= MAX_USER)
	{
		printf("\n User limit reached...");
		printf("\n total User registered...=%d", total_user);
	}
	else if(found == 1 && regnum == 1)
	{
		// Case of Duplication...
		return -2;
	}
	else if(regnum == 0 && found == 0)
	{
		// Case of search fail..Sender does not exists..

		//que_list(sid,1);

		return -1;
	}
	return found;
}*/

//Function to create a sender_list.

int add_sender_list(int sid, int regnum)
{
	int found=0, i;
	printf("\n sender list");

	for(i=0; i<no_of_sender; i++)
	{
		if(sender_list[i] == sid)
		{
			// if Duplicate sender id found...then
			found=1;
			printf("\n Sender already there...\n");
			/*if(regnum == 0)  // check for Searching...
			{
				return 0;  // trying to search & found...
			}*/
		}
	}
	/*
	 * regnum =1 indicates new register for sender
	 * Found = 0 indicates sender not available
	 * */
	if(regnum == 1 && found == 0 && no_of_sender < MAX_SEND)
	{
		// Then register the sender...
		sender_list[no_of_sender] = sid;
		printf("\n The New Sender with id is = \t %d \n", sender_list[no_of_sender]);
		no_of_sender++;

	}
	else if( no_of_sender>= MAX_SEND)
	{
		printf("\n sender limit reached...");
		printf("\n total sender registered...=%d", no_of_sender);
	}
	else if(found == 1 && regnum == 1)
	{
		// Case of Duplication...
		return 1;
	}
	else if(regnum == 0 && found == 0)
	{
		// Case of search fail..Sender does not exists..
		add_sender_list(sid,1);
		return -1;
	}

	return found;
}

// function to create a recever list.
int add_recv_list(int rid, int regnum)
{
	int found=0, i;
	printf("\n Receiver List");

	for(i=0; i<no_of_recv; i++)
	{
		if(recv_list[i] == rid)
		{
			// Receiver found already...
			found=1;
			return found;
		}
	}
	if(no_of_recv < MAX_RECV && regnum == 1)
	{
		recv_list[no_of_recv] = rid;
		printf("\n The New Sender with id is = \t %d \n", recv_list[no_of_recv]);
		no_of_recv++;
	}
	else if(no_of_recv>= MAX_RECV)
	{
		//printf("\n total receiver registered...=%d", no_of_recv);
		printf("\n receiver limit reached...");
	}
	return found;
}


int message_send(char data[], int usrid, int destrec[], int no_of_recv)
{
	int i=0;
	//printf("\n inserting Message");

	if(mess_cnt < 16)
	{
		message1[mess_cnt].msgList.sender_proc_id=usrid;
		message1[mess_cnt].msgList.num_of_read_completed=no_of_recv;
		message1[mess_cnt].msgList.deletedFlag=0;

		strncpy(message1[mess_cnt].msgList.mesg, data, strlen(data));

		for(i=0; i<no_of_recv; i++)
		{
			message1[mess_cnt].msgList.reclist[i]=destrec[i];
		}

		mess_cnt++;
		printf("\n message Count=%d \n", mess_cnt);
	}
	else
	{
		printf("\n Rejecting request...Message Count reached=%d", mess_cnt);
		return QUEUE_OVERFLOW; // return 8 on overflow...
	}
	return 1;
}


int message_receive(char *data[], int recid)
{
	int found=0, i,j, k=0, deletion=0;
	int read_done = -1;
	printf("\n Message for the receiver %d", recid);

	if(mess_cnt == 0)
	{
		printf("\n Underflow !!!");
		return -1;
	}
	for(i=0; i<mess_cnt;i++)
	{
		//printf("\n i=%d & message1[i]=%s", i , message1[i].msgList.mesg);
		for(j=0;j<5;j++)
		{
			if(recid == message1[i].msgList.reclist[j] &&
					message1[i].msgList.deletedFlag == 0 )
			{
				//printf("\n Found a message for you sir");

				message1[i].msgList.num_of_read_completed--;
				data[k]=(char *)malloc(sizeof(char ) * 20);
				strncpy(data[k], message1[i].msgList.mesg,
						sizeof(message1[i].msgList.mesg));
				strcat(data[k], "\0");
				//printf("\n Inserted data k=%d, & msg=%s", k, data[k]);
				k++;
				found=k;

				// to mark read at most once only...
				message1[i].msgList.reclist[j]=read_done;

				if(message1[i].msgList.num_of_read_completed == 0)
				{
				//	printf("\n Receiver:-message1 read by ALL...deleting...");
					message1[i].msgList.deletedFlag=1;
					deletion =1;
					// call garbage collection...
					message_queue_create(i);
					i--;
					j=0;
				}
			}
		}
	}
	if(found==0){
		//printf("\n No Message found bro");
	}

	return found;
}


void message_queue_create(int pos)
{
	printf("\n Clearing Garbage from message...");
	int i, k, j;
	if(pos != -1)
	{
		for(i=pos, j=i+1; j<=mess_cnt; i++, j++)
		{
			strcpy(message1[i].msgList.mesg, message1[j].msgList.mesg);
			//Sender ID copy...
			message1[i].msgList.sender_proc_id=message1[j].msgList.sender_proc_id;
			//Receiver ID Copy...
			message1[i].msgList.rec_proc_id=message1[j].msgList.rec_proc_id;
			//Read Complete & DeleteFlags...
			message1[i].msgList.num_of_read_completed=message1[j].msgList.
					num_of_read_completed;
			message1[i].msgList.deletedFlag=message1[j].msgList.deletedFlag;
			// Receiver's List
			for(k=0; k<5; k++)
			{
				message1[i].msgList.reclist[k]=message1[j].msgList.reclist[k];
			}
			//mess_cnt--;
		}

		mess_cnt--;
		printf("\n After closing: Mess_cnt=%d", mess_cnt);
		for(i=0; i<mess_cnt; i++)
			printf("\n Queue shifted:i=%d & MSG=%s", i,message1[i].msgList.mesg);
	}
	else
		printf("\n No Message in the Queue for that Position=%d...", pos);
}











