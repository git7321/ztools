/*  root.c - generate a path to a file from the root */
#include <stdio.h>
#include <ctype.h>
#include <windows.h>
#include <tools.h>
#include <string.h>

int __stdcall rootpath (char *src, char *dst)
{

    LPSTR FilePart;
    LPSTR p;
    BOOL  Ok;

    Ok =  (!GetFullPathName( (LPSTR) src,
                             (DWORD) MAX_PATH,
                             (LPSTR) dst,
                             &FilePart ));

    if ( !Ok ) {
        p = src + strlen( src ) - 1;
        if ( *p  == '.' ) {
            if ( p > src ) {
                p--;
                if ( *p != '.' && *p != ':' && !fPathChr(*p) ) {
                    strcat( dst, "." );
                }
            }
        }
    }

    return Ok;
}
