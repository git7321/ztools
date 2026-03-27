/* pname.c - form a "pretty" version of a user file name */

#define INCL_ERRORS
#define INCL_DOSFILEMGR
#define INCL_DOSMODULEMGR

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <windows.h>
#include <tools.h>

char * __stdcall pname (char *pszName)
{
    if (!IsMixedCaseSupported (pszName))
	return _strlwr (pszName);

    {
        HANDLE hdir;
	WIN32_FIND_DATA *findbuf;
        DWORD cFound;
	char *pszSrc, *pszDst, *pszEnd, chEnd;

        if( ( findbuf = (WIN32_FIND_DATA *)malloc( sizeof( WIN32_FIND_DATA ) + MAX_PATH ) ) == NULL ) {
            return( pszName );
        }

	pszDst = pszName;
	if (pszDst[1] == ':')
	    pszDst += 2;
	while (fPathChr (pszDst[0]))
	    pszDst++;

        if (*pszDst == '\0')
	    return pszName;

	pszSrc = pszDst;
	while (TRUE) {
	    pszEnd = strbscan (pszSrc, "/\\");
	    chEnd = *pszEnd;
	    *pszEnd = 0;

	    strcpy (pszDst, pszSrc);

	    cFound = 1;
	    if( *strbscan( pszDst, "*?" ) == 0 &&
		strcmp (pszDst, ".") &&
                strcmp (pszDst, "..") &&
                ( ( hdir = FindFirstFile( pszName, findbuf ) ) != (HANDLE)-1 ) ) {

		strcpy (pszDst, findbuf->cFileName);

		FindClose( hdir );
	    }
	    else
		;

	    pszDst += strlen (pszDst);
	    *pszDst++ = '\\';

            if (chEnd == '\0') {
                pszDst[-1] = '\0';
		break;
		}

	    pszSrc = pszEnd + 1;
	    }
	return pszName;
    }
}

#define MCA_UNINIT	123
#define MCA_SUPPORT	TRUE
#define MCA_NOTSUPP	FALSE

static	WORD mca[27] = { MCA_UNINIT, MCA_UNINIT, MCA_UNINIT,
			   MCA_UNINIT, MCA_UNINIT, MCA_UNINIT,
			   MCA_UNINIT, MCA_UNINIT, MCA_UNINIT,
			   MCA_UNINIT, MCA_UNINIT, MCA_UNINIT,
			   MCA_UNINIT, MCA_UNINIT, MCA_UNINIT,
			   MCA_UNINIT, MCA_UNINIT, MCA_UNINIT,
			   MCA_UNINIT, MCA_UNINIT, MCA_UNINIT,
			   MCA_UNINIT, MCA_UNINIT, MCA_UNINIT,
			   MCA_UNINIT, MCA_UNINIT, MCA_UNINIT };

WORD __stdcall QueryMixedCaseSupport (char *psz)
{
    UNREFERENCED_PARAMETER( psz );

    return MCA_SUPPORT;
}

WORD __stdcall IsMixedCaseSupported (char *psz)
{
    WORD mcaSupp;
    DWORD  ulDrvOrd=(DWORD)-1;
    BOOL fUNC;

    fUNC = (BOOL)( ( fPathChr( psz[0] ) && fPathChr( psz[1] ) ) ||
	    ( psz[0] != 0 && psz[1] == ':' &&
	    fPathChr( psz[2] ) && fPathChr( psz[3] ) ) );

    if (!fUNC) {
	if (psz[0] != 0 && psz[1] == ':') {
	    ulDrvOrd = (psz[0] | 0x20) - 'a' + 1;
	} else {
            char buf[5];

            GetCurrentDirectory( 5, buf );
            ulDrvOrd = ( buf[0] | 0x20 ) - 'a' + 1;
        }

	if (mca[ulDrvOrd] != MCA_UNINIT) {
	    return mca[ulDrvOrd];
        }
    }

    mcaSupp = QueryMixedCaseSupport (psz);

    if (!fUNC)
	mca[ulDrvOrd] = mcaSupp;

    return mcaSupp;
}
