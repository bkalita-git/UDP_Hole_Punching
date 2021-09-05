#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/mman.h>
#define SIGNAL_SERVER_IP "103.142.175.221"

void main(){
	int client;
	client = socket(AF_INET, SOCK_DGRAM, 0);
	
	struct sockaddr_in serv_addr;
	
	struct sockaddr_in call_addr;
        
                    
    	char calling_ip[50],calling_port_s[50];  
	short unsigned int calling_port;
	
	unsigned short int *target_port;
	unsigned long *target_ip;
	   
	target_port = mmap(NULL, sizeof *target_port, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	target_ip   = mmap(NULL, sizeof *target_ip, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
		
	struct sockaddr_in src_addr;

	
	char *hello;
	int  n,len;
	len = sizeof(src_addr); //important
	char buffer[1024],MSG[100];
	


	
	pid_t thread1_id = getpid();
	fork();
	if(getpid()==thread1_id){
		//THREAD1 send to server or send to target
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = inet_addr(SIGNAL_SERVER_IP);
		serv_addr.sin_port = htons(8888);
		hello = "REG B";
		sendto(client, (const char *)hello, strlen(hello),MSG_CONFIRM, (const struct sockaddr *) &serv_addr, sizeof(serv_addr));
		while(1){
			//printf("\nENTER A MESSAGE:");
			fgets(MSG, sizeof(MSG), stdin); //char MSG[100];
			MSG[strcspn(MSG, "\n")] = 0;
			if(!strcmp(MSG,"CALL A")){
				sendto(client, (const char *)MSG, strlen(MSG),MSG_CONFIRM, (const struct sockaddr *) &serv_addr, sizeof(serv_addr));
			}else if(!strcmp(MSG,"bye")){
				exit(0);
			}
			else{
				call_addr.sin_family = AF_INET;
				call_addr.sin_addr.s_addr = *target_ip;
				call_addr.sin_port = *target_port;
				sendto(client, (const char *)MSG, strlen(MSG),MSG_CONFIRM, (const struct sockaddr *) &call_addr, sizeof(call_addr));
				
			}
		}
	}else{
		//THREAD2 receive from  SOCKET
		while(1){
			n = recvfrom(client, (char *)buffer, 1024, MSG_WAITALL, (struct sockaddr *) &src_addr,&len); //int n,len;
			//printf("FROM IP: %s\n",inet_ntoa(src_addr.sin_addr));
		    	buffer[n] = '\0'; //char buffer[1024];
		    	if(!strcmp(SIGNAL_SERVER_IP,inet_ntoa(src_addr.sin_addr))){
			    	if(strcmp("NULL",buffer)==0){
			    		printf("TARGET does not exist\n");
				}else if(strcmp("OFF",buffer)==0){
					printf("TARGET is OFFLINE\n");
				}else if(strcmp("REGISTERED",buffer)==0){
					printf("REGISTERED\n");
				}else{
			    		printf("Server : %s\n", buffer);
			    		sscanf(buffer,"%s %s",calling_ip,calling_port_s);
				    	calling_port = atoi(calling_port_s);
					call_addr.sin_family = AF_INET;
					call_addr.sin_addr.s_addr = inet_addr(calling_ip);
					*target_ip   = inet_addr(calling_ip);
					*target_port = htons(calling_port);
					call_addr.sin_port = htons(calling_port);
				}
		    	}else{
		    		printf("\nA : %s\n", buffer);
		    	}
	    	}
	}
    	
}
