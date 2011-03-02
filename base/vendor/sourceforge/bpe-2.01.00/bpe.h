/* bpe.h */

#include "config.h"

/* function declarations/prototypes for all external functions (showing
   the files where they are defined). These prototypes are for Standard
   C compilers and serve as useful declarations for Classic C compilers. */

/* __P is defined for us in sys/cdefs.h on some systems (freebsd) */
#ifdef HAVE_SYS_CDEFS_H
# include <sys/cdefs.h>
#endif

#ifdef __STDC__
#ifndef __P
#define __P(s) s
#endif
#else
#define __P(s) ()
#endif


#ifdef HAVE_STRING_H
# include <string.h>
#else
# include <strings.h>
#endif

#ifndef STDC_HEADERS
# ifndef HAVE_STRCHR
#  define strchr index
#  define strrchr rindex
# endif
  char *strchr (), *strrchr ();
# ifndef HAVE_MEMCPY
#  define memcpy(d, s, n)  bcopy ((s), (d), (n))
#  define memmove(d, s, n) bcopy ((s), (d), (n))
#  define memcmp(a, b, c)  bcmp(b, a, c)
# endif
#endif

#include <stdlib.h>
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif

#include <signal.h>

#ifdef HAVE_NCURSES
# ifdef HAVE_NCURSES_H
#  include <ncurses.h>
# else 
#  ifdef HAVE_NCURSES_CURSES_H
#   include <ncurses/curses.h>
#  else
#   include <curses.h>
#  endif
# endif
#else
# include <curses.h>
#endif

#include <errno.h>

#ifdef NEED_SYS_ERRLIST
   /* extern int errno; */
   extern char *sys_errlist[];
#endif

#if (SIZEOF_OFF_T > SIZEOF_LONG) 
# define LARGE_OFF_T
#endif

#include <sys/types.h>


/* Is job control available on this platform? */
#ifdef SIGSTOP
#define JOBCONTROL
#endif

#ifdef NOJOBCONTROL
#undef JOBCONTROL
#endif



/* this is for testing purposes only */
#ifdef DEBUG
#ifdef SIGWINCH
#undef SIGWINCH
#endif
#endif

/* Flag1 is used MAINLY for indicating the status of options settable from
   the command-line or via the BPE environment variable */

typedef enum Flag1 {off, on} flag1;

#ifndef NOLOCK
/* Flag2 is used for indicating lockfile status - the meanings of the
   first two values are obvious, while the third indicates that a file
   was forced to read-only status because a lockfile already existed.
   The fourth value indicates an error creating the lockfile and the
   fifth indicates that the lockfile was probably created but that
   there was an error writing to or reading from it (an error which
   is not really likely to arise). */

typedef enum Flag2 {nolock, lock, otherlock, errnolock, errlock} flag2;
#endif

/* Flag3 is used for checking backup-file status - the initialised
   state of nobak indicates that no backup has been made, bak that a
   backup has been made, notbak that one is not wanted (set when a
   file already exists but the user does not replace it) and errbak
   that an error has been detected. */

typedef enum Flag3 {nobak, bak, notbak, errbak} flag3;

/* do not change ANY of the following eight define's */

#define BPE_EDITLENGTH	63

#define BPE_SMALLBUFSIZE	(BPE_EDITLENGTH + 1)
#define BPE_LARGEBUFSIZE	1024

#define ASCY		6
#define	ASCX		63
#define HEXY		6
#define HEXX		13

#define MINCOLS		80

#include "struct.h"
/* #include "global.h" */
#include "func.h"
#include "msg.h"
#include "command.h"
#include "key-bind.h"

#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#endif
#ifndef EXIT_FAILURE
#define EXIT_FAILURE 1
#endif

#define BPE_NAME "bpe"

/* Set NAMELENGTH and PATHLENGTH to defaults if they are not defined already
   in Makefile (definition in Makefile is the preferred method if other than
   the defaults are required). NAMELENGTH is the maximum number of characters
   allowed in the filename - PATHLENGTH should be obvious.

   These could be set "automatically" on most systems but too many problems
   have been found with incorrect definitions - particularly on early SysV
   systems using Doug Gwyn's dir suite where the system administrator has
   not set things up properly. It's "safer" to set things here (unless other
   than the default values are set in Makefile).

   For Sys V up to Release 2 (inclusive) NAMELENGTH should be 14 - vary
   as required for other flavours of UNIX/BSD - the default for NAMELENGTH
   is 63 (BPE_NAMELENGTH) and 255 for PATHLENGTH (BPE_PATHLENGTH) not
   including the terminating NULL */

#define BPE_NAMELENGTH	63
#define BPE_PATHLENGTH	255

#if !defined(NAMELENGTH)
#define NAMELENGTH	BPE_NAMELENGTH
#endif
#if !defined(PATHLENGTH)
#define PATHLENGTH	BPE_PATHLENGTH
#endif

#define NAME_BUFSIZE (NAMELENGTH + 1)
#define PATH_BUFSIZE (PATHLENGTH + 1)


/* default directory used for the locking file if not set in Makefile
   (definition in Makefile is the preferred method if other than the default
   is required) - it used to be /usr/tmp but, as this has been changed to
   /var/tmp on some systems, we now settle for /tmp as the default */

#if (!defined(NOLOCK) && !defined(LOCKDIR))
#define LOCKDIR "/tmp"
#endif

/* default prefix for the locking file - we stay with the "convention" of
   using "LCK.." (so the lockfile name ends up as "LCK..filename") but it
   can be changed in Makefile to, for example, "BPE.." or some other
   appropriate string (definition in Makefile is the preferred method if
   other than the default is required) */

#if (!defined(NOLOCK) && !defined(LOCKPREFIX))
#define LOCKPREFIX "LCK.."
#endif

/* default extension for the backup file if not defined in Makefile
   (definition in Makefile is the preferred method if other than the default
   is required) */

#ifndef BAKEXT
#define BAKEXT ".bak"
#endif

/* a few general definitions */

#ifdef NUL
#undef NUL
#endif

#ifdef NULL
#undef NULL
#endif

#define	NUL		0x00
#define NULL		0x00

#ifndef ESC
#define ESC		0x1b
#endif

#define	BELL		0x07
#define BS		0x08
#define CTRLE		0x05	/* CTRL-E - accesses previous entry for
					    editing on command line and
					    toggle between edit modes */
#define CTRLV		0x16	/* CTRL-V - escape next edit char */
#define CTRLX		0x18	/* CTRL-X - set selected marker in edit mode */
#define CTRLZ		0x1a	/* CTRL-Z - job-control */

/* you might want to (carefully) change some of these */

#define MAXFILES	60	/* maximum number of files - set to 60 so that
				   a full list will fit neatly on one 24 x 80
				   screen - increasing this will make a real
				   mess of the display */

#define BADCHARS	" ()<>[]:;!?|*#=`'^&\"\\"  /* these chars, as well as
						      all control chars, are
						      not allowed in file names
						      originated and written by
						      bpe or in backup-file
						      extensions */

/* this is required for old versions of curses (usually including BSD) */
#ifndef cbreak
#  define	cbreak()	crmode()
#endif
#ifndef nocbreak
#  define	nocbreak()	nocrmode()
#endif
#ifndef beep
#  define	beep()		putchar(BELL)
#endif
