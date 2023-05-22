//*** kadai07c.c ***/
/* UDP/IP echo client */
/*     Usage: ./k07c <server IP> <echo word> [<echo port>] */

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
#define DEF_PORT 7

/* PROTOTYPE */
void excep(char *errMsg);	// exception

int main(int argc, char *argv[])
{
	char *svIP;                   // server IP address
	unsigned short svPort;				// server port#
	int sock;											// socket discripter
	struct sockaddr_in svAddr;		// echo server address
	struct sockaddr_in rcvdAddr;	// echo received address
	unsigned int rcvdAddrSize;		// size of struct 'Addr'
	char *echoStr;								// echo string
	unsigned int echoStrLen;			// length of echo string
	char rcvdStr[BUF_LEN];				// received string buffer
	unsigned int rcvdStrLen;			// length of received string
	
	/* アーギュメントのチェック */
	if ((argc < 3) || (argc > 4)) {
		fprintf(stderr, "Usage: %s <server IP> <echo word> [<echo port>]\n", argv[0]);
		exit(1);
	}
	svIP = argv[1];
	echoStr = argv[2];
	if ((echoStrLen = strlen(echoStr)) > BUF_LEN - 1) {
		excep("TOO lONG ECHO STRING !");
	}
	if (argc == 4) {
		svPort = atoi(argv[3]);
	} else {
		svPort = DEF_PORT;
	}

	/* ソケット生成 */
	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
		excep("FAIL:socket()");
	}
	
	/* アドレス構造体初期化 */
	memset(&svAddr, 0, sizeof(svAddr));
    svAddr.sin_family = AF_INET;
    svAddr.sin_addr.s_addr = inet_addr(svIP);
    svAddr.sin_port = htons(svPort);

	/* 文字列送信 */
	if (sendto(sock, echoStr, echoStrLen, 0, 
				(struct sockaddr *)&svAddr, sizeof(svAddr)) != echoStrLen) {
		excep("FAIL:sendto()");
	}
	printf("\nsendto():%s[%dbyte]\n", echoStr, echoStrLen);
	
	/* 応答受信 */
	rcvdAddrSize = sizeof(rcvdAddr);
	if ((rcvdStrLen = recvfrom(sock, rcvdStr, BUF_LEN, 0, 
				(struct sockaddr *)&rcvdAddr, &rcvdAddrSize)) != echoStrLen) {
		excep("FAIL:recvfrom()");
	}
	if (svAddr.sin_addr.s_addr != rcvdAddr.sin_addr.s_addr) {
		excep("PACKET RECEIVED FROM UNKNOWN PC.");
	}
	rcvdStr[rcvdStrLen] = '\0';
	printf("\nECHO STRING:%s\n", rcvdStr);

	close(sock);
	exit(0);
}

void excep(char *errMsg)
{
	perror(errMsg);
	exit(1);
}

