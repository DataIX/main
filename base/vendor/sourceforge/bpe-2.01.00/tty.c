/* bpe - tty.c */

#include "bpe.h"
#include "global.h"

#include <signal.h>
#include <sys/stat.h>

#ifdef HAVE_SYS_IOCTL_H
# include <sys/ioctl.h>
#endif

/* not sure if this is really necessary */
#ifdef B1200
#  undef B50
#  undef B75
#  undef B110
#  undef B134
#  undef B150
#  undef B200
#  undef B300
#  undef B600
#  undef B1200
#  undef B1800
#  undef B2400
#  undef B4800
#  undef B9600
#  undef EXTA
#  undef EXTB
#endif

#if defined(TIOCGETA) && ! defined(TCGETA)
#  define TCGETA TIOCGETA
#  define TCSETA TIOCSETA
#endif

/* #if !defined(ULTRIX) && !defined(OSF)
#  define termio termios
#  define TCGETA TIOCGETA
#  define TCSETA TIOCSETA
#endif */

#ifdef HAVE_SYS_TERMIOS_H 
#include <sys/termios.h>
#define HAVE_TERMIOS
#endif

/* extern char *ttyname(); */

/* set i/o parameters */

char *dev_tty;

struct stat dbuf;
#ifdef HAVE_TERMIOS
struct termios oflags, nflags;
#else
struct termio oflags, nflags;
#endif

#ifdef SIGWINCH
struct winsize owinflags;
#endif

int setup_tty()

{

	/* get user's default tty state and window size */

#ifdef TCGETS
	if (ioctl(0, TCGETS, &oflags) == -1)
#else
	if (ioctl(0, TCGETA, &oflags) == -1)
#endif
		return(errno);
#ifdef SIGWINCH
	if (ioctl(0, TIOCGWINSZ, &owinflags) == -1)
		return(errno);

/*	(void) ioctl(0, TIOCGWINSZ, &nwinflags); */
#endif

	/* set up the curses stuff - cbreak() (crmode() under BSD
	   curses) is used instead of raw() (which would save much
	   of the hassle with ioctl() calls) because raw() will not
	   allow XON-XOFF which some users might require */

	initscr();			/* initialise std screen */
	cbreak();                       /* set single char input */
	noecho();			/* set no echo (surprise) */
	nonl();				/* unset newline mapping */

#ifdef HAVE_KEYPAD
	if (Opt.keypad_flag == on)
		keypad(stdscr, TRUE);
#endif

	/* get the new "curses-set" tty states, add a few things and,
	   from here on, use ioctl() to turn things on and off */

#ifdef TCGETS
	if (ioctl(0, TCGETS, &nflags) == -1)
#else
	if (ioctl(0, TCGETA, &nflags) == -1)
#endif
		return(errno);

	/* set ISIG off so that INTR and QUIT (and SIGSTP where
	   appropriate) will be treated as normal characters - this is
	   more satisfactory than simply ignoring or trapping them and
	   allows the use of CTRL-C and DEL in the program (CTRL-C or
	   DEL are the most-commonly-used characters for INTR) */

	nflags.c_lflag &= ~(ISIG);
#if defined(IEXTEN)
	/* disable implementation-defined extensions - this is included
	   primarily to stop CTRL-V being trapped before bpe gets it but
	   should also catch any other "funny" stuff that some vendors
	   dream up */

	nflags.c_lflag &= ~(IEXTEN);
#endif
	/* disable XON-XOFF (unless flow-control selected by command-line
	   option or environment-variable) otherwise a CTRL-S would appear
	   to lock the terminal - this assumes, of course, that flow-control
	   has been set by the user for normal use */

	if (Opt.flow_flag == off)
		nflags.c_iflag &= ~(IXON | IXOFF | IXANY);

	/* reset tty and, if lockfile enabled, get rid of any lockfile on
	   SIGINT, SIGTERM, SIGQUIT or SIGHUP */

	(void) signal(SIGINT, sig_exit);
	(void) signal(SIGTERM, sig_exit);
	(void) signal(SIGQUIT, sig_exit);
	(void) signal(SIGHUP, sig_exit);
#ifdef SIGWINCH
	(void) signal(SIGWINCH, SIG_IGN);
#endif
	/* get the current /dev/whatever modes if Opt.msg_flag set off */

	if (Opt.msg_flag == off)
		if (stat(dev_tty = ttyname(0), &dbuf))
			Opt.msg_flag = on;

	/* now set the new "curses-set" and user-set tty states
	   and message status */

	return(set_tty());

}

