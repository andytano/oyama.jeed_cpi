/*** utils_12.c ***/

#include "./myTCP.h"


/*** HANDLE EXCEPTION ***/
void excep(char *errMsg)
{
	perror(errMsg);
	exit(1);
}


/*** PRINT MESSAGE (RE-ENTRANT) ***/
void pr_msg(char *header, char *str)
{
	char msg[MSG_LEN] = "";
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


/*** SPLIT STRINGS TO TOKENS ***/
/*** 'strtok' (RE-ENTRANT) ***/
char *strtok_re(char *str, const char *delim, char **laterPart)
{
	char *ptrLeading;					// LEADING DELIMITER SCAN POINTER
	char *ptrToken;						// TOKEN SCAN POINTER
	char delimPlus[16] = " ";	// DELIMITERS (default:space)
	
	if (str == NULL) {	// strにNULLを指定することで連続の処理であることを指示する
		ptrLeading = *laterPart;	// 連続処理用
	} else {
		ptrLeading = str;	// 処理1回めの場合
	}
	
	/* 先頭のデリミタをスキップ */
	strcpy(delimPlus +1, delim);	// 指定デリミタ追加
	while (*ptrLeading && (strchr(delimPlus, *ptrLeading) != NULL)) {
		/* '*ptrLeading'はどれかのデリミタである場合 */
		ptrLeading++;
	}
	
	/* 一つのトークンの終わりを探す */
	ptrToken = ptrLeading;
	while (*ptrToken && (strchr(delim, *ptrToken) == NULL)) {
		/* '*ptrToken'はいずれのデリミタでもない */
		ptrToken++;
	}
	if (*ptrToken) {
		*ptrToken = '\0';
		*laterPart = ptrToken + 1;
	} else {
		*laterPart = ptrToken;
	}
	return (ptrLeading);
}

