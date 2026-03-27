/*  fputl.c - write a CRLF line to a file
 */

#include <stdio.h>
#include <windows.h>
#include <tools.h>

int __stdcall fputl (char *buf, int len, FILE *fh)
{
#if MSDOS
    return (fwrite (buf, 1, len, fh) != (unsigned)len || fputs ("\r\n", fh) == EOF) ? EOF : 0;
#else
    return (fwrite (buf, 1, len, fh) != len || fputs ("\n", fh) == EOF) ? EOF : 0;
#endif
}
