# Draw
### Multi client drawing program
Up to 32 clients are able to draw in a synchronized window, using UDP and a distant server. 

(This program is just made for my own personal training, no real utility).

### Client
The client handle the drawing window, using SDL2 (the SDL interface 'display.c' not beeing made by me, except the CheckEvents function). 

It sends informations about the shape the user want's tu draw to the server, and receive the shapes the server broadcasts.

### Server
The server lists the connected users, waits for new drawings and sends these shapes infos to every connected clients. 

### Small self-made application protocol
Send a Hello message (Connect to server) :

- 'H' (1 Byte)

Send a Bye message (Disonnect from server) :

- 'B' (1 Byte)

Clean the window with a color :

- 'C', Color (5 Bytes)

Draw a Line :

- 'L', X1, Y1, X2, Y2, Color (13 Bytes)

Draw a Line :

- 'R', X, Y, Width, Height, Color, Filled (14 octets)
