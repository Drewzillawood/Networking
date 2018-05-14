#include "header.h"

/*
 * Main function to all of our goodies
 * argv[1] localhost
 * argv[2] client IP
 */
int main(int argc, char *argv[])
{
    // Declare our server socket
    my_socket receiver = 
    {
        "",
        "",
        {
            .sin_family      = AF_INET,
            .sin_port        = htons(PORT),
            .sin_addr.s_addr = htonl(INADDR_ANY)
        }
    };

    // Create our main socketn
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == 0)
    {
        printf("Unable to create socket\n\r");
        exit(0);
    }

    // Allow port reuse, so the kernel will allow the same port to be 
    // Used again if the server is quit and reopened
    int optval = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));

    // Setting TO period to 3 seconds
    timeval tv;
    tv.tv_sec = 3;
    tv.tv_usec = 0;
    // setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*) &tv, sizeof(tv));

    //Bind socket to the port
    if(bind(sock, (struct sockaddr*) &receiver.server, sizeof(receiver.server)) < 0)
    {
        printf("Unable to bind socket\n\r");
        exit(0);
    }

    // Start server listening
    if(listen(sock, 3) < 0)
    {
        printf("Unable to listen\n\r");
        exit(0);
    }

    // File for input
    FILE *f = fopen("input.txt", "r");
    char *message = nextLine(f);
    fclose(f);

    // Adding congestion 
    AddCongestion(message, 0.00001);

    // Connect to our socket
    int connectSocket = accept(sock, NULL, NULL);
    if(connectSocket < 0)
    {
        printf("Unable to accept connection\n\r");
        return 0;
    }
    printf("Connection established\n\r");

    // Iterative server, will go on forever
    for(short int i = 1000, j = 0; j < 1024; i++, j+=2)
    {
        // What we are sending over
        char c[] = {message[j], message[j+1]};
        packet p = 
        {
            i,
            {
                c[0],
                c[1]
            },
            calculate_CCITT16(c, 2, GENERATE_CRC)
        };

        char *temp = toString(&p);
        // Send a message to our socket
        write(connectSocket, temp, 7);
        printf("Server sent message to client\n\r");
        free(temp);

        // Will attempt to read up until timeout
        char ACK[2];
        recv(connectSocket, ACK, 2, 0);
        unsigned int first = (ACK[0] << 8) & 0xFF00;
        unsigned short int second = ACK[1] & 0xFF;
        int combined = first | second;
        printf("%i ONE PRINTY BOI IT IS!\n\r", combined);

    }
    if(close(connectSocket) == 0)
            printf("Connection closed\n\r");
    free(message);

    close(sock);
    printf("Server closed\n\r");
    return 0;
}

/*
 * Handy dandy, string parser! 
 */
char *nextLine(FILE *input)
{
	int buf = 10;
	int read = 0;
	char *buffer = malloc(buf);
	while(true)
	{
		char nextChar = fgetc(input);
		if(read == buf)
		{
			buf *= 2;
			buffer = realloc(buffer, buf);
		}

		if(nextChar == '\n' || nextChar == EOF)
		{
			buffer = realloc(buffer, read + 1);
			buffer[buf] = '\0';
			return buffer;
		}

		buffer[read] = nextChar;
		read++;
	}
}

/*
 * toString method for packet
 */
char *toString(packet *p)
{
    char *send = malloc(7);

    send[0] = ((p->sequenceNum) & 0xFF00) >> 8;
    send[1] = (p->sequenceNum) & 0xFF;
    send[2] = p->data[0];
    send[3] = p->data[1];
    send[4] = ((p->crc) & 0xFF00) >> 8;
    send[5] = (p->crc) & 0xFF;
    send[6] = '\0';

    return send;
}