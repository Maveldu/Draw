#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

//Declaring the socket pointer
static int s;
//Creating connected client list, and an associated _Bool list "isSet"
static _Bool cliIsSet[32] = {0};
static struct sockaddr_in cliList[32];// 32 clients max

// Function that adds a client to the client list
// Returns TRUE if a client is added, false if he can't
_Bool addCli(struct sockaddr_in cli){
	for(int i=0;i<sizeof(cliList);i++){
		if(!cliIsSet[i]){ // Checking for empty spot
			cliList[i]=cli;
			cliIsSet[i]=1;
			return 1; // Client added : TRUE
		}
	}
	return 0; // No spot available : FALSE
}

// Function that remove a client from the client list
// Returns TRUE if a client is deleted, FALSE if he is not in the list
_Bool delCli(struct sockaddr_in cli){
	for(int i=0;i<sizeof(cliList);i++){
		if(cliIsSet[i]){
			if(cliList[i].sin_port==cli.sin_port){ // Checking if the client is at that spot
				cliIsSet[i]=0;
				return 1;
			}
		}
	}
	return 0;
}

// Function that sends a message to every clients in the client list
void sendAll(unsigned char buf[],int size){
	for(int i=0;i<sizeof(cliIsSet);i++){
		if(cliIsSet[i]){
			struct sockaddr_in cli = cliList[i];
			socklen_t cliSize=sizeof(cli);
			if (sendto(s, buf, size, 0, (struct sockaddr*) &cli, cliSize)<size) {
				perror("sendto");
				break;
			}
		}
	}
}

int main(){
	// Server socket initialization
	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s<0) perror("socket");
	struct sockaddr_in srv;
	srv.sin_family = AF_INET;
	srv.sin_port = htons(12345); // The client will have to use 12345 as the receiver port
	srv.sin_addr.s_addr = INADDR_ANY;
	struct sockaddr_in cli;
	socklen_t cliSize = sizeof(struct sockaddr_in);
	if (bind(s, (struct sockaddr*) &srv, sizeof(srv))<0)
		perror("bind");

	// Server starts
	while (1) {
		// Buffers initialization
		char bufRecv[2048];
		unsigned char bufSend[14];
		int recvd;

		// Wait for any message
		recvd=recvfrom(s, &bufRecv, sizeof(bufSend), 0, (struct sockaddr*) &cli, &cliSize);
		// Message received (recvfrom exited)

		if(recvd==-1){
			perror("recvfrom");
			break;
		}else{
			// Received message handling
			int i = 0;
			while(i<recvd){
				switch(bufRecv[i]){
				case 'H':
					// New client
					// Adding client in the client list
					if(!addCli(cli)){
						bufSend[0]='B';
						sendAll(bufSend,1);
					}
					i++;
					break;
				case 'B':
					// Client leaves
					// Deleting the client from the client list
					delCli(cli);
					i++;
					break;
				case 'C':
					// Filling window command
					// Send to every client these 5 Bytes
					for(int j=0;j<5;j++){
						bufSend[j]=bufRecv[i+j];
					}
					i=i+5;
					sendAll(bufSend,5);
					break;
				case 'L':
					// Line drawing command
					// Send to every client these 13 Bytes
					for(int j=0;j<13;j++){
						bufSend[j]=bufRecv[i+j];
					}
					i=i+13;
					sendAll(bufSend,13);
					break;
				case 'R':
					// Line drawing command
					// Send to every client these 14 Bytes
					for(int j=0;j<14;j++){
						bufSend[j]=bufRecv[i+j];
					}
					i=i+14;
					sendAll(bufSend,14);
					break;
				default:
					// Unknown character
					// Ignore and go to the next Byte
					i++;
					break;
				}
			}
		}
	}
}
