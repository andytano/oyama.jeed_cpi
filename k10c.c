/*** k10c.c ***/
/* TCP/IP echo client (k6c for k10s) */
/*     Usage: ./k10c <server IP> <echo word> [<echo port>] */

#include "./myTCP.h"

#define BUF_LEN 32
#define DEF_PORT 7

/*** PROTOTYPE ***/
void excep(char *errMsg);	// HANDLE EXCEPTION

/*** MAIN ROUTINE ***/
int main(int argc, char *argv[])
{
	int sock;
	struct sockaddr_in svAddr;
	unsigned short svPort;
	char *svIP;
	char *echoStr;
	char echoBuf[BUF_LEN];
	unsigned int echoStrLen;
	int bytesRcvd;
	unsigned int totalBytesRcvd;
	char buf[255];

	/* アーギュメントチェック */
	if ((argc < 3) || (argc > 4)) {
		fprintf(stderr, "Usage: %s <server IP> <echo word> [<echo port>]\n", argv[0]);
		exit(1);
	}
	svIP = argv[1];
	echoStr = argv[2];
	if (argc == 4) {
		svPort = atoi(argv[3]);
	} else {
		svPort = DEF_PORT;
	}

	/* ソケット生成 */
	if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
		excep("FAIL:socket()");
	}

	/* アドレス構造体初期化 */
	memset(&svAddr, 0, sizeof(svAddr));
	svAddr.sin_family = AF_INET;
	svAddr.sin_addr.s_addr = inet_addr(svIP);
	svAddr.sin_port = htons(svPort);

	/* コネクション確立 */
	if (connect(sock, (struct sockaddr *)&svAddr, sizeof(svAddr)) == -1) {
		excep("FAIL:connect()");
	}
	printf("\nCONNECTION ESTABLISHED.");

	/* 文字列送信 */
	echoStrLen = strlen(echoStr);
	if (send(sock, echoStr, echoStrLen, 0) != echoStrLen) {
		excep("FAIL:send()");
	}
	printf("\nsend():%s[%dbyte]", echoStr, echoStrLen);

	/* 返信文字列受信 */
	totalBytesRcvd = 0;
	printf("\nECHO RECEIVING: ");
	while (totalBytesRcvd < echoStrLen) {
		if ((bytesRcvd = recv(sock, echoBuf, BUF_LEN-1, 0)) <= 0) {
			excep("FAIL:recv() or CONNECTION CLOSED.");
		}
		echoBuf[bytesRcvd] = '\0';
		strcpy(buf + totalBytesRcvd, echoBuf);
		printf("\nrecv():%s[%dbyte]", echoBuf, bytesRcvd);
		totalBytesRcvd += bytesRcvd;
	}
	printf("\n\nECHO STRING:%s\n", buf);

	/* クローズ */
	if (shutdown(sock, 2) == -1) {
		excep("FAIL:shutdown()");
	}
	close(sock);
	exit(0);
}

/*** HANDLE EXCEPTION ***/ 
void excep(char *errMsg)
{
	perror(errMsg);
	exit(1);
}