/* set tty for bpe */

int set_tty()

{

#ifdef TCSETS
	if (ioctl(0, TCSETS, &nflags) == -1)
#else
	if (ioctl(0, TCSETA, &nflags) == -1)
#endif
		return(errno);

	if (Opt.msg_flag == off)
		if (chmod(dev_tty, (int) (dbuf.st_mode & 0700)))
			Opt.msg_flag = on;

	return(0);

}

/* reset tty to original before shelling-out or quitting */

int reset_tty()

{

#ifdef TCSETS
	if (ioctl(0, TCSETS, &oflags) == -1)
#else
	if (ioctl(0, TCSETA, &oflags) == -1)
#endif
		return(errno);

	if (Opt.msg_flag == off)
		if (chmod(dev_tty, (int) (dbuf.st_mode & 0777)))
			Opt.msg_flag = on;

	return(0);

}

/* get/set page size - this is messy but was made necessary partly by some
   of the strange implementations around and partly because of problems
   encountered when running bpe on telnet or rlogin sessions */

int get_pagesize()

{

#ifdef SIGWINCH
	int win_code = 0;
#endif

	/* this really shouldn't be necessary and is almost a waste of
	   space but, sooner or later, some idiot is bound to change the
	   value of MINCOLS or set MINLINES too low in bpe.h */

	if (MINCOLS < 80 || MINLINES < 10)
		{
		(void) fprintf(stderr, "%s: compiled with illegal \
MINCOLS/MINLINES value\n", Ute.bpe_name);
		(void) fflush(stderr);
		return(1);
		}

#ifdef SIGWINCH
	if (owinflags.ws_row < MINLINES || owinflags.ws_col < MINCOLS)
		{
		win_code = 1;
		if (LINES < MINLINES || COLS < MINCOLS)
			{
			(void) sprintf(Ute.errmsg, "window too small - \
minimum %d lines x %d columns", MINLINES, MINCOLS);
			return(1);
			}
		}
#else
	if (LINES < MINLINES || COLS < MINCOLS)
		{
		(void) sprintf(Ute.errmsg, "window too small - \
minimum %d lines x %d columns", MINLINES, MINCOLS);
		return(1);
		}
#endif
	if (Opt.windef_flag == on)
		{
#ifdef SIGWINCH
		if (owinflags.ws_row < 24 || owinflags.ws_col < 80)
#endif
		if (LINES < 24 || COLS < 80)
			{
			(void) sprintf(Ute.errmsg, "window too small for \
default 24-line x 80-column display");
			return(1);
			}

		Ute.edit_chars = 256;
		Ute.edit_lines = 16;
		Ute.bpe_lines = 24;
		}

	else
#ifdef SIGWINCH
		if (win_code == 0)
			{
			Ute.edit_chars = (int) (owinflags.ws_row - 8) * 16;
			Ute.edit_lines = (int) owinflags.ws_row - 8;
			Ute.bpe_lines = (int) owinflags.ws_row;
			}

		else
#endif
		{
		Ute.edit_chars = (LINES - 8) * 16;
		Ute.edit_lines = LINES - 8;
		Ute.bpe_lines = LINES;
		}

	if (!Opt.slines)
		Opt.slines = Ute.edit_lines / 2;

	return(0);

}

/* reset tty and, if lockfile enabled, remove any lockfile on SIGINT,
   SIGTERM, SIGQUIT or SIGHUP */

RETSIGTYPE sig_exit(sig)
int sig;
  
{

	(void) signal(sig, SIG_IGN);

	if (Crnt.fd != -1)
		(void) close(Crnt.fd);
#ifndef NOLOCK
	rm_lock(0);
#endif
	if (sig != SIGHUP)
		(void) reset_tty();

	exit(1);

}
