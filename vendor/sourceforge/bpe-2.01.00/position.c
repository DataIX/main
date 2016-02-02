/* bpe - position.c */

#include "bpe.h"
#include "global.h"

/* set file position and markers to user-selected value */

#include <sys/stat.h>

/* adjust file position according to align and line-position options */

void pos_opts()

{

	/* set fil.sfpos so that it's the same as the selected-address or
	   the address of the searched-for string */

	Fil[optind].sfpos = Fil[optind].fpos;

	/* adjust the starting position if the -l option has been used */

	if (Opt.tpos)
		if (Fil[optind].fpos >= 16 * (off_t) Opt.tpos)
			Fil[optind].fpos -= 16 * (off_t) Opt.tpos;

		else
			Fil[optind].fpos = 0;

	/* adjust the starting position if the -a option has not been used */

	if (Opt.align_flag == off)
		Fil[optind].fpos &= ~0xf;

	/* set fil.sfpos to the required cursor offset for edit modes */

	Fil[optind].sfpos =
	    (off_t) abs((int) (Fil[optind].sfpos - Fil[optind].fpos));

}

/* goto first page of current file */

void first_page()

{

	if (Fil[optind].fpos)
		{
		Fil[optind].fpos = Fil[optind].sfpos = 0;
		Fil[optind].search_flag = Fil[optind].hsearch_flag = off;
		(void) dump(0);
		}

}

/* goto last page of current file */

void last_page()

{

	if (Fil[optind].fpos != Fil[optind].lpage)
		{
		Fil[optind].fpos = Fil[optind].lpage;
		Fil[optind].sfpos = 0;
		Fil[optind].search_flag = Fil[optind].hsearch_flag = off;
		(void) dump(0);
		}

}

/* goto next page of current file */

void next_page()

{

	if (Fil[optind].fpos == Fil[optind].lpage)
		stl_msg((char *) NULL, (char *) NULL, NONPAGE, 1);

	else
		{
		if ((Fil[optind].fpos += Ute.edit_chars) > Fil[optind].lpage)
			Fil[optind].fpos = Fil[optind].lpage;

		Fil[optind].sfpos = 0;
		Fil[optind].search_flag = Fil[optind].hsearch_flag = off;
		(void) dump(0);
		}

}


/* goto previous page of current file */

void prev_page()

{

	if (Fil[optind].fpos ==  0)
		stl_msg((char *) NULL, (char *) NULL, NOPPAGE, 1);

	else
		{
		if ((Fil[optind].fpos -= Ute.edit_chars) < 0)
			Fil[optind].fpos = 0;

		Fil[optind].sfpos = 0;
		Fil[optind].search_flag = Fil[optind].hsearch_flag = off;
		(void) dump(0);
		}

}

/* goto page containing last edit */

void last_edit()

{

	if (Fil[optind].xed.fpos ==  -1)
		stl_msg((char *) NULL, (char *) NULL, NOLEAD, 1);

	else
		if (Fil[optind].xed.fpos != Fil[optind].fpos)
			{
			Fil[optind].fpos = Fil[optind].xed.fpos;
			Fil[optind].sfpos = Fil[optind].xed.sfpos;
			Fil[optind].search_flag =
			Fil[optind].hsearch_flag = off;
			(void) dump(0);
			}

}

/* goto last successfully-searched-for string */

void last_search()

{

	if (Fil[optind].srch.search_flag == off)
		stl_msg((char *) NULL, (char *) NULL, NOPSSAD, 1);

	else
		{
		Crnt.ed.write_count = 0;
		Fil[optind].slen = Fil[optind].srch.slen;
		Fil[optind].fpos = Fil[optind].srch.fpos;
		Fil[optind].sfpos = Fil[optind].srch.sfpos;
		(void) rdpage();
		cp_page();
		if (Opt.add_flag == off)
			dispos();

		if (strncmp((char *) &Buf.pagebuf[Fil[optind].srch.sfpos],
			    Fil[optind].srch.last_str, Fil[optind].srch.slen))
			{
			Fil[optind].search_flag =
			    Fil[optind].hsearch_flag = off;
			disp(0);
			}

		else
			{
			Fil[optind].search_flag =
			    Fil[optind].hsearch_flag = on;

			disp((Opt.high_flag == on &&
			    Fil[optind].hsearch_flag == on) ? 1 : 0);
			}
		}

}

/* get and set user-selected address */

void set_address()

{

	static char a_last_entry[BPE_SMALLBUFSIZE];
	off_t tmp_pos;

	if ((tmp_pos = get_address("Address", a_last_entry, 9, 15)) == -1)
		reset_dispos();

	else
		{
		Fil[optind].lfpos = Fil[optind].fpos = tmp_pos;
		pos_opts();
		clr_command();
		(void) dump(0);
		}

}

