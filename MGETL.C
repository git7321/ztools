/*  mgetl.c - expand tabs and return lines w/o separators
 *
 *  Modifications
 *	05-Aug-1988 mz	Make exact length lines work correctly
 *
 */

#include <stdio.h>
#include <windows.h>
#include <tools.h>

char * __stdcall mgetl (char *buf, int len, char *pSrc)
{
    int c;
    char *p;

    if ( *pSrc == '\0' ) {
        *buf = 0;
        return NULL;
    }
    len--;
    p = buf;
    while (TRUE) {
        c = *pSrc++;
        if (c == '\0' || c == '\n')
            break;
        if (c != '\r')
	    if (len == 0) {
		pSrc--;
		break;
		}
	    else
            if (c != '\t') {
                *p++ = (char) c;
                len--;
                }
            else {
                c = min (8 - ((p-buf) & 0x0007), len);
                Fill (p, ' ', c);
                p += c;
                len -= c;
                }
        }
    *p = 0;
    return ( pSrc );
}
