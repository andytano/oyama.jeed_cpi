/**************************************/
/*      TCP/IP client program         */
/*      Usage: ./k05c HostName        */
/**************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

#define BUFLEN 64
#define PORT 50001

int main(int argc, char *argv[])
{
	int SessionID;
	struct hostent *HostInfo;
	struct sockaddr_in ServerAddr;
	char buf[BUFLEN];
	
	/* 入力チェック */
	if (argc != 2) {
		fprintf(stderr, "Usage: ./k05c hostname !\n");
		exit(1);
	}
	
	/* ソケット生成 */
	if ((SessionID = socket(AF_INET, SOCK_STREAM, 0 /*IPPROTO_TCP*/)) == -1) {
		fprintf(stderr, "Fail to make a socket !\n");
		exit(1);
	}
	
	/* ホスト名から接続情報を取り出す */
	if ((HostInfo = gethostbyname(argv[1])) == NULL) {
		fprintf(stderr, "Fail to find host !\n");
		exit(1);
	}

	/* 接続情報の準備 */
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(PORT);
	memcpy((char *)&ServerAddr.sin_addr, (char *)HostInfo->h_addr_list[0], HostInfo->h_length);

	/* ソケットの接続 */
	if (connect(SessionID, (struct sockaddr *)&ServerAddr, sizeof(ServerAddr)) == -1) {
		fprintf(stderr, "Fail to connect with the server !\n");
		exit(1);	
	}
	
	/* メッセージ受信 */
	if (recv(SessionID, buf, BUFLEN, 0) == -1) {
		fprintf(stderr, "Fail to send or recv !\n");
		exit(1);	
	}
	printf("%s\n", buf);
	
	/* セッション終了 */
	if (shutdown(SessionID, 2) == -1) {
		fprintf(stderr, "Failed to shutdown !\n");
		exit(1);
	}
	close(SessionID);
	
	exit(0);
}








