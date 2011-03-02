/* bpe - display.c */

#include "bpe.h"
#include "global.h"

/* display header (file name and size) and "grid" on main screen */

void dispheader(disp_code, filename)
int disp_code;
char *filename;

{

	static char *grid = "------------------------------------------------\
-------------------------------";

	int len, nlen;
	char *cptr;

	move(0, 0);
	clrtoeol();

	/* display current filename with appropriate number of leading
	   characters removed and replaced with a single > if filename
           will not fit (taking into account size of "File [n of n]: "
	   display) */

	nlen = (optind > 8) ? 46 : 47;
	if (Ute.numfiles > 9)
		nlen--;

	cptr = ((len = strlen(filename)) > nlen) ?
	    &filename[len - (nlen - 1)] : filename;
	printw("File [%d of %d]: %s%s",
	    optind + 1, Ute.numfiles, (len > nlen) ? ">" : "", cptr);
	if (Crnt.fd == -1 && Opt.clear_flag == off)
		{
		move(4, 0);
		clrtoeol();
		clr_edit(0);
		}

	if (Fil[optind].flength < 1)
		mvprintw(0, 63, "Size: empty");

#ifdef LARGE_OFF_T
	else if (Fil[optind].flen_isll_flag) {
		long long ll_value;
		ll_value = Fil[optind].flength;
		mvprintw(0, 54, "Size: 0x%08lX.%08lX",
			(long)(ll_value >> 32),
			(long)(ll_value));
	}
#endif
	else
		mvprintw(0, 63, "Size: 0x%08lX", (long) Fil[optind].flength);

	if (disp_code)
		{
		mvprintw(5, 0, "%s", grid);
		mvprintw(Ute.bpe_lines - 2, 0, "%s", grid);
		}

}

/* show position in file */

void dispos()

{

#ifdef LARGE_OFF_T
	if (Fil[optind].flen_isll_flag) {
		long long ll_value;
		ll_value = Fil[optind].fpos;
		mvprintw(2, 45, "File Position: 0x%08lX.%08lX",
			(long)(ll_value >> 32),
			(long)(ll_value));
		return;
        }
#endif
	mvprintw(2, 54, "File position: 0x%08lX", (long) Fil[optind].fpos);

}

/* disp sets up the "scale" for the editing windows and calls dispwin() or
   hdispwin for the edit windows */

void disp(hcode)
int hcode;

{

	register int i;
	register off_t j;

	/* output headings adjusted for the starting position */

	mvprintw(4, 0, "ADDRESS     ");
	for (i = 0, j = Fil[optind].fpos & 0x0f; i < 16; i++, j = (j + 1) % 16)
		printw(" 0%c", "0123456789ABCDEF"[j]);

	printw("   ASCII");
	if (hcode)
		hs_dispwin();

	else
		dispwin();

}

/* set-up/refresh the editing windows */

void dispwin()

{

	register int i, j, c;

	for (i = 0; i < Ute.edit_lines; i++)
		{
		mvprintw(HEXY + i, 0, "0x%08lX",
				      (long)Fil[optind].fpos + i * 16);
		for (j = 0; j < 16; j++)
			{
			if ((i * 16 + j ) >= Crnt.bytes)
				{
				clrtoeol();
				break;
				}

			mvprintw(HEXY + i, HEXX + j * 3, "%02X",
				 (Buf.pagebuf[i * 16 + j] & 0xff));
			}

		for (j = 0; j < 16; j++)
			{
			if ((i * 16 + j ) >= Crnt.bytes)
				{
				clr_edit((j == 0) ? i : i + 1);
				return;
				}

			mvprintw(ASCY + i, ASCX + j, "%c",
				((c = Buf.pagebuf[i * 16 + j]) < 0x20 ||
				c > 0x7e) ? 0x2e : c);
			}
		}

	refresh();

}

/* set-up/refresh the editing windows with highlighting after search */

void hs_dispwin()

