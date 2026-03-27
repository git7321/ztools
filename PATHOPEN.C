/* reasonable imitation of logical names
 *
 *      4/14/86     dl  findpath: test for trailing && leading \ before
 *                          appending a \
 *      29-Oct-1986 mz  Use c-runtime instead of Z-alike
 *      03-Sep-1987 dl  fPFind: rtn nonzero iff exists AND is ordinary file
 *                      i.e., return false for directories
 *      11-Sep-1987 mz  Remove static declaration from findpath
 *      01-Sep-1988 bw  Allow $filenam.ext as a filename in findpath
 *      23-Nov-1988 mz  Use pathcat, allow $(VAR)
 *
 *      30-Jul-1990 davegi  Removed unreferenced local vars
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <windows.h>
#include <tools.h>
#include <sys/types.h>
#include <sys/stat.h>

flagType __stdcall fPFind (char *p, va_list ap)
{
    char    *pa[2];

    pa[1] = (char *)va_arg(ap, PCHAR);
    pa[0] = (char *)va_arg(ap, PCHAR);

    va_end(ap);

    strcpy ((char *)pa[0], p);
    pathcat ((char *) pa[0], (char *) pa[1]);

    {
        HANDLE TmpHandle;
        WIN32_FIND_DATA buffer;

        TmpHandle = FindFirstFile((LPSTR)pa[0],&buffer);

        if (TmpHandle == INVALID_HANDLE_VALUE) {
            return FALSE;
        }

        FindClose(TmpHandle);

        if ((buffer.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            return FALSE;
        }

    }
    pname ((char *) pa[0]);
    return TRUE;
}

static char szEmpty[2] = {'\0', '\0'};

flagType __stdcall findpath( char *filestr, char *pbuf, flagType fNew )
{
    char *p;
    char c, *pathstr;

    pathstr = NULL;

    if( *filestr == '$' ) {

        if (filestr[1] == '(') {

            if (*(p = strbscan (filestr, ")")) != '\0') {
                *p = 0;
                pathstr = getenvOem (filestr + 2);
                *p++ = ')';
                filestr = p;
            }
        }
        else if (*(p = strbscan (filestr, ":")) != '\0') {
            *p = 0;
            pathstr = getenvOem (filestr + 1);
            *p++ = ':';
            filestr = p;
        }
    }

    if (pathstr == NULL) {
        pathstr = (char *)szEmpty;
    }

    if (forsemi (pathstr, fPFind, filestr, pbuf)) {
        return TRUE;
    }

    if( !fNew ) {
        return FALSE;
    }

    p = strbscan (pathstr, ";");
    c = *p;
    *p = 0;
    strcpy (pbuf, pathstr);
    if (*pathstr == 0) {
        strcat (pbuf, filestr);
    }
    else {
        pathcat (pbuf, filestr);
    }

    *p = c;

    return TRUE;
}

FILE * __stdcall pathopen (char *name, char *buf, char *mode)
{
    return findpath (name, buf, TRUE) ? fopen (buf, mode) : NULL;
}
