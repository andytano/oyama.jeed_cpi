/*** k09s.c  (test program for k09c) ***/
/* UDP/IP echo server with SIGIO & SIGINT */
/*     Usage: ./k09s <server port> */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/file.h>
#include <errno.h>

#define BUF_LEN 32

/* PROTOTYPE */
void excep(char *errMsg);	// Exception
void idling(void);	// Idling
void SIGIOHandler(int sigType);	// signal handler
void pr_msg(char *header, char *str);	// print message

/* GLOBAL */
int sock;


int main(int argc, char *argv[])
{
	struct sockaddr_in svAddr;
	unsigned short svPort;
	struct sigaction ioAction;	// signal action

	/* CHECK ARGS. */
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <server port>\n", argv[0]);
		exit(1);
	}
	svPort = atoi(argv[1]);

	/* CREATE SOCKET */
	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
		excep("FAIL:socket()");
	}

	/* INITIALIZE ADDRESS STRUCT. */
	memset(&svAddr, 0, sizeof(svAddr));
	svAddr.sin_family = AF_INET;
	svAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	svAddr.sin_port = htons(svPort);

	/* BIND (LOCAL SERVER ADDRESS) */
	if (bind(sock, (struct sockaddr *)&svAddr, sizeof(svAddr)) == -1) {
		excep("FAIL:bind()");
	}

	/* SIGIO */
	ioAction.sa_handler = SIGIOHandler;
	if (sigfillset(&ioAction.sa_mask) == -1) {	// mask all SIGs
		excep("FAIL:sigfillset()");
	}
	if (sigdelset(&ioAction.sa_mask, SIGINT) == -1) {	// unmask SIGINT
		excep("FAIL:sigdelset()");
	}
	ioAction.sa_flags = 0;	// no flag
	if (sigaction(SIGIO, &ioAction, 0) == -1) {	// set SIG action handler
		excep("FAIL:sigaction()");
	}
	if (fcntl(sock, F_SETOWN, getpid()) == -1) {	// set SIG receiving process
		excep("FAIL:fcntl(),SIG RECEIVER");
	}
	if (fcntl(sock, F_SETFL, O_NONBLOCK | FASYNC) == -1) {	// set flag:non-blocking & asynchronous(signal-driven I/O) mode
		excep("FAIL:fcntl(),NON-BLOCKING");
	}

	while (1) {
		idling();
	}
}


/* IDLING */
void idling(void)
{
	printf(".\n");
	sleep(2);
	return;
}


/* signal handler(SIGIO) */
void SIGIOHandler(int sigType)
{
	struct sockaddr_in clAddr;
	unsigned int clAddrLen;	// Client Address Struct. Size
	char echoBuf[BUF_LEN];
	int bytesRcvd;
	// char msg[64] = "";
	int dummy;

	//write(fileno(stdout), msg, strlen(msg));
	pr_msg("SIG DETECTED:", "SIGIO");	// write():system func.(re-entrant)
	do {
		/* RECEIVE */
		clAddrLen = sizeof(clAddr);
		if ((bytesRcvd = recvfrom(sock, echoBuf, BUF_LEN - 1, 0, (struct sockaddr *)&clAddr, &clAddrLen)) < 0) {
			if (errno == EWOULDBLOCK) {
				// BLOCK IS NOT ERR.
			} else {
				excep("FAIL:recvfrom()");
			}
		} else {
			/* RECEIVE SUCCESS */
			pr_msg("CLIENT : ", inet_ntoa(clAddr.sin_addr));
			echoBuf[bytesRcvd] = '\0';
			pr_msg("recvfrom():", echoBuf);
			/* ECHO BACK */

			//sleep(25);  // FOR DEBUG
			printf("MAKE SLEEP (type 0 & enter!)");
			scanf("%d",&dummy);
				//リエントリしないように注意すること

			if (sendto(sock, echoBuf, bytesRcvd, 0, (struct sockaddr *)&clAddr, clAddrLen) != bytesRcvd) {
				excep("FAIL:sendto()");
			}
		}
	} while (bytesRcvd >= 0);
	//close(sock);
	//return 0;
}


void pr_msg(char *header, char *str)
{
	char msg[64] = "";
	char *ptr;
	ptr = msg;
	strcpy(ptr, header);
	ptr += strlen(header);
	strcpy(ptr, str);
	ptr += strlen(str);
	strcpy(ptr, "\n");
	write(fileno(stdout), msg, strlen(msg));
	return;
}


void excep(char *errMsg)
{
    perror(errMsg);
    exit(1);
}

