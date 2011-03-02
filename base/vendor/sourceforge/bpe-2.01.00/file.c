/* bpe - file.c */

#include "bpe.h"
#include "global.h"

#ifdef HAVE_FCNTL_H
# include <fcntl.h>
#endif

#include <sys/stat.h>
#include <sys/errno.h>
#include <utime.h>

struct stat Sbuf;

/* get next file in arg list */

int next_file(argf)
char **argf;

{

	if (Ute.numfiles == 1)
		stl_msg((char *) NULL, (char *) NULL, NOOFILE, 1);

	else
		if ((optind + 1) == Ute.numfiles)
			stl_msg((char *) NULL, (char *) NULL, NONFILE, 1);

	else
		{
		optind++;
		return(do_file(argf[optind - 1], argf[optind]));
		}

	return(0);

}

/* get previous file in arg list */

int prev_file(argf)
char **argf;

{

	if (Ute.numfiles == 1)
		stl_msg((char *) NULL, (char *) NULL, NOOFILE, 1);

	else
		if ((optind - 1) < 0)
			stl_msg((char *) NULL, (char *) NULL, NOPFILE, 1);

	else
		{
		optind--;
		return(do_file(argf[optind + 1], argf[optind]));
		}

	return(0);

}

/* do actual jump for j and J commands */

int do_jump(jump_code, argf)
int jump_code;
char **argf;

{

	stl_clr(0);

	/* if only two files, toggle between them - "only-one-file" error
	   caught by next_file() and prev_file() - else go to number or
	   name functions according to jump_code */

	return((Ute.numfiles < 3) ?
	    (!optind) ? next_file(argf) : prev_file(argf) :
	    (jump_code == 'J') ? sel_name(argf) : sel_num(argf));

}

/* select file number from arg list for j command */

int sel_num(argf)
char **argf;

{

	int c, d;
	char inbuf[8];

	clr_command();
	printw("File number [1 - %d]: ", Ute.numfiles);
	refresh();

	/* the following call to get_str() allows the entry of up to 6
	   chars even though 2 is the legal maximum - this has been
	   done because with only 2 chars the line editing might be a
	   bit confusing if a char was inserted at the beginning of a
	   2-char string causing an error bell */

	if (get_str(inbuf, (WINDOW *) NULL, 2, (Ute.numfiles > 9) ? 22 : 21,
	    0, 6, 0, 0) || inbuf[0] == NUL || (c = atoi(inbuf)) == optind + 1)
		{
		if (Ute.statline_flag == on)
			stl_clr(0);

		reset_dispos();
		return(0);
		}

	if (c < 1 || c > Ute.numfiles)
		{
		reset_dispos();
		/* illegal file number message */
		stl_msg((char *) NULL, inbuf, ILLFNO, 1);
		return(0);
		}

	d = optind;
	optind = c - 1;
	if (Opt.clear_flag == off)
		clr_command();

	return(do_file(argf[d], argf[optind]));

}

/* select file name from arg list for J command */

int sel_name(argf)
char **argf;

{

	int c, d;
	char inbuf[PATHLENGTH];
	static char last_file[PATHLENGTH];

	if (get_entry(inbuf, last_file, "File name", NOPFNAM))
		{
		reset_dispos();
		return(0);
		}

	if ((c = ck_flist(last_file, argf)) == -1)
		{
		reset_dispos();
		/* no such file message */
		stl_msg((char *) NULL, last_file, NOSFILE, 1);
		return(0);
		}

	if (c == optind)
		{
		reset_dispos();
		return(0);
		}

	d = optind;
	if (Opt.clear_flag == off)
		clr_command();

	return(do_file(argf[d], argf[optind = c]));

}

/* add file name to file list and optionally edit */

int sel_new(edit_code, argf)
int edit_code;
char **argf;

