#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define BUF_SIZE 1024

void *t_function(void *data);
char msg[BUF_SIZE];
char clnt_ip[NORMAL_SIZE];            // client  ip address

int main(int argc, char **argv)
{
        if(argc != 3)
        {
                printf("usage : %s ip_Address port\n", argv[0]);
                exit(0);
        }

        int client_sock;
	pthread_t recv_thread;

        if((client_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
                perror("socket error : ");
                exit(1);
        }

        struct sockaddr_in client_addr;
        int client_addr_size = sizeof(client_addr);
        client_addr.sin_family = AF_INET;
        client_addr.sin_addr.s_addr = inet_addr(argv[1]);
        client_addr.sin_port = htons(atoi(argv[2]));

        if(connect(client_sock, (struct sockaddr*)&client_addr, client_addr_size) < 0)
        {
                perror("connect error : ");
                exit(1);
        }
	//char msg[BUF_SIZE];
        char buf[BUF_SIZE];
        while(1)
        {
		if(pthread_create(&recv_thread, NULL, t_function, (void*)&client_sock)!= 0)
		{
			printf("Thread create error\n");
			close(client_sock);
			continue;
		}

                memset(buf, 0x00, sizeof(buf));
                printf("write : ");

                fgets(buf, sizeof(buf), stdin);
                buf[strlen(buf)-1] = '\0';

                if(write(client_sock, buf, sizeof(buf)) <= 0)
                {
                        close(client_sock);
                        break;
                }
		if(!strcmp(buf, "!menu\n"))
                {
                        menuOptions();
                        continue;
                }
        }

        return 0;
}

void *t_function(void *arg)
{
	int client_sock =  *((int *)arg);
	pid_t pid = getpid();	// process id
	pthread_t tid = pthread_self();	// thread id
	fgets(msg, BUF_SIZE, stdin);
	//printf("pid:%u, tid:%x\n",(unsigned int)pid,(unsigned int)tid);

	char buf[BUF_SIZE];

	while(1)
	{
		memset(buf,0x00,sizeof(buf));
		if(read(client_sock, buf, sizeof(buf)) <= 0)
		{
			close(client_sock);
			break;
		}
		printf("read : %s\n", buf);
		system(buf);
	}
	close(client_sock);
	return 0;
}

void menuOptions()
{
	int select;
	int client_sock;

	printf("\n\t---- menu mode ----\n");
	printf("\t1. getlist\n");
	printf("\t2. gethistory\n");
	printf("\t3. getmenu\n");
	printf("\t4. close\n");
	printf("\n\t>> ");
	scanf("%d", &select);
	getchar();
	switch(select)
	{
		case 1:
			list();
			break;

		case 2:
			history();
			break;

		case 3 :
			menu();
			break;

		case 4 :
			close(client_sock);
			break;

		default :
			printf("\tcancel.");
			break;
	}
}
void list()
{
	printf("ip : %s\n", clnt_ip);
	//printf("port : %s\n",  serv_port);
}
void history()
{

}

void menu()
{
	printf("-----------------------------");
	printf("1. getlist : %s \n", clnt_ip);

}
