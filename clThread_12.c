/*** clThread_12.c ***/

#include "./myTCPSv.h"

/*** MAIN THREAD ***/
void *threadMain(void *thrArgs)
{
	int clSock;	// SOCKET DESCRIPTOR for CLIENT

	pthread_detach(pthread_self());		// --> RELEASE RESOURCES

	/* ARGUMENTS */
	clSock = ((struct threadArgs *)thrArgs)->clSock;
	free(thrArgs);

	/* HANDLE A CLIENT REQUEST */
	clHandler(clSock);
	return (NULL);
}


/*** HANDLE A CLIENT REQUEST ***/
void clHandler(int clSock)
	// int clSock: SOCKET DESCRIPTOR for CLIENT
{
	char buf[BUF_LEN];
	int bytesRcvd;
	char replyBuf[R_BUF_LEN];
	char *bufPtr;
	char *nextPtr;
	int i;
	char str[6];

	/* CLOSE SOCKET */
	if (shutdown(clSock, 2) == -1) {
		excep("FAIL:shudown()");
	}
	close(clSock);
	pr_msg("CONNECTION CLOSED.", "\n");
	return;
}
