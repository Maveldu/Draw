#ifndef __DATAHANDLER__
#define __DATAHANDLER__


/** Tests if a string of numbers corresponds to a valid byte value
 * @param byteStr pointing to the char array to test
 * @return false if not valid, true if valid
 */
_Bool isByte(char * byteStr);

/** Wait for input from user to set the server's IP address
 * @param addr[] the char array to fill with the inputed address
 */
void addrSelect(char addr[]);

/** Turn a long integer to a 2 Bytes array in Big Endian
 * @param x the integer to turn to Bytes
 * @param buf[] the char array to put the Bytes output
 * @param cur a Byte counter
 * @return the updated Byte counter
 */
int longToChar(long int x, unsigned char buf[], int cur);

/** Constructs a Hello message
 *  @param buf[] the char array to fill with the Hello message
 *  @return the message's length
 */
int makeHello(unsigned char buf[]);

/** Constructs a Bye message
 *  @param buf[] the char array to fill with the Bye message
 *  @return the message's length
 */
int makeBye(unsigned char buf[]);

/** Constructs a Fill message
 *  @param buf[] the char array to fill with the Fill message
 *  @param a, r, g, b the alpha, red, green, blue component
 *  @return the message's length
 */
int makeClean(int a, int r, int g, int b, unsigned char buf[]);

/** Constructs a Line message
 *  @param buf[] the char array to fill with the Line message
 *  @param a, r, g, b the alpha, red, green, blue component
 *  @param x1, y1, x2, y2 the coordinates of the line's first and second points
 *  @return the message's length
 */
int makeLine(int x1,int y1,int x2,int y2,int a,int r,int g,int b,unsigned char buf[]);

/** Constructs a Rectangle message
 *  @param buf[] the char array to fill with the Rectangle message
 *  @param a, r, g, b the alpha, red, green, blue component
 *  @param x, y the rectangle's coordinates
 *  @param w, h the rectangle's with and height
 *  @return the message's length
 */
int makeRect(int x,int y,int w,int h,int a,int r,int g,int b,_Bool fill,unsigned char buf[]);

/**
 * Color selection dialogue
 * @param fill the filling boolean to set
 * @param a, r, g, b the alpha, red, green, blue component to set
 */
void selColor(_Bool * fill,int * a,int * r,int * g,int * b);

#endif