/* goto last user-selected address */

void last_set_address()

{

	if (Fil[optind].lfpos == -1)
		stl_msg((char *) NULL, (char *) NULL, NOPSAD, 1);

	else
		{
		Fil[optind].fpos = Fil[optind].lfpos;
		pos_opts();
		(void) dump(0);
		}

}

/* get and set user-selected marker */

void set_mark()

{

	int inval;

	if ((inval = get_mark()) == -1)
		return;

	Fil[optind].markers[inval] = (Opt.tpos) ?
	    Fil[optind].fpos + Fil[optind].sfpos : Fil[optind].fpos;
	Fil[optind].mark_flag = on;
	if (Ute.statline_flag == on)
		stl_clr(0);

}

/* get and set marker to cursor address from edit mode */

void set_cmark(ed_mode)
char *ed_mode;

{

	int inval;

	stl_clr(0);
	if ((inval = get_mark()) != -1)
		{
		Fil[optind].markers[inval] = (off_t) (Curset.cury * 16) +
		    (off_t) Curset.curx + Curset.fpos;
		Fil[optind].mark_flag = on;
		}

/*	mvprintw(2, 0, "%s-edit", ed_mode); */
	set_hdr(ed_mode);
	stl_msg((char *) NULL, (char *) NULL, EDINF, 0);

}

/* get and set marker to user-selected address */

void set_amark()

{

	static char m_last_entry[BPE_SMALLBUFSIZE];

	int inval;
	off_t tmp_pos;

	clr_command();
	if ((inval = get_mark()) == -1)
		return;

/*	if (Ute.statline_flag == on)
		clr_msg(0); */

	if ((tmp_pos = get_address("Address", m_last_entry, 9, 15)) == -1)
		return;

	Fil[optind].markers[inval] = tmp_pos;
	Fil[optind].mark_flag = on;

}

void goto_offset()

{


	if (Opt.offset > Fil[optind].flength)
		stl_msg((char *) NULL, (char *) NULL, ADEOF, 1);

	else
		if (Opt.offset < 1L)
			stl_msg((char *) NULL, (char *) NULL, NOOFFAD, 1);

	else
		if (Fil[optind].fpos != Opt.offset)
			{
			Fil[optind].fpos = Opt.offset;
			pos_opts();
			(void) dump(0);
			}

}

/* go to user-selected marker */

void goto_mark()

{

	int inval;
	off_t tmp_pos;

	if (ck_marks())
		return;

	for (;;)
		{
		if ((inval = get_mark()) == -1)
			return;

		if ((tmp_pos = get_maddress(inval)) != -1)
			break;
		}

	if (Ute.statline_flag == on)
		stl_clr(1);

	Fil[optind].fpos = tmp_pos;
	pos_opts();
	Fil[optind].search_flag = Fil[optind].hsearch_flag = off;
	(void) dump(0);

}

/* check if any markers have been set */

int ck_marks()

{

	if (Fil[optind].mark_flag == off)
		{
		stl_msg((char *) NULL, (char *) NULL, NOMARKS, 1);
		return(1);
		}

	return(0);

}

/* set all markers to initial, "unset" state - this is done as each
   file is accessed for the first time or if clear-marks is selected
   with the "k" command */

void init_marks()

{

	int c;

	for (c = 0; c < 26; c++)
		Fil[optind].markers[c] = (off_t) -1;

	Fil[optind].mark_flag = off;

}

int get_mark()

{

	int inval;

	for (;;)
		{
		mvprintw(2, 0, "Marker:   ");
		move(2, 8);
		refresh();
		if ((inval = getch()) == CANCEL_1 || inval == CANCEL_2 ||
		    inval == Ute.xcancel)
			{
			if (Ute.statline_flag == on)
				stl_clr(0);

			return(-1);
			}

		if (inval == CTRLE)
			{
			stl_msg((char *) NULL, (char *) NULL, NOLEE, 1);
			continue;
			}

		if (inval < 0x61 || inval > 0x7a)
			{
			stl_msg((char *) NULL, (char *) NULL, ILLMARK, 1);
			continue;
			}

		if (Ute.statline_flag == on)
			stl_clr(1);

		return(inval - 97);
		}

}

/* get user-selected addresses */

off_t get_address(prompt, last_entry, xpos, len)
char *prompt, *last_entry;
int xpos, len;

