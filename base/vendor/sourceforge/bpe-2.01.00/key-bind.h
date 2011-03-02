/* key-bind.h for for bpe 2.00.08 */

#define CANCEL_1	0x03	/* CTRL-C */
#define CANCEL_2	0x04	/* CTRL-D */
#define CANCEL_3	0x1b	/* ESC */
#define CANCEL_4	0x06	/* CTRL-F - this is used in place of CANCEL_3
				   if the -k option is used i.e. ESC is
				   disabled for escaping from edit-mode etc. */
#define CANCELSTR	"ESC"	/* this string is displayed on the status
				   line in edit mode and on the help
				   screens - change to the string
				   of your preference representing one of
				   CANCEL_[1-3] but keep it short, i.e. ^E
				   for CTRL-E and so on. Note that it is
				   replaced by CANCELOPT if the -k option
				   is used */
#define CANCELOPT	"^C"
#define	DEL		0x7f	/* DEL */
#define REDRAW		0x12	/* CTRL-R - if you change this you must also
				   change REDRAWSTR but don't pick ^L */
#define REDRAWSTR	"^R"	/* this string is displayed on the help
				   screen and should be no more than 7 chars */

/* these definitions are required for curses that do not have keypad()
   capability or for systems where the termcap entry is broken or if
   "vi-style" cursor keys are preferred - note that the "vi-style"
   keys are still available if keypad() is available.  You should not
   really need to but if you change any of the K_* definitions you
   must also change the matching STR_* definition and edit the man
   page.  And note that control-characters MUST be used because only a
   single character can be used with non-keypad() operations and
   because they have to be used in edit mode.  You should also keep in
   mind what other control-characters have been used already to avoid
   duplication.  */

#define	 K_DOWN		0x0a	/* CTRL-J */
#define	 K_UP		0x0b	/* CTRL-K */
#define	 K_LEFT		0x08	/* CTRL-H */
#define	 K_RIGHT	0x0c	/* CTRL-L */
#define	 K_HOME		0x1e	/* CTRL-^ */
#define	 STR_DOWN	"^J"
#define	 STR_UP		"^K"
#define	 STR_LEFT	"^H"
#define	 STR_RIGHT	"^L"
#define	 STR_HOME	"^^"
