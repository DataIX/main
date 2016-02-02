/* bpe - lock.c */

#include "bpe.h"
#include "global.h"

#ifdef HAVE_FCNTL_H
# include <fcntl.h>
#endif

#include <sys/errno.h>
#include <sys/stat.h>
#include <signal.h>


#ifndef NOLOCK

char lockfile[PATH_BUFSIZE];

/* check for errors after creating lockfile and remove it if it is not
   required */

void ck_lock(filename)
char *filename;

{

	if (Fil[optind].write_flag == on)
		{
		do_lock(filename);
		if (Opt.lock_flag == off && (Crnt.lock_flag == lock ||
		    Crnt.lock_flag == errlock))
			{
			if (Crnt.lock_flag == lock)
				Crnt.lock_flag = nolock;

			if (unlink(lockfile))
				{
				Crnt.lock_flag = errnolock;
				stl_msg(sys_errlist[errno],
				      (char *) NULL, LFDERR, 1);
				}
			}
		}
}

/* check if lockfile has been created - if not, create it */

void do_lock(filename)
char *filename;

{

	struct lock_id
		{
		unsigned int l_pid;
		char l_pname[16];
		} lock_bpe;

	int c, cmask, fdl;
	unsigned int pid;
	char *cp;

	/* strip path */

	if ((cp = strrchr(filename, '/')) == NULL)
		cp = filename;

	else
		if (*++cp == NUL)
			{
			/* Cannot create lockfile */
			stl_msg((char *) NULL, (char *) NULL, LFCERR, 1);
			Crnt.lock_flag = errnolock;
			return;
			}

	/* check that total lockfile name is not longer than
           the maximum pathlength size */

	if ((c = (strlen(LOCKDIR) + strlen(LOCKPREFIX) + 1)) >= PATHLENGTH)
			{
			/* Cannot create lockfile */
			stl_msg("Lockfile pathname too long", (char *) NULL,
			    LFCERR, 1);
			Crnt.lock_flag = errnolock;
			return;
			}

	if ((c += strlen(cp)) > PATHLENGTH)
		c = c - PATHLENGTH;

	else
		c = 0;

	/* make lockfile path from lockfile directory, lockfile prefix
	   and filename with truncated trailing characters if required
	   to not exceed PATHLENGTH */

	(void) strncat(strcat(strcat(strcpy(lockfile, LOCKDIR), "/"),
	    LOCKPREFIX), cp, strlen(cp) - c);

	cmask = umask(000);

	/* check/create the lockfile */

	if ((fdl = open(lockfile, O_WRONLY | O_CREAT | O_EXCL, 0644)) == -1)
		{
		if (errno == EEXIST)
			{
			Crnt.lock_flag = otherlock;
			Fil[optind].write_flag = off;
			}

		else
			{
			/* Cannot create lockfile */
			stl_msg(sys_errlist[errno], (char *) NULL, LFCERR, 1);
			Crnt.lock_flag = errnolock;
			}

		(void) umask(cmask);
		return;
		}

	pid = lock_bpe.l_pid = getpid();
	(void) strcpy(lock_bpe.l_pname, "bpe");
	if (write(fdl, &lock_bpe, sizeof(struct lock_id)) !=
	    sizeof(struct lock_id))
		{
		/* Cannot write to lockfile */
		stl_msg(sys_errlist[errno], (char *) NULL, LFWERR, 1);
		(void) close(fdl);
		(void) umask(cmask);
		Crnt.lock_flag = errlock;
		return;
		}

	(void) close(fdl);
	(void) umask(cmask);

	/* the operation for creating the lockfile is supposed to be atomic
	   but there have allegedly been documented cases where it has
	   failed so we check to see if this process actually created it
	   (paranoia at its most-rampant ... maybe) */

	if ((fdl = open(lockfile, O_RDONLY)) == -1)
		{
		/* now we're really getting paranoid - if the following
		   error happens the file was created by another process that
		   has just removed it - so we go back and try again */

		if (errno == ENOENT)
			{
			(void) umask(cmask);
			Crnt.lock_flag = nolock;
			do_lock(filename);
			return;
			}

		else
			{
			/* lockfile error */
			stl_msg(sys_errlist[errno], (char *) NULL, LFERR, 1);
			Crnt.lock_flag = errlock;
			return;
			}
		}

	if (read(fdl, &lock_bpe, sizeof(struct lock_id)) !=
	    sizeof(struct lock_id))
		{
		/* Cannot read lockfile */
		stl_msg(sys_errlist[errno], (char *) NULL, LFRERR, 1);
		(void) close(fdl);
		Crnt.lock_flag = errlock;
		return;
		}

	(void) close(fdl);
	Crnt.lock_flag = (lock_bpe.l_pid != pid) ? otherlock : lock;
	return;

}

void rm_lock(status_code)
int status_code;

{

	if (Crnt.lock_flag == lock || Crnt.lock_flag == errlock)
		if (unlink(lockfile) && (status_code))
			stl_msg(sys_errlist[errno], (char *) NULL, LFDERR, 1);

}


#endif