{

	off_t tmp_address;
	char inbuf[BPE_SMALLBUFSIZE], *ptr;

	for (;;)
		{
		clr_command();
		printw("%s: ", prompt);
		if (get_str(inbuf, (WINDOW *) NULL, 2, xpos, 0, len, 1, 1))
			{
			if (Ute.statline_flag == on)
				stl_clr(0);

			return((off_t) -1);
			}

		if (Ute.statline_flag == on)
			stl_clr(0);

		switch (inbuf[0])
			{
			case 'S':
				if (Fil[optind].lfpos == -1)
					{
					/* no previous set-address message */
					stl_msg((char *) NULL, (char *) NULL,
					    NOPSAD, 1);
					continue;
					}

				return(Fil[optind].lfpos);

			case CTRLE:
			case NUL:
				clr_command();
				if (last_entry[0] == NUL)
					{
					/* no previous address message */
					stl_msg((char *) NULL, (char *) NULL,
					    NOPAD, 1);
					continue;
					}

				if (inbuf[0] == NUL)
					{
					(void) strcpy(inbuf, last_entry);
					break;
					}

				switch (edit_entry(inbuf, last_entry, prompt,
				    strlen(last_entry), strlen(prompt)))
					{
					case 0:
						continue;

					case 1:
						if (Ute.statline_flag == on)
							stl_clr(0);

						return((off_t) -1);

					default:
						break;
					}

				break;

			case 'B':
				return((off_t) 0);

			case 'C':
				return((Opt.tpos) ?
				    Fil[optind].fpos + Fil[optind].sfpos :
				    Fil[optind].fpos);

			case 'E':
				return(Fil[optind].flength - 1);

			case 'G':
				return(Fil[optind].lpage);

			case 'L':
				if (Fil[optind].srch.search_flag == off)
					{
					stl_msg((char *) NULL, (char *) NULL,
					    NOPSSAD, 1);
					continue;
					}

				return(Fil[optind].srch.fpos +
				    Fil[optind].srch.sfpos);

			case 'O':
				if (Opt.offset < Fil[optind].flength &&
				    Opt.offset > 0L)
					return(Opt.offset);

				stl_msg((char *) NULL, (char *) NULL,
				    NOOFFAD, 1);
				continue;

			case 'X':
				if (Fil[optind].xed.fpos ==  -1)
					{
					stl_msg((char *) NULL, (char *) NULL,
					    NOLEAD, 1);
					continue;
					}

				return(Fil[optind].xed.fpos);

			default:
				break;
			}

		/* if leading char is a-z check for set marker and return
		   marker address if found */

		if (inbuf[0] > 0x60 && inbuf[0] < 0x7b)
			if (ck_marks() ||
			    (tmp_address = get_maddress(inbuf[0] - 97)) == -1)
				continue;

			else
				return(tmp_address);

		(void) strcpy(last_entry, inbuf);

		/* it's an octal address if there's a leading zero unless the
	   	zero is followed by an X or x in which case it's hexadecimal
	   	otherwise it's decimal - see strtol(3C) */

		if ((tmp_address = strtol(inbuf, &ptr, 0)) < 0 || *ptr)
			{
			stl_msg((char *) NULL, (char *) NULL, ILLAD, 1);
			continue;
			}

		if (tmp_address > Fil[optind].flength - 1)
			{
			stl_msg((char *) NULL, (char *) NULL, ADEOF, 1);
			continue;
			}

		return(tmp_address);
		}

}

/* get address stored in marker */

off_t get_maddress(marker_index)
int marker_index;

{

	char str[2];

	if (Fil[optind].markers[marker_index] == -1)
		{
		str[0] = marker_index + 97;
		str[1] = NUL;
		stl_msg((char *) NULL, str, NOTMARK, 1);
		}

	return(Fil[optind].markers[marker_index]);

}

/* display marker settings and other information */

int show_address(filename)
char *filename;

