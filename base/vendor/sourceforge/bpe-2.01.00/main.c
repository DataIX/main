
/* bpe - main.c */

#include "bpe.h"
#include "global.h"

#include <ctype.h>


#ifndef lint
	static char *sccsid[] = 
		{
 		"@(#)Copyright (C) 1991/1992/1993/1994/1995/1996/1997/1998/1999 MSM",
 		"@(#) bpe 2.00.08(r) 99/12/11 (ran)"
		};
#endif

int main(argc, argv)
int argc;
char **argv;

{

	extern int optind;
	extern char *optarg;

	int count, errflag = 0, ttyflag = 0, option;
	char *argf[MAXFILES], *bpe_env, *buptmp, *term, *ptr;
	char otmp[16];

	Opt.tpos = Opt.slines = Ute.tfile1 = Ute.tfile2 = 0;
	Opt.offset = 0L;
	Opt.align_flag = Opt.backup_flag = Opt.clear_flag = Opt.flow_flag =
	Opt.readonly_flag = Opt.windef_flag = off;
	Opt.cursor_flag = Opt.esc_flag = Opt.generr_flag = Opt.inperr_flag =
	Opt.high_flag = Opt.msg_flag = Opt.add_flag = on;
#ifdef HAVE_KEYPAD
	Opt.keypad_flag = on;
#endif
#ifndef NOLOCK
	Opt.lock_flag = on;
#endif
	if ((Ute.bpe_name = strrchr(*argv, '/')) != NULL)
		Ute.bpe_name++;

	else
		Ute.bpe_name = *argv;

	Ute.errmsg[0] = Ute.bakext[0] = NUL;
	Ute.statline_flag = off;
	buptmp = Ute.bakext;

	/* a "quick-and-dirty check" 'cos curses often doesn't handle
	   this well - this will catch the two most-obvious errors and is
	   necessary as bpe is likely to be installed on systems where
	   inexperienced users will run anything in sight */

	if ((term = getenv("TERM")) == NULL || !strcmp(term, "dumb") ||
	    !strcmp(term, "unknown"))
		{
		(void) fprintf(stderr, "%s: insufficient \
terminal-capabilities\n", Ute.bpe_name);
		(void) fflush(stderr);
		return(EXIT_FAILURE);
		}

	if (!isatty(0) || !isatty(1))
		{
		(void) fprintf(stderr, "%s: illegal I/O redirection\n",
		    Ute.bpe_name);
		(void) fflush(stderr);
		return(EXIT_FAILURE);
		}

	if (NAMELENGTH > PATHLENGTH)
		{
		(void) fprintf(stderr, "%s: compiled with illegal \
NAMELENGTH/PATHLENGTH values\n", Ute.bpe_name);
		(void) fflush(stderr);
		return(EXIT_FAILURE);
		}

	/* getopt.c is available for systems that do not have it as
	   part of the standard library - see Makefile */

#ifndef NOLOCK
#ifdef HAVE_KEYPAD
	while ((option = getopt(argc, argv, "abcdeEhHkKLmrvVwxyB:l:o:s:"))
	    != EOF)
#else
	while ((option = getopt(argc, argv, "abcdeEhHkLmrvVwxyB:l:o:s:"))
	    != EOF)
#endif
#else
#ifdef HAVE_KEYPAD
	while ((option = getopt(argc, argv, "abcdeEhHkKmrvVwxyB:l:o:s:"))
	    != EOF)
#else
	while ((option = getopt(argc, argv, "abcdeEhHkmrvVwxyB:l:o:s:"))
	    != EOF)
#endif
#endif
		switch (option)
			{
			case 'a':	/* address to top of window */
				Opt.align_flag = on;
				break;

			case 'b':	/* automatic backups */
				Opt.backup_flag = on;
				break;

			case 'B':	/* backup-file extension */
				if (strlen(optarg) > 15)
					return(usage(1));

				(void) strcpy(Ute.bakext, optarg);
				break;

			case 'c':	/* cursor to top of window */
				Opt.cursor_flag = off;
				break;

			case 'd':	/* clear display between files */
				Opt.clear_flag = on;
				break;

			case 'e':	/* command/file error-bell off */
				Opt.generr_flag = off;
				break;

			case 'E':	/* input/edit error-bell off */
				Opt.inperr_flag = off;
				break;

			case 'h':	/* highlighting off */
				Opt.high_flag = off;
				break;

			case 'H':	/* option list */
				return(opt_help());

			case 'k':	/* disable ESC as escape key */
				Opt.esc_flag = off;
				break;
#ifdef HAVE_KEYPAD
			case 'K':	/* disable keypad */
				Opt.keypad_flag = off;
				break;
#endif
			case 'l':	/* set window position */
				if ((Opt.tpos = atoi(optarg)) < 1
				     || Opt.tpos > 15)
					return(usage(1));

				break;
#ifndef NOLOCK
			case 'L':	/* disable file-locking */
				Opt.lock_flag = off;
				break;
#endif
			case 'm':	/* disable message-receive */
				Opt.msg_flag = off;
				break;

			case 'o':	/* set start offset */
				if ((Opt.offset = strtol(optarg, &ptr, 0)) < 0
				     || *ptr)
					return(usage(1));

				break;

			case 'r':	/* open files in read-only mode */
				Opt.readonly_flag = on;
				break;

			case 's':	/* set number of scroll lines */
				if ((Opt.slines = atoi(optarg)) < 1)
					return(usage(1));

				break;

			case 'v':	/* show version/date */
			case 'V':	/* show version/date and defaults */
				return(show_ver(option));

			case 'w':	/* use 24 x 80 display */
				Opt.windef_flag = on;
				break;

			case 'x':	/* enable XON/XOFF */
				Opt.flow_flag = on;
				break;

			case 'y':	/* disable file-position display
					   during edit and search */
				Opt.add_flag = off;
				break;

			case '?':
				errflag++;
				break;
			}

	if (errflag || (Ute.numfiles = argc - optind) < 1)
		return(usage(1));

	/* getopt() leaves things in a rather "messy" state so we'll set
	   argv so that argv[0] is the first file and then set optind to 0 -
	   if nothing else, it makes it easier to keep count */

	argv += optind;
	optind = 0;

	if (Ute.numfiles > MAXFILES)
		{
		(void) fprintf(stderr, "%s: too many files in arg list \
(%d) - maximum: %d\n", Ute.bpe_name, Ute.numfiles, MAXFILES);
		(void) fflush(stderr);
		return(EXIT_FAILURE);
		}

	/* inital "quick check" if there's only one file in arglist to
	   avoid most of the setting-up if the file's inaccessible */

	if (Ute.numfiles == 1 && access(argv[optind], 4))
		{
		(void) fprintf(stderr, "%s: cannot access: \"%s\" - %s\n",
		    Ute.bpe_name, argv[optind], sys_errlist[errno]);
		(void) fflush(stderr);
		return(EXIT_FAILURE);
		}

	/* check the BPE environment variable for user-selected, run-time
	   defaults */

	/* errflag = 0; */
	if ((bpe_env = getenv("BPE")) != NULL)
		while (*bpe_env)
			{
			switch (*bpe_env)
				{
				case 'a':    /* address to top of window */
					Opt.align_flag =
					    (Opt.align_flag == off) ? on : off;
					break;

				case 'b':    /* automatic backups */
					Opt.backup_flag =
					    (Opt.backup_flag == off) ? on : off;
					break;

				case 'B':    /* backup-file extension */
					bpe_env++;
					count = 0;
					if (!*buptmp)
						{
						while (*bpe_env &&
						       *bpe_env != ' ' &&
						       count++ < 15)
							*buptmp++ = *bpe_env++;

						*buptmp = NUL;
						}

					else
						while (*bpe_env &&
						       *bpe_env != ' ')
							bpe_env++;

					continue;

				case 'c':    /* cursor to top of window */
					Opt.cursor_flag =
					    (Opt.cursor_flag == on) ? off : on;
					break;

				case 'd':    /* clear display between files */
					Opt.clear_flag =
					    (Opt.clear_flag == on) ? off : on;
					break;

				case 'e':    /* command/file error-bell off */
					Opt.generr_flag =
					    (Opt.generr_flag == on) ? off : on;
					break;

				case 'E':    /* input/edit error-bell off */
					Opt.inperr_flag =
					    (Opt.inperr_flag == on) ? off : on;
					break;

				case 'h':    /* highlighting off */
					Opt.high_flag = (Opt.high_flag == on) ?
					    off : on;
					break;

				case 'k':    /* disable ESC as escape key */
					Opt.esc_flag = (Opt.esc_flag == on) ?
					    off : on;
					break;
#ifdef HAVE_KEYPAD
				case 'K':    /* disable keypad */
					Opt.keypad_flag =
					    (Opt.keypad_flag == on) ? off : on;
					break;
#endif
				case 'l':    /* set window position */
					if (Opt.tpos)
						break;

					if ((Opt.tpos = atoi(++bpe_env)) < 1
					     || Opt.tpos > 15)
						errflag++;

					while (isdigit(*bpe_env))
						bpe_env++;

					continue;
#ifndef NOLOCK
				case 'L':    /* disable file-locking */
					Opt.lock_flag = (Opt.lock_flag == on) ?
					    off : on;
					break;
#endif
				case 'm':    /* disable message-receive */
					Opt.msg_flag = (Opt.msg_flag == on) ?
					    off : on;
					break;

				case 'o':    /* set start offset */
					bpe_env++;
					count = 0;
					if (!Opt.offset)
						{
						while (*bpe_env &&
						       *bpe_env != ' ' &&
						       count < 15)
							otmp[count++] =
							    *bpe_env++;

						otmp[count] = NUL;
						if ((Opt.offset =
						     strtol(otmp, &ptr, 0))
						     < 0 || *ptr)
							errflag++;
						}

					else
						while (*bpe_env &&
						       *bpe_env != ' ')
							bpe_env++;

					continue;

				case 'r':    /* open files in read-only mode */
					if (Opt.readonly_flag == on)
						Opt.readonly_flag = off;

					else
						Opt.readonly_flag = on;

					break;

				case 's':    /* set number of scroll lines */
					if (Opt.slines)
						break;

					if ((Opt.slines = atoi(++bpe_env)) < 1)
						errflag++;

					while (isdigit(*bpe_env))
						bpe_env++;

					continue;

				case 'w':    /* use 24 x 80 display */
					Opt.windef_flag =
					    (Opt.windef_flag == on) ? off : on;
					break;

				case 'x':    /* enable XON/XOFF */
					Opt.flow_flag =
					    (Opt.flow_flag == off) ? on : off;
					break;

				case 'y':    /* disable file-position display
						during edit and search */
					Opt.add_flag =
					    (Opt.add_flag == off) ? on : off;
					break;

				case ' ':
					break;

				default:
					errflag++;
					break;
				}

			bpe_env++;
			}

	if (errflag)
		{
		(void) fprintf(stderr, "%s: illegal BPE environment \
variable entry\n", Ute.bpe_name);
		(void) fflush(stderr);
		return(EXIT_FAILURE);
		}

	if (Opt.esc_flag == on)
		{
		(void) strncpy(Ute.cancelstr, CANCELSTR, 8);
		Ute.xcancel = CANCEL_3;
		}

	else
		{
		(void) strncpy(Ute.cancelstr, CANCELOPT, 8);
		Ute.xcancel = CANCEL_4;
		}

	/* set-up backup extension if not already set via the -B
	   command-line option */

	if (Ute.bakext[0] == NUL)
		(void) strcpy(Ute.bakext, BAKEXT);

	/* check for dis-allowed characters (as defined in bpe.h) in
	   the backup-file extension */

	if (ck_chars(Ute.bakext, 0))
		{
		(void) fprintf(stderr, "%s: illegal backup-file \
extension - \"%s\"\n", Ute.bpe_name, Ute.bakext);
		(void) fflush(stderr);
		return(EXIT_FAILURE);
		}

	if (Opt.tpos)
		Opt.tpos--;

	if (set_structs())
		return(EXIT_FAILURE);

	set_farrays(argv, argf);

	/* set tty as required for curses and turn off XON/XOFF flow
	   control unless selected */

	if ((ttyflag = setup_tty()) != 0)
		{
		(void) fprintf(stderr, "%s: ioctl error - %s\n",
		    Ute.bpe_name, sys_errlist[ttyflag]);
		(void) fflush(stderr);
		return(EXIT_FAILURE);
		}

	/* get the terminal window size, allocate the buffers and call
	   the main user interface - then clear screen unless get_pagesize(),
           set_buffers() or command() has returned other than 0 */ 

	if ((errflag = get_pagesize()) == 0)
		if ((errflag = set_buffers()) == 0)
			if ((errflag = command(argf)) == 0)
				{
				clear();
				refresh();
				}

	endwin();
	(void) fflush(stdout);

	/* set tty back to its original state */

	(void) reset_tty();

	/* if get_pagesize(), set_buffers() or command() has returned
	   other than 0 and if there's something stored in errmsg send
	   its contents to stderr */

	if (errflag && Ute.errmsg[0])
		{
		(void) fprintf(stderr, "\n\07%s: %s\n", Ute.bpe_name,
		    Ute.errmsg);
		(void) fflush(stderr);
		}

	return(errflag);

}
