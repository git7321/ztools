/* delnode - removes a node and its descendants */

#include <stdio.h>
#include <windows.h>
#include <tools.h>
#include <string.h>
#include <direct.h>
#include <errno.h>

void __stdcall
fDoDel (
    char	    *name,
    struct findType *pBuf,
    void	    *dummy
    )
{
    char *p;

    if (!TESTFLAG(pBuf->fbuf.dwFileAttributes, FILE_ATTRIBUTE_DIRECTORY)) {

        if (TESTFLAG(pBuf->fbuf.dwFileAttributes, FILE_ATTRIBUTE_READONLY)) {
            if(!SetFileAttributes(name, pBuf->fbuf.dwFileAttributes & ~FILE_ATTRIBUTE_READONLY)) {
                return;
            }
        }
        _unlink (name);

    } else if( strcmp( pBuf->fbuf.cFileName, "." ) &&
               strcmp( pBuf->fbuf.cFileName, ".." ) ) {

        if (TESTFLAG(pBuf->fbuf.dwFileAttributes, FILE_ATTRIBUTE_READONLY)) {
            if(!SetFileAttributes(name, pBuf->fbuf.dwFileAttributes & ~FILE_ATTRIBUTE_READONLY)) {
                return;
            }
        }

        p = strend( name );
        pathcat( name, "*.*" );
        forfile(name, FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_DIRECTORY, fDoDel, NULL);
        *p = 0;
        _rmdir (name);
    }

    dummy;
}

flagType __stdcall delnode (char *name)
{
    return (flagType)forfile(name, FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_DIRECTORY, fDoDel, NULL) ;
}
