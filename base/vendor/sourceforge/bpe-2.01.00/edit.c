/* bpe - edit.c */

#include "bpe.h"
#include "global.h"

/* edit in and toggle between ASCII/Hex windows */

void do_edit(inval)
int inval;

{

	char *ed_mode[2];

	ed_mode[0] = "ASCII";
	ed_mode[1] = "Hex";

	set_edit();
	for (;;)
		switch (inval)
			{
			case ASCII_EDIT:
				set_hdr(ed_mode[0]);
				inval = edit_ascii(ed_mode[0]);
				break;

			case HEX_EDIT:
				set_hdr(ed_mode[1]);
				inval = edit_hex(ed_mode[1]);
				break;

			default:
				cmp_page();
				stl_clr(0);
				clr_command();
				dispos();
				return;
			}

}

/* set initial cursor-position (and a couple of other things) for editing */

void set_edit()

{

	if (Opt.cursor_flag == on && Fil[optind].sfpos)
		{
		Curset.cury = (int) Fil[optind].sfpos / 16;
		Curset.curx = (int) Fil[optind].sfpos % 16;
		}

	else
		Curset.cury = Curset.curx = 0;

	Curset.edlen = Crnt.bytes - 1;

	/* set the number of lines in the page and the number of characters
	   in the last line to allow for cursor wrap on "short" pages */

	Curset.rows = Crnt.bytes / 16;
	if ((Curset.last_row = (Crnt.bytes % 16)) == 0)
		Curset.rows--;

	else
		Curset.last_row--;

	if (Fil[optind].hsearch_flag == on)
		{
		Fil[optind].hsearch_flag = off;
		dispwin();
		}

	Curset.fpos = Fil[optind].fpos;
	clr_command();
	if (Opt.add_flag == on)
		c_dispos();

	stl_msg((char *) NULL, (char *) NULL, EDINF, 0);

}

void set_hdr(ed_mode)
char *ed_mode;

{

	mvprintw(2, 0, "%s-edit  ", ed_mode);

}
