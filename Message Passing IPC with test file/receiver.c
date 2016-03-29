#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "proj.h"


#define MAX_MSGS 16
#define MAX_RECV 5
#define MAX_SEND 5
#define MAX_MESG_LEN 100

//mq_receiver system call implementation
void receive_mesg()
{
    printf("\n----receive message from queue.---\n");
    int recvID;
    char msg[100];
    int check2;

    bzero(msg, 100);

    printf("\nEnter your Receiver ID :");
    scanf("%d",&recvID);

    // ADD:- check for receiver_list...
    int check;
    check = search_reciver(recvID, 0);

    if(check == 1)
    {
        int status;
        char queu_id[50];
        bzero(queu_id,50);
        printf("\n Enter the queue id from which the message has to be received");
                scanf(" %[^\n]%*c",queu_id);


        status = mq_receive(queu_id,recvID, msg); // add q_uid here
        if(status == -1)
        {
            printf("\n QUEUE Underflow...");
        }
        else if(status > 0)
        {
            printf("\n retrieved message is= : %s\n", msg);
        }
        else if(status == 0)
            printf("\n No Message There...");

    }
    else
    {
        printf("\n Error on Registering Receiver...");
        printf("\n Do you want to register the receiver...");
        printf("\nEnter your Receiver ID :");
    }
}

int main(int argc, char **argv)
{
    int input=-1;
    int tmp;
    receive_mesg();

    return 0;

}
