#include <time.h>
#include <stdio.h>

int main(void)
{
	const time_t t = time(NULL);
	const char *lt = asctime(localtime(&t));
	printf("%s", lt);
	return 0;
}
