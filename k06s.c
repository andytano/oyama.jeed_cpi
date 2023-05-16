//*** kadai06s.c ***/
/* TCP/IP echo server */
/*     Usage: ./k06s <server port> */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>

#define BUF_LEN 32
#define QUEUE_MAX 5

/* PROTOTYPE */
void clHandler(int clSock);  // Client Handler
void excep(char *errMsg);    // Exception

int main(int argc, char *argv[])
{
	int svSock;
	int clSock;
	struct sockaddr_in svAddr;
	struct sockaddr_in clAddr;
	int svPort;
	int clAddrLen;  // Size of "Client Address Struct"

	/* CHECK ARGS. */
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <server port>\n", argv[0]);
		exit(1);
	}
	svPort = atoi(argv[1]);

	/* CREATE SOCKET */
	if ((svSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
		excep("FAIL:socket()");
	}

	/* INITIALIZE ADDRESS STRUCT */
	memset(&svAddr, 0, sizeof(svAddr));
	svAddr.sin_family = AF_INET;
	svAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	svAddr.sin_port = htons(svPort);

	/* BIND (LOCAL SERVER ADDRESS) */
	if (bind(svSock, (struct sockaddr *)&svAddr, sizeof(svAddr)) == -1) {
		excep("FAIL:bind()");
	}

	/* LISTENING */
	if (listen(svSock, QUEUE_MAX) == -1) {
		excep("FAIL:listen()");
	}

	printf("\nLISTENING...");

	while (1) {
		/* WAITING REQUEST FROM CLIENTS */
		printf("\naccept()");
		if ((clSock = accept(svSock, (struct sockaddr *)&clAddr, &clAddrLen)) == -1) {
    			excep("FAIL:accept()");
		}

		printf("\nCONNECTED CLIENT : %s", inet_ntoa(clAddr.sin_addr));
    		printf(", port%d", ntohs(clAddr.sin_port));

		/* CALL CLIENT HANDLER */
		clHandler(clSock);
	}
	// close(svSock);
	// return 0;
}

/*** CLIENT HANDLER ***/
void clHandler(int clSock)
// int clSock: client socket
{
	char echoBuf[BUF_LEN];
	int bytesRcvd;

	if ((bytesRcvd = recv(clSock, echoBuf, BUF_LEN - 1, 0)) == -1) {
		excep("FAIL:recv()[1]");
	}
	echoBuf[bytesRcvd] = '\0';
	printf("\nrecv():%s[%dbyte]", echoBuf, bytesRcvd);

	while (bytesRcvd > 0) {
		/* ECHO-BACK */
		if (send(clSock, echoBuf, bytesRcvd, 0) != bytesRcvd) {
			excep("FAIL:send()");
		}
		printf("\necho-back sent.");

		/* RECEIVE REMAIN PART */
		if ((bytesRcvd = recv(clSock, echoBuf, BUF_LEN - 1, 0)) == -1) {
			excep("FAIL:recv()[2]");
		}
		echoBuf[bytesRcvd] = '\0';
		printf("\nrecv():%s[%dbyte]", echoBuf, bytesRcvd);
	}

	/* CLOSE SOCKET */
	close(clSock);
	printf("\nCONNECTION CLOSED.");
	return;
}

void excep(char *errMsg)
{
	perror(errMsg);
	exit(1);
}
