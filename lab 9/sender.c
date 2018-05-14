#include "header.h"

/*
 * Main function to run all of our goodies
 * argv[1] localhost
 * argv[2] destination IP
 */
int main(int argc, char *argv[])
{
    // Declare our simple socket
    my_socket sender =
    {
        argv[1],
        argv[2], 
        {
            .sin_family = AF_INET,
            .sin_port   = htons(PORT)
        }
    };  

    // Create our socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == 0)
    {
        printf("Unable to create socket\n\r");
        exit(0);
    }

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, sender.destIP, &sender.server.sin_addr) <= 0)
    {
        printf("Invalid address/ Address not supported \n");
        exit(-1);
    }

    // Connect to the server
    if(connect(sock, (struct sockaddr *) &sender.server, sizeof(sender.server)) < 0)
    {
        printf("Connection Failed on %s\n", sender.destIP);
        exit(-1);
    }
    printf("Connection established\n\r");

    char buffer[BUFFER/2][2];

    memset(buffer, '.', 512);

    for(int i = 0; i < 500; i++)
    {
        char message[7];
        read(sock, message, 7);
        message[6] = '\0';
        printf("%s : %s\n\r", sender.destIP, message);

        char temp[] = 
        {
            message[4],
            message[5]
        };

        if(calculate_CCITT16(temp, 2, CHECK_CRC))
        {
            sleep(1);
            temp[0] = message[0];
            temp[1] = message[1];
            write(sock, temp, 2);
        }
    }

    if(close(sock) == 0)
        printf("Connection closed\n\r");

    return 0;
}