# Draw
### Multi client drawing program
Up to 32 clients can draw in a synchronized window, using UDP and a distant server. 

(This program is just made for my own personal training, no real utility).

### Client
The client handle the drawing window, using SDL2 (the SDL interface 'display.c' isn't made by me, except for the "CheckEvents" function). 

Sends informations to the server about the shape the user wants to draw, and receives the shapes the server broadcasts.

### Server
The server list the connected users, wait for new drawings and send these shapes infos to every connected clients. 

### Small home-made application protocol
Send a Hello message (Connect to server) :

- 'H' (1 Byte)

Send a Bye message (Disonnect from server) :

- 'B' (1 Byte)

Fill the window with a color :

- 'C', Color (5 Bytes)

Draw a Line :

- 'L', X1, Y1, X2, Y2, Color (13 Bytes)

Draw a Rectangle :

- 'R', X, Y, Width, Height, Color, Filled (14 octets)
