//*** kadai07s.c ***/
/* UDP/IP echo server */
/*     Usage: ./k07s <server port> */

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

/* PROTOTYPE */
void excep(char *errMsg); // exception

int main(int argc, char *argv[])
{
	unsigned short svPort;      // server port#
	int sock;                   // socket discripter
	struct sockaddr_in svAddr;  // echo server address
	struct sockaddr_in clAddr;  // echo client address
	unsigned int clAddrSize;    // size of struct 'Addr'
	char rcvdStr[BUF_LEN];      // received string buffer
	int rcvdStrLen;             // length of received string

	/* アーギュメントのチェック */
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <server port>\n", argv[0]);
		exit(1);
	}
	svPort = atoi(argv[1]);

	/* ソケット生成 */
	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
		excep("FAIL:socket()");
	}

	/* アドレス構造体初期化 */
	memset(&svAddr, 0, sizeof(svAddr));
	svAddr.sin_family = AF_INET;
	svAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	svAddr.sin_port = htons(svPort);

	/* バインド(LOCAL SERVER ADDRESS) */
	if (bind(sock, (struct sockaddr *)&svAddr, sizeof(svAddr)) == -1) {
		excep("FAIL:bind()");
	}
	while (1) {
		/* 受信 */
		clAddrSize = sizeof(clAddr);
		if ((rcvdStrLen = recvfrom(sock, rcvdStr, BUF_LEN - 1, 0,	(struct sockaddr *)&clAddr, &clAddrSize)) < 0) {
			excep("FAIL:recvfrom()");
		}
		printf("\nCLIENT : %s", inet_ntoa(clAddr.sin_addr));	// (* Ex08-BLOCKED)
		printf(", port%u\n", ntohs(clAddr.sin_port));
		rcvdStr[rcvdStrLen] = '\0';
		printf("\nrecvfrom():%s[%dbyte]\n", rcvdStr, rcvdStrLen);

		/* エコーバック */
		if (sendto(sock, rcvdStr, rcvdStrLen, 0, (struct sockaddr *)&clAddr, clAddrSize) != rcvdStrLen) {
			excep("FAIL:sendto()");
		}
		printf("\nEcho-back sent.\n");
	}
	// close(sock);
	// return 0;
}

void excep(char *errMsg)
{
	perror(errMsg);
	exit(1);
}
