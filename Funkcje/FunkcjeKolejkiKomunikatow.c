#include "FunkcjeKolejkiKomunikatow.h"


int create_message_queue(const char *path, int identifier, int flags)
{
	key_t key = ftok(path, identifier);
	if ( key == - 1 )
    {
        perror("Ftok failed");
        exit(1);
    }

	int mesg_queue_ID =  msgget(key, flags);
	if ( mesg_queue_ID == -1 )
	{
		perror("Msgget failed");
		exit(2);
	}

	return mesg_queue_ID;
}


void send_message(int mesg_queue_ID, struct message *msg_ptr, int msg_flag)
{
	if (msgsnd(mesg_queue_ID, (void*)msg_ptr, sizeof(msg_ptr->pidProcesu), msg_flag) == -1 ) 
	{
        perror("Msgsnd failed");
        exit(3);
	}
}


int recive_message(int mesg_queue_ID, struct message *msg_ptr,int message_type, int msg_flag)
{
	if (msgrcv(mesg_queue_ID, (void*)msg_ptr, sizeof(msg_ptr->pidProcesu), message_type, msg_flag) == -1 )
	{ 
		if (errno == EINTR)
			return 1;
		else 
		{
			perror("Msg Recive failed");
			printf("%d\n", getpid());
			exit(4);
		}
	}
	return 0;
}


void delete_message_queue(int mesg_queue_ID)
{
	if ( msgctl(mesg_queue_ID, IPC_RMID, NULL) == -1 )
	{
		perror("Msgctl IPC_RMID failed");
		exit(5);
	}
}