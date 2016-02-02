/* bpe - backup.c */

#include "bpe.h"
#include "global.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/errno.h>
#include <ctype.h>

/* extern int optind; */

extern struct stat Sbuf;

/* create backup file - filename.bak in current directory */

void do_backup(filename)
char *filename;

{

	int c, d = -1, fdb;
	int cmask;
	char bfile[NAME_BUFSIZE], inbuf[BPE_LARGEBUFSIZE], *cp;

	if (Fil[optind].ftype != S_IFREG)
		{
		stl_msg((char *) NULL, (char *) NULL, FNOTORD, 1);
		return;
		}

	/* strip path */

	if ((cp = strrchr(filename, '/')) == NULL)
		cp = filename;

	else
		cp++;

	if ((strlen(cp) + strlen(Ute.bakext)) > NAMELENGTH)
		{
		stl_msg("Filename/extension too long", (char *) NULL,
		    BUC2ERR, 1);
		Fil[optind].bak_status = errbak;
		return;
		}

	(void) strcat(strcpy(bfile, cp), Ute.bakext);
	if (!access(bfile, 0))
		{
		stl_msg((char *) NULL, bfile, BUEXIST, 1);
		c = confirm(1);
		stl_clr(0);
		reset_dispos();
		if (!c)
			{
			if (Fil[optind].bak_status == nobak)
				Fil[optind].bak_status = notbak;

			return;
			}
		}

	cmask = umask(000);
	if ((fdb = open(bfile, O_WRONLY | O_CREAT | O_TRUNC,
		   (int) (Sbuf.st_mode & 0777))) == -1)
		{
		stl_msg(sys_errlist[errno], bfile, BUC1ERR, 1);
		(void) umask(cmask);
		Fil[optind].bak_status = errbak;
		return;
		}

	if (lseek(Crnt.fd, 0L, 0) == -1)
		{
		stl_msg(sys_errlist[errno], (char *) NULL, LSERR, 1);
		(void) umask(cmask);
		(void) close(fdb);
		Fil[optind].bak_status = errbak;
		return;
		}

	stl_msg((char *) NULL, bfile, BUCRING, 0);
	while ((c = read(Crnt.fd, (char *) inbuf, 256)) > 0)
		if ((d = write(fdb, (char *) inbuf, (unsigned) c)) != c)
			break;

	if (c < 0 || d < 0)
		stl_msg(sys_errlist[errno], (char *) NULL,
		      (c < 0) ? RDERR : WRERR, 1);

	(void) close(fdb);

	/* reset the mod-time of the backup file to that of the
	   current file */

	Fil[optind].bak_status = (set_times(bfile)) ? errbak : bak;
	(void) umask(cmask);
	stl_msg((char *) NULL, bfile, BUCRE, 0);

}

/* check for illegal chars in filename or backup-file extension */

int ck_chars(filename, code)
char *filename;
int code;

{

	int c = 0;
	char *cp;

	if (!code)
		for (cp = filename; *cp; cp++)
			if (*cp < 0x20 || *cp > 0x7e)
				{
				c = 1;
				break;
				}

	if (c || strpbrk(filename, BADCHARS))
		{
		if (code)
			/* illegal filename message */
			stl_msg((char *) NULL, filename, ILLFNAM, 1);

		return(1);
		}

	return(0);

}

/* write (part of) the current file to a new file */

void wrtofile(argf)
char **argf;

{

	static char last_file[PATHLENGTH], last_start[BPE_SMALLBUFSIZE],
		    last_fin[BPE_SMALLBUFSIZE];

	int c;
	off_t end, start;
	char inbuf[BPE_SMALLBUFSIZE];

	flag1 trunc_flag = on;

	if (Fil[optind].ftype != S_IFREG)
		{
		stl_msg((char *) NULL, (char *) NULL, FNOTORD, 1);
		return;
		}

	for (;;)
		{
		if (get_entry(inbuf, last_file, "New file name", NOPFNAM))
			return;

		if (ck_chars(last_file, 1))
			continue;

		/* if file exists give choice of replacing, appending or
		   quitting unless file is current file - the current-file
		   check is just a simple one and won't defeat anyone who
		   is determined to overwrite it - it's just meant to guard
		   against complete bozos */

		c = -1;
		if (!access(last_file, 0))
			{
			if ((c = ck_flist(last_file, argf)) == optind)
				{
				/* current-filename message */
				stl_msg((char *) NULL, last_file, FCURR, 1);
				continue;
				}

			stl_msg((char *) NULL, last_file, (c != -1) ?
			    FINLST : FEXIST, 1);

			switch (tolower(confirm(2)))
				{
				case 'a':
					trunc_flag = off;

				case 'r':
					break;

				default:
					stl_clr(0);
					continue;
/*					return; */
				}
			}

		break;
		}

	if (Ute.statline_flag == on)
		stl_clr(0);

	for (;;)
		{
		if ((start = get_address("Start address", last_start, 15, 15))
		    == -1 || (end = get_address("End address", last_fin,
		    13, 15)) == -1)
			return;

		if (start > end)
			{
			/* illegal-address-order message */
			stl_msg((char *) NULL, (char *) NULL, ILLADO, 1);
			continue;
			}

		break;
		}

	/* if file to be written to is one that is in file-list we force
	   resetting of all parameters in case it has already been accessed
	   for editing in this session */

	if (c != -1)
		Fil[c].status = 2;

	wfile(last_file, start, end, trunc_flag);

}

void wfile(filename, start, end, trunc_flag)
char *filename;
off_t start, end;
flag1 trunc_flag;

{

	int cmask, fdf;
	off_t c, d, e, blen, llen, length;
	char inbuf[BPE_LARGEBUFSIZE];

	length = (end - start) + 1;
	blen = length / 256;
	llen = length % 256;

	clr_command();
	cmask = umask(000);
	if ((fdf = open(filename, (trunc_flag == off) ?
		   O_WRONLY | O_APPEND : O_WRONLY | O_CREAT | O_TRUNC,
		   (~cmask) & 0666)) == -1)
		{
		stl_msg(sys_errlist[errno], filename, (trunc_flag == off) ?
		       FAERR : FCERR, 1);
		(void) umask(cmask);
		return;
		}

	if (lseek(Crnt.fd, start, 0) == -1)
		{
		stl_msg(sys_errlist[errno], (char *) NULL, LSERR, 1);
		(void) umask(cmask);
		(void) close(fdf);
		return;
		}

	stl_msg((char *) NULL, filename, (trunc_flag == off) ?
	    FAPING : FCRING, 0);
	for (c = d = e = 0; c < blen && d >= 0 && e >= 0; c++)
		if ((d = read(Crnt.fd, (char *) inbuf, 256)) == 256)
			e = write(fdf, (char *) inbuf, (unsigned) 256);

	if (d >= 0 && e >= 0 && llen > 0)
		if ((d = read(Crnt.fd, (char *) inbuf, (unsigned) llen)) ==
		     llen)
			e = write(fdf, (char *) inbuf, (unsigned) llen);

	if (d < 0 || e < 0)
		/* read/write-error message */
		stl_msg(sys_errlist[errno], (char *) NULL,
		      (d < 0) ? RDERR : WRERR, 1);

	else
		/* file-appended/created message */;
		stl_msg((char *) NULL, filename,
		       (trunc_flag == off) ? FAPP : FCRE, 0);

	(void) close(fdf);
	(void) umask(cmask);

}
