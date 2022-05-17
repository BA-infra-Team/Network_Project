#include<netinet/in.h> // sockaddr_in 
#include<sys/types.h>  // socket 
#include<sys/socket.h> // socket 
#include<stdio.h>    // printf 
#include<stdlib.h>   // exit 
#include<string.h>   // bzero 

#define SERVER_PORT 8000 
#define BUFFER_SIZE 1024 

// 메뉴를 출력하는 UI 함수
int PrintUI()
{
	int nInput = 0;
	// system("cls");
	printf("===================================================\n");
	printf("서버 Start\n");
	printf("---------------------------------------------------\n");
	printf("[1] 연결현황출력\t [2] 연결종료\t [3] 명령어기록보기\t [0] 종료\n");
	printf("===================================================\n");

	// 사용자가 선택한 메뉴의 값을 반환한다.
	scanf("%d", &nInput);
	getchar();
	// getchar();//버퍼에 남은 엔터 제거용
	return nInput;
}

void getList()
{
}

int disConnect(int index)
{
}

void getHistory(int index)
{
}

int main(int argc , char *argv[])
{
	int pid;
	//Declare and initialize a server-side socket address structure
	struct sockaddr_in server_addr; 
	bzero(&server_addr, sizeof(server_addr)); 
	server_addr.sin_family = AF_INET; 
	server_addr.sin_addr.s_addr = htons(INADDR_ANY); 
	server_addr.sin_port = htons(SERVER_PORT); 
 
	//Creates a socket and returns the socket descriptor if successful
	int server_socket_fd = socket(PF_INET, SOCK_STREAM, 0); 
	if(server_socket_fd < 0) 
  	{ 
    		perror("Create Socket Failed:"); 
    		exit(1); 
  	} 
  	int opt = 1; 
 	setsockopt(server_socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); 
 
	//Bind the socket and socket address structure
 	if(-1 == (bind(server_socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)))) 
 	{ 
		perror("Server Bind Failed:"); 
		exit(1); 
	} 
   
	//The socket listening
	if(-1 == (listen(server_socket_fd, LENGTH_OF_LISTEN_QUEUE))) 
	{ 
		perror("Server Listen Failed:"); 
    		exit(1); 
  	} 
	
	while(1)
	{
		//Defines the socket address structure of the client
    		struct sockaddr_in client_addr; 
    		socklen_t client_addr_length = sizeof(client_addr); 
 
    		//Accept the connection request and return a new socket(descriptor) for communicating with the connected client
    		//The accept function writes client information to client_addr
    		int new_server_socket_fd = accept(server_socket_fd, (struct sockaddr*)&client_addr, &client_addr_length); 
    		if(new_server_socket_fd < 0) 
    		{ 
      			perror("Server Accept Failed:"); 
      			break; 
    		} 
		if (pid>0)	// 부모 프로세스 
		{
			PrintUI();
			continue;
		}
		else if (pid == 0)	// 자식 프로세스 
		{
			printf("Child");
		}
	}
}
