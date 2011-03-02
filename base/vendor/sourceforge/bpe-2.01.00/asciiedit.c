/* bpe - asciiedit.c */

#include "bpe.h"
#include "global.h"

/* edit in ASCII window */

int edit_ascii(ed_mode)
char *ed_mode;

{

	int c, d, inval;

	flag1 ctrlv_flag = off;

	/* The main loop - in fact, THE loop - where everything is done */

	for (;;)
		{
		move(ASCY + Curset.cury, ASCX + Curset.curx);
		refresh();
		inval = getch();
		if (ctrlv_flag == on)
			{
			ctrlv_flag = off;
			ae_print(1, inval);
			continue;
			}

		switch (inval)
			{
			case K_DOWN:
#ifdef HAVE_KEYPAD
			case KEY_DOWN:
#endif
				cur_down();
				break;

			case K_UP:
#ifdef HAVE_KEYPAD
			case KEY_UP:
#endif
				cur_up();
				break;

			case K_LEFT:
#ifdef HAVE_KEYPAD
			case KEY_LEFT:
#endif
				cur_left();
				break;

			case K_RIGHT:
#ifdef HAVE_KEYPAD
			case KEY_RIGHT:
#endif
				cur_right();
				break;

			case K_HOME:
#ifdef HAVE_KEYPAD
			case KEY_HOME:
#endif
				cur_home();
				break;

			case REDRAW:
				redraw((WINDOW *) NULL);
				break;

			case DEL:
				/* restore the single ASCII character under
				   the cursor to its original (i.e. after the
				   last write) value */

				c = Curset.cury * 16 + Curset.curx;
				d = Buf.pagebuf[c] = Buf.tmpbuf[c];
				ae_print(0, d);
				break;

			case CTRLX:
				/* set marker to address represented by
				   cursor position */

				set_cmark(ed_mode);
				break;

			case CTRLV:
				/* escape next char, i.e. accept even if not
				   an ASCII-printing char */

				ctrlv_flag = on;
				break;

			case CTRLE:
				/* toggle to hex-edit */
				return(HEX_EDIT);

			default:
				/* exit edit mode if any of the quit
				   characters */

				if (inval == CANCEL_1 || inval == CANCEL_2 ||
				    inval == Ute.xcancel)
					return(-1);

				/* change character if ASCII-printing char */

				if (inval > 0x1f && inval < 0x7f)
					ae_print(1, inval);

				else
					if (Opt.inperr_flag == on)
						(void) beep();

				break;
			}
		}
}

/* change character in both hex and ascii windows - highlight if highlighting
   turned-on, scode == 1 and c is different from original - unhighlight if
   previously changed and c is same as original */

void ae_print(scode, c)
int scode, c;

{

	int d;

	/* so_flag is used to check if standout() has been called as a few
	   curses/brain-damaged-terminal combinations puke if an attribute
	   is turned off when it hasn't been turned on */

	flag1 so_flag = off;

	if (scode)
		{
		d = Curset.cury * 16 + Curset.curx;
		if ((Buf.pagebuf[d] = c) != Buf.tmpbuf[d] &&
		    Opt.high_flag == on)
			{
			so_flag = on;
			standout();
			}
		}

	printw("%c", (c < 0x20 || c > 0x7e) ? 0x2e : c);
	mvprintw(HEXY + Curset.cury, HEXX + Curset.curx * 3, "%02X", c);
	if (so_flag == on)
		standend();

	cur_right();

}
