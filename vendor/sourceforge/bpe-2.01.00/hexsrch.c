/* bpe - hexsrch.c */

/* Added by Jon LaBadie jon@jonlab.UUCP
   to implement the H (hex search) option */

/* modified by ran to fix repeat-search bug, to add BS and DEL capability
   with get_str(), to stop searches going beyond EOF and to fix long/int
   mis-match that asssumed that longs and ints were the same */

#include "bpe.h"
#include "global.h"

#include <ctype.h>

/* find hex "string" */

int find_hex()

{

	static char last_string[BPE_SMALLBUFSIZE];
	static off_t re_search = 0, old_filpos = 0;

	int c, found = 0, slen, stlen;
	off_t searchpos, tmpos = Fil[optind].fpos;
	char *bptr, *hptr, *pptr, *sptr, inbuf[BPE_SMALLBUFSIZE];
	unsigned char hexstr[BPE_SMALLBUFSIZE];

	if (get_entry(inbuf, last_string, "Hex string", NOPHEX))
		return(1);

	clr_command();
	if (strlen(inbuf) % 2)
		{
		inbuf[0] = '0';
		(void) strcpy(&inbuf[1], last_string);
		}

	if ((stlen = cvt_str(inbuf, hexstr)) < 0)
		{
		stl_msg((char *) NULL, inbuf, ILLHEX, 1);
		return(1);
		}

	slen = stlen - 1;
	hptr = (char *) (hexstr + 1);
	sptr = (char *) hexstr;
	searchpos = (Fil[optind].fpos == old_filpos) ? re_search : 1;
	stl_msg((char *) NULL, inbuf, STRSRCH, 0);
	while (found == 0)
		{
		for (c = Ute.edit_chars - searchpos,
		     bptr = (char *) (Buf.pagebuf + searchpos),
		     pptr = (char *) (Buf.pagebuf + searchpos + 1);
		     c >= stlen; c--, bptr++, pptr++, searchpos++)
			{
			if (*bptr != *sptr || memcmp(pptr, hptr, slen))
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
			searchpos = 0;
			}

		if (rdpage() == 0)
			{
			old_filpos = Fil[optind].fpos = tmpos;
			Fil[optind].sfpos = 0;
			(void) rdpage();
			if (Fil[optind].search_flag == on)
				{
				Fil[optind].search_flag =
				    Fil[optind].hsearch_flag = off;
				if (Opt.high_flag == on)
					dispwin();
				}

			stl_msg((char *) NULL, inbuf, NOHEX, 1);
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

	(void) strcpy(Fil[optind].srch.last_str, (char *) hexstr);
	cp_page();
	stl_clr(1);
	disp((Opt.high_flag == on && Fil[optind].hsearch_flag) ? 1 : 0);
	if (Opt.add_flag == off)
		dispos();

	return(0);

}

/* hex_2_byte returns the integer value of a byte represented by the two
   characters passed to it. For example, passed an '8' and an 'A', it will
   return 170 (8 * 16 + 10). Returns -1 on any error. */

int hex_2_byte(a, b)
char a, b;

{

	int v;

	if (!isxdigit(a) || !isxdigit(b))
		return(-1);
	
	a = (islower(a)) ? toupper(a) : a;
	b = (islower(b)) ? toupper(b) : b;

	v = (isdigit(a)) ? (a - '0') * 16 : (a - 'A' + 10) * 16;
	return(v += (isdigit(b)) ? (b - '0') : (b - 'A' + 10));
	
}

/* Take two strings as arguments. First is a sequence of hex digit pairs.
   Each pair is to be converted into the equivalent unsigned 1 byte value
   and stored in the second array. */

int cvt_str(s, h)
char *s;
unsigned char *h;

{

	int c, len = 0;

	while (*s)
		{
		if (*(s + 1) == NUL)
			return(-1);

		if ((c = hex_2_byte(*s, *(s + 1))) >= 0)
			*h++ = c;

		else
			return(-1);

		len++;
		s += 2;
		}

	*h = NUL;
	return(len);

}
