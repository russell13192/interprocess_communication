/**
 * @defgroup   SERVER server
 *
 * @brief      This file implements server.
 *
 * @author     Russell
 * @date       2019
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_NAME "/tmp/DemoSocket"
#define BUFFER_SIZE 128

int main(int argc, char const *argv[])
{
	
	struct sockaddr_un name;
#if 0
	struct sockaddr_un {
		sa_family_t sun_family;				/* AF_UNIX */
		char 		sun_path[108];			/* pathname */
	}
#endif

	int ret;
	int connection_socket;
	int data_socket;
	int result;
	int data;
	char buffer[BUFFER_SIZE];

	/* In case the program exited inadvertently on the last run, remove the socket
	*/

	unlink(SOCKET_NAME);

	/* Create Master Socket */

	/* SOCK_DGRAM for Datagram based communication */
	connection_socket = socket(AF_UNIX, SOCK_STREAM, 0);

	/* Error handling */

	if (connection_socket == -1)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}

	printf("Master socket created\n");

	/* Initiailize */
	memset(&name, 0, sizeof(struct sockaddr_un));

	/* Specify the socket credentials */
	name.sun_family = AF_UNIX;
	strncpy(name.sun_path, SOCKET_NAME, sizeof(name.sun_path) - 1);
	
	/* Bind socket to socket name.
	Purpose of bind() system call is that application() dictates to the underlying 
	operating systems the criteria of receiving the data. Here, bind() system call is
	telling the OS that if sender process sends the data destined to socket "/tmp/DemoSocket",
	then such data needs to be delivered to this server process (the server process)
	*/
	ret = bind(connection_socket, (const struct sockaddr *) &name, sizeof(struct sockaddr_un));

	if (ret == -1)
	{
		perror("bind");
		exit(EXIT_FAILURE);
	}

	printf("bind() call succeded\n");

	/* 
	Prepare for accepting connections. The backlog size is set to 20. So while one request is 
	being processed other requests can be waiting. 
	*/

	ret = listen(connection_socket, 20);
	if (ret == -1)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}

	/*
	This is the main loop for handling connections. All server processes usually run 24 x 7.
	Good servers should always be up and running and should never go down. 
	*/
	for (;;)
	{
		/* Wait for incoming connection */
		printf("Waiting on accept() sys call\n"); 

		data_socket = accept(connection_socket, NULL, NULL);

		if (data_socket == -1)
		{
			perror("accept");
			exit(EXIT_FAILURE);
		}

		printf("connection accepted from client\n");

		result = 0;

		for (;;)
		{
			/* Prepare the buffer to recv the data */
			memset(buffer, 0, BUFFER_SIZE); 

			/* Wait for the next data packet.
			The server is blocked here. Waiting for the data to arrive from client/
			'read' is a blocking system call 
			*/
			printf("Waiting for data from the client\n");
			ret = read(data_socket, buffer, BUFFER_SIZE);

			if (ret == -1)
			{
				perror("read");
				exit(EXIT_FAILURE);
			} 
			/* Add received summand. */
			memcpy(&data, buffer, sizeof(int));
			if (data == 0) break;
			result += data;
		}

		/* Send result */
		memset(buffer, 0, BUFFER_SIZE);
		sprintf(buffer, "Result = %d", result);

		printf("Sending final result back to client\n");
		ret = write(data_socket, buffer, BUFFER_SIZE);
		if (ret == -1)
		{
			perror("write");
			exit(EXIT_FAILURE);
		}

		/* Close socket. */ 
		close(data_socket);
	}
	/* Close the master socket */
	close(connection_socket); 
	printf("Connection closed...\n", );

	/* 
	Server should release resources before getting terminated.
	Unlink the socket. 
	*/

	unlink(SOCKET_NAME);
	exit(EXIT_SUCCESS);
}