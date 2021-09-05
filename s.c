#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<netdb.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<stdlib.h>


void main(){
	char* get_ip_port(char* HOST){
		char line[40];
		FILE *fp;
		fp = fopen ("hosts", "a+");
		char host_domain[50];
		char host_ip[50];
		char host_port[50];
		char* HOST_IP_PORT;
		while(fgets(line,40,fp)){
			sscanf(line,"%s %s %s",host_domain,host_ip,host_port);
			if(!strcmp(HOST,host_domain)){
				HOST_IP_PORT = (char*)malloc(strlen(host_ip)+strlen(host_port)+2);
				snprintf(HOST_IP_PORT,102,"%s %s",host_ip,host_port);
				fclose(fp);
				//check if HOST_IP_PORT is actually Online
				
				//
				return HOST_IP_PORT;
			}
		}
		HOST_IP_PORT = (char*)malloc(strlen("NULL")+1);
		strcpy(HOST_IP_PORT,"NULL");
		return HOST_IP_PORT;
	}
	
	int store_ip(char* LINE){
		char HOST_DOMAIN[50],HOST_IP[50];
		sscanf(LINE,"%s %s",HOST_DOMAIN,HOST_IP);
		FILE *fp,*temp;
		fp = fopen ("hosts", "a+");
		temp = fopen("temp","w");
		char line[40],host_domain[50],host_ip[50];
		int dirty=0;
		while(fgets(line,40,fp)){
			sscanf(line,"%s %s",host_domain,host_ip);
			if(!strcmp(HOST_DOMAIN,host_domain)){
				fputs(LINE, temp);
				dirty=1;
			}
			else
				fputs(line, temp);
		}
		if(dirty==0)
			fputs(LINE, temp);
		else
		   dirty=1;
		fclose(fp);
		fclose(temp);	
		remove("hosts");
		rename("temp","hosts");
		return 0;
	
	}
	/*this is how you return an IP*/
	//char* HOST_IP_PORT;
	//HOST_IP_PORT = get_ip_port("google.com");
	//printf("%s",HOST_IP_PORT);
	//free(HOST_IP_PORT);
	/******************************/
	
	/*this is how you store an IP*/
	//store_ip("google.com 0.0.0.0\n");
	/******************************/
	
	struct sockaddr_in serv_addr, cli_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(8888);
	
	int server = socket(AF_INET, SOCK_DGRAM, 0);
	bind(server, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
	char buffer[1024];
	int len = sizeof(cli_addr);
	char CMD[50];
	char ID[50];
	char PREFIX[100]="";
	char* MSG;
	while(1){
		//udp does not establish connection. it just send and receive.
		int n = recvfrom(server, (char *)buffer, 1024, MSG_WAITALL, ( struct sockaddr *) &cli_addr,&len);
    		buffer[n] = '\0';
    		//printf("Client : %s\n", buffer);
    		sscanf(buffer,"%s %s",CMD,ID);
    		if(strcmp(CMD,"REG")==0){
    			MSG = "REGISTERED";
    			snprintf(PREFIX,sizeof(PREFIX),"%s %s %hu\n",ID,inet_ntoa(cli_addr.sin_addr),ntohs(cli_addr.sin_port));
    			store_ip(PREFIX);
    			sendto(server,(const char*)MSG, strlen(MSG), MSG_CONFIRM, (const struct sockaddr *) &cli_addr,len);
		}else if(strcmp(CMD,"CALL")==0){
			MSG = get_ip_port(ID);
    			sendto(server,(const char*)MSG, strlen(MSG), MSG_CONFIRM, (const struct sockaddr *) &cli_addr,len);
    			free(MSG);
		}
		printf("Client IP:PORT %s:%hu\n",inet_ntoa(cli_addr.sin_addr),ntohs(cli_addr.sin_port));
	}
}
