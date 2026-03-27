/* find.c - MSDOS find first and next matching files */

#define INCL_DOSERRORS
#define INCL_DOSMODULEMGR

#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include <windows.h>
#include <tools.h>

#define SRCHATTR  (FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_DIRECTORY)
BOOL __stdcall AttributesMatch( NPFIND fbuf );

#define NO_MORE_FILES  FALSE
#define STATUS_OK      TRUE

BOOL __stdcall usFileFindNext (NPFIND fbuf);

int __stdcall ffirst (char *file, int attr, NPFIND fbuf)
{
    DWORD erc;
    fbuf->type = FT_DONE;
    { LPSTR p = file;
    UNREFERENCED_PARAMETER( attr );

    if (p[0] != 0 && p[1] == ':')
        p += 2;
    }

    {
        fbuf->type = FT_FILE;
        fbuf->attr = attr;
        erc = ( ( fbuf->dir_handle = FindFirstFile( file, &( fbuf->fbuf ) ) ) == (HANDLE)-1 ) ? 1 : 0;
        if ( (erc == 0) && !AttributesMatch( fbuf ) ) {
            erc = fnext( fbuf );
        }
    }

    if( fbuf->dir_handle != (HANDLE)-1 ) {
    if (!IsMixedCaseSupported (file)) {
        _strlwr( fbuf->fbuf.cFileName );
    } else {
        SETFLAG( fbuf->type, FT_MIX );
        }
    }

    return erc;
}

int __stdcall fnext (NPFIND fbuf)
{
    int erc;

    switch (fbuf->type & FT_MASK ) {
    case FT_FILE:
    erc = !usFileFindNext (fbuf);
    break;

    default:
    erc = NO_MORE_FILES;
    }

    if( erc == STATUS_OK && !TESTFLAG( fbuf->type, FT_MIX ) ) {
    _strlwr (fbuf->fbuf.cFileName);
    }
    return erc;
}

void __stdcall findclose (NPFIND fbuf)
{
    switch (fbuf->type & FT_MASK ) {
    case FT_FILE:
    FindClose( fbuf->dir_handle );
    break;
    }
    fbuf->type = FT_DONE;
}

BOOL __stdcall AttributesMatch( NPFIND fbuf )
{
    fbuf->fbuf.dwFileAttributes &= (0x000000FF & ~(FILE_ATTRIBUTE_NORMAL));

    if (! ((fbuf->fbuf.dwFileAttributes & SRCHATTR) & ~(fbuf->attr))) {
        return TRUE;
    } else {
        return FALSE;
    }
}

BOOL __stdcall usFileFindNext (NPFIND fbuf)
{

    while ( TRUE ) {
        if ( !FindNextFile( fbuf->dir_handle, &( fbuf->fbuf ) ) ) {
            return FALSE;
        } else if ( AttributesMatch( fbuf ) ) {
            return TRUE;
        }
    }
}
