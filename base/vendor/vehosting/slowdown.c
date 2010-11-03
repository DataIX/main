
/*
 * Copyright (c) 2010,
 *      Daan Vreeken <Daan @ Vitsch . nl> - Vitsch Electronics
 *      All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS `AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * This tiny library wrapper can be used to limit the disk I/O pressure caused
 * by applications that call *stat() in a loop, like 'rsync' or 'ls -R'.
 * The application wraps libc's *stat() calls and inserts usleep() calls now
 * and then.
 *
 * It can be compiled with :
 *	gcc -Wall -fPIC -shared -o slowdown.so slowdown.c
 *
 * To use it start a random application like :
 *	(
 *		export LD_PRELOAD=slowdown.so
 *		export LD_LIBRARY_PATH=.:${LD_LIBRARY_PATH}
 *		ls -R /a/random/huge/directory/
 *	)
 *
 * or
 *	(
 *		export LD_PRELOAD=slowdown.so
 *		export LD_LIBRARY_PATH=.:${LD_LIBRARY_PATH}
 *		rsync
 *	)
 *
 * These examples assume that the library is installed in the current
 * working directory. If this is not the case, change 'LD_LIBRARY_PATH'
 * accordingly.
 *
 */



#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


struct stat;
int _stat(const char *path, struct stat *sb);
int _fstat(int fd, struct stat *sb);
int _lstat(const char *path, struct stat *sb);


void tiny_pause(void)
{
	struct timeval		tv;

	// only allow work to be done during the first half of every second
	gettimeofday(&tv, NULL);
	if (tv.tv_usec > 500 * 1000) {
		// sleep the rest of this second
		usleep(1000 * 1000 - tv.tv_usec);
	}
}


int stat(const char *path, struct stat *sb)
{
	tiny_pause();
	return _stat(path, sb);
}

int fstat(int fd, struct stat *sb)
{
	tiny_pause();
	return _fstat(fd, sb);
}

int lstat(const char *path, struct stat *sb)
{
	tiny_pause();
	return _lstat(path, sb);
}
      
      

