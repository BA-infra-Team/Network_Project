#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#define MAX_CLIENT 5
#define BUF_SIZE 1024
#define BUFFER_SIZE 4096
#define BUFF_SIZE 100 
void *t_function(void *data);

int client_index = 0;

int g_sockList[5];

void childHandler(int signal)
{
	int status;
	pid_t spid;
	while((spid = waitpid(-1, &status, WNOHANH)) > 0)
	{
		printf("자식프로세스 wait한 결과 \n");
		printf("===============================\n");
		printf("PID        :%d\n", spid);
		printf(Exit Value  :%d\n", WEXITSTATUS(status));
		print("Exit Stat   :%d\n", WIFEXITED(status));

	}
}

int main(int argc, char **argv)
{
	signal(SIGCHLD, (void *)childHandler);
	struct  sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));

	int listenFD = socket(AF_INET, SOCK_STREAM, 0);
	int connectFD;

	ssize_t receivedBytes;
	char readBuff[BUFFER_SIZE];   
	char sendBuff[BUFFER_SIZE];
	pid_t pid;

        if (argc != 2)
        {
                printf("Usage : %s [port]\n", argv[0]);
                return 1;
        }

        int server_sock, client_sock;
        pthread_t thread_client[MAX_CLIENT];

        if((server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0 )
        {
                printf("socket create error\n");
                return -1;
        }

        int on = 1;
        if(setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
        {
                printf("socket option set error\n");
                return -1;
        }

        struct sockaddr_in server_addr, client_addr;
        int client_addr_size = sizeof(client_addr);
        server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(atoi(argv[1]));

        if(bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0 )
        {
                printf("bind error\n");
                return -1;
        }

        if(listen(server_sock, 5) < 0)
        {
                printf("listen error\n");
                return -1;
        }

        while(1)
        {

                printf("accept...\n");

                client_sock = accept(server_sock, (struct sockaddr *)&client_addr, (socklen_t *)&client_addr_size);
                if(client_sock < 0)
                {
                        printf("accept error\n");
                }

                if(client_index == MAX_CLIENT)
                {
                        printf("client accept full(max client count : %d)\n", MAX_CLIENT);
                        close(client_sock);
                        continue;
                }

		g_sockList[client_index] = client_sock;

                if(pthread_create(&thread_client[client_index], NULL, t_function, (void *)&client_sock) != 0 )
                {
                        printf("Thread create error\n");
                        close(client_sock);
                        continue;
                }

                client_index++;

                printf("client accepted(Addr: %s, Port: %d)\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        }

        return 0;

}

void *t_function(void *arg)
{
        int client_sock = *((int *)arg);
        pid_t pid = getpid();      // process id
        pthread_t tid = pthread_self();  // thread id

        printf("pid:%u, tid:%x\n", (unsigned int)pid, (unsigned int)tid);

        char buf[BUF_SIZE];

        while(1)
        {
                memset(buf, 0x00, sizeof(buf));
                if (read(client_sock, buf, sizeof(buf)) <= 0)
                {
                        printf("Client %d close\n", client_sock);
                        client_index--;
                        close(client_sock);
                        break;
                }

                printf("read : %s\n", buf);

                if(write(client_sock, buf, sizeof(buf)) <= 0)
                {
                        printf("Client %d close\n", client_sock);
                        client_index--;
                        close(client_sock);
                        break;
                }

		for (int i=0; i<client_index;i++)
		{
			if (g_sockList[i] == client_sock)
			{
				continue;
			}
			if(write(g_sockList[i], buf, sizeof(buf)) <=0)
			{
				printf("Client %d close\n", g_sockList[i]);
				client_index--;
				close(g_sockList[i]);
				break;
			}
			printf("write : %s\n",buf);
		}
        }

}
