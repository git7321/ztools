/* fmove.c - fast copy between two file specs
 *
 *   5/10/86  daniel lipkie	Added frenameNO.  fmove uses frenameNO
 * 17-Oct-90  w-barry		Switched 'C'-runtime function 'rename' for
 *				private version 'rename' until DosMove
 *				is completely implemented.
 *
 */

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <malloc.h>
#include <dos.h>
#include <io.h>
#include <stdio.h>
#include <windows.h>
#include <tools.h>
#include <errno.h>

#define IBUF	10240

int __stdcall frenameNO(char *strNew, char *strOld)
{
    return( rename(strOld, strNew) );
}

char * __stdcall fmove (char *src, char *dst)
{
    char *result;

    if( !rename(src, dst) )
	return NULL;

    if ( GetFileAttributes(src) == 0xFFFFFFFF ) {
        return "Source file does not exist";
    }

    if (rename(src, dst) == -1) {
	if (errno != EXDEV) {
	    return error ();
	}
	else
	if ((result = fcopy (src, dst)) != NULL)
	    return result;

	DeleteFile(src);
	}

    return NULL;
}
