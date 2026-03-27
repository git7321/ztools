/* pathchr.c - return configuration dependent info on MSDOS */
#include <stdio.h>
#include <windows.h>
#include <tools.h>

char __stdcall fPathChr( int c )
{
    return (char)( c == '\\' || c == '/' );
}

char __stdcall fSwitChr( int c )
{
    return (char)( c == '/' || c == '-' );
}
