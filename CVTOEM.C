#include <stdlib.h>
#include "windows.h"

void __stdcall
ConvertAppToOem( unsigned argc, char* argv[] )
{
    unsigned i;
    LPSTR pSrc;
    LPSTR pDst;
    WCHAR Wide;

    for( i=0; i<argc; i++ ) {
        pSrc = argv[i];
        pDst = argv[i];

        do {
            MultiByteToWideChar(
                CP_ACP,
                MB_PRECOMPOSED,
                pSrc++,
                1,
                &Wide,
                1
                );

            WideCharToMultiByte(
                CP_OEMCP,
                0,
                &Wide,
                1,
                pDst++,
                1,
                "_",
                NULL
                );

        } while (*pSrc);

    }
    SetFileApisToOEM();
}

char* __stdcall
getenvOem( char* p )
{
    char* OemBuffer;
    char* AnsiValue;

    OemBuffer = NULL;
    AnsiValue = getenv( p );

    if( AnsiValue != NULL ) {
        OemBuffer = _strdup( AnsiValue );
        if( OemBuffer != NULL ) {
            CharToOem( OemBuffer, OemBuffer );
        }
    }
    return( OemBuffer );
}

int __stdcall
putenvOem( char* p )
{
    char* AnsiBuffer;
    int   rc;

    if( p == NULL ) {
        return( _putenv( p ) );
    }

    AnsiBuffer = _strdup( p );
    if( AnsiBuffer != NULL ) {
        OemToChar( AnsiBuffer, AnsiBuffer );
    }
    rc = _putenv( AnsiBuffer );
    if( AnsiBuffer != NULL ) {
        free( AnsiBuffer );
    }
    return( rc );
}
