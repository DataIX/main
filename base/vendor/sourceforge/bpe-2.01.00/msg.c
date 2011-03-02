/* bpe - msg.c */

#include "bpe.h"
#include "global.h"

#include <sys/stat.h>

/* (almost) all of the status-line message functions */

/* file/command error and information messages */

static char *status_msg[] =
	{
	"Searching for:",
	"No previous ASCII string",
	"ASCII string not found:",
	"No previous hex string",
	"Illegal hex string:",
	"Hex string not found:",
	"Illegal address",
	"Illegal address-order",
	"Address beyond EOF",
	"No previous address",
	"No previous set-current address",
	"No previous string-search address",
	"No offset address",
	"No last-edit address",
	"Illegal marker - must be from \"a\" to \"z\"",
	"No markers set",
	"Marker not set:",
	"Markers cleared",
	"No next page",
	"No previous page",
	"Edit-buffer modifications unchanged",
	"Edit-buffer modifications undone",
	"No edit-buffer modifications exist",
	"No edit-buffer modifications exist - buffer not written to disk",
	"Edit-buffer written to disk",
	"Original-buffer restored and written to disk",
	"No write since last edit-buffer modification",
	"No file-page modifications exist",
	"Read-only mode",
	"Lseek error",
	"Read error",
	"Write error",
	"Cannot stat:",
	"Cannot open:",
	"Empty file:",
	"File opened in read-only mode:",
	"Not accessible:",
	"No other files",
	"No next file",
	"No previous file",
	"No such file:",
	"File list full",
	"File already in list:",
	"File added to list:",
	"Illegal file number:",
	"Cannot reset mod-time:",
	"Backup file already exists:",
	"Cannot create backup file:",
	"Cannot create backup file",
	"Creating backup file:",
	"Backup file created:",
	"File already exists:",
	"Not an ordinary file",
	"File same as current file:",
	"Illegal file name:",
	"Null string",
	"No previous filename",
	"Cannot create file:",
	"Cannot append to file:",
	"Creating file:",
	"Appending to file:",
	"File created:",
	"File appended-to:",
	"Address-display off",
	"Address-display on",
	"Cursor-positioning off",
	"Cursor-positioning on",
	"Display-clear off",
	"Display-clear on",
	"Highlighting off",
	"Highlighting on",
	"No last-entry editing at this prompt",
	"Shell-escape not available",
	"Cannot allocate memory",
#ifdef NOLOCK
	"Illegal command - ? for help"
	};
#else
	"Illegal command - ? for help",
	"Cannot create lockfile",
	"Lockfile error",
	"Cannot write to lockfile",
	"Cannot read lockfile",
	"Error removing lockfile"
	};
#endif

void stl_msg(s1, s2, msgcode, belcode)
const char *s1;
char *s2;
int msgcode, belcode;

{

	int len;
	char *cptr;

	stl_clr(0);
	Ute.statline_flag = on;
	switch (msgcode)
		{
		case EDINF:
#ifndef HAVE_KEYPAD
			printw("Down %s - up %s - left %s - right %s - home \
%s - exit edit mode with <%s>", STR_DOWN, STR_UP, STR_LEFT, STR_RIGHT,
				STR_HOME, Ute.cancelstr);
#else
			if (Opt.keypad_flag == off)
				printw("Down %s - up %s - left %s - right %s \
- home %s - exit edit mode with <%s>", STR_DOWN, STR_UP, STR_LEFT, STR_RIGHT,
					STR_HOME, Ute.cancelstr);

			else
				printw("Exit edit mode with <%s>",
					Ute.cancelstr);
#endif
			break;

		case VERINF:
			show_sver();
			break;

		default:
			if (belcode)
				{
				if (Opt.generr_flag == on)
					(void) beep();

				if (Opt.high_flag == on)
					standout();
				}

			printw("%s", status_msg[msgcode]);
			if (s2)
				{
				cptr = ((len = strlen(s2)) > 19) ?
				   &s2[len - 18] : s2;
				printw(" \"%s%s\"",
				   (len > 19) ? ">" : "", cptr);
				}

			if (s1)
				printw(" - %s", s1);

			if (belcode && Opt.high_flag == on)
				standend();

			break;
		}

	refresh();

}

