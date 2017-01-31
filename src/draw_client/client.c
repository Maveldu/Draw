#include "display.h"
#include "datahandler.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>

static int const window_width = 800;
static int const window_height = 600;

int main() {
	// Socket, server structure and buffer initialization
	unsigned char buf[2048];
	int size;
	int s;
	s = socket(AF_INET, SOCK_DGRAM, 0);
	if(s<0)perror("socket");
	struct sockaddr_in srv;
	srv.sin_family = AF_INET;
	srv.sin_port = htons(12345);
	char addr[16];
	addrSelect(addr);
	srv.sin_addr.s_addr = inet_addr(addr);
	unsigned int sizeSrv = sizeof(srv);

	// Creating the drawing window
	if (displayInit("Draw",window_width,window_height) == 0) {
		// Send a Hello message to the server
		size=makeHello(buf);
		sendto(s, &buf, size, 0,(struct sockaddr*) &srv, sizeof(srv));

		// Initialize drawing variables
		unsigned long x,y,x1,y1;
		_Bool fill,set=false;
		int a=255,r=255,g=255,b=255,type=0,c;

		// User interaction
		printf("Vous pouvez dessiner, tapez :\n- q pour quitter\n- c pour choisir la couleur\n- l pour dessiner une ligne\n- f pour remplir la fenêtre\n- r pour dessiner un rectangle\n");
		while((c=checkEvents(&x,&y))>0){
			if(6>c && c>1){
				// Setting the current option
				type=c;
			}
			if(type==2){
				// 'c' last key pressed, enter the color selection dialogue
				selColor(&fill,&a,&r,&g,&b);
				type=1;
			}else if(type==3){
				// 'f' last key pressed, send a Fill message
				size=makeClean(a,r,g,b,buf);
				sendto(s, &buf, size, 0,(struct sockaddr*) &srv, sizeof(srv));
				type=1;
			}else if(type==4 && c==6){
				// 'l' last key pressed and click, set a Line and send it
				if(!set){ // First coordinate selected
					x1=x;y1=y;set=true;
				}else{ // Second coordinate selected : send
					size=makeLine(x1,y1,x,y,a,r,g,b,buf);
					sendto(s, &buf, size, 0,(struct sockaddr*) &srv, sizeof(srv));
					type=1;set=false;
				}
			}else if(type==5 && c==6){
				// 'r' last key pressed and click, set a Rectangle and send it
				if(!set){ // First coordinate selected
					x1=x;y1=y;set=true;
				}else{ // Second coordinate selected : send
					unsigned long int w,h;
					if(x1<x){w=x-x1;}else{w=x1-x;x1=x;}
					if(y1<y){h=y-y1;}else{h=y1-y;y1=y;}
					size=makeRect(x1,y1,w,h,a,r,g,b,fill,buf);
					sendto(s, &buf, size, 0,(struct sockaddr*) &srv, sizeof(srv));
					type=1;set=false;
				}
			}

			// Checking the socket
			int recvd=recvfrom(s, &buf, sizeof(buf),MSG_DONTWAIT,(struct sockaddr*)&srv, &sizeSrv);

			int i=0;
			// Something received
			while(i<recvd){
				// Setting buffers
				unsigned char bufType=buf[i];
				int x,y,x1,y1,x2,y2,w,h;
				switch(bufType){
				case 'C':
					// Fill message received
					// Fill the window with the received informations
					displayClear((int)buf[i+1], (int)buf[i+2], (int)buf[i+3], (int)buf[i+4]);
					printf("Ecran rempli (C)\n");
					i=i+5;
					break;
				case 'L':
					// Line message received
					// Draw a line with the received informations
					x1=(int)buf[i+1]*256+(int)buf[i+2];
					y1=(int)buf[i+3]*256+(int)buf[i+4];
					x2=(int)buf[i+5]*256+(int)buf[i+6];
					y2=(int)buf[i+7]*256+(int)buf[i+8];
					displayDrawLine(x1,y1,x2,y2,(int)buf[i+9],(int)buf[i+10],(int)buf[i+11],(int)buf[i+12]);
					printf("Ligne tracée (L)\n");
					i=i+13;
					break;
				case 'R':
					// Rectangle message received
					// Draw a rectangle with the received informations
					x=(int)buf[i+1]*256+(int)buf[i+2];
					y=(int)buf[i+3]*256+(int)buf[i+4];
					w=(int)buf[i+5]*256+(int)buf[i+6];
					h=(int)buf[i+7]*256+(int)buf[i+8];
					displayDrawRect(x,y,w,h,(int)buf[i+9],(int)buf[i+10],(int)buf[i+11],(int)buf[i+12],(int)buf[i+13]);
					printf("Rectangle tracé (R)\n");
					i=i+14;
					break;
				case 'B':
					// Bye message received, the server can't accept you
					// Close this client
					printf("Le serveur ne peut pas vous accepter\n");
					// Closing socket
					close(s);
					// Closing window
					displayQuit();
					// End of main
					return 0;
				default:
					// Unknown character
					// Ignore and go to the next Byte
					i++;
					break;
				}
			}
		}
		// The client pressed 'q', 'while' broke

		// Send a Bye message to the server to warn it
		size=makeBye(buf);
		sendto(s, &buf, size, 0,(struct sockaddr*) &srv, sizeof(srv));

		// Closing socket
		close(s);
		// Closing window
		displayQuit();
	}
	else{
		// Drawing window initialization problem
		printf("L'affichage n'a pas pu être initialisé");
		close(s);
	}

	// End of main
	return 1;
}
