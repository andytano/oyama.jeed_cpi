#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void *threadFunc(void *arg) {	/* スレッド関数 （関数名は任意）*/
	pthread_exit(NULL);
	return NULL;
}

int main(void) {
	pthread_t thread;
	if(pthread_create((pthread_t *)&thread,
		 NULL,
		 (void *)threadFunc,
		 NULL) != 0) {
		printf("Error: Failed to create new thread.\n");
		exit(1);
	}
	printf("*** thread func. ***\n");
	return 0;
}

