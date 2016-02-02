/* bpe - cursor.c */

#include "bpe.h"
#include "global.h"

/* functions for cursor movement - arrow keys, character-edit and
   character-undo wrap at all window boundaries */

void cur_up()

{

	if (Curset.cury)
		Curset.cury--;

	else
		if ((Curset.cury = Curset.rows) * 16 + Curset.curx >
		     Curset.edlen)
			Curset.curx = Curset.last_row;

	if (Opt.add_flag == on)
		uc_dispos();

}

void cur_down()

{


	if (Curset.cury < Curset.rows)
		{
		if (++Curset.cury * 16 + Curset.curx > Curset.edlen)
			Curset.curx = Curset.last_row;
		}

	else
		Curset.cury = 0;

	if (Opt.add_flag == on)
		uc_dispos();

}

/* cur_right() is also used for cursor-movement after edit and
   and single-character-undo operations */

void cur_right()

{

	if (Curset.curx < 15)
		{
		if (Curset.cury * 16 + ++Curset.curx > Curset.edlen)
			Curset.curx = Curset.cury = 0;
		}

	else
		{
		Curset.curx = 0;
		if (Curset.cury < Curset.rows)
			Curset.cury++;

		else
			Curset.cury = 0;
		}

	if (Opt.add_flag == on)
		uc_dispos();

}

void cur_left()

{

	if (Curset.curx)
		Curset.curx--;

	else
		{
		Curset.curx = 15;
		if (Curset.cury)
			Curset.cury--;

		else
			if ((Curset.cury = Curset.rows) * 16 +
			     Curset.curx > Curset.edlen)
				Curset.curx = Curset.last_row;
		}

	if (Opt.add_flag == on)
		uc_dispos();

}

/* at least this one was easy to calculate! */

void cur_home()

{

	Curset.curx = Curset.cury = 0;
	if (Opt.add_flag == on)
		uc_dispos();
/*		uc_dispos(Curset.fpos); */

}

/* toggle cursor-offset */

void tog_offset()

{

	Opt.cursor_flag = (Opt.cursor_flag == on) ? off : on;
	stl_msg((char *) NULL, (char *) NULL,
	    (Opt.cursor_flag == on) ? CPON : CPOFF, 0);

}
