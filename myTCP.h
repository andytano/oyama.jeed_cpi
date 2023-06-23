/*** myTCPSv.h ***/
/* HEADER FILES for TCP/IP SERVER */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <arpa/inet.h>

#include <netdb.h>
#include <netinet/in.h>

#include <pthread.h>

//#define DEBUG 0
#define BUF_LEN 1024	// RECEIVE BUFFER SIZE
#define QUEUE_MAX 5
#define HEADERS_MAX 20	// HTTP HEADERS
#define MSG_LEN 1024	// CONSOLE MESSAGE BUFFER SIZE

/* FUNCTION PROTOTYPE */
//*** clThread
extern void *threadMain(void *thrArgs);														// MAIN THREAD
extern void clHandler(int clSock);  															// HANDLE A CLIENT REQUEST
//*** tcpip
extern int createTCPSvSock(unsigned short port);									// CREATE SOCKET
extern int acceptTCPConn(int svSock);															// ACCEPT TCP REQUEST
extern int recvOneLine(int clSock, char *buf);										// RECEIVE ONE LINE
//*** utils
extern void excep(char *errMsg);    															// HANDLE EXCEPTION
extern void inet_ntoa_re(struct in_addr addr, char *str);					// INET_NtoA_RE (RE-ENTRANT)
extern void numToStr(long int num, char *str, int digit);					// CONVERT NUMBER to STRING (RE-ENTRANT)
extern void pr_msg(char *header, char *str);											// PRINT MESSAGE (RE-ENTRANT)
char *strtok_re(char *str, const char *delim, char **laterPart);	// 'strtok' (RE-ENTRANT)

/* EXTERNAL VALUES */

/* STRUCTURE TEMPLATE */
struct threadArgs {
	int clSock;	// SOCKET DESCRIPTOR for CLIENT
};

struct httpReqLine {
	char *method;
	char *uri;
	char *httpVer;
};

struct httpReqHeader {
	char *name;
	char *val;
};

