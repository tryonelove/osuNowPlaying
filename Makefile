CC=gcc
ARGS=-O -Wall
LIBS=-lX11

all:
	$(CC) $(REQ) main.c -o NowPlaying $(LIBS)