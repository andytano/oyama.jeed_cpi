/*---------------------------------------------------------
プロジェクト:	スレッドでのOpenCV画像ファイル表示 
モジュール：	mainモジュール、スレッドモジュール			
作成者:			Yamada Tarou
作成日時:		2020/8/19
機能：			メインモジュールでスレッド起動
				スレッドでOpenCV画像ファイル表示
バージョン:		0.1
---------------------------------------------------------*/
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <fcntl.h>

extern int init_window(char *fname);
extern int disp_graph(int idx, char *fname);
extern int close_window();

//-----------------------------------------------------------------------------
//　Linux用kbhit関数
//-----------------------------------------------------------------------------
int kbhit(void)
{
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

//-----------------------------------------------------------------------------
//　スレッド関数
//-----------------------------------------------------------------------------
void *threadFunc(void *arg) {
	int r_cd = 1;
	int idx = 0;
	char fname[] = "sample";

	init_window(fname);

	while(r_cd)	{
		printf("Thread1...\n");

		disp_graph(idx, fname);
		idx++;
		if( idx > 2)	idx = 0;

		if( kbhit() )	{
			if( getchar() == 'q' )	{
				break;
			}
		}

		sleep(1);
	}

	close_window();		//Window閉じる

	pthread_exit(NULL);		//スレッド終了

	return NULL;
}

//-----------------------------------------------------------------------------
//　メイン関数
//-----------------------------------------------------------------------------
int main(void) {
	pthread_t thread;
	if(pthread_create((pthread_t *)&thread,
		 NULL,
		 (void *)threadFunc,
		 NULL) != 0) {
		printf("Error: Failed to create new thread.\n");
		exit(1);
	}

   	if(pthread_join(thread,NULL) != 0){		/* スレッドの終了待ち */
		printf("Error: Failed to wait for the thread termination.\n");
		exit(1);
	}
	printf("*** OpenCV & thread End ***\n");
	return 0;
}