{

	int c;
	char inbuf[PATHLENGTH];
	static char last_file[PATHLENGTH];

	if (Ute.numfiles == MAXFILES)
		{
		/* file list full message */
		stl_msg((char *) NULL, (char *) NULL, FULLLST, 1);
		return(2);
		}

	stl_clr(0);
	if (get_entry(inbuf, last_file, "File name", NOPFNAM))
		return(1);

	if ((c = ck_flist(last_file, argf)) != -1)
		{
		/* file current file/already in list message */
		stl_msg((char *) NULL, last_file,
		    (c == optind) ? FCURR : FINLST, 1);
		return(1);
		}

	if (access(last_file, 4))
		{
		/* no such file/file not accessible messages */
		if (errno == ENOENT)
			stl_msg((char *) NULL, last_file, NOSFILE, 1);

		else
			stl_msg(sys_errlist[errno], last_file, ACCERR, 1);

		return(1);
		}

	if (add_farray(argf, last_file))
		return(1);

	(void) strcpy(argf[Ute.numfiles++], last_file);
	if (!edit_code)
		{
		dispheader(0, argf[optind]);
		/* file added to list message */
		stl_msg((char *) NULL, last_file, FADLST, 0);
		return(0);
		}

	c = optind;
	if (Opt.clear_flag == off)
		clr_command();

	return(do_file(argf[c], argf[optind = Ute.numfiles - 1]));

}

/* toggle between last-two-accessed-files */

int do_toggle(argf)
char **argf;

{

	if (Ute.numfiles == 1)
		{
		stl_msg((char *) NULL, (char *) NULL, NOOFILE, 1);
		return(0);
		}

	if (Ute.tfile1 == Ute.tfile2)
		{
		stl_msg((char *) NULL, (char *) NULL, NOPFILE, 1);
		return(0);
		}

	return(do_file(argf[Ute.tfile2], argf[optind = Ute.tfile1]));

}

/* select a file by number from the displayed file list for the F command */

int sel_file(argf)
char **argf;

{

	int cury, curx, col1, col2, c = 0, d;
	char inbuf[8];

	char *f_mesg = "nter file number or <RETURN> to exit:";

	WINDOW *filscr;

	if (Ute.numfiles == 1)
		{
		stl_msg((char *) NULL, (char *) NULL, NOOFILE, 1);
		return(0);
		}

/*	filscr = newwin(0, 0, 0, 0); */
	filscr = newwin(LINES, COLS, 0, 0);
	wclear(filscr);
	wrefresh(filscr);
	mvwprintw(filscr, 0, 5, "File list [1 - %d]:", Ute.numfiles);
	if ((col1 = (Ute.numfiles / 3)) < 1)
		{
		col1 = 1;
		col2 = 2;
		}

	else
		{
		col2 = col1 * 2;
		switch (Ute.numfiles % 3)
			{
			case 2:
				col2++;

			case 1:
				col1++;
				col2++;

			default:
				break;
			}
		}

	dispflist(&c, 2, 5, col1, argf, filscr);
	dispflist(&c, 2, 30, col2, argf, filscr);
	dispflist(&c, 2, 55, Ute.numfiles, argf, filscr);
	mvwprintw(filscr, cury = col1 + 3, 5, "E%s", f_mesg);
	wrefresh(filscr);
	curx = 44;
	for (;;)
		{
		/* the following call to get_str() allows the entry of up to 6
		   chars even though 2 is the legal maximum - this has been
		   done because with only 2 chars the line editing might be a
		   bit confusing if a char was inserted at the beginning of a
		   2-char string causing an error bell */

		if (get_str(inbuf, filscr, cury, curx, 0, 6, 0, 0) ||
		    inbuf[0] == NUL || inbuf[0] == 'q' ||
		    (c = atoi(inbuf)) == optind + 1)
			{
			wclear(filscr);
			wrefresh(filscr);
			delwin(filscr);
			touchwin(stdscr);
			return(0);
			}

		if (c < 1 || c > Ute.numfiles)
			{
			curx = 61;
			wmove(filscr, cury, 0);
			wclrtoeol(filscr);
			mvwprintw(filscr, cury, 5, "Illegal number - e%s",
			    f_mesg);
			(void) beep();
			wrefresh(filscr);
			continue;
			}

		break;
		}

	d = optind;
	optind = c - 1;
	wclear(filscr);
	wrefresh(filscr);
	delwin(filscr);
	touchwin(stdscr);
	return(do_file(argf[d], argf[optind]));

}

/* display numbered file list */

void dispflist(c, cury, curx, filnum, argf, filscr)
int *c, cury, curx, filnum;
char **argf;
WINDOW *filscr;

