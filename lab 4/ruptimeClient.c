#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

static const int BUFFER = 1024;
#define PORT 52440

int main(int argc, char *argv[])
{


    //can be many arguments
    //possibly argv[0] is executable file name, argv[1] is localhost's IP address,
    // argv[2] is server's IP address
    int clientSocket;
    char* ipAddress;
    char uptimes[argc-1][BUFFER]; //array to store uptimes

    struct sockaddr_in serv_addr;
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    for (int i = 0; i < argc - 1; i++){   
        //create socket
        if((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0){
            printf("Unable to create socket\n\r");
            return 0;
        }

        //Get ipAddress
        ipAddress = argv[i+1];
        
        // Convert IPv4 and IPv6 addresses from text to binary form
        if(inet_pton(AF_INET, ipAddress, &serv_addr.sin_addr)<=0) 
        {
            printf("\nInvalid address/ Address not supported \n");
            return -1;
        }

        //connect to server
        if (connect(clientSocket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        {
            printf("\nConnection Failed on %s\n", ipAddress);
            return -1;
        }
        printf("Connection %i established\n\r", i+1);

        //read uptime from server
        read(clientSocket , uptimes[i], BUFFER);
        printf("%s : %s\n",ipAddress, uptimes[i]);

        //close connection
        if( close(clientSocket) == 0){
            printf("Connection %i closed\n\r\n\r", i+1);
        }
    }

    //display server address and uptime info
    printf("IP ADDRESS \t  SERVER UPTIME\n\r");

    for (int i = 0; i < argc - 1; i++){   
        printf("%s \t %s\n",ipAddress, uptimes[i]);
    }

    return 0;
}

