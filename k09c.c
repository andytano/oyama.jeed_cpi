//*** kadai09c.c ***/
/* UDP/IP echo client (SIGALARM / Timeout) */
/*     Usage: ./k09c <server IP> <echo word> [<echo port>] */

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
#include <errno.h>

#define BUF_LEN 32
#define DEF_PORT 7
#define TIMEOUT_SEC 3
#define RETRY_MAX 5

/* PROTOTYPE */
void excep(char *errMsg);	// exception
void SIGALRMHandler(int sigType);	// signal handler
void pr_msg(char *header, char *str);	// print message

/* GLOBAL */
int retry_cnt = 0;	// retry count

int main(int argc, char *argv[])
{
	char *svIP;                   // server IP address
	unsigned short svPort;				// server port#
	char *echoStr;								// echo string
	unsigned int echoStrLen;			// length of echo string
	int sock;											// socket discripter
	struct sockaddr_in svAddr;		// echo server address
	struct sockaddr_in rcvdAddr;	// echo received address
	unsigned int rcvdAddrSize;		// size of struct 'Addr'
	char rcvdStr[BUF_LEN];				// received string buffer
	unsigned int rcvdStrLen;			// length of received string
	struct sigaction almAction;		// signal action

	/* アーギュメントのチェック */
	if ((argc < 3) || (argc > 4)) {
		fprintf(stderr, "Usage: %s <server IP> <echo word> [<echo port>]\n", argv[0]);
		exit(1);
	}
	svIP = argv[1];
	echoStr = argv[2];
	if ((echoStrLen = strlen(echoStr)) >= BUF_LEN) {
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
	if (sendto(sock, echoStr, echoStrLen, 0, (struct sockaddr *)&svAddr, sizeof(svAddr)) != echoStrLen) {
		excep("FAIL:sendto()");
	}
	pr_msg("sendto():", echoStr);

	/* シグナルハンドラの準備 */
	almAction.sa_handler = SIGALRMHandler;	// シグナルハンドラの指定
	if (sigfillset(&almAction.sa_mask) == -1) {	// 全シグナルをマスクする
		excep("FAIL:sigfillset()");
	}
	almAction.sa_flags = 0;	// フラグなし
	if (sigaction(SIGALRM, &almAction, 0) == -1) {	// シグナルハンドラのセット
		excep("FAIL:sigaction()");
	}

	/* タイムアウトの設定 */
	alarm(TIMEOUT_SEC);

	/* 応答受信 */
	rcvdAddrSize = sizeof(rcvdAddr);
	while ((rcvdStrLen = recvfrom(sock, rcvdStr, BUF_LEN, 0, (struct sockaddr *)&rcvdAddr, &rcvdAddrSize)) != echoStrLen) {
		if (errno == EINTR) {
			/* ALARM終了 */
			if (retry_cnt < RETRY_MAX) {
				/* RETRY */
				printf("TIMEOUT! (%d)\n", retry_cnt);
				/* 文字列再送信 */
				if (sendto(sock, echoStr, echoStrLen, 0, (struct sockaddr *)&svAddr, sizeof(svAddr)) != echoStrLen) {
					excep("FAIL:sendto()");
				}
				alarm(TIMEOUT_SEC);
			} else {
				/* RETRY回数オーバー */
				excep("NO RESPONSE !");
			}
		} else {
			/* TIMEOUT以外 */
			excep("FAIL:recvfrom()");
		}
	}
	if (svAddr.sin_addr.s_addr != rcvdAddr.sin_addr.s_addr) {
		excep("PACKET RECEIVED FROM UNKNOWN PC.");
	}
	alarm(0);	// ALARMキャンセル
	rcvdStr[rcvdStrLen] = '\0';
	printf("\nECHO STRING:%s\n", rcvdStr);
	close(sock);
	exit(0);
}

/* signal handler */
void SIGALRMHandler(int sigType)
{
	pr_msg("SIG DETECTED:", "SIGALRM");
	retry_cnt++;
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

