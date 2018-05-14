#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

/*
 *
 * FOR TA USAGE TO RUN OUR PROGRAM
 * 
 * 1.) make
 * 2.) ./runScript
 * 3.) Run the VLC media receiver 
 * 
 */

static const int BUFFER = 2048;

int stringToInteger(char *inputString);

int main(int argc, char *argv[])
{
	if(argc != 6)
	{
		printf("Invalid entry, need args as follows: source IP, source port, destination IP, destination port, and loss rate as  integer from 0-100");
		exit(1);
	}

	//variable to hold the datagrams
	unsigned char datagram[BUFFER];
	int receiveLength;
	int lossRate = stringToInteger(argv[5]);

	//source IP Address we are expecting from
	struct sockaddr_in sourceAddress;
	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(PF_INET, argv[1], &sourceAddress.sin_addr)<=0) 
	{
		printf("\nInvalid source address/ Address not supported \n");
		return -1;
	}


	int serverSocket; //socket
	
	//struct that holds socket and port info
	struct sockaddr_in serverAddress, fromAddress, toAddress;
	socklen_t fromAddresslength, toAddressLength;
	toAddressLength = sizeof(toAddress);
	int addresslength = sizeof(serverAddress);
	
	//server info
	serverAddress.sin_family = PF_INET;
	serverAddress.sin_port = htons(stringToInteger(argv[2])); //port that source will send to
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);

	//toAddress info
	toAddress.sin_family = PF_INET;
	toAddress.sin_port = htons(stringToInteger(argv[4]));
	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(PF_INET, argv[3], &toAddress.sin_addr)<=0) 
	{
		printf("\nInvalid destination address/ Address not supported \n");
		return -1;
	}

    //create main socket
    if((serverSocket = socket(PF_INET, SOCK_DGRAM, 0)) < 0 )
    {
		printf("Unable to create socket\n\r");
		exit(1);
	}
	
	//allow port reuse, so the kernel will allow the same port to be used again if the server is quit and opened
	int optval = 1;
	setsockopt(serverSocket, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));

    //bind socket to port
    if( bind(serverSocket, (struct sockaddr*) &serverAddress, addresslength) < 0)
    {
		printf("Unable to bind socket\n\r");
		return 0;
	}

//////////////////////////////////////////////////////////////////////////////////////////

	//iterative server, will go on forever
	int i = 0;
	while(1)
    {	
        //receive datagram
        receiveLength = recvfrom(serverSocket, datagram, BUFFER, 0, (struct sockaddr*) &fromAddress, &fromAddresslength);

		printf("Received %d bytes of packet %i\n\r", receiveLength, i);

        if ( fromAddress.sin_addr.s_addr == sourceAddress.sin_addr.s_addr )
        {
			if(lossRate < (rand() % 101))
			{
				printf("Valid packet %i\n\r", i);
				//send datagram
				receiveLength = sendto(serverSocket, datagram, BUFFER, 0, (struct sockaddr*) &toAddress, toAddressLength);
				printf("Sent %d bytes of packet %i\n\r", receiveLength, i);
			}
        }

		i++;
	}


	return 0;
}

/*
 * String to integer conversion 
 */
int stringToInteger(char *inputString)
{
	int length = strlen(inputString);
	int ret = 0;
	for(int i = 0; i < length; i++)
	{
		ret *= 10;
		ret += inputString[i] - 48;
	}
	return ret;
}
