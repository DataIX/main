/* bpe - hexedit.c */

#include "bpe.h"
#include "global.h"
#include <ctype.h>

/* edit in hex window */

int edit_hex(ed_mode)
char *ed_mode;

{

	int c, inval, charnum = 0;

	flag1 ctrlv_flag = off;

	for (;;)
		{
		move(HEXY + Curset.cury, HEXX + Curset.curx * 3 + charnum);
		refresh();
		inval = getch();
		if (ctrlv_flag == on)
			{
			ctrlv_flag = off;
			he_print(1, c = Curset.cury * 16 + Curset.curx,
			    inval);
			cur_right();
			charnum = 0;
			continue;
			}

		switch (inval)
			{
			case K_DOWN:
#ifdef HAVE_KEYPAD
			case KEY_DOWN:
#endif
				cur_down();
				charnum = 0;
				break;

			case K_UP:
#ifdef HAVE_KEYPAD
			case KEY_UP:
#endif
				cur_up();
				charnum = 0;
				break;

			case K_LEFT:
#ifdef HAVE_KEYPAD
			case KEY_LEFT:
#endif
				if (!charnum)
					cur_left();

				charnum = 0;
				break;

			case K_RIGHT:
#ifdef HAVE_KEYPAD
			case KEY_RIGHT:
#endif
				cur_right();
				charnum = 0;
				break;

			case K_HOME:
#ifdef HAVE_KEYPAD
			case KEY_HOME:
#endif
				cur_home();
				charnum = 0;
				break;

			case REDRAW:
				redraw((WINDOW *) NULL);
				break;

			/* restore the two-character hex digit under the
			   cursor to its original (i.e. after the last
			   write) value and reflect the change in the
			   both windows */

			case DEL:
				c = Curset.cury * 16 + Curset.curx;
				he_print(0, c,
				    (int) (Buf.pagebuf[c] = Buf.tmpbuf[c]));
				cur_right();
				charnum = 0;
				break;

			case CTRLV:
				ctrlv_flag = on;
				break;

			case CTRLX:
				set_cmark(ed_mode);
				break;

			case CTRLE: /* toggle to ascii-edit */
				return(ASCII_EDIT);

				/* exit edit mode if any of the quit
				   characters */
			default:
				if (inval == CANCEL_1 || inval == CANCEL_2 ||
				    inval == Ute.xcancel)
					return(-1);

				do_hex(inval, &charnum);
				break;
			}
		}

}

void do_hex(inval, charnum)
int inval, *charnum;

{

	static int c, value;
	static char *hexvals = "0123456789ABCDEF";

	char *wkptr;

	inval = (islower(inval)) ? toupper(inval) : inval;
	if ((wkptr = strchr(hexvals, inval)) == NULL)
		{
		if (Opt.inperr_flag == on)
			(void) beep();

		return;
		}

	else
		inval = wkptr - hexvals;

	switch (*charnum)
		{
		case 0:
			he_print(1, c = Curset.cury * 16 + Curset.curx,
			    value = (inval << 4));
			*charnum = 1;
			break;

		case 1:
			he_print(1, c, value += inval);
			cur_right();
			*charnum = 0;
			break;
			}

}

void he_print(scode, c, value)
int scode, c, value;

{

	/* so_flag is used to check if standout() has been called as a few
	   curses/brain-damaged-terminal combinations puke if an attribute
	   is turned off when it hasn't been turned on */

	flag1 so_flag = off;

/*	if (scode)
		if ((Buf.pagebuf[c] = value) != Buf.tmpbuf[c] &&
		    Opt.high_flag == on)
			{
			so_flag = on;
			standout();
			} */

	if (scode && (Buf.pagebuf[c] = value) != Buf.tmpbuf[c] &&
	    Opt.high_flag == on)
		{
		so_flag = on;
		standout();
		}

	mvprintw(HEXY + Curset.cury, HEXX + Curset.curx * 3, "%02X", value);
	mvprintw(ASCY + Curset.cury, ASCX + Curset.curx, "%c",
	    (value < 0x20 || value > 0x7e) ? 0x2e : value);
	if (so_flag == on)
		{
		so_flag = off;
		standend();
		}

}
