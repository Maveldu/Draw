gcc -c -Wall client.c display.c datahandler.c
gcc -Wall client.o display.o datahandler.o -o client -lSDL2

