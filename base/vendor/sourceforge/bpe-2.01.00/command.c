/* bpe - command.c */

#include "bpe.h"
#include "global.h"

/* This is the main user interface */

int command(argf)
char **argf;

{

	int inval, tmpval = -1;

	refresh();
	if (get_file(argf[optind]))
		dispos();

	for (;;)
		{
		if (tmpval != -1)
			{
			inval = tmpval;
			tmpval = -1;
			}

		else
			{
			mvprintw(2, 0, "COMMAND: ");
			refresh();
			inval = getch();
			}

		switch (inval)
			{
			case ASCII_EDIT:	/* ASCII-edit mode */
				if (!ck_file(argf[optind]) && !ck_rd())
					do_edit(inval);

				break;

			case ADD_FILE:	/* add file to file list */
				if (sel_new(0, argf) != 2)
					reset_dispos();

				break;

			/* the use of the home key from command-mode to
			   return to the start of the file isn't documented
			   but I found it useful and others will, no doubt,
			   discover it (ran) */
			case START_OF_FILE1:
			case START_OF_FILE2:
			case K_HOME:
#ifdef HAVE_KEYPAD
			case KEY_HOME:
#endif
				if (!ck_file(argf[optind]) && !ck_mod())
					first_page();

				break;

			case MAKE_BACKUP:	/* create backup file */
				if (!ck_file(argf[optind]))
					do_backup(argf[optind]);

				break;

			case CLEAR_STATUS:	/* clear status line */
				stl_clr(0);
				break;

			case TOGGLE_COFFSET:	/* toggle cursor-offset */
				tog_offset();
				break;

			case RESTORE_HIGH:	/* display highlighting */
				res_highlight();
				break;

			case TOGGLE_CLEAR:	/* toggle screen clear */
				tog_clear();
				break;

			case END_OF_FILE1: /* jump to last page of file */
			case END_OF_FILE2:
				if (!ck_file(argf[optind]) && !ck_mod())
					last_page();

				break;

			case ADD_EDIT_FILE:	/* add file to list and edit */
				if (!ck_mod())
					if (sel_new(1, argf) == 1)
						reset_dispos();

				break;

			case FIND_ASCII1: 	/* find ASCII string */
			case FIND_ASCII2:
				if (!ck_file(argf[optind]) && !ck_mod())
					if (find_string())
						reset_dispos();

				break;

			case SELECT_FILE:	/* select from file list */
				if (!ck_mod())
					if (sel_file(argf))
						return(1);

				break;

			case HEX_EDIT:	/* hex-edit mode */
				if (!ck_file(argf[optind]) && !ck_rd())
					do_edit(inval);

				break;

			case TOGGLE_HIGH:	/* toggle highlighting */
				tog_highlight();
				break;

			case JUMP_FILE_NUM:	/* jump to file number */
			case JUMP_FILE_NAME:	/* jump to file name */
				if (!ck_mod())
					if (do_jump(inval, argf))
						return(1);

				break;

			case CLEAR_MARKERS:	/* clear marker(s) */
				if (!ck_marks())
					{
					init_marks();
					stl_msg((char *) NULL, (char *) NULL,
					    CLRMARK, 0);
					}

				break;

			case LOCATE_HEX1:	/* locate hex "string" */
			case LOCATE_HEX2:
				if (!ck_file(argf[optind]) && !ck_mod())
					if (find_hex())
						reset_dispos();

				break;

			case GOTO_LAST_STR: /* goto last searched-for string */
				if (!ck_file(argf[optind]) && !ck_mod())
					last_search();

				break;

			case SET_MARKER_PAGE: /* set marker to current page */
				if (!ck_file(argf[optind]))
					set_mark();

				break;

			case SET_MARKER_ADD: /* set marker to user address */
				if (!ck_file(argf[optind]))
					set_amark();

				reset_dispos();
				break;

			case NEXT_PAGE1:	/* display next page */
			case NEXT_PAGE2:
			case NEXT_PAGE3:
				if (!ck_file(argf[optind]) && !ck_mod())
					next_page();

				break;

			case NEXT_FILE1:  /* edit next file in file list */
			case NEXT_FILE2:
			case NEXT_FILE3:
				if (!ck_mod())
					if (next_file(argf))
						return(1);

				break;

			case GOTO_OFFSET:	/* goto command-line offset */
				if (!ck_file(argf[optind]) && !ck_mod())
					goto_offset();

				break;

			case SHOW_OPTIONS:	/* show options */
				show_options();
				break;

			case PREV_PAGE1:	/* display previous page */
			case PREV_PAGE2:
				if (!ck_file(argf[optind]) && !ck_mod())
					prev_page();

				break;

			case PREV_FILE1: /* edit previous file in file list */
			case PREV_FILE2:
			case PREV_FILE3:
				if (!ck_mod())
					if (prev_file(argf))
						return(1);

				break;

			case SOFT_QUIT:	/* quit bpe */
				if (ck_mod())
					break;

				return(finish(argf[optind], 1));

			case HARD_QUIT1:	/* quit bpe unconditionally */
			case HARD_QUIT2:
			case HARD_QUIT3:
			case HARD_QUIT4:
				if (!ck_mod() || confirm(0))
					return(finish(argf[optind], 0));

				stl_clr(0);
				reset_dispos();
				break;

			case REMOVE_HIGH:	/* remove highlighting */
				rem_highlight();
				break;

			case SCREEN_REDRAW:	/* redraw entire screen */
			case REDRAW:
				redraw((WINDOW *) NULL);
				break;

			case SELECT_ADDRESS: /* select and jump to address */
				if (!ck_file(argf[optind]) && !ck_mod())
					set_address();

				break;

			case GOTO_LAST_ADD:	/* go to last set-address */
				if (!ck_file(argf[optind]) && !ck_mod())
					last_set_address();

				break;

			case SHOW_STATUS: /* show file-type/edit-status */
				show_status();
				break;

			case TOGGLE_FILES1: /* toggle between last two files */
			case TOGGLE_FILES2:
				if (!ck_mod())
					if (do_toggle(argf))
						return(1);

				break;

			case SHOW_TOGGLE:	/* show toggle file names */
				show_toggle(argf);
				break;

			case UNDO_BUFFER:	/* undo buffer mods */
				if (!ck_file(argf[optind]) && !ck_rd())
					undo();

				break;

			case UNDO_FILE:	/* undo mods written to disk */
				if (!ck_file(argf[optind]) && !ck_rd())
					restore();

				break;

			case SHOW_VERSION:	/* show version/version-date */
				stl_msg((char *) NULL, (char *) NULL,
				    VERINF, 0);
				break;

			case WRITE_BUFFER:	/* write buffer to disk */
				if (!ck_file(argf[optind]) && !ck_rd())
					wr_buf();

				break;

			case WRITE_NEW_FILE: /* write to user-selected file */
				if (!ck_file(argf[optind]))
					wrtofile(argf);

				reset_dispos();
				break;

			case GOTO_LAST_EDIT:	/* goto last-edited page */
				if (!ck_file(argf[optind]) && !ck_mod())
					last_edit();

				break;

			case SHOW_MARKERS:	/* show marks */
				if (!ck_file(argf[optind]))
					tmpval = show_address(argf[optind]);

				break;

			case TOGGLE_ADD_DISP:	/* toggle address display  */
				tog_address();
				break;

			case QUIT_FROM_HELP:	/* quit bpe from help */
				if (Ute.statline_flag == on)
					stl_clr(0);

				(void) beep();
				if (confirm(0))
					return(finish(argf[optind], 0));

				reset_dispos();
				break;

			case GOTO_MARKER1:	/* goto marker */
			case GOTO_MARKER2:
				if (!ck_file(argf[optind]) && !ck_mod())
					goto_mark();

				break;

			case SCROLL_D_LINES:	 /* scroll down n lines */
				if (!ck_file(argf[optind]) && !ck_mod())
					down_scroll(Opt.slines);

				break;

			case K_DOWN:	 /* scroll down 1 line */
#ifdef HAVE_KEYPAD
			case KEY_DOWN:
#endif
				if (!ck_file(argf[optind]) && !ck_mod())
					down_scroll(1);

				break;

			case SCROLL_U_LINES:	 /* scroll up n lines */
				if (!ck_file(argf[optind]) && !ck_mod())
					up_scroll(Opt.slines);

				break;

			case K_UP:	 /* scroll up 1 line */
#ifdef HAVE_KEYPAD
			case KEY_UP:
#endif
				if (!ck_file(argf[optind]) && !ck_mod())
					up_scroll(1);

				break;

			case CTRLE:	/* no command-line editing here */
				stl_msg((char *) NULL, (char *) NULL, NOLEE, 1);
				break;

			case CTRLZ:	/* job-control */
#ifdef JOBCONTROL
				do_job();
				break;
#endif
			case SHELL_ESCAPE:	/* shell escape */
				do_shell();
				break;

			case SHOW_HELP:	/* help screens */
				tmpval = help();
				break;

			default:	/* illegal command */
				stl_msg((char *) NULL, (char *) NULL,
				    ILLCOM, 1);
				break;
			}
		}
}
