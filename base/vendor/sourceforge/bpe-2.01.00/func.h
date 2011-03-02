/* func.h */

/* asciiedit.c */

int edit_ascii __P((char *ed_mode));
void ae_print __P((int scode, int c));

/* asciisrch.c */

int find_string __P((void));

/* backup.c */

void do_backup __P((char *filename));
int ck_chars __P((char *filename, int code));
void wrtofile __P((char **argf));
void wfile __P((char *filename, off_t start, off_t end, flag1 trunc_flag));

/* buffer.c */

int dump __P((int disp_code));
int rdpage __P((void));
void cmp_page __P((void));
void cp_page __P((void));
void undo __P((void));
void restore __P((void));
void wr_buf __P((void));
int wr_page __P((void));
int ck_mod __P((void));
int set_structs __P((void));
int set_buffers __P((void));
void set_farrays __P((char **argv, char **argf));
int add_farray __P((char **argf, char *filename));

/* command.c */

int command __P((char **argf));

/* cursor.c */

void cur_up __P((void));
void cur_down __P((void));
void cur_right __P((void));
void cur_left __P((void));
void cur_home __P((void));
void tog_offset __P((void));

/* display.c */

void dispheader __P((int disp_code, char *filename));
void dispos __P((void));
void disp __P((int hcode));
void dispwin __P((void));
void hs_dispwin __P((void));
void hm_dispwin __P((void));
void c_dispos __P((void));
void uc_dispos __P((void));
void clr_command __P((void));
void clr_edit __P((int line_num));
void reset_dispos __P((void));
void redraw __P((WINDOW *win));
void tog_address __P((void));
void tog_clear __P((void));
void tog_highlight __P((void));
void rem_highlight __P((void));
void res_highlight __P((void));

/* edit.c */

void do_edit __P((int inval));
void set_edit __P((void));
void set_hdr __P((char *ed_mode));

/* file.c */

int next_file __P((char **argf));
int prev_file __P((char **argf));
int do_jump __P((int jump_code, char **argf));
int sel_num __P((char **argf));
int sel_name __P((char **argf));
int sel_new __P((int edit_code, char **argf));
int do_toggle __P((char **argf));
int sel_file __P((char **argf));
void dispflist __P((int *c, int cury, int curx, int filnum, char **argf,
		    WINDOW *filscr));
int do_file __P((char *old_filename, char *new_filename));
int get_file __P((char *filename));
int finish __P((char *filename, int quitcode));
int get_stat __P((char *filename));
int set_times __P((char *filename));
int ck_file __P((char *filename));
int ck_flist __P((char *filename, char **argf));
int ck_rd __P((void));

/* help.c */

int help __P((void));
void help_1 __P((void));
void help_2 __P((void));
int opt_help __P((void));

/* hexedit.c */

int edit_hex __P((char *ed_mode));
void do_hex __P((int inval, int *charnum));
void he_print __P((int scode, int c, int value));

/* hexsrch.c */

int find_hex __P((void));
int hex_2_byte __P((int a, int b));
int cvt_str __P((char *s, unsigned char *h));

/* input.c */

int get_str __P((char *s1, WINDOW *win,
		 int cury, int curx, int count, int len,
		 int ed_code, int sc_code));
int get_entry __P((char *inbuf, char *last_entry, char *prompt, int msg_code));
int edit_entry __P((char *inbuf, char *last_entry, char *prompt,
		    int l_len, int p_len));
int confirm __P((int msg_code));

/* lock.c */

#ifndef NOLOCK
void ck_lock __P((char *filename));
void do_lock __P((char *filename));
void rm_lock __P((int status_code));
#endif

/* main.c */

int main __P((int argc, char **argv));

/* misc.c */

int show_ver __P((int option));
void show_sver __P((void));
int usage __P((int ucode));
#ifndef HAVE_GETOPT
int getopt __P((int argc, char **argv, char *opts));
#endif

/* msg.c */

void stl_msg __P((const char *s1, char *s2, int msgcode, int belcode));
void stl_clr __P((int rcode));
void show_options __P((void));
void show_status __P((void));
void show_toggle __P((char **argf));

/* position.c */

void pos_opts __P((void));
void first_page __P((void));
void last_page __P((void));
void next_page __P((void));
void prev_page __P((void));
void last_edit __P((void));
void last_search __P((void));
void set_address __P((void));
void last_set_address __P((void));
void set_mark __P((void));
void set_cmark __P((char *ed_mode));
void set_amark __P((void));
void goto_offset __P((void));
void goto_mark __P((void));
int ck_marks __P((void));
void init_marks __P((void));
int get_mark __P((void));
off_t get_address __P((char *prompt, char *last_entry, int xpos, int len));
off_t get_maddress __P((int marker_index));
int show_address __P((char *filename));
void down_scroll __P((int slines));
void up_scroll __P((int slines));

/* shell.c */

void do_shell __P((void));
#ifdef JOBCONTROL
void do_job __P((void));
#endif

/* tty.c */

int setup_tty __P((void));
int set_tty __P((void));
int reset_tty __P((void));
int get_pagesize __P((void));
RETSIGTYPE sig_exit __P((int sig));