/* clear status line and (optionally) refresh */

void stl_clr(rcode)
int rcode;

{

	Ute.statline_flag = off;
	move(Ute.bpe_lines - 1, 0);
	clrtoeol();
	if (rcode)
		refresh();

}

/* show toggle options on status line */

void show_options()

{

	stl_clr(0);
	Ute.statline_flag = on;
	printw("[%saddress-display] [%scursor-positioning] [%sdisplay-clear] \
[%shighlighting]",
	      (Opt.add_flag == off) ? "no" : "",
	      (Opt.cursor_flag == off) ? "no" : "",
	      (Opt.clear_flag == off) ? "no" : "",
	      (Opt.high_flag == off) ? "no" : "");
	refresh();

}

/* show file/buffer/search status on "status" line */

void show_status()

{

	stl_clr(0);
	Ute.statline_flag = on;
	switch (Fil[optind].ftype)
		{
		case S_IFDIR:
			printw("[directory] ");
			break;

/*		case S_IFBLK:
			printw("[block special file] ");
			break;

		case S_IFCHR:
			printw("[character special file] ");
			break; */

		case S_IFREG:
			printw("[ordinary file] ");
			break;

		case S_IFIFO:
			printw("[FIFO] ");
			break;
#ifdef S_IFLNK
		case S_IFLNK:
			printw("[symbolic link] ");
			break;
#endif
		default:
			printw("[file-error] ");
			break;
		}

	if (Crnt.fd != -1)
		printw("[read%s] ", (Fil[optind].write_flag == off) ?
		       "-only" : "/write");

#ifndef NOLOCK
	printw("[%slockfile%s%s] [%sbackup%s]%s%s%s%s",
	      (Crnt.lock_flag == nolock) ? "no" : "",
	      (Crnt.lock_flag == otherlock) ? " exists" : "",
	      (Crnt.lock_flag == errlock || Crnt.lock_flag == errnolock) ?
	      "-error" : "",
#else
	printw("[%sbackup%s]%s%s%s%s",
#endif
	      (Fil[optind].bak_status == nobak ||
	      Fil[optind].bak_status == notbak) ? "no" : "",
	      (Fil[optind].bak_status == errbak) ? "-error" : "",
	      (Crnt.ed.bmod_flag == on || Crnt.ed.fmod_count || Crnt.fd == -1) ?
	      "" : " [nomod]",
	      (Crnt.ed.bmod_flag) ? " [buffermod]" : "",
	      (Crnt.ed.fmod_count) ? " [filemod]" : "",
	      (Fil[optind].hsearch_flag == on && Opt.high_flag == on) ?
	      " [search]" : "");
	refresh();

}

/* show names of last-two-accessed-files on status-line with name of
   current file highlighted */

void show_toggle(argf)
char **argf;

{

	char *cp1, *cp2;

	if (Ute.numfiles == 1)
		{
		stl_msg((char *) NULL, (char *) NULL, NOOFILE, 1);
		return;
		}

	if (Ute.tfile1 == Ute.tfile2)
		{
		stl_msg((char *) NULL, (char *) NULL, NOPFILE, 1);
		return;
		}

	stl_clr(0);
	Ute.statline_flag = on;
	if ((cp1 = strrchr(argf[Ute.tfile1], '/')) == NULL ||
	    *(cp1 + 1) == NUL)
		cp1 = argf[Ute.tfile1];

	else
		cp1++;

	if ((cp2 = strrchr(argf[Ute.tfile2], '/')) == NULL ||
	    *(cp2 + 1) == NUL)
		cp2 = argf[Ute.tfile2];

	else
		cp2++;

	if (Crnt.fd == -1)
		printw("%s", cp2);

	else
		if (Opt.high_flag == off)
			printw("[%s]", cp2);

		else
			{
			standout();
			printw("%s", cp2);
			standend();
			}

	printw(" %s", cp1);

}
