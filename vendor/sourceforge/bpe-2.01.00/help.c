/* bpe - help.c */

#include "bpe.h"
#include "global.h"

WINDOW *hlpscr;

/* the help screens */

int help()

{

	int c = -2, d, count = 1;
	hlpscr = newwin(LINES, COLS, 0, 0);

	/* this involves a "redundant" call to refresh() but it's needed
	   to overcome a bug in many curses implementations with mixed
	   highlighted/non-highlighted characters */

	if (Ute.statline_flag == on)
		stl_clr(1);

	for (;;)
		{
		if (c != -1)
			{
			if (c == -2 || Opt.clear_flag == on)
				{
				wclear(hlpscr);
				wrefresh(hlpscr);
				}

			else
				for (d = 1; d < 23; d++)
					{
					wmove(hlpscr, d, 0);
					wclrtoeol(hlpscr);
					}

			switch (count)
				{
				case 1:
					help_1();
					break;

				case 2:
					help_2();
					break;

				}

			mvwprintw(hlpscr, 22, 4, "Press <?> for %s help \
screen or <RETURN> to exit help: ", (count == 1) ? "second" : "first");
			wrefresh(hlpscr);
			}

		switch (c = getch())
			{
			case SCREEN_REDRAW:
			case REDRAW:
				c = -2;
				continue;

			case '?':
				count = (count == 1) ? 2 : 1;
				continue;

			case CANCEL_1:
			case CANCEL_2:
			case '\r':
			case '\n':
				c = -1;
				break;

			default:
				if (c == 'q' || c == 'z')
					c = (Crnt.ed.bmod_flag == on) ?
					    'q' : 'z';

				else
					if (c == Ute.xcancel)
						c = -1;

				break;

			}

		switch (c)
			{
			case SHOW_MARKERS:
				break;

			case SELECT_FILE:
				/* this test is something of a duplication but
				   is needed to avoid the current-file display
				   being redrawn before the file-list */

				if (Ute.numfiles > 1 &&
				   Crnt.ed.bmod_flag == off)
					break;

			default:
				touchwin(stdscr);
				wclear(hlpscr);
				wrefresh(hlpscr);
				break;
			}

		delwin(hlpscr);
		return(c);
		}

}

void help_1()

