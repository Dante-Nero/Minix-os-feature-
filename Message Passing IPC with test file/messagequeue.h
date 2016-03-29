#define MAX_RECV 5
#define MAX_SEND 5
#define MAX_USER 10
#define MAX_MESG_LEN 100

#define	MESSAGING_SUCCESS 0
#define	DUPLICATE_SENDER 4
#define	DUPLICATE_RECEIVER 5
#define	MSG_LEN_OVERFLOW_ERROR 6
#define	QUEUE_OVERFLOW 8
#define	QUEUE_UNDERFLOW 9


int mess_cnt;      //count of messages in queue; mess_cnt <16
int no_of_sender; //maintain sender list
int no_of_recv;  // maintain receiver list
int total_user;

int sender_list[5];
int recv_list[5];
int user_list[MAX_USER];

char queue_name[10][30];

int que_array[10];

int message_send(char data[], int uid, int destrec[], int no_of_recv);
int message_receive(char *data[], int recid);
void queue_creat();
int add_sender_list(int sid, int regnum);
int add_recv_list(int rid, int regnum);
void message_queue_create(int pos);
int que_list(char q_name[]);



struct node
{
	int recv_id;
	int pending_mesg;
};


struct msgpack
{
	char mesg[MAX_MESG_LEN];
	int reclist[5];
	//List of receivers who can read the message
	int sender_proc_id;
	int rec_proc_id;
	int num_of_read_completed;
	// If 1 it means that the message has been deleted
	int deletedFlag;

};

typedef struct message_queue
{
	struct msgpack msgList;
	struct message_queue *next;
}message_queue;

message_queue message1[16];

