/*  fgetl.c - expand tabs and return lines w/o separators
 *
 *  Modifications
 *	05-Aug-1988 mz	Make exact length lines work correctly
 *
 *	28-Jul-1990 davegi  Changed Fill to memset (OS/2 2.0)
 *      18-Oct-1990 w-barry Removed 'dead' code.
 *
 */

#include <string.h>
#include <stdio.h>
#include <windows.h>
#include <tools.h>

char * __cdecl
fgetl (char *buf, int len, FILE *fh)
{
    int c=-1;
    char *pch;
    int cchline;

    pch = buf;
    cchline = 0;

    while (TRUE) {
	c = getc (fh);

	if (c == EOF)
	    break;

	if (c == '\r')
	    continue;

	if (c == '\n')
	    break;

	if (c != '\t') {
	    *pch++ = (char) c;
	    cchline++;
	    }

	else {
	    c = min (8 - ((pch - buf) & 0x7), len - 1 - cchline);
	    memset (pch, ' ', c);
	    pch += c;
	    cchline += c;
	    }

	if (cchline >= len - 1)
	    break;
	}

    *pch = 0;

    return ((c == EOF) && (pch == buf)) ? NULL : buf;
}
