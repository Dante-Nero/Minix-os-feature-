#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "proj.h"

#define MAX_MSGS 16   //NUMBER OF MESSAGES IN THE QUEUE
#define MAX_RECV 5		//NUMBER OF RECEIVER
#define MAX_SEND 5		//NUMBER OF SENDER
//#define MAX_MESG_LEN 100	//LENGTH OF THE MESSAGE
int ret2= 100;
int list_queue()
{
	int status;
	char que_name[50];
	bzero(que_name,50);
    printf("\n Enter name for message:");
   	 scanf(" %[^\n]%*c",que_name);
	status=queue_search(que_name);
	if(status == 1)
	{
		printf("\n The Queue already exists \n ");

	}
	else if(status ==0)
	{

		printf("\n Queue registered successfully");

	}
	return status;

}

int set_attr()
{

	printf("SET ATTRIBUTE");
	int fdd;
	printf("Enter the MAXIMUM MESSAGE LEN YOU WANT TO USE");
	scanf("%d",&fdd);
	ret2 = mq_set(fdd);
	return 1;

}
int list_sender()
{
	/*it registers as a sender to PM server*/
	int sid;
	int status;
	int regnum=1;
	printf("\n Manually registering Sender !");
	printf("\n Enter the sender ID:");
	scanf("%d", &sid);
	status=search_sender(sid, regnum);
	if(status == 1)
	{
		printf("\nDuplicate: Sender already there...");   //Duplication Verification
	}
	else if(status == 0)
	{
		printf("\nSender registered successfully \n");
	}
	return status;
}

void list_receiver(int flag)
{
	/*it registers as a sender to PM server*/
	int i;
	int status, recid, regnum=1;

	if(flag==0)
	{
		printf("\n Manually registering receiver !");
		printf("\n Enter receiver ID:");
		scanf("%d", &recid);
		status=search_reciver(recid, regnum);
		if(status == 1)
		{
			//Duplication Case....
			printf("\n Duplicate: Register already there...");
		}
		else if(status == 0)
			printf("\n Receiver registered successfully");
	}
	else if(flag==1)
	{
		status=search_reciver(4, regnum);
		status=search_reciver(5, regnum);
	}
	else
		printf("\n Error in receiving function invoked by main program");
}


void mess_send()
{
    printf("\nImplementing mq_send system call\n");

    int sendID;
    printf("\n Enter your Sender ID :");
    scanf("%d",&sendID);
    /* * add check for sender list....      * */
    int check;
    check = search_sender(sendID, 0);
    if(check == -2)
    {
    	//Duplication
    	printf("\n Duplicate Sender ID Found, Register New ID:");
    }
    else if(check == 1)
    {
    	//Message and receiver list entered
    	char queu_id[50];
    	bzero(queu_id,50);
    	char recvID[50];
   	    bzero(recvID,50);
   	    /* * Only 3 receivers per message are allowed.. * user can only send message to 3 receiver's at max...   * */
   	    printf("\n Enter Receiver's ID for message:");
   	    scanf(" %[^\n]%*c",recvID);
   	    printf("\n Enter the queue id to which the message has been sent");
   	    scanf(" %[^\n]%*c",queu_id);

   	    char msg[ret2]; //The maximum length of the message that can be sent using the QUEUE
   	    printf("\n Enter a message and the message length should be less than %d:",ret2);
    	scanf (" %[^\n]%*c", msg);

    	if(strlen(msg) > ret2)
    	{
    		printf("\n message length max reached...");
    	}
    	else
    	{
    		int status;
    		status = mq_send(queu_id,sendID, recvID, msg);

    		if(status == 8)
    		{
    			printf("\n QUEUE Overflow....System Halted");

    		}
    		else if(status == 1)
    		{
    			printf("\n Status:Deposite_msg : %d\n", status);
    		}
    		else if(status == -1)
    		{
    			printf("Queue not found \n");
    		}
    		else if(status == 0)
    		{
    			printf("\n Default was printed");
    		}
    	}
    }
    else if( check == -1)
    {
    	printf("\n Sender not was not registered.... Don't worry we have done that for you\n");
    	//printf("\n Registering the new sender automatically");

    }
}

void message_clean()
{
	int ret, val;

	printf("Press ...........\n 1.Sender List delete, \n2.Receiver list delete");
	printf("\n 2.Receiver list delete     ");
	printf("\n 3.Clear receiver , sender and message delete        ");
	printf("\n 4. Clearing the queue      ");
	printf("\n -1. Message delete        ");
	scanf("\n %d", &val);

	ret= mq_close(val);

	if(ret == 1)
		printf("\n Garbage Collected Sender...\n");
	else if(ret == 2)
		printf("\n Garbage Collected Receiver...\n");
	else if(ret == -1)
	{
		printf("\n All Messages are Deleted......\n");
	}
}

void get_attr()
{
	int nume;
	printf("1. Queue Size \n");
	printf("2.Queue type \n");


	scanf("%d",&nume);
	if (nume ==2)
	{
		int d;

		 d = mq_get(nume);


	}
	else if(nume == 1)
	{
		mq_get(nume);
	}
	else
	{
		printf("Wrong input \n");
	}
	 		//mq_get system call
}

void reqnotify()
{
	int check,recvID;
	char que_name[50];
	bzero(que_name,50);
	printf("\n Enter name for the queue:");
	scanf(" %[^\n]%*c",que_name);
	//char recvID[50];
	//bzero(recvID,50);
	printf("\n Enter Receiver's ID for message:");
	//scanf(" %[^\n]%*c",recvID);
	scanf("%d",&recvID);
	int status;

	check = search_reciver(recvID, 0);
	if(check ==1)
	 {
		status = mq_reqnotify(que_name);
	}
	else
		{
		   printf("THE CHECK FAILED");
		   	   	   }
}

int main(int argc, char **argv)
{
	int input=-1;

	printf("\n===============WELCOME TO THE TEST INTERFACE TO VERIFY THE IPC================\n");
	//list_receiver(1);

	while(1)
	{
		printf("\n			-----Available Operations---	\n");
		printf("\n 1. MESSAGE QUEUE Creation \n");	//this calls to the mq_open
		printf(" 2. Register Sender in the queue \n");   //this calls the  msender system call
		printf(" 3. Register Receiver in the queue \n"); //this call the mreceiver system call
		printf(" 4. Send a message using the system call mq_send \n");  // This calls the mq_send system call
		printf(" 5. Clear and Close Message Queue\n"); //This calls the mq_close system call
		printf(" 6. Get Attributes of the message queue  \n");
		printf(" 7. Set Attributes of the message queue  \n");//this calls to the mq_getattr system call
		printf(" 8. Notify system call \n");
        printf(" 0. Exit the Program \n");
		printf("Choose the operation you want to perform \t ");
		scanf(" %d",&input);
        switch(input)
        {
        	case 1:
        			list_queue();
        			break;
        	case 2:
        			list_sender();
        			break;
        	case 3:
        			list_receiver(0);
		            break;
		    case 4:
		    		mess_send();	//sender can be created dynamically also.
		            break;
		    case 5:
		    		message_clean();
		    		break;
		    case 6:

		    		get_attr();

		    		break;
		    case 7:
		    		set_attr();
		    		break;
		    case 8 :
		    		reqnotify();
		    		break;
		    case 0:
		            exit(0);
		            break;
		    default :
		            printf("\nIncorrect input. Try again. \n");
		            break;
	    }
	}
}
