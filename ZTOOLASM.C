/*++

Copyright (c) 1990  Microsoft Corporation

Module Name:

    move.c

Abstract:

    Move and file routines that were previously in asm
    Done to ease porting of utilities (wzmail)

--*/

#include <stdio.h>
#include <windows.h>
#include <tools.h>
#include <memory.h>
#include <string.h>

void __stdcall
Move (
    void * src,
    void * dst,
    unsigned int count)
    {

    memmove(dst, src, count);
}

void __stdcall
Fill (
    char * dst,
    char value,
    unsigned int count)
    {

    memset(dst, (int) value, count);
}

flagType __stdcall
strpre (
    char * s1,
    char * s2)
    {
    if ( _strnicmp ( s1, s2, strlen(s1)) == 0 )
	return -1;
    else
	return 0;
}
