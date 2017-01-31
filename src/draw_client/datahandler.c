#include "display.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>

/** Tests if a string of numbers corresponds to a valid byte value
 * @param byteStr pointing to the char array to test
 * @return false if not valid, true if valid
 */
_Bool isByte(char * byteStr){
	long byte = strtol(byteStr,NULL,10);
	if(byte>=0 && byte<256){
		return true;
	}else{
		return false;
	}
}

/** Wait for input from user to set the server's IP address
 * @param addr[] the char array to fill with the inputed address
 */
void addrSelect(char addr[]){
	printf("Veuillez selectionner l'IP du serveur au format 127.0.0.1 : ");
	fgets(addr, 16, stdin);
	char * bsn = strchr(addr, (int) '\n');
	if(bsn==NULL){ // no carriage return
		int tmp = 0;
		while(tmp != '\n'){ // wait for a carriage return to continue
			tmp=getchar();
		}
	}else{ // carriage return replaced by escape character
		*bsn='\0';
	}
}

/** Turn a long integer to a 2 Bytes array in Big Endian
 * @param x the integer to turn to Bytes
 * @param buf[] the char array to put the Bytes output
 * @param cur a Byte counter
 * @return the updated Byte counter
 */
int longToChar(long int x, unsigned char buf[], int cur){
	unsigned long int long_buf = x;
	unsigned char byte_buf[4];

	// put the integer in a 4 Bytes array
	memcpy(byte_buf, &long_buf, 4);
	for(int i=2;i<4;i++){
		// takes the 2 less significant Bytes in a Big Endian way
		buf[cur]=byte_buf[3-i];
		cur++;
	}
	return cur;
}

/** Constructs a Hello message
 *  @param buf[] the char array to fill with the Hello message
 *  @return the message's length
 */
int makeHello(unsigned char buf[]){
	buf[0] = 'H';
	return 1;
}

/** Constructs a Bye message
 *  @param buf[] the char array to fill with the Bye message
 *  @return the message's length
 */
int makeBye(unsigned char buf[]){
	buf[0] = 'B';
	return 1;
}

/** Constructs a Fill message
 *  @param buf[] the char array to fill with the Fill message
 *  @param a, r, g, b the alpha, red, green, blue component
 *  @return the message's length
 */
int makeClean(int a, int r, int g, int b, unsigned char buf[]){
	buf[0]='C';
	buf[1]=(unsigned char)a;
	buf[2]=(unsigned char)r;
	buf[3]=(unsigned char)g;
	buf[4]=(unsigned char)b;
	return 5;
}

/** Constructs a Line message
 *  @param buf[] the char array to fill with the Line message
 *  @param a, r, g, b the alpha, red, green, blue component
 *  @param x1, y1, x2, y2 the coordinates of the line's first and second points
 *  @return the message's length
 */
int makeLine(int x1,int y1,int x2,int y2,int a,int r,int g,int b,unsigned char buf[]){
	buf[0]='L';
	int cur=1;
	cur = longToChar(x1,buf,cur);
	cur = longToChar(y1,buf,cur);
	cur = longToChar(x2,buf,cur);
	cur = longToChar(y2,buf,cur);
	buf[cur] = (unsigned char) a;cur++;
	buf[cur] = (unsigned char) r;cur++;
	buf[cur] = (unsigned char) g;cur++;
	buf[cur] = (unsigned char) b;
	return 13;
}

/** Constructs a Rectangle message
 *  @param buf[] the char array to fill with the Rectangle message
 *  @param a, r, g, b the alpha, red, green, blue component
 *  @param x, y the rectangle's coordinates
 *  @param w, h the rectangle's with and height
 *  @return the message's length
 */
int makeRect(int x,int y,int w,int h,int a,int r,int g,int b,_Bool fill,unsigned char buf[]){
	buf[0]='R';
	int cur=1;
	cur = longToChar(x,buf,cur);
	cur = longToChar(y,buf,cur);
	cur = longToChar(w,buf,cur);
	cur = longToChar(h,buf,cur);
	buf[cur] = (unsigned char) a;cur++;
	buf[cur] = (unsigned char) r;cur++;
	buf[cur] = (unsigned char) g;cur++;
	buf[cur] = (unsigned char) b;cur++;
	buf[cur] = (unsigned char) fill;
	return 14;
}

/**
 * Color selection dialogue
 * @param fill the filling boolean to set
 * @param a, r, g, b the alpha, red, green, blue component to set
 */
void selColor(_Bool * fill,int * a,int * r,int * g,int * b){
	char alpha[4]={'2','5','6'};
	while(!isByte(alpha)){
		printf("Veuillez selectionner l'alpha (entre 0 et 255) : ");
		fgets(alpha, 4, stdin);
		char * bsn = strchr(alpha, (int) '\n');
		if(bsn==NULL){
			int tmp = 0;
			while(tmp != '\n' && tmp != EOF){
				tmp=getchar();
			}
		}else{
			*bsn='\0';
		}
	}
	*a = strtol(alpha,NULL,10);
	char red[4]={'2','5','6'};
	while(!isByte(red)){
		printf("Veuillez selectionner le rouge (entre 0 et 255) : ");
		fgets(red, 4, stdin);
		char * bsn = strchr(red, (int) '\n');
		if(bsn==NULL){
			int tmp = 0;
			while(tmp != '\n' && tmp != EOF){
				tmp=getchar();
			}
		}else{
			*bsn='\0';
		}
	}
	*r = strtol(red,NULL,10);
	char green[4]={'2','5','6'};
	while(!isByte(green)){
		printf("Veuillez selectionner le vert (entre 0 et 255) : ");
		fgets(green, 4, stdin);
		char * bsn = strchr(green, (int) '\n');
		if(bsn==NULL){
			int tmp = 0;
			while(tmp != '\n' && tmp != EOF){
				tmp=getchar();
			}
		}else{
			*bsn='\0';
		}
	}
	*g = strtol(green,NULL,10);
	char blue[4]={'2','5','6'};
	while(!isByte(blue)){
		printf("Veuillez selectionner le bleu (entre 0 et 255) : ");
		fgets(blue, 4, stdin);
		char * bsn = strchr(blue, (int) '\n');
		if(bsn==NULL){
			int tmp = 0;
			while(tmp != '\n' && tmp != EOF){
				tmp=getchar();
			}
		}else{
			*bsn='\0';
		}
	}
	*b = strtol(blue,NULL,10);
	char f[2];
	printf("Voulez-vous remplir vos rectangles ? [Y/n] : ");
	fgets(f, 2, stdin);
	char * bsn = strchr(f, (int) '\n');
	if(bsn==NULL){
		int tmp = 0;
		while(tmp != '\n' && tmp != EOF){
			tmp=getchar();
		}
	}
	if(f[0]=='Y'){
		*fill=true;
	}else{
		*fill=false;
	}
}



