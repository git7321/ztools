/*  init.c - routines for managing TOOLS.INI-like files
 *
 *  Modifications
 *      15-Jul-87   danl    Start of section is <optionalwhitespace>[...]
 *      05-Aug-1988 mz      Use buffer equate for swgoto.
 *      05-Jul-1989 bw      Use MAXPATHLEN
 *
 */

#include <string.h>
#include <stdio.h>
#include <windows.h>
#include <tools.h>

#define BUFLEN 256

static char *space = "\t ";

static flagType fMatchMark (char *pMark, char *pTag)
{
    char *p, c;

    while (*pMark != 0) {
        pMark = strbscan (p = strbskip (pMark, space), space);
        c = *pMark;
        *pMark = 0;
        if (!_stricmp (p, pTag))
            return TRUE;
        *pMark = c;
        }
    return FALSE;
}

char * __stdcall ismark (char *buf)
{
    char *p;

    buf = strbskip (buf, space);
    if (*buf++ == '[')
        if (*(p = strbscan (buf, "]")) != '\0') {
            *p = 0;
            return buf;
            }
    return NULL;
}

flagType __stdcall swgoto (FILE *fh, char *tag)
{
    char buf[BUFLEN];

    if (fh) {
        while (fgetl (buf, BUFLEN, fh) != 0) {
            char *p;

            if ((p = ismark (buf)) != NULL) {
                if (fMatchMark (p, tag))
                    return TRUE;
                }
            }
        }
    return FALSE;
}

FILE * __stdcall swopen (char *file, char *tag)
{
    FILE *fh;
    char buf[MAX_PATH];
    char buftmp[MAX_PATH];

    strncpy(buftmp, file, MAX_PATH);

    if ((fh = pathopen (buftmp, buf, "rb")) == NULL)
        return NULL;

    if (swgoto (fh, tag))
        return fh;

    fclose (fh);
    return NULL;
}

int __stdcall swclose (FILE *fh)
{
    return fclose (fh);
}

int __stdcall swread (char *buf, int len, FILE *fh)
{
    char *p;

    while (fgetl (buf, len, fh) != 0)
        if (ismark (buf) != NULL)
            return 0;
        else {
            p = strbskip (buf, space);
            if (*p != 0 && *p != ';') {
                strcpy (buf, p);
                return -1;
            }
        }
    return 0;
}

char * __stdcall swfind (char *pstrEntry, FILE *fh, char *pstrTag)
{
    char *p;
    char *q;
    FILE *fhIn = fh;
    char buf[BUFLEN];

    q = NULL;
    if (fh != NULL || (fh = swopen ("$INIT:\\TOOLS.INI", pstrTag))!= NULL) {
        while (swread (buf, BUFLEN, fh) != 0 && !ismark(buf) ) {
            if ( *(p = strbscan (buf, "=" )) ) {
                *p++ = '\0';
                if (!strcmpis (buf, pstrEntry)) {
                    if (*(p = strbskip (p, space)))
                        q = _strdup (p);
                    break;
                    }
                }
            }
        }
    if (fhIn == NULL && fh != NULL)
        swclose (fh);
    return q;
}
