//*** kadai08s.c ***/
/* UDP/IP echo server (async.) */
/*     Usage: ./k08s <server port> */

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
void excep(char *errMsg); 						// exception
void SIGIOHandler(int sigType);				// signal handler
void pr_msg(char *header, char *str);	// print message
void idling(void);										// idling

/* GLOBAL */
int sock;                   // socket discripter

int main(int argc, char *argv[])
{
	unsigned short svPort;      // server port#
	struct sockaddr_in svAddr;  // echo server address
	struct sigaction ioAction;	// signal action

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

	/* シグナル受付準備 */
	ioAction.sa_handler = SIGIOHandler;	// シグナルハンドラの指定
	if (sigfillset(&ioAction.sa_mask) == -1) {	// 全シグナルをマスクする
		excep("FAIL:sigfillset()");
	}
	ioAction.sa_flags = 0;	// フラグなし
	if (sigaction(SIGIO, &ioAction, 0) == -1) {	// シグナルハンドラのセット
		excep("FAIL:sigaction()");
	}
	if (fcntl(sock, F_SETOWN, getpid()) == -1) {	// シグナル受信プロセスの指定
		excep("FAIL:fcntl(), SIG RECEIVER");
	}
	if (fcntl(sock, F_SETFL, O_NONBLOCK | FASYNC) == -1) {	// flagをノンブロッキング, 非同期モード(シグナル駆動I/O)に設定
		excep("FAIL:fcntl(), NON-BLOCKING");
	}

	/* 待機ループ */
	while (1) {
		/* 何もしていない -> アイドリングさせる */
		idling();
	}
}

void idling(void)
{
	printf(".\n");
	sleep(2);
	return;
}

/* signal handler */
void SIGIOHandler(int sigType)
{
	struct sockaddr_in clAddr;  // echo client address
	unsigned int clAddrSize;    // size of struct 'Addr'
	char rcvdStr[BUF_LEN];      // received string buffer
	int rcvdStrLen;             // length of received string

	pr_msg("SIG DETECTED:", "SIGIO");  // write():system func.(re-entrant)
	do {
		/* 受信 */
		clAddrSize = sizeof(clAddr);
		if ((rcvdStrLen = recvfrom(sock, rcvdStr, BUF_LEN - 1, 0, (struct sockaddr *)&clAddr, &clAddrSize)) < 0) {
			/* エラーのとき */
			if (errno == EWOULDBLOCK) {
				// ブロックは実質的にはエラーではない
			} else {
				excep("FAIL:recvfrom()");
			}
		} else {
			/* 受信成功 */
			pr_msg("CLIENT : ", inet_ntoa(clAddr.sin_addr));
			rcvdStr[rcvdStrLen] = '\0';
			pr_msg("recvfrom():", rcvdStr);

			/* エコーバック */
			if (sendto(sock, rcvdStr, rcvdStrLen, 0, (struct sockaddr *)&clAddr, clAddrSize) != rcvdStrLen) {
				excep("FAIL:sendto()");
			}
			pr_msg("sendto():", "Echo-back sent.");
		}
	} while (rcvdStrLen >= 0);
}

/* print message */
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
