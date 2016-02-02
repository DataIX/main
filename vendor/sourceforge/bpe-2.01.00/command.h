/* command.h for bpe 2.00.08 */

/* the following defines are for the keyboard commands acted on by
   command() to make it (relatively) easy to customise if you prefer
   a different layout */

#define ASCII_EDIT	'a'	/* ASCII-edit mode */
#define ADD_FILE	'A'	/* add file to file list */
#define START_OF_FILE1	'b'	/* goto beginning of file */
#define START_OF_FILE2	'g'	/* goto beginning of file */
#define MAKE_BACKUP	'B'	/* create backup file */
#define CLEAR_STATUS	'c'	/* clear status line */
#define TOGGLE_COFFSET	'C'	/* toggle cursor-offset */
#define RESTORE_HIGH	'd'	/* add and edit new file */
#define	TOGGLE_CLEAR	'D'	/* display-clear on/off */
#define END_OF_FILE1	'e'	/* display last page */
#define END_OF_FILE2	'G'	/* display last page */
#define ADD_EDIT_FILE	'E'	/* add and edit new file */
#define FIND_ASCII1	'f'	/* find ASCII string */
#define FIND_ASCII2	'/'	/* find ASCII string */
#define SELECT_FILE	'F'	/* display/select-from list */
#define	HEX_EDIT	'h'	/* edit in hex window */
#define	TOGGLE_HIGH	'H'	/* highlighting on/off */
#define JUMP_FILE_NUM	'j'	/* jump to file number */
#define JUMP_FILE_NAME	'J'	/* jump to file name */
#define CLEAR_MARKERS	'k'	/* clear markers */
#define LOCATE_HEX1	'l'	/* locate hex string */
#define LOCATE_HEX2	'\\'	/* locate hex string */
#define GOTO_LAST_STR	'L'	/* goto last string */
#define SET_MARKER_PAGE	'm'	/* set marker to current page */
#define SET_MARKER_ADD	'M'	/* set marker to address */
#define NEXT_PAGE1	'n'	/* display next page */
#define NEXT_PAGE2	' '	/* display next page */
#define NEXT_FILE1	'N'	/* move to next file */
#define NEXT_FILE2	'>'	/* move to next file */
#define NEXT_FILE3	'.'	/* move to next file */
#define GOTO_OFFSET	'o'	/* goto offset address */
#define	SHOW_OPTIONS	'O'	/* show option status */
#define PREV_PAGE1	'p'	/* display previous page */
#define PREV_FILE1	'P'	/* move to previous file */
#define PREV_FILE2	'<'	/* move to previous file */
#define PREV_FILE3	','	/* move to previous file */
#define SOFT_QUIT	'q'	/* quit */
#define HARD_QUIT1	'Q'	/* unconditional quit */
#define HARD_QUIT2	'Z'	/* unconditional quit */
#define REMOVE_HIGH	'r'	/* remove highlighting */
#define	SCREEN_REDRAW	'R'	/* redraw current screen */
#define SELECT_ADDRESS	's'	/* set current address */
#define GOTO_LAST_ADD	'@'	/* goto last current address */
#define SHOW_STATUS	'S'	/* show buffer/file status */
#define TOGGLE_FILES1	't'	/* toggle last two files */
#define TOGGLE_FILES2	'#'	/* toggle last two files */
#define SHOW_TOGGLE	'T'	/* show toggle filenames */
#define UNDO_BUFFER	'u'	/* undo changes */
#define UNDO_FILE	'U'	/* restore original buffer */
#define SHOW_VERSION	'V'	/* show version number/date */
#define WRITE_BUFFER	'w'	/* write modified buffer */
#define WRITE_NEW_FILE	'W'	/* write to new file */
#define GOTO_LAST_EDIT	'x'	/* goto last-edit page */
#define SHOW_MARKERS	'X'	/* display addresses */
#define TOGGLE_ADD_DISP 'Y'	/* address-display on/off */
#define QUIT_FROM_HELP	'z'
#define GOTO_MARKER1	'\''	/* goto marker */
#define GOTO_MARKER2	'`'	/* goto marker */
#define SCROLL_D_LINES	'+'	/* scroll forward n line(s) */
#define SCROLL_U_LINES	'-'	/* scroll back n line(s) */
#define SHELL_ESCAPE	'!'	/* shell escape */
#define SHOW_HELP	'?'	/* display help screens */

#define HARD_QUIT3	0x03	/* CTRL-C - this definition used only as a
					    synonym for Q or Z in command
					    mode */
#define HARD_QUIT4	0x04	/* CTRL-D - this definition used only as a
					    synonym for Q or Z in command
					    mode */
#define NEXT_PAGE3	0x06	/* CTRL-F - synonym for "n" and "space" */
#define PREV_PAGE2	0x02	/* CTRL-B - synoym for "p" */