{

	int c, d;
	char *cptr, *ns = "not set";

	WINDOW *markscr;

	markscr = newwin(LINES, COLS, 0, 0);

	if (Ute.statline_flag == on)
		stl_clr(1);

	wclear(markscr);
	wrefresh(markscr);

	for (;;)
		{
		if ((c = strlen(filename)) > 55)
			cptr = &filename[c - 55];

		else
			cptr = filename;

		mvwprintw(markscr, 1, 5, "Current file: %s%s",
		    (c > 55) ? ">" : "", cptr);
		mvwprintw(markscr, 2, 5, "File status:  ");

		switch (Fil[optind].ftype)
			{
			case S_IFDIR:
				wprintw(markscr, "directory");
				break;

/*			case S_IFBLK:
				wprintw(markscr, "block special file");
				break;

			case S_IFCHR:
				wprintw(markscr, "character special file");
				break; */

			case S_IFREG:
				wprintw(markscr, "ordinary file");
				break;

			case S_IFIFO:
				wprintw(markscr, "FIFO");
				break;
#ifdef S_IFLNK
			case S_IFLNK:
				wprintw(markscr, "symbolic link");
				break;
#endif
			default:
				break;
			}

		wprintw(markscr, " - read%s", (Fil[optind].write_flag == off) ?
		       "-only" : "/write");
		mvwprintw(markscr, 3, 5, "File number:  %02d (of %02d)",
		    optind + 1, Ute.numfiles);
		mvwprintw(markscr, 5, 5, "Markers:");
		for (c = 0, d = 13; c < 13; c++, d++)
			{
			mvwprintw(markscr, c + 7, 5, "%c - ", c + 97);
			if (Fil[optind].markers[c] == -1)
				wprintw(markscr, "%s", ns);

			else
				wprintw(markscr, "0x%08lX",
				    Fil[optind].markers[c]);

			mvwprintw(markscr, c + 7, 25, "%c - ", d + 97);
			if (Fil[optind].markers[d] == -1)
				wprintw(markscr, "%s", ns);

			else
				wprintw(markscr, "0x%08lX",
				    Fil[optind].markers[d]);
			}

		/* mvwprintw(markscr, 7, 45, "%-20s0x%08lX", "Length:", */
		mvwprintw(markscr, 7, 45, "%-20s0x%08lX", "Size:",
		    Fil[optind].flength);
		mvwprintw(markscr, 9, 45, "%-20s0x%08lX", "Current page:",
		    Fil[optind].fpos);
		c = 11;
		if (Fil[optind].ftype == S_IFREG)
			{
			mvwprintw(markscr, c++, 45, "%-20s",
			    "Last-edit page:");
			if (Fil[optind].xed.fpos == -1)
				wprintw(markscr, "%s", ns);

			else
				wprintw(markscr, "0x%08lX",
				    Fil[optind].xed.fpos);
			}

		mvwprintw(markscr, c++, 45, "%-20s", "Set-address:");
		if (Fil[optind].lfpos == -1)
			wprintw(markscr, "%s", ns);

		else
			wprintw(markscr, "0x%08lX", Fil[optind].lfpos);

		mvwprintw(markscr, c++, 45, "%-20s", "String-search:");
		if (Fil[optind].srch.search_flag == off)
			wprintw(markscr, "%s", ns);

		else
			wprintw(markscr, "0x%08lX",
			    Fil[optind].srch.fpos + Fil[optind].srch.sfpos);

		mvwprintw(markscr, c++, 45, "%-20s", "Offset:");
		if (Opt.offset > 0L && Opt.offset < Fil[optind].flength)
			wprintw(markscr, "0x%08lX", Opt.offset);

		else
			wprintw(markscr, "%s", (Opt.offset < 1L) ?
			    ns : "beyond EOF");

		mvwprintw(markscr, ++c, 45, "%-20s0x%08X", "Buffer size:",
		    Ute.edit_chars);

		mvwprintw(markscr, ++c, 65, "(%d x %d)", LINES, COLS);

		mvwprintw(markscr, 21, 5, "Press <RETURN> to exit: ");
		wrefresh(markscr);
		switch (c = getch())
			{
			case 'R':
			case REDRAW:
				wclear(markscr);
				wrefresh(markscr);

			case 'X':
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
			case '?':
				break;

			case 'F':
				/* this test is something of a duplication but
				   is needed to avoid the current-file display
				   being redrawn before the file-list */

				if (Ute.numfiles > 1 &&
				   Crnt.ed.bmod_flag == off)
					break;

			default:
				touchwin(stdscr);
				wclear(markscr);
				wrefresh(markscr);
				break;
			}

		delwin(markscr);
		return(c);
		}

}

void down_scroll(slines)
int slines;

{

	off_t tmp_pos = Fil[optind].fpos;

	if ((Fil[optind].fpos += slines * 16) > Fil[optind].lpage)
		Fil[optind].fpos = Fil[optind].lpage;

	if ((Fil[optind].sfpos -= (Fil[optind].fpos - tmp_pos)) < 0)
		{
		Fil[optind].sfpos = 0;
		Fil[optind].search_flag = Fil[optind].hsearch_flag = off;
		}

	(void) dump((Opt.high_flag == on &&
	    Fil[optind].hsearch_flag == on) ? 1 : 0);

}

void up_scroll(slines)
int slines;

{

	off_t tmp_pos = Fil[optind].fpos;

	if ((Fil[optind].fpos -= slines * 16) < 0)
		Fil[optind].fpos = 0;

	if ((Fil[optind].sfpos += (tmp_pos - Fil[optind].fpos)) >
	    (Ute.edit_chars - Fil[optind].slen))
		{
		Fil[optind].sfpos = 0;
		Fil[optind].search_flag = Fil[optind].hsearch_flag = off;
		}

	(void) dump((Opt.high_flag == on &&
	    Fil[optind].hsearch_flag == on) ? 1 : 0);

}
