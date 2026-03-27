/*
 * update takes a def string and update and fills the
 * update with missing defs the update allowing
 * specification of missing parameters.
 * the parts are: ^{[~:]#:}{%#</|\>}{[~.]#}{.[~./\:]}$
 * maximum size of MAXPATHLEN (80) bytes
 *
 *  Modifications:
 *	 4/14/86    dl	use U_ flags
 *	29-May-1987 mz	treat . and .. specially
 *
 *	30-Jul-1990 davegi  Removed unreferenced local vars
 *			    Added prototypes for string functions
 *
 */

#include <string.h>

#include <stdio.h>
#include <windows.h>
#include <tools.h>

static char szDot[]     = ".";
static char szDotDot[]  = "..";
static char szColon[]   = ":";
static char szPathSep[] = "\\/:";

int __stdcall upd (char *def, char *update, char *dst)
{
    char *p, buf[MAX_PATH];
    int f;

    f = 0;
    p = buf;

    if (!fPathChr (update[0]) || !fPathChr (update[1])) {
	if (drive(update, p) || drive (def, p))
	    SETFLAG(f, U_DRIVE);
	p += strlen (p);
	}

    if (path(update, p) || path (def, p))
        SETFLAG(f, U_PATH);
    p += strlen (p);

    if (filename(update, p) || filename (def, p))
        SETFLAG(f, U_NAME);

    if (strcmp (p, szDot) && strcmp (p, szDotDot)) {
	p += strlen (p);

	if (extention(update, p) || extention (def, p))
	    SETFLAG(f, U_EXT);
	}

    strcpy (dst, buf);

    return f;
}

int __stdcall drive (char *src, char *dst)
{

    if (src[0] != 0 && src[1] == ':') {
	dst[0] = src[0];
	dst[1] = src[1];
	dst[2] = 0;
	return TRUE;
	}
    else {
	dst[0] = 0;
	return FALSE;
	}
}

static char *FindFilename (char *psz)
{
    char *p;

    while (TRUE) {
	p = strbscan (psz, szPathSep);
	if (*p == 0)
	    return psz;
	psz = p + 1;
	}
}

static char *FindExtention (char *psz)
{
    char *p;

    p = strbscan (psz, szDot);

    if (*p == 0)
	return p;

    while (TRUE) {
	psz = p;
	p = strbscan (psz + 1, szDot);
	if (*p == 0)
	    return psz;
	}
}

int __stdcall extention (char *src, char *dst)
{
    char *p1;

    p1 = FindFilename (src);

    if (!strcmp (p1, szDot) || !strcmp (p1, szDotDot))
	p1 = "";
    else
	p1 = FindExtention (p1);

    strcpy (dst, p1);

    return dst[0] != 0;
}

int __stdcall filename (char *src, char *dst)
{
    char *p, *p1;

    p1 = FindFilename (src);

    if (!strcmp (p1, szDot) || !strcmp (p1, szDotDot))
	p = strend (p1);
    else
	p = FindExtention (p1);

    strcpy (dst, p1);
    dst[p-p1] = 0;

    return dst[0] != 0;
}

int __stdcall fileext  (char *src, char *dst)
{
    if ( filename (src, dst) ) {
        dst += strlen (dst);
        extention (src, dst);
        return TRUE;
        }
    return FALSE;
}

int __stdcall path (char *src, char *dst)
{
    char *p;

    if (src[0] != 0 && src[1] == ':')
	src += 2;

    p = FindFilename (src);

    strcpy (dst, src);
    dst[p - src] = 0;
    return dst[0] != 0;
}
