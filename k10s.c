/*** k10s.c ***/
/* TCP/IP ECHO SERVER with THREAD */
/*     Usage: ./k10s <server port> */

#include "./myTCP.h"

#define DEBUG 1
#define BUF_LEN 32
#define QUEUE_MAX 5

struct threadArgs {
	int clSock;	// SOCKET DESCRIPTOR for CLIENT
};

/*** PROTOTYPE ***/
void *threadMain(void *thrArgs);										// MAIN THREAD
void clHandler(int clSock);  												// HANDLE A CLIENT REQUEST
int createTCPSvSock(unsigned short port);						// CREATE SOCKET
int acceptTCPConn(int svSock);											// ACCEPT TCP REQUEST
void pr_msg(char *header, char *str);								// PRINT MESSAGE (RE-ENTRANT)
void inet_ntoa_re(struct in_addr addr, char *str);	// INET_NtoA_RE (RE-ENTRANT)
void numToStr(long int num, char *str, int digit);	// CONVERT NUMBER to STRING (RE-ENTRANT)
void excep(char *errMsg);    												// HANDLE EXCEPTION

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
	char echoBuf[BUF_LEN];
	int bytesRcvd;

	if ((bytesRcvd = recv(clSock, echoBuf, BUF_LEN, 0)) == -1) {
		excep("FAIL:recv()[1]");
	}
	pr_msg("recv():", echoBuf);
	// pr_msg(itoa(bytesRcvd), "byte");	itoa NG !!

#if DEBUG
	long j; char s[40];
	numToStr((long int)clSock, s, 10);
	strcpy(s +10, ",loop:");
	for (j = 0; j < 10; j++) {
		numToStr(j, s +16, 10);
		pr_msg("clSock:", s);
		sleep(2);
	}
#endif

	while (bytesRcvd > 0) {
		/* ECHO-BACK */
		if (send(clSock, echoBuf, bytesRcvd, 0) != bytesRcvd) {
			excep("FAIL:send()");
		}
		pr_msg("ECHO-BACK MSG -> ", "SENT.");

		/* RECEIVE REMAIN PART */
		if ((bytesRcvd = recv(clSock, echoBuf, BUF_LEN, 0)) == -1) {
			excep("FAIL:recv()[2]");
		}
		pr_msg("recv():", echoBuf);
	}
	/* CLOSE SOCKET */
	close(clSock);
	pr_msg("CONNECTION CLOSED.", "\n");
	return;
}

/*** ACCEPT TCP REQUEST ***/
int acceptTCPConn(int svSock)
{
	int clSock;									// CLIENT SOCKET
	struct sockaddr_in clAddr;	// REMOTE(CLIENT) ADDRESS
	unsigned int clAddrLen;			// REMOTE(CLIENT) ADDRESS STRUCT. SIZE
	char addrStr[16];
	char portStr[6];

	clAddrLen = sizeof(clAddr);
	if ((clSock = accept(svSock, (struct sockaddr *)&clAddr, (socklen_t *)&clAddrLen)) == -1) {
  	excep("FAIL:accept()");
  }
	inet_ntoa_re(clAddr.sin_addr, addrStr);
				// ここはThread Contextではないけれど, せっかくなのでリエントラントで処理.
	pr_msg("CONNECTED CLIENT : ", addrStr);
	numToStr((long int)ntohs(clAddr.sin_port), portStr, 5);
	pr_msg("PORT : ", portStr);
	return (clSock);
}

/*** CREATE SOCKET ***/
int createTCPSvSock(unsigned short port)
{
	int sock;										// SOCKET
	struct sockaddr_in svAddr;	// LOCAL ADDRESS

	if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
		excep("FAIL:socket()");
	}

	/* アドレス構造体初期化 */
	memset(&svAddr, 0, sizeof(svAddr));
	svAddr.sin_family = AF_INET;
	svAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	svAddr.sin_port = htons(port);

	/* バインド (LOCAL SERVER ADDRESS) */
	if (bind(sock, (struct sockaddr *)&svAddr, sizeof(svAddr)) == -1) {
		excep("FAIL:bind()");
	}

	/* "LISTENING"状態に設定 */
	if (listen(sock, QUEUE_MAX) == -1) {
		excep("FAIL:listen()");
	}
	pr_msg("LISTENING...", "\n");

	return (sock);
}

/*** PRINT MESSAGE (RE-ENTRANT) ***/
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

/*** inet_ntoa_re (RE-ENTRANT) ***/
void inet_ntoa_re(struct in_addr addr, char *str)
	// str: char 16バイト
{
	unsigned char *p_addr;

	p_addr = (unsigned char *)&addr;
	numToStr((long int)*(p_addr +0), str +0, 3);
	*(str +3) = '.';
	numToStr((long int)*(p_addr +1), str +4, 3);
	*(str +7) = '.';
	numToStr((long int)*(p_addr +2), str +8, 3);
	*(str +11) = '.';
	numToStr((long int)*(p_addr +3), str +12, 3);
	return;
}

/*** CONVERT NUMBER TO STRING (RE-ENTRANT) ***/
void numToStr(long int num, char *str, int digit)
	// strはchar型で<digit>+1バイトの領域
{
	long int q;
	long int remain;

	str[digit] = '\0';
	for (digit--; digit >= 0; digit--) {
		q = num / 10L;
		remain = num - q * 10L;
		str[digit] = (char)remain + '0';
		num = q;
	}
	return;
}

/*** HANDLE EXCEPTION ***/
void excep(char *errMsg)
{
	perror(errMsg);
	exit(1);
}

