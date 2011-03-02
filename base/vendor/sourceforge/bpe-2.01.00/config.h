/* config.h.  Generated automatically by configure.  */
/* config.h.in.  Generated automatically from configure.in by autoheader.  */

/* Define to empty if the keyword does not work.  */
/* #undef const */

/* Define if you have <sys/wait.h> that is POSIX.1 compatible.  */
#define HAVE_SYS_WAIT_H 1

/* Define to `long' if <sys/types.h> doesn't define.  */
/* #undef off_t */

/* Define as the return type of signal handlers (int or void).  */
#define RETSIGTYPE void

/* Define if you have the ANSI C header files.  */
#define STDC_HEADERS 1

#define BPE_OS "freebsd8.2"

/* if defined, we need a prototype for sys_errlist[] */
/* #undef NEED_SYS_ERRLIST */

/* if defined, we need a prototype for "struct utimbuf" */
/* #undef NEED_STRUCT_UTIMBUF */

/* if defined, we have ncurses */
#define HAVE_NCURSES 1

/* MINLINES= This should be set to the minimum number of terminal
 *  lines with which bpe will work. The editing window will
 *  use (MINLINES - 8) lines so anything less than 10 lines
 *  would be a bit silly. The default for MINLINES is 24. */
#define MINLINES 24

/* BAKEXT= This can be set (e.g. -DBAKEXT=\".orig\") if you wish
 *  to change the default extension of ".bak" for backup
 *  files. */
#define BAKEXT ".bak"

/* LOCKDIR= This can be changed from the default (/tmp) if you wish
 *  to change the directory used for lock files by adding, 
 *  e.g. -DLOCKDIR=\"/var/tmp\" */
#define LOCKDIR "/tmp"

/* LOCKPREFIX= This can be set if you want to use other than "LCK.."
 *  for the lockfile prefix, e.g. -DLOCKPREFIX=\"BPE..\" */
#define LOCKPREFIX "LCK.."

/* DISABLE_JOBCONTROL  - Disable job control (control-z) */
#define NOJOBCONTROL YES

/* NOSHELL This can be set if shell escapes are to be disallowed. */
#define NOSHELL YES

/* The number of bytes in a long.  */
#define SIZEOF_LONG 4

/* The number of bytes in a off_t.  */
#define SIZEOF_OFF_T 8

/* Define if you have the getopt function.  */
#define HAVE_GETOPT 1

/* Define if you have the keypad function.  */
#define HAVE_KEYPAD 1

/* Define if you have the memcpy function.  */
#define HAVE_MEMCPY 1

/* Define if you have the strchr function.  */
#define HAVE_STRCHR 1

/* Define if you have the <fcntl.h> header file.  */
#define HAVE_FCNTL_H 1

/* Define if you have the <ncurses.h> header file.  */
#define HAVE_NCURSES_H 1

/* Define if you have the <ncurses/curses.h> header file.  */
/* #undef HAVE_NCURSES_CURSES_H */

/* Define if you have the <string.h> header file.  */
#define HAVE_STRING_H 1

/* Define if you have the <sys/cdefs.h> header file.  */
#define HAVE_SYS_CDEFS_H 1

/* Define if you have the <sys/ioctl.h> header file.  */
#define HAVE_SYS_IOCTL_H 1

/* Define if you have the <sys/termios.h> header file.  */
#define HAVE_SYS_TERMIOS_H 1

/* Define if you have the <unistd.h> header file.  */
#define HAVE_UNISTD_H 1

/* Name of package */
#define PACKAGE "bpe"

/* Version number of package */
#define VERSION "2.01.00"

