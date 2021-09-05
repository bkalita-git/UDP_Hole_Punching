#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

void main(){
	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	//serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_addr.s_addr = inet_addr("103.142.175.221");
	serv_addr.sin_port = htons(8888);
	
	
	int client = socket(AF_INET, SOCK_DGRAM, 0);
	
	/*struct sockaddr_in cli_addr;
	cli_addr.sin_family = AF_INET;
    	cli_addr.sin_addr.s_addr = INADDR_ANY;
    	cli_addr.sin_port = htons(23456);
    	if (bind(client, (struct sockaddr*) &cli_addr, sizeof(struct sockaddr_in)) == 0)
		printf("Binded Correctly\n");
    	else
		printf("Unable to bind\n");
	*/
		
	char *hello = "REG A";
	sendto(client, (const char *)hello, strlen(hello),MSG_CONFIRM, (const struct sockaddr *) &serv_addr, sizeof(serv_addr));
	
	int n,len;
	char buffer[1024];
	n = recvfrom(client, (char *)buffer, 1024, MSG_WAITALL, (struct sockaddr *) &serv_addr,&len);
    	buffer[n] = '\0';
    	printf("Server : %s\n", buffer);
    	
    	printf("READY? ENTER:");
    	getchar();
    	
    	
    	hello = "CALL B";
	sendto(client, (const char *)hello, strlen(hello),MSG_CONFIRM, (const struct sockaddr *) &serv_addr, sizeof(serv_addr));
    	n = recvfrom(client, (char *)buffer, 1024, MSG_WAITALL, (struct sockaddr *) &serv_addr,&len);
    	buffer[n] = '\0';
    	if(strcmp("NULL",buffer)==0){
    		printf("TARGET does not exist\n");
    		exit(0);
	}
    	printf("Server : %s\n", buffer);
    	char calling_ip[50];
	char calling_port_s[50];  	
    	sscanf(buffer,"%s %s",calling_ip,calling_port_s);
    	short unsigned int calling_port = atoi(calling_port_s);
    	
    	struct sockaddr_in call_addr;
	call_addr.sin_family = AF_INET;
	call_addr.sin_addr.s_addr = inet_addr(calling_ip);
	call_addr.sin_port = htons(calling_port);
	hello = "HI I AM A";
	char MSG[100];
	sendto(client, (const char *)hello, strlen(hello),MSG_CONFIRM, (const struct sockaddr *) &call_addr, sizeof(call_addr));
	
	/*n = recvfrom(client, (char *)buffer, 1024, MSG_WAITALL, (struct sockaddr *) &call_addr,&len);
    	buffer[n] = '\0';
    	printf("CALLER : %s\n", buffer);
    	
    	hello = "HI I AM A";
	sendto(client, (const char *)hello, strlen(hello),MSG_CONFIRM, (const struct sockaddr *) &call_addr, sizeof(call_addr));*/
	
	
	pid_t par_pid   = getpid();
	pid_t child_pid = fork(); 
	if(getpid()!=par_pid){
		while(1){
			n = recvfrom(client, (char *)buffer, 1024, MSG_WAITALL, (struct sockaddr *) &call_addr,&len);
    			buffer[n] = '\0';
    			printf("\nB : %s\n", buffer);
		}
	}else{
		while(1){
			//printf("\nENTER A MESSAGE:");
			fgets(MSG, sizeof(MSG), stdin);
			sendto(client, (const char *)MSG, strlen(MSG),MSG_CONFIRM, (const struct sockaddr *) &call_addr, sizeof(call_addr));
		}

	}
    	
}
