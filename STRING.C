/***************************************************************************\

MEMBER:     strbscan

SYNOPSIS:   Returns pointer to first character from string in set

ALGORITHM:

ARGUMENTS:  const LPSTR	    - search string
	    const LPSTR	    - set of characters

RETURNS:    LPSTR     - pointer to first matching character

\***************************************************************************/

#include <assert.h>
#include <process.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>

LPSTR __stdcall
strbscan (
    const LPSTR	pszStr,
    const LPSTR	pszSet
    ) {

    assert( pszStr );
    assert( pszSet );

    return pszStr + strcspn( pszStr, pszSet );
}

LPSTR __stdcall
strbskip (
    const LPSTR	pszStr,
    const LPSTR	pszSet
    ) {

    assert( pszStr );
    assert( pszSet );

    return pszStr + strspn( pszStr, pszSet );
}
