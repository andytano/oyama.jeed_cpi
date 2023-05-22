//*** kadai06c.c ***/
/* TCP/IP echo client */
/*     Usage: ./k06c <server IP> <echo word> [<echo port>] */

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
void excep(char *errMsg);    // Exception

int main(int argc, char *argv[])
{
  char *svIP;
  char *echoStr;
  unsigned int echoStrLen;
  short int svPort;
  int sock;
  struct sockaddr_in svAddr;
  int bytesRcvd;
  int totalBytesRcvd;
  char echoBuf[BUF_LEN];
  char buf[255];

  /* CHECK ARGS. */
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

  /* CREATE SOCKET */
  if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
    excep("FAIL:socket()");
  }

  /* INITIALIZE ADDRESS STRUCT */
  memset(&svAddr, 0, sizeof(svAddr));
  svAddr.sin_family = AF_INET;
  svAddr.sin_addr.s_addr = inet_addr(svIP);
  svAddr.sin_port = htons(svPort);

  if (connect(sock, (struct sockaddr *)&svAddr, sizeof(svAddr)) == -1) {
    excep("FAIL:connect()");
  }
  printf("\nCONNECTION ESTABLISHED.");

  /* SEND STRINGS */
  echoStrLen = strlen(echoStr);
  if (send(sock, echoStr, echoStrLen, 0) != echoStrLen) {
    excep("FAIL:send()");
  }
  printf("\nsend():%s[%dbyte]", echoStr, echoStrLen);

  /* RECEIVE ECHO-BACK MESSAGE */
  printf("\nECHO RECEIVING: ");
  totalBytesRcvd = 0;
  while (totalBytesRcvd < echoStrLen) {
    if ((bytesRcvd = recv(sock, echoBuf, BUF_LEN - 1, 0)) <= 0) {
      excep("FAIL:recv() or CONNECTION CLOSED.");
    }
    echoBuf[bytesRcvd] = '\0';
    strcpy(buf + totalBytesRcvd, echoBuf);
    printf("\nrecv():%s[%dbyte]", echoBuf, bytesRcvd);
    totalBytesRcvd += bytesRcvd;
  }
  printf("\n\nECHO STRING:%s\n", buf);

  if (shutdown(sock, 2) == -1) {
    excep("FAIL:shutdown()");
  }
  close(sock);
  exit(0);
}

void excep(char *errMsg)
{
  perror(errMsg);
  exit(1);
}
