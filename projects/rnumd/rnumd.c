/*
 *
 * For use with crontab(1), crontab(5) or at(1)
 *
 * Usage: rnumd ;sysctl kern.randompid=$? >/dev/null
 *
 * $Id$
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[]){
	srandom(time(NULL) % 255);

	unsigned int rseed = (unsigned int)random() % time(NULL);
	srandom(rseed);
	int r = ((random() % 1013) + (random() % 7));
	
	unsigned int lseed = (unsigned int)random() % time(NULL);
	srandom(lseed);
	int l = ((random() % 1327) + (random() % 3));

	int e = (r % l);

	printf("%d", e);

	return 0;
}
