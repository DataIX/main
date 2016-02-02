/* bpe - misc.c (was version.c/utils.c) */

#include "bpe.h"
#include "global.h"
#include "version.h"

/* show version-number/date and compile-time defaults */

int show_ver(option)
int option;

{

	(void) fprintf(stdout, "%s %s\n\n", BPE_VERSION, BPE_VERDATE);
	if (option == 'V')
		{
		(void) fprintf(stdout, "%s - compile-time defaults:\n\n",
		    BPE_OS);
#ifdef NOSHELL
		(void) fprintf(stdout, "shell-escape disabled\n");
#else
		(void) fprintf(stdout, "shell-escape enabled\n");
#endif
#ifdef JOBCONTROL
		(void) fprintf(stdout, "job-control enabled\n");
#else
		(void) fprintf(stdout, "no job-control\n");
#endif
#ifndef HAVE_KEYPAD
		(void) fprintf(stdout, "no keypad\n");
#else
		(void) fprintf(stdout, "keypad enabled\n");
#endif
#ifdef NOLOCK
		(void) fprintf(stdout, "lock-file disabled\n");
#else
		(void) fprintf(stdout, "lock-file enabled\n");
		(void) fprintf(stdout, "lock-file directory: \"%s\"\n",
			       LOCKDIR);
		(void) fprintf(stdout, "lock-file prefix: \"%s\"\n",
			       LOCKPREFIX);
#endif
		(void) fprintf(stdout, "backup-file extension: \"%s\"\n",
			       BAKEXT);
		(void) fprintf(stdout, "maximum filename length: %3d \
characters\n", NAMELENGTH);
		(void) fprintf(stdout, "maximum pathname length: %3d \
characters\n", PATHLENGTH);
		(void) fprintf(stdout, "minimum window size: %d lines by %d \
columns\n", MINLINES, MINCOLS);
		}

	return(0);

}

/* show version-number/date on status line - put here instead of in msg.c
   so that only misc.c needs to be recompiled for version.h changes during
   development */

void show_sver()

{

	printw("%s %s %s", BPE_VERSION, BPE_VERDATE, BPE_OS);

}

int usage(ucode)
int ucode;

{

#ifndef NOLOCK
#ifdef HAVE_KEYPAD
	(void) fprintf(stderr,"Usage: %s [ -abcdeEhkKLmrwxy ] [ -Bext ] \
[ -ln ] [ -ooffset ] [ -sn ] file ...\n", Ute.bpe_name);
#else
	(void) fprintf(stderr,"Usage: %s [ -abcdeEhkLmrwxy ] [ -Bext ] \
[ -ln ] [ -ooffset ] [ -sn ] file ...\n", Ute.bpe_name);
#endif
#else
#ifdef HAVE_KEYPAD
	(void) fprintf(stderr,"Usage: %s [ -abcdeEhkKmrwxy ] [ -Bext ] \
[ -ln ] [ -ooffset ] [ -sn ] file ...\n", Ute.bpe_name);
#else
	(void) fprintf(stderr,"Usage: %s [ -abcdeEhkmrwxy ] [ -Bext ] \
[ -ln ] [ -ooffset ] [ -sn ] file ...\n", Ute.bpe_name);
#endif
#endif

	if (ucode)
		(void) fprintf(stderr,"       %s -H (for help)\n", Ute.bpe_name);

	(void) fflush(stderr);
	return(1);

}

#ifndef HAVE_GETOPT
#define P_ERR(s, c)       if(opterr) \
				{ \
				fputs(argv[0], stderr); \
				fputs(s, stderr); \
				fputc(c, stderr); \
				fputc('\n', stderr); \
				}

int opterr = 1;
int optind = 1;
char *optarg;

int getopt(argc, argv, opts)
int argc;
char **argv, *opts;

{

	static int sp = 1;
	register int c;
	register char *cp;

	if (sp == 1)
		if (optind >= argc || argv[optind][0] != '-' ||
		    argv[optind][1] == '\0')
			return(EOF);

		else
			if (strcmp(argv[optind], "--") == NULL)
				{
				optind++;
				return(EOF);
				}


	c = argv[optind][sp];
	if (c == ':' || (cp = strchr(opts, c)) == NULL)
		{
		P_ERR(": illegal option -- ", c);
		if (argv[optind][++sp] == '\0')
			{
			optind++;
			sp = 1;
			}

		return('?');
		}

	if (*++cp == ':')
		{
		if (argv[optind][sp + 1] != NULL)
			optarg = &argv[optind++][sp + 1];

		else
			if (++optind >= argc)
				{
				P_ERR(": option requires an argument -- ", c);
				sp = 1;
				return('?');
				}

		else
			optarg = argv[optind++];

		sp = 1;
		}

	else
		{
		if (argv[optind][++sp] == '\0')
			{
			sp = 1;
			optind++;
			}

		optarg = NULL;
		}

	return(c);

}
#endif /* ! HAVE_GETOPT */
