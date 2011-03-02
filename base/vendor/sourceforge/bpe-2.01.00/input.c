/* bpe - input.c */

#include "bpe.h"
#include "global.h"

/* get a string from the terminal under curses with cbreak, noecho and
   nonl still on (assuming that they've been thus set) and with BS, DEL,
   simple command-line editing and cancel capability - this is aimed at
   being a fairly-generic routine which is why there are some
   seemingingly-superfluous defines, comments etc.

   s1 is the target string, win is the current curses window (the calling
   function should pass (WINDOW *) NULL if stdscr is in use), cury and
   curx are self-explanatory, count is the length of any string displayed
   from the curx position while len is the maximum length of the string -
   this is not necessarily the size of the array pointed to by s1 and the
   calling function must ensure that s1 is large enough for len + 1

   Characters are placed in s1 using the mvwinch() function after a <CR>
   is entered.

   ed_code == 0 indicates that previously-entered-string-editing is not
   available at the current prompt.

   sc_code != 0 indicates that "single-key input" is in effect, i.e.
   return after first legal character.

   This is a somewhat-kludged method of getting everything to work but
   it's one that I think is reasonably sure to work under most
   implementations of curses (ran) */

int get_str(s1, win, cury, curx, count, len, ed_code, sc_code)
char *s1;
WINDOW *win;
int cury, curx, count, len, ed_code, sc_code;

{

#ifndef CANCEL_1
#define CANCEL_1	0x03	/* set as required */
#endif

#ifndef CANCEL_2
#define CANCEL_2	0x04	/* set as required */
#endif

#ifndef CANCEL_3
#define CANCEL_3	0x33	/* set as required */
#endif

#ifndef CTRLE
#define CTRLE		0x05	/* set as required */
#endif

#ifndef REDRAW
#define REDRAW		0x12	/* set as required or remove - see below */
#endif

#ifndef BS
#define BS		0x08
#endif

#ifndef DEL
#define	DEL		0x7f
#endif

#ifndef K_LEFT
#define K_LEFT		0x08	/* CTRL-H */
#endif

#ifndef K_RIGHT
#define K_RIGHT		0x0c	/* CTRL-L */
#endif

	int c, position = 0, cur_start = curx;

	WINDOW *input_scr;

	input_scr = (win) ? win : stdscr;

	wmove(input_scr, cury, curx);
	wrefresh(input_scr);
	for (;;)
		switch (c = getch())
			{
			case BS:
/*			case K_LEFT: */
#ifdef HAVE_KEYPAD
			case KEY_LEFT:
#endif
				if (position)
					{
					position--;
					wmove(input_scr, cury, --curx);
					wrefresh(input_scr);
					}

				else
					if (Opt.inperr_flag == on)
						(void) beep();

				break;

			case DEL:
				if (!count && !position)
					{
					if (Opt.inperr_flag == on)
						(void) beep();

					break;
					}

				if (position)
					{
					position--;
					mvwdelch(input_scr, cury, --curx);
					}

				else
					wdelch(input_scr);

				count--;
				wrefresh(input_scr);
				break;

			case K_RIGHT:
#ifdef HAVE_KEYPAD
			case KEY_RIGHT:
#endif
				if (!count || position == count)
					{
					if (Opt.inperr_flag == on)
						(void) beep();

					break;
					}

				else
					{
					position++;
					wmove(input_scr, cury, ++curx);
					wrefresh(input_scr);
					}

				break;

			/* this is somewhat bpe-specific - the calling
			   function - in this case, get_entry() - pulls-in
			   a previously-entered string (stored in a static
			   char array) and prints it on the command line
			   for editing using edit_entry() */

			case CTRLE:
				if (ed_code)
					{
					s1[0] = c;
					s1[1] = NUL;
					return(0);
					}

				/* no command-line editing allowed here - the
				   test for stdscr is bpe-specific as status
				   line messages are not wanted on the
				   file-list display */

				if (input_scr == stdscr)
					{
					stl_msg((char *) NULL, (char *) NULL,
					    NOLEE, 1);
					wmove(input_scr, cury, curx);
					wrefresh(input_scr);
					}

				else
					if (Opt.inperr_flag == on)
						(void) beep();

				break;

			/* this is a handy facility to have but depends, of
			   course, on your application - comment-out if you
			   don't need it */

			case REDRAW:
				redraw(input_scr);
				break;

			/* put charcters in display into s1 */

			case '\n':
			case '\r':
				for (c = 0; c < count; c++)
					*s1++ = mvwinch(input_scr, cury,
					    cur_start++);

				*s1 = NUL;
				return(0);

			default:
				if (c == CANCEL_1 || c == CANCEL_2 ||
				    c == Ute.xcancel)
					return(1);

				/* this is bpe-specific - it gives
				   single-character input for get_address() for
				   just the noted chars while still allowing
				   normal string entry when called from other
				   functions */

				if (sc_code && !count && (c == 'B' || c == 'C'
				    || c == 'E' || c == 'G' || c == 'L'
				    || c == 'O' || c == 'S' || c == 'X' ||
				    (c > 0x60 && c < 0x7b)))
					{
					*s1++ = c;
					*s1 = NUL;
					return(0);
					}

				if (c > 0x1f && c < 0x7f && count < len)
					{
					if (position < count)
						{
						winsch(input_scr, c);
						wmove(input_scr, cury, ++curx);
						}

					else
						{
						waddch(input_scr, c);
						curx++;
						}

					count++;
					position++;
					wrefresh(input_scr);
					}

				else
					if (Opt.inperr_flag == on)
						(void) beep();

				break;
			}

}

