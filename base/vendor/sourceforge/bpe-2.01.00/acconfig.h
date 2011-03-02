
#undef BPE_OS

/* if defined, we need a prototype for sys_errlist[] */
#undef NEED_SYS_ERRLIST

/* if defined, we need a prototype for "struct utimbuf" */
#undef NEED_STRUCT_UTIMBUF

/* if defined, we have ncurses */
#undef HAVE_NCURSES

/* MINLINES=	This should be set to the minimum number of terminal
 *		lines with which bpe will work. The editing window will
 *		use (MINLINES - 8) lines so anything less than 10 lines
 *		would be a bit silly. The default for MINLINES is 24. */
#undef MINLINES

/* BAKEXT=	This can be set (e.g. -DBAKEXT=\".orig\") if you wish
 *		to change the default extension of ".bak" for backup
 *		files. */
#undef BAKEXT

/* LOCKDIR=	This can be changed from the default (/tmp) if you wish
 *		to change the directory used for lock files by adding, 
 *		e.g. -DLOCKDIR=\"/var/tmp\" */
#undef LOCKDIR

/* LOCKPREFIX=	This can be set if you want to use other than "LCK.."
 *		for the lockfile prefix, e.g. -DLOCKPREFIX=\"BPE..\" */
#undef LOCKPREFIX

/* DISABLE_JOBCONTROL  - Disable job control (control-z) */
#undef NOJOBCONTROL

/* NOSHELL	This can be set if shell escapes are to be disallowed. */
#undef NOSHELL

/* NOLOCK	Use this if you wish to disable file locking. But think
 *              carefully before deciding on this option! */
#undef NOLOCK

/*
 * NAMELENGTH=	This should be set if the maximum permissible length of
 *		the file base-name is other than the default of 63
 * PATHLENGTH=	This should be set if the maximum permissible length of
 *		the file pathname is other than the default of 255
 */

