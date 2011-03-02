/* bpe - buffer.c */

#include "bpe.h"
#include "global.h"

#include <memory.h>

/* most of the various buffer-manipulation/checking functions */

/* composite function to display new page read in from disk */

int dump(disp_code)
int disp_code;

{

	if (rdpage() == -1)
		return(-1);

	cp_page();
	disp(disp_code);
	if (Opt.add_flag == off)
		dispos();

	Crnt.ed.write_count = 0;
	return(0);

}

/* read new page from disk */

int rdpage()

{

	if (Opt.add_flag == on)
		dispos();

	if (lseek(Crnt.fd, Fil[optind].fpos, 0) == -1)
		stl_msg(sys_errlist[errno], (char *) NULL, LSERR, 1);

	else
/*		if ((Crnt.bytes = read(Crnt.fd, (char *) Buf.pagebuf,
		     Ute.edit_chars)) == -1)
			stl_msg(sys_errlist[errno], (char *) NULL, RDERR, 1); */
		if ((Crnt.bytes = read(Crnt.fd, (unsigned char *) Buf.pagebuf,
		     Ute.edit_chars)) == -1)
			stl_msg(sys_errlist[errno], (char *) NULL, RDERR, 1);

	return(Crnt.bytes);

}

/* compare pagebuf with tmpbuf on exiting edit mode to see if any
   changes have been made */

void cmp_page()

{

	if (memcmp((char *) Buf.tmpbuf, (char *) Buf.pagebuf, Crnt.bytes))
		{
		Fil[optind].xed.fpos = Fil[optind].fpos;
		Fil[optind].xed.sfpos = Fil[optind].sfpos;
		Crnt.ed.bmod_flag = on;
		if (Opt.high_flag == on)
			Crnt.ed.hmod_flag = on;
		}

	else
		Crnt.ed.bmod_flag = Crnt.ed.hmod_flag = off;

}

/* copy page buffer to temporary buffers (used for checking changes,
   restoring, etc.) */

void cp_page()

{

	register int c;

	for (c = 0; c < Crnt.bytes; c++)
		Buf.filebuf[c] = Buf.tmpbuf[c] = Buf.pagebuf[c];

}

/* undo changes that have NOT be written to disk */

void undo()

{

	if (Crnt.ed.bmod_flag == on)
		{
		(void) memcpy((char *) Buf.pagebuf, (char *) Buf.tmpbuf,
			       Crnt.bytes);
		Crnt.ed.bmod_flag = Crnt.ed.hmod_flag = off;
		dispwin();
		stl_msg((char *) NULL, (char *) NULL, RESBMOD, 0);
		}

	else
		stl_msg((char *) NULL, (char *) NULL, NOBMOD, 1);

}

/* undo changes that HAVE been written to disk */

void restore()

{

	if (Crnt.ed.write_count)
		{
		(void) memcpy((char *) Buf.pagebuf, (char *) Buf.filebuf,
			       Crnt.bytes);
		Crnt.ed.fmod_count--;
		Crnt.ed.write_count = 0;
		Crnt.ed.bmod_flag = Crnt.ed.hmod_flag = off;
		if (!wr_page())
			{
			dispwin();
			stl_msg((char *) NULL, (char *) NULL, RESFMOD, 0);
			}
		}

	else
		stl_msg((char *) NULL, (char *) NULL, NOFMOD, 1);

}

/* reset parameters after successful page write */

void wr_buf()

{

	if (Crnt.ed.bmod_flag == off)
		stl_msg((char *) NULL, (char *) NULL, NOWBMOD, 1);

	else
		if (!wr_page())
			{
			Crnt.ed.fmod_flag++;
			if (!Crnt.ed.write_count++)
				Crnt.ed.fmod_count++;

			if (Crnt.ed.hmod_flag == on)
				{
				Crnt.ed.hmod_flag = off;
				dispwin();
				}

			Crnt.ed.bmod_flag = off;
			stl_msg((char *) NULL, (char *) NULL, WBMOD, 0);
			}

}

/* write page to disk */

int wr_page()

{

	if (lseek(Crnt.fd, Fil[optind].fpos, 0) == -1)
		{
		stl_msg(sys_errlist[errno], (char *) NULL, LSERR, 1);
		return(-1);
		}

	if (write(Crnt.fd, (char *) Buf.pagebuf,
	    (unsigned) Crnt.bytes) != Crnt.bytes)
		{
		stl_msg(sys_errlist[errno], (char *) NULL, WRERR, 1);
		return(-1);
		}

	(void) memcpy((char *) Buf.tmpbuf, (char *) Buf.pagebuf, Crnt.bytes);
	return(0);

}

/* check if buffer-mods have been made and not written before
   changing file-position, quitting, etc. */

int ck_mod()

{

	if (Crnt.ed.bmod_flag == off)
		{
		if (Ute.statline_flag == on)
			stl_clr(1);

		return(0);
		}

	stl_msg((char *) NULL, (char *) NULL, NOWRITE, 1);
	return(1);

}

/* set up the Fil (plus srch and xed) arrays of structures */

int set_structs()

{

	int c;

	if ((Fil = (struct files *) calloc((unsigned) MAXFILES,
	   (unsigned) sizeof (struct files))) == NULL)
		{
		(void) fprintf(stderr,"%s: cannot allocate memory: %s\n",
		    Ute.bpe_name, sys_errlist[errno]);
		(void) fflush(stderr);
		return(1);
		}

	/* this initialisation shouldn't be necessary but you know
	   how it is with rampant paranoia ... */

	for (c = 0; c < MAXFILES; c++)
		Fil[c].status = 0;

	return(0);

}

/* set up the Buf.pagebuf, Buf.tmpbuf and Buf.filebuf buffers */

int set_buffers()

{

	if ((Buf.pagebuf =
	    (unsigned char *) malloc((unsigned) Ute.edit_chars)) == NULL ||
	    (Buf.tmpbuf =
	    (unsigned char *) malloc((unsigned) Ute.edit_chars)) == NULL ||
	    (Buf.filebuf =
	    (unsigned char *) malloc((unsigned) Ute.edit_chars)) == NULL)
		{
		(void) sprintf(Ute.errmsg, "cannot allocate buffer memory: %s",
		    sys_errlist[errno]);
		return(1);
		}

	return(0);

}

/* set up the file-name arrays */

void set_farrays(argv, argf)
char **argv, **argf;

{

	int c;

	for (c = 0; c < Ute.numfiles; c++)
		*argf++ = *argv++;

}

/* add new filename to file-list array */

int add_farray(argf, filename)
char **argf, *filename;

{

	if ((argf[Ute.numfiles] = (char *) malloc((unsigned)
	    (strlen(filename) + 1))) == NULL)
		{
		/* cannot allocate memory message */
		stl_msg(sys_errlist[errno], (char *) NULL, MEMERR, 1);
		return(1);
		}

	return(0);

}
