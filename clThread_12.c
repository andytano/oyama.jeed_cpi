/*** clThread_12.c ***/

#include "./myTCP.h"

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
	// int clSock: client socket
{
	char buf[BUF_LEN];
	int bytesRcvd;
	char *bufPtr;
	char *nextPtr;
	int i;
	char str[6];
	struct httpReqLine line = {NULL, NULL, NULL};
	struct httpReqHeader head[HEADERS_MAX];

	/* 1行受信 */
	bytesRcvd = recvOneLine(clSock, buf);
	pr_msg("recv():", buf);
	numToStr(bytesRcvd, str, 5);
	pr_msg("<SIZE> ", str);

	/* トークン取り出し */
	line.method = strtok_re(buf, " \t\r\n", &nextPtr);
	pr_msg("METHOD:", line.method);
	line.uri = strtok_re(NULL, " \t\r\n", &nextPtr);
	pr_msg("URI:", line.uri);
	line.httpVer = strtok_re(NULL, " \t\r\n", &nextPtr);
	pr_msg("HTTP_VER:", line.httpVer);
	
	bufPtr = nextPtr;
	for (i = 0; i < HEADERS_MAX; i++) {
		/* 1行ずつ受信 */
		bytesRcvd = recvOneLine(clSock, bufPtr);
		if (bytesRcvd == 0) break;
		numToStr(bytesRcvd, str, 5);
		pr_msg("<SIZE> ", str);
		pr_msg("recv():", bufPtr);

		/* トークン取り出し */
		numToStr(i, str, 3);
		pr_msg("HEAD No.", str);
		head[i].name = strtok_re(bufPtr, ":", &nextPtr);
		pr_msg("HEAD_NAME:", head[i].name);
		head[i].val = strtok_re(NULL, "\t\r\n", &nextPtr);
		pr_msg("HEAD_VAR:", head[i].val);
	}

	/* CLOSE SOCKET */
	if (shutdown(clSock, 2) == -1) {
		excep("FAIL:shudown()");
	}
	close(clSock);
	pr_msg("CONNECTION CLOSED.", "\n");
	return;
}


