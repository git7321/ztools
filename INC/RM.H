/* rm.h - include file for rm and undel facility
 *
 * Index file format:
 *  The indexed file is composed of records of length RM_RECLEN.
 *	The old index file was composed of entries each the size of a record
 *  and composed of filename padded by NULs. The hash function mapped the
 *  Nth record (i.e. Nth INDEXED entry) to 'deleted.xxx', where, 'xxx' is (N+1)
 *  padded by leading zeroes.
 *	The new index file has an header record, rm_header, followed by entries
 *  of one or more records padded by NULs. Longfilenames occupy multiple
 *  records. The hash function maps the entry starting at Nth record to
 *  'deleted.xxx' where xxx is (N+1) padded by leading zeroes. This works out
 *  to be basically the same as that for the old format. The differences are :-
 *	-No entry is mapped to 'deleted.000'
 *	-Entries for filenames longer than (RM_RECLEN-1) bytes cause gaps in
 *	    mapping.
 *
 * Notes:
 *  RM/EXP/UNDEL work as follows:-
 *	    'RM foo' saves a copy of 'foo' and places it in an hidden
 *	sub-directory of RM_DIR (of foo) as file 'deleted.xxx', where, xxx is
 *	determined from the index file RM_IDX in RM_DIR. An entry is made in
 *	the index file for this.
 *	    'UNDEL foo' reads the index file in RM_DIR and determines xxx for
 *	foo and renames 'deleted.xxx' as foo. The entry for foo in the index
 *	file is filled with NULLs.
 *	    'EXP' picks up the index file from RM_DIR and deletes 'deleted.xxx'
 *	for each entry in the index file. It then deletes the index file and
 *	RM_DIR.
 *
 *  The new index file format can coexist with the old one because :-
 *	The header has a starting NULL which causes it to be ignored by
 *	    the old utilities,
 *	When the old utilities attempt to read in a long filename entry they
 *	fail without harm as the hashed 'deleted.xx' does not exist.
 */

#define RM_DIR     "deleted."
#define RM_IDX     "index."
#define RM_RECLEN  16

#define RM_SIG     (char)0x00
#define RM_MAGIC   "IX"
#define RM_VER     "1.01"
#define RM_NULL    "\0"

extern char rm_header[RM_RECLEN];

int __stdcall convertIdxFile (int fhidx, char *dir);

flagType __stdcall fIdxHdr (char *rec);

int __stdcall readIdxRec (int fhIdx, char *rec);

int __stdcall readNewIdxRec (int fhIdx, char *szRec, unsigned int cbMax);

int __stdcall writeIdxHdr (int fhIdx);

int __stdcall writeIdxRec (int fhIdx, char *rec);

int __stdcall writeNewIdxRec (int fhIdx, char *szRec);
