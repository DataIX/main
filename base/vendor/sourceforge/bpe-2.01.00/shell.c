/* bpe - shell.c */

#include "bpe.h"
#include "global.h"

#include <signal.h>
#ifdef HAVE_SYS_WAIT_H
#include <sys/wait.h>
#endif

/* shell escape - just a simple one with no arguments accepted
   /bin/sh is used if SHELL not set  */

void do_shell()

{

#ifdef NOSHELL
	stl_msg((char *) NULL, (char *) NULL, NOSH, 1);
#else
	WINDOW *tmpscr;

	char *cp, *shell;

	stl_clr(0);
	if ((shell = getenv("SHELL")) == NULL)
		shell = "/bin/sh";

	if (cp = strrchr(shell, '/'))
		cp++;

	else
		cp = shell;

	tmpscr = newwin(LINES, COLS, 0, 0);
	wclear(tmpscr);
	wrefresh(tmpscr);

	/* set tty back to "normal" before forking */

	(void) reset_tty();

	/* and make sure that bpe won't be bombed from the shell */

	(void) signal(SIGINT, SIG_IGN);
	(void) signal(SIGTERM, SIG_IGN);
	(void) signal(SIGQUIT, SIG_IGN);

	if (!fork())
		{
		(void) signal(SIGINT, SIG_DFL);
		(void) signal(SIGTERM, SIG_DFL);
		(void) signal(SIGQUIT, SIG_DFL);
		(void) signal(SIGHUP, SIG_DFL);
#ifdef SIGWINCH
		(void) signal(SIGWINCH, SIG_DFL);
#endif
		(void) execl(shell, cp, (char *) NULL);
		}

	(void) wait((int *) 0);

	/* and now back to all the settings required for bpe */

	(void) signal(SIGINT, sig_exit);
	(void) signal(SIGTERM, sig_exit);
	(void) signal(SIGQUIT, sig_exit);

	(void) set_tty();
	wclear(tmpscr);
	wrefresh(tmpscr);
	delwin(tmpscr);
	touchwin(stdscr);
#endif

}

#ifdef JOBCONTROL
/* function for handling job-control - this is necessary because
   CTRL-Z is treated as a normal character by the shell with the
   tty setting in use */

void do_job()

{

	WINDOW *tmpscr;

	stl_clr(0);
	tmpscr = newwin(LINES, COLS, 0, 0);
	wclear(tmpscr);
	wrefresh(tmpscr);

	/* set tty back to "normal" before sending SIGSTOP */

	(void) reset_tty();

	(void) kill(getpid(), SIGSTOP);

	/* and now back to all the settings required for bpe */

	(void) set_tty();

	wclear(tmpscr);
	wrefresh(tmpscr);
	delwin(tmpscr);
	touchwin(stdscr);

}
#endif
