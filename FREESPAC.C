/* return amount of freespace on a drive */

#include <stdio.h>
#include <windows.h>
#include <tools.h>

static char root[5];
static DWORD cSecsPerClus, cBytesPerSec, cFreeClus, cTotalClus;

__int64 __stdcall freespac( int d )
{
    char root[5];

    DWORD cSecsPerClus, cBytesPerSec, cFreeClus, cTotalClus;

    root[0] = (char)( 'a' + d - 1 );
    root[1] = ':';
    root[2] = '\\';
    root[3] = '\0';

    if ( !GetDiskFreeSpace( root, &cSecsPerClus, &cBytesPerSec, &cFreeClus, &cTotalClus ) ) {
        return (unsigned long)-1L;
    }

    return( (__int64)cBytesPerSec * (__int64)cSecsPerClus * (__int64)cFreeClus );
}

__int64 __stdcall sizeround( __int64 l, int d )
{
    char root[5];
    DWORD cSecsPerClus, cBytesPerSec, cFreeClus, cTotalClus;
    __int64 BytesPerCluster;

    root[0] = (char)( 'a' + d - 1 );
    root[1] = ':';
    root[2] = '\\';
    root[3] = '\0';

    if ( !GetDiskFreeSpace( root, &cSecsPerClus, &cBytesPerSec, &cFreeClus, &cTotalClus ) ) {
        return (unsigned long)-1L;
    }

    BytesPerCluster = (__int64)cSecsPerClus * (__int64)cBytesPerSec;
    l += BytesPerCluster - 1;
    l /= BytesPerCluster;
    l *= BytesPerCluster;

    return l;
}