{

	register int i, j, c, endpos, pos;

	/* so_flag is used to check if standout() has been called as a few
	   curses/brain-damaged-terminal combinations puke if an attribute
	   is turned off when it hasn't been turned on */

	flag1 so_flag = off;

	endpos = Fil[optind].sfpos + Fil[optind].slen;

	for (i = 0; i < Ute.edit_lines; i++)
		{
		mvprintw(HEXY + i, 0, "0x%08lX",
				      (long) Fil[optind].fpos + i * 16);
		for (j = 0; j < 16; j++)
			{
			if ((i * 16 + j ) >= Crnt.bytes)
				{
				clrtoeol();
				break;
				}

			if ((pos = i * 16 + j) >= Fil[optind].sfpos &&
			     pos < endpos)
				{
				standout();
				so_flag = on;
				}

			mvprintw(HEXY + i, HEXX + j * 3, "%02X",
				 (Buf.pagebuf[pos] & 0xff));
			if (so_flag == on)
				{
				standend();
				so_flag = off;
				}
			}

		for (j = 0; j < 16; j++)
			{
			if ((i * 16 + j ) >= Crnt.bytes)
				{
				clr_edit((j == 0) ? i : i + 1);
				return;
				}

			if ((pos = i * 16 + j) >= Fil[optind].sfpos &&
			    pos < endpos)
				{
				standout();
				so_flag = on;
				}

			mvprintw(ASCY + i, ASCX + j, "%c",
				((c = Buf.pagebuf[pos]) < 0x20 || c > 0x7e)
				? 0x2e : c);
			if (so_flag == on)
				{
				standend();
				so_flag = off;
				}
			}
		}

	refresh();

}

/* refresh the editing windows with highlighting after highlighting
   toggle if there are any page buffer mods */

void hm_dispwin()

{

	register int i, j, c, pos;

	/* so_flag is used to check if standout() has been called as a few
	   curses/brain-damaged-terminal combinations puke if an attribute
	   is turned off when it hasn't been turned on */

	flag1 so_flag = off;

	for (i = 0; i < Ute.edit_lines; i++)
		{
		mvprintw(HEXY + i, 0, "0x%08lX",
				      (long) Fil[optind].fpos + i * 16);
		for (j = 0; j < 16; j++)
			{
			if ((i * 16 + j ) >= Crnt.bytes)
				{
				clrtoeol();
				break;
				}

			pos = i * 16 + j;
			if (Buf.pagebuf[pos] != Buf.tmpbuf[pos])
				{
				standout();
				so_flag = on;
				}

			mvprintw(HEXY + i, HEXX + j * 3, "%02X",
				 (Buf.pagebuf[pos] & 0xff));
			if (so_flag == on)
				{
				standend();
				so_flag = off;
				}
			}

		for (j = 0; j < 16; j++)
			{
			if ((i * 16 + j ) >= Crnt.bytes)
				{
				clr_edit((j == 0) ? i : i + 1);
				return;
				}

			pos = i * 16 + j;
			if (Buf.pagebuf[pos] != Buf.tmpbuf[pos])
				{
				standout();
				so_flag = on;
				}

			mvprintw(ASCY + i, ASCX + j, "%c",
				((c = Buf.pagebuf[pos]) < 0x20 || c > 0x7e)
				? 0x2e : c);
			if (so_flag == on)
				{
				standend();
				so_flag = off;
				}
			}
		}

	refresh();

}

/* show cursor position in file at start of edit session */

void c_dispos()

{

#ifdef LARGE_OFF_T
	if (Fil[optind].flen_isll_flag) {
		long long ll_value;
		ll_value = (Opt.cursor_flag == on) ?
			Fil[optind].fpos + Fil[optind].sfpos :
			Fil[optind].fpos;
		mvprintw(2, 43, "Cursor Position: 0x%08lX.%08lX",
				(long)(ll_value >> 32),
				(long)(ll_value));
		return;
	}
#endif
	mvprintw(2, 52, "Cursor position: 0x%08lX", (Opt.cursor_flag == on) ?
	    (long) Fil[optind].fpos + Fil[optind].sfpos :
	    (long) Fil[optind].fpos);

}

/* update cursor position in file during edit session */

void uc_dispos()

{

#ifdef LARGE_OFF_T
	if (Fil[optind].flen_isll_flag) {
		long long ll_value;
		ll_value = (Curset.cury * 16 + Curset.curx + Curset.fpos);
		mvprintw(2, 43, "Cursor Position: 0x%08lX.%08lX",
				(long)(ll_value >> 32),
				(long)(ll_value));
		return;
	}
#endif
	mvprintw(2, 69, "0x%08lX", (long) (Curset.cury * 16 + Curset.curx +
	    Curset.fpos));

}