{

	if (Opt.high_flag == on)
		wstandout(hlpscr);

	mvwprintw(hlpscr, 1, 4, "File-select commands");
	mvwprintw(hlpscr, 13, 4, "Search commands");
	mvwprintw(hlpscr, 1, 42, "File-position commands");
	if (Opt.high_flag == on)
		wstandend(hlpscr);

 	mvwprintw(hlpscr, 3, 4, "%c or %c   move to next file",
 	    NEXT_FILE1, NEXT_FILE2);
 	mvwprintw(hlpscr, 4, 4, "%c or %c   move to previous file",
 	    PREV_FILE1, PREV_FILE2);
 	mvwprintw(hlpscr, 5, 4, "%c        display/select-from list",
 	    SELECT_FILE);
 	mvwprintw(hlpscr, 6, 4, "%c        add file to list", ADD_FILE);
 	mvwprintw(hlpscr, 7, 4, "%c        add and edit new file",
 	    ADD_EDIT_FILE);
 	mvwprintw(hlpscr, 8, 4, "%c        jump to file number",
 	    JUMP_FILE_NUM);
 	mvwprintw(hlpscr, 9, 4, "%c        jump to file name",
 	    JUMP_FILE_NAME);
 	mvwprintw(hlpscr, 10, 4, "%c or %c   toggle last two files",
 	    TOGGLE_FILES1, TOGGLE_FILES2);
 	mvwprintw(hlpscr, 11, 4, "%c        show toggle filenames",
 	    SHOW_TOGGLE);
 	mvwprintw(hlpscr, 15, 4, "%c or %c   find ASCII string",
 	    FIND_ASCII1, FIND_ASCII2);
 	mvwprintw(hlpscr, 16, 4, "%c or %c   locate hex string",
 	    LOCATE_HEX1, LOCATE_HEX2);
 	mvwprintw(hlpscr, 17, 4, "%c        goto last string",
 	    GOTO_LAST_STR);
 
 	mvwprintw(hlpscr, 3, 42, "%c or %c   display first page",
 	    START_OF_FILE1, START_OF_FILE2);
 	mvwprintw(hlpscr, 4, 42, "%c or %c   display last page",
 	    END_OF_FILE1, END_OF_FILE2);
 	mvwprintw(hlpscr, 5, 42, "%c        display next page", NEXT_PAGE1);
 	mvwprintw(hlpscr, 6, 42, "%c        display previous page", PREV_PAGE1);
#ifndef HAVE_KEYPAD
  	mvwprintw(hlpscr, 7, 42, "%-9sscroll forward 1 line", STR_DOWN);
#else
  	mvwprintw(hlpscr, 7, 42, "%-9sscroll forward 1 line",
  		 (Opt.keypad_flag == on) ? "dn-key" : STR_DOWN);
#endif
 	mvwprintw(hlpscr, 8, 42, "%c        scroll forward %d line%s",
 		  SCROLL_D_LINES, Opt.slines, (Opt.slines == 1) ? "" : "s");
#ifndef HAVE_KEYPAD
  	mvwprintw(hlpscr, 9, 42, "%-9sscroll back 1 line", STR_UP);
#else
  	mvwprintw(hlpscr, 9, 42, "%-9sscroll back 1 line",
  		 (Opt.keypad_flag == on) ? "up-key" : STR_UP);
#endif
 	mvwprintw(hlpscr, 10, 42, "%c        scroll back %d line%s",
 		  SCROLL_U_LINES, Opt.slines, (Opt.slines == 1) ? "" : "s");
 	mvwprintw(hlpscr, 11, 42, "%c        set current address",
 	    SELECT_ADDRESS);
 	mvwprintw(hlpscr, 12, 42, "%c        set marker to current page",
 	    SET_MARKER_PAGE);
 	mvwprintw(hlpscr, 13, 42, "%c        set marker to address",
 	    SET_MARKER_ADD);
 	mvwprintw(hlpscr, 14, 42, "%c        clear markers", CLEAR_MARKERS);
 	mvwprintw(hlpscr, 15, 42, "%c or %c   goto marker",
 	    GOTO_MARKER1, GOTO_MARKER2);
 	mvwprintw(hlpscr, 16, 42, "%c        goto last current address",
 	    GOTO_LAST_ADD);
 	mvwprintw(hlpscr, 17, 42, "%c        goto offset address",
 	    GOTO_OFFSET);
 	mvwprintw(hlpscr, 18, 42, "%c        goto last-edit page",
 	    GOTO_LAST_EDIT);
 	mvwprintw(hlpscr, 19, 42, "%c        display addresses", SHOW_MARKERS);
  
}

void help_2()

