#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

// this server has 2 ways of running uptime. Uncomment the relevant lines to use it

static const int BUFFER = 1024;
#define PORT 52440 	//port number  52442

void runUptimeCommand( char* output );
void runUptimeCommand2( char* output );


int main(int argc, char *argv[])
{
	//variable to hold the uptime
	char *uptime = malloc(BUFFER);
	//char *uptime2 = malloc(BUFFER); //used for way 2

	int serverSocket, connectSocket; //sockets
	char readBuffer[BUFFER];

	//struct that holds socket and port info
	struct sockaddr_in serverAddress;
	int addresslength = sizeof(serverAddress);
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(PORT);
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);


    //create main socket
    if( (serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0 ){
		printf("Unable to create socket\n\r");
		return 0;
    }

	//allow port reuse, so the kernel will allow the same port to be used again if the server is quit and opened
	int optval = 1;
	setsockopt(serverSocket, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));

    //bind socket to port
    if( bind(serverSocket, (struct sockaddr*) &serverAddress, addresslength) < 0){
		printf("Unable to bind socket\n\r");
		return 0;
    }

	//start server listening
	if(listen(serverSocket, 3 ) < 0){
		printf("Unable to listen\n\r");
		return 0;
	}

//////////////////////////////////////////////////////////////////////////////////////////

	//iterative server, will go on forever
	int i = 1;
	while(1){

		//accept the connection 
		if( (connectSocket = accept(serverSocket, NULL, NULL)) < 0){
			printf("Unable to accept connection\n\r");
			return 0;
		}
		printf("Connection %i established\n\r", i);

		//get the uptime of the server
		//way 1
		runUptimeCommand(uptime);
		//change newline \n character to \0
		uptime[strlen(uptime) - 1] = '\0';
		//uptime[strcspn( uptime, "\n" )] = '\0';

		// //way 2
		// runUptimeCommand2(uptime2);
		// //change newline \n character to \0
		// uptime2[strlen(uptime) - 1] = '\0';
		// //uptime2[strcspn( uptime, "\n" )] = '\0';

		printf("Server Uptime: %s \n\r", uptime);
		//printf("%s \n\r", uptime2);

		//send uptime to client
		write(connectSocket, uptime, BUFFER);
		printf("Server Uptime sent to client\n\r");
		// write(connectSocket, uptime2, BUFFER);
		// printf("message sent back to client\n\r");

		//close connection
		if( close(connectSocket) == 0){
			printf("Connection %i closed\n\r\n\r", i);
		}

		i++;
	}

	//close listening server
	close(serverSocket);
	printf("Server closed\n\r");

	//free uptime
	free(uptime);
	//free(uptime2);

	return 0;
}

void runUptimeCommand( char* output )
{
    FILE *fp;

	//run uptime command
	if((fp = popen("uptime", "r")) == NULL){
		printf("Unable to open pipe\n\r");
		return;
	}

	//store in string
	fgets(output, BUFFER, fp);

	//close pipestream
	pclose(fp);

	return;
}


void runUptimeCommand2( char* output )
{
	FILE *fp;

	//clear file contents
	if((fp = fopen("output.txt", "w")) == NULL){
		printf("Unable to create or clear output file\n\r");
		return;
	}
	fclose(fp);

	//run uptime command and store output in textfile
	system("uptime >> output.txt");

	//read ouput textfile 
	if((fp = fopen("output.txt", "r")) == NULL){
		printf("Unable to read output file\n\r");
		return;
	}

	//store in string
	fgets(output, BUFFER, fp);

	//close filestream
	fclose(fp);

	return;
}