/* clear command line to EOL */

void clr_command()

{

	move(2, 0);
	clrtoeol();

}

/* clear edit windows */

void clr_edit(line_num)
int line_num;

{

	for (; line_num < Ute.edit_lines; line_num++)
		{
		move(HEXY + line_num, 0);
		clrtoeol();
		}

	refresh();

}

void reset_dispos()

{

	clr_command();
	refresh();
	dispos();

}

/* redraw current screen preserving all information */

void redraw(win)
WINDOW *win;

{

	WINDOW *tmpscr, *scrn;

/*	tmpscr = newwin(0, 0, 0, 0); */
	tmpscr = newwin(LINES, COLS, 0, 0);
	wclear(tmpscr);
	wrefresh(tmpscr);
	delwin(tmpscr);
	touchwin(scrn = (win) ? win : stdscr);
	wrefresh(scrn);
	return;

}

/* toggle address-display */

void tog_address()

{

	Opt.add_flag = (Opt.add_flag == on) ? off : on;
	stl_msg((char *) NULL, (char *) NULL,
	    (Opt.add_flag == on) ? ADDON : ADDOFF, 0);

}

/* toggle display-clear */

void tog_clear()

{

	Opt.clear_flag = (Opt.clear_flag == on) ? off : on;
	stl_msg((char *) NULL, (char *) NULL,
	    (Opt.clear_flag == on) ? DCON : DCOFF, 0);

}

/* toggle highlighting */

void tog_highlight()

{

	if (Opt.high_flag == on)
		{
		Opt.high_flag = off;
		if (Crnt.ed.bmod_flag == on && Crnt.ed.hmod_flag == on)
			{
			Crnt.ed.hmod_flag = off;
			if (Ute.statline_flag == on)
				stl_clr(1);

			dispwin();
			stl_msg("buffer-modifications unchanged",
				    (char *) NULL, HIOFF, 0);
			}

		else
			{
			if (Fil[optind].hsearch_flag == on)
				{
/*				Fil[optind].hsearch_flag = off; */
				if (Ute.statline_flag == on)
					stl_clr(1);

				dispwin();
				}

			stl_msg((char *) NULL, (char *) NULL, HIOFF, 0);
			}

		}

		/* turn highlighting on and re-highlight */
	else
		{
		Opt.high_flag = on;
		if (Crnt.ed.bmod_flag == on)
			{
			Crnt.ed.hmod_flag = on;
			hm_dispwin();
			}

		else
			if (Fil[optind].hsearch_flag == on &&
			    !strncmp((char *) &Buf.pagebuf[Fil[optind].sfpos],
			    Fil[optind].srch.last_str, Fil[optind].srch.slen))
				hs_dispwin();

		stl_msg((char *) NULL, (char *) NULL, HION, 0);
		}

}

/* remove highlighting from display for r command */

void rem_highlight()

{

	if (Ute.statline_flag == on)
		stl_clr(1);

	if (Opt.high_flag == off)
		return;

	if (Fil[optind].hsearch_flag == on || Crnt.ed.hmod_flag == on)
		{
		dispwin();
		if (Crnt.ed.hmod_flag == on)
			{
			Crnt.ed.hmod_flag = off;
			stl_msg((char *) NULL, (char *) NULL, BMODINF, 0);
			}

		else
			Fil[optind].hsearch_flag = off;
		}

}

/* restore highlighting to display */

void res_highlight()

{

	if (Ute.statline_flag == on)
		stl_clr(1);

	if (Opt.high_flag == off)
		return;

	if (Crnt.ed.bmod_flag == on)
		{
		if (Crnt.ed.hmod_flag == off)
			{
			Crnt.ed.hmod_flag = on;
			hm_dispwin();
			}
		}

	else
		if (Fil[optind].search_flag == on &&
		    Fil[optind].hsearch_flag == off)
			if (!strncmp((char *) &Buf.pagebuf[Fil[optind].sfpos],
			    Fil[optind].srch.last_str, Fil[optind].srch.slen))
				{
				Fil[optind].hsearch_flag = on;
				hs_dispwin();
				}

}
