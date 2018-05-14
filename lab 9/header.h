#include <stdlib.h>
#include <stdio.h> 
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <string.h>
#include <sys/time.h>
#include "ccitt16.h"
#include "AddCongestion.h"

// Port we will be using
#define PORT 52440

// Typical buffer size
static const int BUFFER = 1024;

// I REALLY don't like typing out struct...
typedef struct sockaddr_in sockaddr_in;
typedef struct timeval timeval;

// Socket struct
typedef struct
{
    char* thisIP;
    char* destIP;
    sockaddr_in server;
} my_socket;

// Packet struct
typedef struct
{
    uint16_t sequenceNum;
    char data[2];
    short int crc;
} packet;

// Reused from lab4
void runUptimeCommand(char* output);

// My parser
char *nextLine(FILE *input);

// toString method
char *toString(packet *p);