{

	int len, nlen;
	char *cptr, *sptr;

	for (; *c < filnum; (*c)++, cury++)
		{
		/* handle file-names longer than 19 characters - if there are
		   more than 19, show just last 18 to allow for leading > but
		   if highlighting turned off show just last 16 for current
		   file to allow for [] markers */

		cptr = ((len = strlen(argf[*c])) > (nlen = (*c != optind) ?
		    19 : (Opt.high_flag == on) ? 19 : 17)) ?
		    &argf[*c][len - (nlen - 1)] : argf[*c];

		sptr = (len > nlen) ? ">" : "";
		mvwprintw(filscr, cury, curx, "%02d - ", *c + 1);
		if (*c == optind)
			if (Opt.high_flag == on)
				{
				wstandout(filscr);
				wprintw(filscr, "%s%s", sptr, cptr);
				wstandend(filscr);
				}

			else
				wprintw(filscr, "%s[%s]", sptr, cptr);

		else
			wprintw(filscr, "%s%s", sptr, cptr);
		}

}

/* this is the primary interface between command.c and get_file() */

int do_file(old_filename, new_filename)
char *old_filename, *new_filename;

{

	if (Opt.clear_flag == on)
		{
		clear();
		refresh();
		}

	if (Crnt.fd != -1)
		if (finish(old_filename, 1))
			return(1);

	if (get_file(new_filename))
		dispos();

	return(0);

}

/* get the requested file */

int get_file(filename)
char *filename;

{

	Crnt.ed.fmod_flag = Crnt.ed.fmod_count = Crnt.ed.write_count =
	Curset.cury = Curset.curx = Curset.edlen = Curset.rows =
	Curset.last_row = 0;
	Curset.fpos = 0L;
	Crnt.ed.bmod_flag = Crnt.ed.hmod_flag = off;
#ifndef NOLOCK
	Crnt.lock_flag = nolock;
#endif
	/* set everything up for file if it has not yet been accessed in
	   current bpe session or if file has been accessed but has been
	   written-to using the W command */

	if (Fil[optind].status != 1)
		{
		/* the following setting-to-zero is necessary not only for
		   already-accessed files that have been written-to as above
	           but also because I've already come across one compiler
		   that doesn't do things properly ... */

		Fil[optind].ftype = 0;
		Fil[optind].slen = Fil[optind].srch.slen = 0;
		Fil[optind].sfpos = Fil[optind].xed.sfpos =
		Fil[optind].srch.fpos = Fil[optind].srch.sfpos = 0L;
		Fil[optind].search_flag = Fil[optind].hsearch_flag =
		Fil[optind].srch.search_flag = off;

		/* these settings, however, are ALWAYS necessary */

		Fil[optind].lfpos = Fil[optind].xed.fpos = -1L;
		if (!Fil[optind].status)
			Fil[optind].bak_status = nobak;

		Crnt.fd = get_stat(filename);
		dispheader(1, filename);
		init_marks();
		if (Crnt.fd == -1)
			return(1);

		if (Opt.offset < Fil[optind].flength && Opt.offset > 0L)
			{
			Fil[optind].fpos = Opt.offset;
			pos_opts();
			}

		else
			Fil[optind].fpos = 0L;

		Fil[optind].status = 1;
		}

/*	if (Fil[optind].search_flag == on && Fil[optind].hsearch_flag == off &&
	    Opt.high_flag == on)
		Fil[optind].hsearch_flag = on; */
		
	Fil[optind].write_flag = (Fil[optind].ftype != S_IFREG ||
	     Opt.readonly_flag == on || access(filename, 02)) ? off : on;

	dispheader(1, filename);

#ifndef NOLOCK
	ck_lock(filename);
#endif
	if ((Crnt.fd = open(filename, (Fil[optind].write_flag == off) ?
		  O_RDONLY : O_RDWR )) == -1)
		{
		Fil[optind].ftype = 0;
		dispheader(1, filename);
		stl_msg(sys_errlist[errno], filename, OPERR, 1);
		return(1);
		}

	if (optind != Ute.tfile2)
		{
		Ute.tfile1 = Ute.tfile2;
		Ute.tfile2 = optind;
		}

	if (dump((Opt.high_flag == on &&
	    Fil[optind].hsearch_flag == on) ? 1 : 0) == -1)
		{
		Fil[optind].ftype = 0;
		dispheader(1, filename);
		return(1);
		}

#ifndef NOLOCK
	if (Opt.backup_flag == on && Fil[optind].write_flag == on &&
	    Fil[optind].bak_status == nobak && Crnt.lock_flag != errnolock &&
	    Crnt.lock_flag != errlock)
#else
	if (Opt.backup_flag == on && Fil[optind].write_flag == on &&
	    Fil[optind].bak_status == nobak)
#endif
		do_backup(filename);

#ifndef NOLOCK
	if (Opt.readonly_flag == off && Fil[optind].write_flag == off)
		stl_msg((Crnt.lock_flag == otherlock) ?
		        "Already open for editing" :
			(char *) NULL, filename, RDOINF, 1);
#else
	if (Opt.readonly_flag == off && Fil[optind].write_flag == off)
		stl_msg((char *) NULL, filename, RDOINF, 1);
#endif

	return(0);

}

