/***	pathcat.c - concatenate a string onto another, handing path seps
 *
 *	Modifications
 *	    23-Nov-1988 mz  Created
 */

#include <stdio.h>
#include <windows.h>
#include <tools.h>
#include <string.h>

char * __stdcall pathcat (char *pDst, char *pSrc)
{
    if (*pDst == '\0')
	return strcpy (pDst, pSrc);

    if (*pDst == '\0' || !fPathChr (strend (pDst)[-1]))
	strcat (pDst, PSEPSTR);

    while (fPathChr (*pSrc))
	pSrc++;

    return strcat (pDst, pSrc);
}
