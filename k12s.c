/*** k12s.c ***/
/* HTTP SERVER PART 1              */
/*     Usage: ./k12s <server port> */

#include "./myTCPSv.h"

/*** MAIN ROUTINE ***/
int main(int argc, char *argv[])
{
	int svSock;										// SOCKET DESCRIPTOR for SERVER
	int clSock;										// SOCKET DESCRIPTOR for CLIENT
	unsigned short svPort;				// PORT NUMBER for SERVER
	pthread_t threadID;						// THREAD ID
	struct threadArgs *thrArgs;		// POINTER OF THREAD ARGUMENT STRUCTURE
	char thrIDStr[21];
	
	/* 初期化 */
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <server port>\n", argv[0]);
		exit(1);
	}
	svPort = atoi(argv[1]);
	svSock = createTCPSvSock(svPort);
	
	/* リクエスト待ちループ */
	while (1) {
		/* リクエスト受付け */
		clSock = acceptTCPConn(svSock);
		
		/* スレッド生成 */
		thrArgs = (struct threadArgs *)malloc(sizeof(struct threadArgs));
		if (thrArgs == NULL) {
			excep("FAIL:malloc()");
		}
		thrArgs->clSock = clSock;
		if (pthread_create(&threadID, NULL, threadMain, (void *)thrArgs) != 0) {
			excep("FAIL:pthread_create()");
		}
		numToStr((long int)threadID, thrIDStr, 20);
		pr_msg("THREAD ID : ", thrIDStr);
	}
	// close(svSock);
	// return 0;
}


