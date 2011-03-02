/* msg.h */

/* the various status-line error/information messages */

#define STRSRCH	 0	/* Searching for: */
#define	NOPASC	 1	/* No previous ASCII string */
#define	NOASC	 2	/* ASCII string not found */
#define	NOPHEX	 3	/* No previous hex string */
#define	ILLHEX	 4	/* Illegal hex string */
#define	NOHEX	 5	/* Hex string not found */
#define	ILLAD	 6	/* Illegal address */
#define	ILLADO	 7	/* Illegal address-order */
#define	ADEOF	 8	/* Address beyond EOF */
#define	NOPAD	 9	/* No previous address */
#define	NOPSAD	10	/* No previous set-address */
#define	NOPSSAD	11	/* No previous string-search address */
#define NOOFFAD 12	/* No offset address */
#define NOLEAD	13	/* No last-edit address */
#define	ILLMARK	14	/* Illegal marker - must be from "a" to "z" */
#define	NOMARKS 15	/* No markers set */
#define	NOTMARK	16	/* Marker not set: */
#define CLRMARK 17	/* Markers cleared */
#define	NONPAGE	18	/* No next page */
#define	NOPPAGE	19	/* No previous page */
#define	BMODINF	20	/* Buffer-modifications unchanged */
#define	RESBMOD	21	/* Buffer-modifications undone */
#define	NOBMOD	22	/* No buffer-modifications exist */
#define	NOWBMOD	23	/* No buffer-modifications exist - buffer
			   not written to disk */
#define	WBMOD	24	/* Buffer written to disk */
#define	RESFMOD	25	/* Original buffer restored and written to disk */
#define	NOWRITE	26	/* No write since last buffer-modification */
#define	NOFMOD	27	/* No file-page-modifications exist */
#define	RDMODE	28	/* Read-only mode */
#define	LSERR	29	/* Lseek error */
#define	RDERR	30	/* Read error */
#define	WRERR	31	/* Write error */
#define	STERR	32	/* Cannot stat: */
#define	OPERR	33	/* Cannot open: */
#define	MTFINF	34	/* Empty file: */
#define	RDOINF	35	/* File opened in read-only mode: */
#define	ACCERR	36	/* Not accessible: */
#define	NOOFILE	37	/* No other files */
#define	NONFILE	38	/* No next file */
#define	NOPFILE	39	/* No previous file */
#define	NOSFILE	40	/* No such file: */
#define	FULLLST	41	/* File-list full */
#define	FINLST	42	/* File already in list: */
#define	FADLST	43	/* File added to list: */
#define	ILLFNO	44	/* Illegal file-number: */
#define	TIMERR	45	/* Cannot reset mod-time: */
#define	BUEXIST	46	/* Backup-file already exists: */
#define	BUC1ERR	47	/* Cannot create backup-file: */
#define	BUC2ERR	48	/* Cannot create backup-file */
#define	BUCRING	49	/* Creating backup-file: */
#define	BUCRE	50	/* Backup-file created: */
#define	FEXIST	51	/* File already exists: */
#define	FNOTORD	52	/* Not an ordinary file */
#define	FCURR	53	/* File same as current-file: */
#define	ILLFNAM	54	/* Illegal file-name: */
#define	NLSTR	55	/* Null string */
#define	NOPFNAM	56	/* No previous filename */
#define	FCERR	57	/* Cannot create file: */
#define	FAERR	58	/* Cannot append to file: */
#define	FCRING	59	/* Creating file: */
#define	FAPING	60	/* Appending to file: */
#define	FCRE	61	/* File created: */
#define	FAPP	62	/* File appended-to: */
#define ADDOFF	63	/* Address-display off */
#define ADDON	64	/* Address-display on */
#define	CPOFF	65	/* Cursor-positioning off */
#define	CPON	66	/* Cursor-positioning on */
#define	DCOFF	67	/* Display-clear off */
#define	DCON	68	/* Display-clear on */
#define	HIOFF	69	/* Highlighting off */
#define	HION	70	/* Highlighting on */
#define NOLEE	71	/* No last-entry editing at this prompt */
#define	NOSH	72	/* Shell-escape not available */
#define	MEMERR	73	/* Cannot allocate memory */
#define	ILLCOM	74	/* Illegal command - ? for help */
#define	LFCERR	75	/* Cannot create lockfile */
#define	LFERR	76	/* Lockfile error */
#define	LFWERR	77	/* Cannot write to lockfile */
#define	LFRERR	78	/* Cannot read lockfile */
#define	LFDERR	79	/* Error removing lockfile */
#define EDINF	80	/* [ edit-mode information ] */
#define VERINF	81	/* [ version and version-date ] */
