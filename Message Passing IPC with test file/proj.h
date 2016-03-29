#include<lib.h>
#include<unistd.h>

int que_init()
{
	message ms;
	//printf("\n .h queue");
	return (_syscall(PM_PROC_NR, MQ_OPEN, &ms));
}

int queue_search(char q_name[]) //58
{
	message ms;
	ms.m7_p1=q_name;

	//printf("\n Searching for sender/ receiver...");

	return(_syscall(PM_PROC_NR, 58, &ms));
}

int search_sender(int sid, int regnum)//108
{
	message ms;

	ms.m7_i1 = sid;
	ms.m7_i2 = regnum;
	//printf("\n Registering sender for sendID=%d", sid);

	return( _syscall(PM_PROC_NR, 108, &ms) );
}

int search_reciver(int rid, int regnum)//109
{
	message ms;
	ms.m7_i1=regnum;
	ms.m7_i2 = rid;
	//printf("\n calling mreceiver=109 for rid=%d", rid);

	return( _syscall(PM_PROC_NR, 109, &ms) );
}

int mq_send(char que_id[],int sendID, char recvID[], char msg[])//69
{
	int ret, i=0, no_of_recv=0, recv[5];
	message ms;
	char *token;

	token=strtok(recvID, ",");
	while(token != NULL)
	{

		recv[no_of_recv]=atoi(token);
		no_of_recv++;
		token=strtok(NULL,",");
	}

	/* 	 * assign multiple recvs to m7 msg structure...	 * ms.m7_i3, ms.m7_i4, ms.m7_i5	 *	 * */
	ms.m7_i1=sendID;
	ms.m7_i2=no_of_recv;

	ms.m7_i3=recv[0];
	ms.m7_i4=recv[1];
	ms.m7_i5=recv[2];

	ms.m7_p2=que_id;

	ms.m7_p1=msg;


	ret=  _syscall(PM_PROC_NR, 69, &ms);

	return( ret );
}

int mq_receive(char que_id[],int recvID, char *msg)//70
{
	int ret;
	message ms;

	ms.m7_i2=recvID;
	ms.m7_p1=msg;

	ret=  _syscall(PM_PROC_NR, 70, &ms);

	return( ret );
}



int mq_close(int val) //56
{
	int ret=-1;

	message ms;
	ms.m7_i1=val;

	ret= _syscall(PM_PROC_NR, 56, &ms);

	return ret;
}

int mq_set(int val)
{
	message ms;
	ms.m7_i1=val;
		printf("\n Registering set for sendID");
			return (_syscall(PM_PROC_NR, MQ_SET, &ms));
}

int mq_get(int val)
{

	message ms;
	ms.m7_i1=val;
	printf("\n Registering sender for sendID %d",val);
		return (_syscall(PM_PROC_NR, MQ_GET, &ms));
}

int mq_reqnotify(char que_name[])
{
	message ms;
	ms.m7_p1 = que_name;

		printf("\n Registering otify  sender for sendID");
			return (_syscall(PM_PROC_NR, 97, &ms));
}