/* get_entry() and edit_entry() are generic, but bpe-specific, functions that
   are a "front-end" for get_str() allowing access to previously-entered
   string but are not part of get_str() - however, such a front end will
   be needed in any program that uses editing with get_str() */

int get_entry(inbuf, last_entry, prompt, msg_code)
char *inbuf, *last_entry, *prompt;
int msg_code;

{

	int l_len = strlen(last_entry), p_len = strlen(prompt);

	for (;;)
		{
		clr_command();
		printw("%s:", prompt);
		if (get_str(inbuf, (WINDOW *) NULL, 2, p_len + 2,
		    0, BPE_EDITLENGTH, 1, 0))
			{
			if (Ute.statline_flag == on)
				stl_clr(0);

			return(1);
			}

		if (inbuf[0] == NUL || inbuf[0] == CTRLE)
			if (last_entry[0] == NUL)
				{
				/* no previous entry message */
				stl_msg((char *) NULL, (char *) NULL,
				    msg_code, 1);
				continue;
				}

			else
				{
/*				clr_command(); */
				if (inbuf[0] == NUL)
					{
					(void) strcpy(inbuf, last_entry);
					return(0);
					}

				switch (edit_entry(inbuf, last_entry, prompt,
				    l_len, p_len))
					{
					case 0:
						continue;

					case 1:
						return(1);

					default:
						break;
					}

				}
		break;
		}

	(void) strcpy(last_entry, inbuf);
	return(0);

}

/* edit_entry is separate partly because it made a couple of things a little
   easier but mainly because it is called directly by functions that cannot
   use get_entry() */

int edit_entry(inbuf, last_entry, prompt, l_len, p_len)
char *inbuf, *last_entry, *prompt;
int l_len, p_len;

{

	for (;;)
		{
		if (Ute.statline_flag == on)
			stl_clr(0);

		/* clr_command() simply clears the line that is to be used - in
		   bpe it is "hard-wired" to curx == 2 as follows:

		void clr_command()

		{

			move(2, 0);
			clrtoeol();

		}

		If it is not used, the following printw() will have to be
		replaced by mvprintw() or the cursor otherwise positioned */

		clr_command();
		printw("%s: %s", prompt, last_entry);
		if (get_str(inbuf, (WINDOW *) NULL, 2, p_len + 2,
		    l_len, BPE_EDITLENGTH, 1, 0))
			{
			if (Ute.statline_flag == on)
				stl_clr(0);

			return(1);
			}

		switch (inbuf[0])
			{
			case NUL:
				stl_msg((char *) NULL, (char *) NULL, NLSTR, 1);
				return(0);

			case CTRLE:
				clr_command();
				continue;

			default:
				return(2);
			}
		}
}

/* This is a generic, but bpe-specific, function for user-confirmation
   and is not part of get_str() */

int confirm(msg_code)
int msg_code;

{

	int c;

	clr_command();
	switch(msg_code)
		{
		case 0:
			printw("Confirm QUIT");
			break;

		case 1:
			printw("Replace");
			break;

		case 2:
			printw("Replace/Append/Quit (R/A/Q) <Q>? ");
			refresh();
			return(getch());
		}

	printw(" (Y/N) <N>? ");
	refresh();
	if ((c = getch()) == 'y' || c == 'Y')
		return(1);

	return(0);

}