/* clean-up - close current file and remove lockfile */

int finish(filename, quitcode)
char *filename;
int quitcode;

{

	if (Crnt.fd == -1)
		return(0);

	/* the following message is stored in errmsg for sending to stderr
	   AFTER the screen has been cleared on forced exit */

	if (close(Crnt.fd) == -1)
		{
		(void) sprintf(Ute.errmsg, "cannot close: \"%s\" - %s",
			       filename, sys_errlist[errno]);
		return(1);
		}

	if (Crnt.ed.fmod_flag && !Crnt.ed.fmod_count && quitcode)
		(void) set_times(filename);
#ifndef NOLOCK
	rm_lock(1);
#endif
	return(0);

}

/* get file stats (owner, mod-time, size, file-type) and set up starting
   position of last page */

int get_stat(filename)
char *filename;

{

	int c;

#ifdef LARGE_OFF_T			  /* this OS has off_t as 'long long' */
	Fil[optind].flen_isll_flag = off;    /* assume flength is only 'long' */
#endif

	if (stat(filename, &Sbuf))
		{
		stl_msg(sys_errlist[errno], filename, STERR, 1);
		Fil[optind].flength = -1L;
		return(-1);
		}

	Fil[optind].ftype = (Sbuf.st_mode & S_IFMT);
	if ((Fil[optind].flength = Sbuf.st_size) < 1)
		{
		stl_msg((char *) NULL, filename, MTFINF, 1);
		return(-1);
		}

#ifdef LARGE_OFF_T
	/* if length of this file is greater than a 'long's-worth of bytes,
	 * different formatting will need to be used */
	if ((unsigned long) (Fil[optind].flength >> 32) > 0 )
		Fil[optind].flen_isll_flag = on;
#endif

	if ((c = Sbuf.st_size % (off_t) Ute.edit_chars) == 0)
		c = Ute.edit_chars;

	Fil[optind].lpage = Sbuf.st_size - c;
/*	Fil[optind].ftype = (Sbuf.st_mode & S_IFMT); */
	return(0);

}

/* reset modification-time if file has been written to but all mods
   have been undone and set modification-time of backup file to that
   of original */

int set_times(filename)
char *filename;

{

	unsigned short uid;

	struct stat tsbuf;
	struct utimbuf
#ifdef NEED_STRUCT_UTIMBUF
		{
		time_t actime;
		time_t modtime;
		}
#endif
		tbuf;

	if (stat(filename, &tsbuf))
		{
		stl_msg(sys_errlist[errno], filename, STERR, 1);
		return(1);
		}

	/* utime() can be used under UNIX to reset the modification time
	   of the file in this manner only by the owner of the file or
	   the super-user even though the file can be written to by any
	   user.

	   It would, therefore, fail without this check but we don't
	   want an error message from such circumstances */

	if ((uid = (unsigned short) getuid()) != tsbuf.st_uid && uid)
		return(0);

	tbuf.modtime = Sbuf.st_mtime;
	tbuf.actime = tsbuf.st_atime;
	if (utime(filename, &tbuf))
		{
		stl_msg(sys_errlist[errno], filename, TIMERR, 1);
		return(1);
		}

	return(0);

}

/* check if there has been a file error */

int ck_file(filename)
char *filename;

{

	if (Fil[optind].flength == 0)
		{
		stl_msg((char *) NULL, filename, MTFINF, 1);
		return(1);
		}

	if (Crnt.fd == -1)
		{
		stl_msg((char *) NULL, filename, ACCERR, 1);
		return(1);
		}

	if (Ute.statline_flag == on)
		stl_clr(1);

	return(0);

}

/* check if filename in arglist */

int ck_flist(filename, argf)
char *filename, **argf;

{

	int c;

	for (c = 0; c < Ute.numfiles; argf++, c++)
		if (!strcmp(filename, *argf))
			return(c);

	return(-1);

}

/* check if file has been opened for read-only */

int ck_rd()

{

	if (Fil[optind].write_flag == on)
		{
		if (Ute.statline_flag == on)
			stl_clr(1);

		return(0);
		}

	stl_msg((char *) NULL, (char *) NULL, RDMODE, 1);
	return(1);

}
