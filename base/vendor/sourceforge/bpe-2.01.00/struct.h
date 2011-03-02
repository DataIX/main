/* struct.h */

/* the options that can be set on the command-line or in the BPE
   environment variable - the comments following the flag1 variables
   indicate the DEFAULT actions/conditions */

struct options
	{
	int slines;		/* number of lines to scroll with + and - */
	int tpos;		/* position of target line in window */
	off_t offset;		/* start address for display of file */
	flag1 add_flag;		/* display cursor-position address during
				   edit and file-position address during
				   search */
	flag1 align_flag;	/* align on 16-byte boundaries */
	flag1 backup_flag;	/* do not do automatic backups */
	flag1 clear_flag;	/* do not clear screen when changing files */
	flag1 cursor_flag;	/* set cursor-position at edit start
				   following set-position/string-search */
	flag1 esc_flag;		/* ESC used as escape character in edit-modes
				   and data entry */
#ifdef HAVE_KEYPAD
	flag1 keypad_flag;	/* keypad enabled */
#endif
	flag1 inperr_flag;	/* sound bell on illegal char during edit
				   and string/address input */
	flag1 generr_flag;	/* sound bell on other errors */
	flag1 flow_flag;	/* XON/XOFF flow-control disabled */
	flag1 high_flag;	/* highlighting enabled */
#ifndef NOLOCK
	flag1 lock_flag;	/* create lockfile */
#endif
	flag1 msg_flag;		/* user's default message status */
	flag1 readonly_flag;	/* open all files in read/write mode */
	flag1 windef_flag;	/* follow terminal size at start-up */
	};

/* the three buffers maintained for editing, undoing, etc. */

struct buffers
	{
	unsigned char *pagebuf;		/* page buffer */
	unsigned char *tmpbuf;		/* temporary check buffer */
	unsigned char *filebuf;		/* original page buffer */
	};

/* used for all cursor movement in edit modes */

struct cursor
	{
	int cury;	/* current y position in edit windows */
	int curx;	/* current x position in edit windows */
	int edlen;	/* number of chars in each edit window */
	int rows;	/* number of rows in each edit window */
	int last_row;	/* number of columns in last row */
	off_t fpos;	/* position in current file - this is the same
			   value as fil[optind].fpos */
	};

/* the edit-status of the current file - the comments following the
   flag1 variables indicate the DEFAULT conditions */

struct edit
	{
	int fmod_flag;		/* has file been edited */
	int fmod_count;		/* file-write/restore count */
	int write_count;	/* writes-per-page count */
	flag1 bmod_flag;	/* buffer has not been edited */
	flag1 hmod_flag;	/* no highlighted buffer mods in current
				   display */
	};

/* information that's needed only for the current file - the comment
   following the flag2 variable indicates the DEFAULT condition */

struct current
	{
	int bytes;		/* length of read page */
	int fd;			/* file-descriptor for current file */
	struct edit ed;		/* buffer and file status info */
#ifndef NOLOCK
	flag2 lock_flag;	/* no lockfile created */
#endif
	};

/* a "search" structure is maintained for each file in the file list - the
   comment following the flag1 variable indicates the DEFAULT condition */

struct search
	{
	int slen;		/* length of search string */
	off_t fpos;		/* address of page containing search string */
	off_t sfpos;		/* relative-address of search string */
	char last_str[BPE_SMALLBUFSIZE]; /* last searched-for string */
	flag1 search_flag;	/* no successful search operation */
	};

/* an "xedit" structure is maintained for each file in the file list */

struct xedit
	{
	off_t fpos;		/* start-address of last-edited page */
	off_t sfpos;		/* relative-address after search/set-pos */
	};

/* a "files" structure is maintained for each file in the file list - the
   comments following the flag1 and flag3 variables indicate the DEFAULT
   conditions */

struct files
	{
	unsigned short ftype;	/* file-type */
	int status;		/* file-access status */
	int slen;		/* length of searched-for string */
	off_t fpos;		/* position in file */
	off_t flength;		/* length of file */
	off_t lpage;		/* page before end of file */
	off_t lfpos;		/* file-position after last set-pos */
	off_t markers[26];	/* address markers */
	off_t sfpos;		/* relative-address after search/set-pos */
	struct search srch;	/* string-search info */
	struct xedit xed;	/* last-edited page addresses */
#ifdef LARGE_OFF_T
	flag1 flen_isll_flag;	/* on if this file is > 'long'-bytes long */
				/* (can only happen on OS's where off_t is
				   defined as 'long long') */
#endif
	flag1 mark_flag;	/* no markers set */
	flag1 hsearch_flag;	/* no highlighted last-successfully-searched-for
				   string in current display - this flag is not
				   turned-off if highlighting is toggled */
	flag1 search_flag;	/* no last-successfully-searched-for string
				   in current display */
	flag1 write_flag;	/* file opened in read/write mode - this
				   provides a check that a file has been
				   forced to read-only */
	flag3 bak_status;	/* check backup-file status */
	};

/* all the rest of the stuff that really has no "place" in any of the
   other structures - the comment following the flag1 variable indicates
   the DEFAULT condition */

struct utils
	{
	int numfiles;		/* number of files in arglist */
	int edit_chars;		/* number of chars in edit window */
	int edit_lines;		/* number of lines in edit window */
	int bpe_lines;		/* lines in window used by bpe */
	int tfile1, tfile2;	/* indices (argv) for toggle-files */
	int xcancel;
	char *bpe_name;		/* program name from argv[0] (usually bpe) */
	char bakext[16];	/* backup-file extension */
	char cancelstr[16];
	char errmsg[BPE_SMALLBUFSIZE];	/* close() error message */
	flag1 statline_flag;	/* no message on status line */
	};
