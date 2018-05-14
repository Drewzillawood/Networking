all: sender receiver

sender: sender.c header.h ccitt16.o
	$(CC) -g -std=gnu99 -o sender ccitt16.o sender.c -I.

receiver: receiver.c header.h ccitt16.o AddCongestion.c
	$(CC) -g -std=gnu99 -o receiver ccitt16.o receiver.c AddCongestion.c -I.