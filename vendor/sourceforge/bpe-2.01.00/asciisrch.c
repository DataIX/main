/* bpe - asciisrch.c */

#include "bpe.h"
#include "global.h"

/* search for ASCII string */

int find_string()

{

	static off_t re_search = 0L, old_filpos = 0L;
	static char last_string[BPE_SMALLBUFSIZE];

	int c, slen, stlen, found = 0;
	off_t searchpos, tmpos = Fil[optind].fpos;

	char *bptr, *cptr, *iptr, *sptr, inbuf[BPE_SMALLBUFSIZE];

	if (get_entry(inbuf, last_string, "ASCII string", NOPASC))
		return(1);

	searchpos = (Fil[optind].fpos == old_filpos) ? re_search : 0L;
	stlen = strlen(last_string);
	slen = stlen - 1;
	iptr = last_string;
	sptr = (char *) (last_string + 1);
	clr_command();
	stl_msg((char *) NULL, inbuf, STRSRCH, 0);
	while (found == 0)
		{
		for (c = Ute.edit_chars - searchpos,
		     bptr = (char *) (Buf.pagebuf + searchpos),
		     cptr = (char *) (Buf.pagebuf + searchpos + 1);
		     c >= stlen; c--, bptr++, cptr++, searchpos++)
			{
			if (*bptr != *iptr || strncmp(cptr, sptr, slen))
				continue;

			Fil[optind].fpos += searchpos;
			old_filpos = Fil[optind].fpos;
			pos_opts();
			re_search = old_filpos - Fil[optind].fpos + 1;
			old_filpos = Fil[optind].fpos;
			found = 1;
			break;
			}

		if (found == 0)
			{
			Fil[optind].fpos += searchpos;
			searchpos = 0L;
			}

		if (rdpage() == 0)
			{
			old_filpos = Fil[optind].fpos = tmpos;
			Fil[optind].sfpos = 0L;
			(void) rdpage();
			if (Fil[optind].search_flag == on)
				{
				Fil[optind].search_flag =
				    Fil[optind].hsearch_flag = off;
				if (Opt.high_flag == on)
					dispwin();
				}

			stl_msg((char *) NULL, inbuf, NOASC, 1);
			return(1);
			}

		refresh();
		}

	Crnt.ed.write_count = 0;
	Fil[optind].srch.slen = Fil[optind].slen = stlen;
	Fil[optind].srch.fpos = Fil[optind].fpos;
	Fil[optind].srch.sfpos = Fil[optind].sfpos;
	Fil[optind].srch.search_flag = Fil[optind].search_flag =
	Fil[optind].hsearch_flag = on;

	(void) strcpy(Fil[optind].srch.last_str, last_string);
	cp_page();
	stl_clr(1);
	disp((Opt.high_flag == on && Fil[optind].hsearch_flag == on) ? 1 : 0);
	if (Opt.add_flag == off)
		dispos();

	return(0);

}
