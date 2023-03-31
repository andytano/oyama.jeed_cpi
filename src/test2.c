#include <stdio.h>

extern char *dsp(int, char *);

int main(int argc, char *argv[])
{
	for (int i = 0; i < argc; i++) {
		dsp(i, argv[i]);
	}
}