{

	if (Opt.high_flag == on)
		wstandout(hlpscr);

	mvwprintw(hlpscr, 1, 4, "Edit commands");
	mvwprintw(hlpscr, 11, 4, "Screen commands");
	mvwprintw(hlpscr, 1, 42, "Edit-mode commands");
	mvwprintw(hlpscr, 10, 42, "Utility commands");
	if (Opt.high_flag == on)
		wstandend(hlpscr);

	mvwprintw(hlpscr, 3, 4, "%c        edit in ASCII window",
	    ASCII_EDIT);
	mvwprintw(hlpscr, 4, 4, "%c        edit in hex window",
	    HEX_EDIT);
	mvwprintw(hlpscr, 5, 4, "%c        cursor-positioning on/off",
	    TOGGLE_COFFSET);
	mvwprintw(hlpscr, 6, 4, "%c        undo changes", UNDO_BUFFER);
	mvwprintw(hlpscr, 7, 4, "%c        restore original buffer",
	    UNDO_FILE);
	mvwprintw(hlpscr, 8, 4, "%c        write modified buffer",
	    WRITE_BUFFER);
	mvwprintw(hlpscr, 9, 4, "^E       edit last entry");

	mvwprintw(hlpscr, 13, 4, "%c        clear status line",
	    CLEAR_STATUS);
	mvwprintw(hlpscr, 14, 4, "%c        display-clear on/off",
	    TOGGLE_CLEAR);
	mvwprintw(hlpscr, 15, 4, "%c        highlighting on/off",
	    TOGGLE_HIGH);
	mvwprintw(hlpscr, 16, 4, "%c        remove highlighting",
	    REMOVE_HIGH);
	mvwprintw(hlpscr, 17, 4, "%c        display highlighting",
	    RESTORE_HIGH);
	mvwprintw(hlpscr, 18, 4, "%c or ^R  redraw current screen",
	    SCREEN_REDRAW);
	mvwprintw(hlpscr, 19, 4, "%c        address-display on/off",
	    TOGGLE_ADD_DISP);

	mvwprintw(hlpscr, 3, 42, "^E       toggle edit mode");
	mvwprintw(hlpscr, 4, 42, "^V       escape next edit char");
	mvwprintw(hlpscr, 5, 42, "%-9sredraw current screen", REDRAWSTR);
	mvwprintw(hlpscr, 6, 42, "^X       set marker to cursor");
	mvwprintw(hlpscr, 7, 42, "DEL      single-character undo");
	mvwprintw(hlpscr, 8, 42, "%-9sexit edit mode", Ute.cancelstr);

	mvwprintw(hlpscr, 12, 42, "%c        create backup file", MAKE_BACKUP);
	mvwprintw(hlpscr, 13, 42, "%c        write to new file",
	    WRITE_NEW_FILE);
	mvwprintw(hlpscr, 14, 42, "%c        show option status",
	    SHOW_OPTIONS);
	mvwprintw(hlpscr, 15, 42, "%c        show buffer/file status",
	    SHOW_STATUS);
	mvwprintw(hlpscr, 16, 42, "%c        quit %s", SOFT_QUIT, Ute.bpe_name);
	mvwprintw(hlpscr, 17, 42, "%c or %c   unconditional quit",
	    HARD_QUIT1, HARD_QUIT2);
	mvwprintw(hlpscr, 18, 42, "%c        show version number/date",
	    SHOW_VERSION);
#ifdef NOSHELL
	mvwprintw(hlpscr, 19, 42, "%c        display help screens", SHOW_HELP);
#else
#ifdef JOBCONTROL
	mvwprintw(hlpscr, 19, 42, "%c        shell escape", SHELL_ESCAPE);
#else
	mvwprintw(hlpscr, 19, 42, "%c or ^Z  shell escape", SHELL_ESCAPE);
#endif
	mvwprintw(hlpscr, 20, 42, "%c        display help screens", SHOW_HELP);
#endif

}

int opt_help()

{

	(void) usage(0);
	(void) fprintf(stderr, "\n-a\t - align searched-for string or \
selected address at first column\n");
	(void) fprintf(stderr, "-b\t - automatically create backup file with \
default extension \"%s\"\n", BAKEXT);
	(void) fprintf(stderr, "-Bext\t - specify extension other \
than \"%s\" for backup files\n", BAKEXT);
	(void) fprintf(stderr, "-c\t - set initial cursor position to top, \
left-hand corner of window\n");
	(void) fprintf(stderr, "-d\t - clear display before accessing each \
file\n");
	(void) fprintf(stderr, "-e\t - disable bell for command and \
file errors\n");
	(void) fprintf(stderr, "-E\t - disable bell for edit-mode and \
string/address data-entry errors\n");
	(void) fprintf(stderr, "-h\t - disable highlighting\n");
	(void) fprintf(stderr, "-k\t - disable ESC as escape character\n");
#ifdef HAVE_KEYPAD
	(void) fprintf(stderr, "-K\t - disable keypad\n");
#endif
	(void) fprintf(stderr, "-ln\t - set position of target line\n");
#ifndef NOLOCK
	(void) fprintf(stderr, "-L\t - disable file locking\n");
#endif
	(void) fprintf(stderr, "-m\t - disable message-receive\n");
	(void) fprintf(stderr, "-ooffset - specify file-start address\n");
	(void) fprintf(stderr, "-r\t - open all files in read-only mode\n");
	(void) fprintf(stderr, "-sn\t - set number of scroll lines\n");
	(void) fprintf(stderr, "-v\t - show version number/date\n");
	(void) fprintf(stderr, "-V\t - show compile-time defaults\n");
	(void) fprintf(stderr, "-w\t - force 24 x 80 display\n");
	(void) fprintf(stderr, "-x\t - do not disable XON-XOFF flow-\
control\n");
	(void) fprintf(stderr, "-y\t - disable address display during \
edit/search\n");
	(void) fflush(stderr);

	return(0);

}
