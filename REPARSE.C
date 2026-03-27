/*  reparse.c - parse a regular expression */
#include <ctype.h>

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <tools.h>
#include <remi.h>

#include "re.h"

#if DEBUG
#define DEBOUT(x)  printf x; fflush (stdout)
#else
#define DEBOUT(x)
#endif

extern	char XLTab[256];

#define CCH_SMPLUS     0
#define CCH_SMCLOSURE  1
#define CCH_POWER      2
#define CCH_CLOSURE    3
#define CCH_PLUS       4
#define CCH_NONE       5
#define CCH_ERROR      -1

#define SR_BOL		0
#define SR_EOL		1
#define SR_ANY		2
#define SR_CCLBEG	3
#define SR_LEFTOR	4
#define SR_CCLEND	5
#define SR_ABBREV	6
#define SR_RIGHTOR	7
#define SR_ORSIGN	8
#define SR_NOTSIGN	9
#define SR_LEFTARG	10
#define SR_RIGHTARG	11
#define SR_LETTER	12
#define SR_PREV 	13

int EndAltRE[] = { SR_ORSIGN, SR_RIGHTOR, -1 };
int EndArg[]   = { SR_RIGHTARG, -1 };

char *pAbbrev[] = {
    "a[a-zA-Z0-9]",
    "b([ \t]#)",
    "c[a-zA-Z]",
    "d[0-9]",
    "f([~/\\\\ \\\"\\[\\]\\:<|>+=;,.]#!..!.)",
    "h([0-9a-fA-F]#)",
    "i([a-zA-Z_$][a-zA-Z0-9_$]@)",
    "n([0-9]#.[0-9]@![0-9]@.[0-9]#![0-9]#)",
    "p(([A-Za-z]\\:!)(\\\\!/!)(:f(.:f!)(\\\\!/))@:f(.:f!.!))",
    "q(\"[~\"]@\"!'[~']@')",
    "w([a-zA-Z]#)",
    "z([0-9]#)",
    NULL
};

static char *digits = "0123456789";

static flagType fZSyntax = TRUE;

static unsigned int cArg;

int __stdcall RECharType (char *p)
{
    if (fZSyntax)
	switch (*p) {
	case '^':
	    return SR_BOL;
	case '$':
	    if (isdigit (p[1]))
		return SR_PREV;
	    else
		return SR_EOL;
	case '?':
	    return SR_ANY;
	case '[':
	    return SR_CCLBEG;
	case '(':
	    return SR_LEFTOR;
	case ']':
	    return SR_CCLEND;
	case ':':
	    return SR_ABBREV;
	case ')':
	    return SR_RIGHTOR;
	case '!':
	    return SR_ORSIGN;
	case '~':
	    return SR_NOTSIGN;
	case '{':
	    return SR_LEFTARG;
	case '}':
	    return SR_RIGHTARG;
	default:
	    return SR_LETTER;
	    }
    else
	switch (*p) {
	case '^':
	    return SR_BOL;
	case '$':
	    return SR_EOL;
	case '.':
	    return SR_ANY;
	case '[':
	    return SR_CCLBEG;
	case ']':
	    return SR_CCLEND;
	case '\\':
	    switch (p[1]) {
	    case ':':
		return SR_ABBREV;
	    case '(':
		return SR_LEFTARG;
	    case ')':
		return SR_RIGHTARG;
	    case '~':
		return SR_NOTSIGN;
	    case '{':
		return SR_LEFTOR;
	    case '}':
		return SR_RIGHTOR;
	    case '!':
		return SR_ORSIGN;
		}
	    if (isdigit (p[1]))
		return SR_PREV;
	default:
	    return SR_LETTER;
	    }
}

int __stdcall RECharLen (char *p)
{
    if (fZSyntax)
	if (RECharType (p) == SR_PREV)
	    return 2;
	else
	if (RECharType (p) == SR_ABBREV)
	    return 2;
	else
	    return 1;
    else {
	if (*p == '\\')
	    switch (p[1]) {
	    case '{':
	    case '}':
	    case '~':
	    case '(':
	    case ')':
	    case '!':
		return 2;
	    case ':':
		return 3;
	    default:
		if (isdigit (p[1]))
		    return 2;
		else
		    return 1;
		}
	return 1;
	}
}

int __stdcall REClosureLen (char *p)
{
    p;

    return 1;
}

char * __stdcall REParseRE (PACT pAction, char *p, int *pEnd)
{
    int *pe;
    unsigned u;

    DEBOUT (("REParseRE (%04x, %s)\n", pAction, p));

    while (TRUE) {
        if (*p == '\0')
	    if (pEnd == NULL)
		return p;
	    else {
		DEBOUT (("REParse expecting more, ERROR\n"));
		return NULL;
		}

	if (pEnd != NULL)
	    for (pe = pEnd; *pe != -1; pe++)
		if (RECharType (p) == *pe)
		    return p;

	if (RECharType (p) == SR_LEFTARG) {
            u = (*pAction) (LEFTARG, 0, '\0', '\0');
	    if ((p = REParseRE (pAction, p + RECharLen (p), EndArg)) == NULL)
		return NULL;
            (*pAction) (RIGHTARG, u, '\0', '\0');
	    cArg++;
	    p += RECharLen (p);
	    }
	else
	if ((p = REParseE (pAction, p)) == NULL)
	    return NULL;
	}
}

char * __stdcall REParseE (PACT pAction, char *p)
{
    DEBOUT (("REParseE (%04x, %s)\n", pAction, p));

    switch (REClosureChar (p)) {
    case CCH_SMPLUS:
	if (REParseSE (pAction, p) == NULL)
	    return NULL;
    case CCH_SMCLOSURE:
	return REParseClosure (pAction, p);

    case CCH_PLUS:
	if (REParseSE (pAction, p) == NULL)
	    return NULL;
    case CCH_CLOSURE:
	return REParseGreedy (pAction, p);

    case CCH_POWER:
	return REParsePower (pAction, p);

    case CCH_NONE:
	return REParseSE (pAction, p);

    default:
	return NULL;
	}
}

char * __stdcall REParseSE (PACT pAction, char *p)
{
    DEBOUT (("REParseSE (%04x, %s)\n", pAction, p));

    switch (RECharType (p)) {
    case SR_CCLBEG:
	return REParseClass (pAction, p);
    case SR_ANY:
	return REParseAny (pAction, p);
    case SR_BOL:
	return REParseBOL (pAction, p);
    case SR_EOL:
	return REParseEOL (pAction, p);
    case SR_PREV:
	return REParsePrev (pAction, p);
    case SR_LEFTOR:
	return REParseAlt (pAction, p);
    case SR_NOTSIGN:
	return REParseNot (pAction, p);
    case SR_ABBREV:
	return REParseAbbrev (pAction, p);
    default:
	return REParseChar (pAction, p);
	}
}

char * __stdcall REParseClass (PACT pAction, char *p)
{
    char c;
    unsigned u;

    DEBOUT (("REParseClass (%04x, %s)\n", pAction, p));

    p += RECharLen (p);
    if ((fZSyntax && *p == '~') || (!fZSyntax && *p == '^')) {
        u = (*pAction) (CCLNOT, 0, '\0', '\0');
	p += RECharLen (p);
	}
    else
        u = (*pAction) (CCLBEG, 0, '\0', '\0');

    while (RECharType (p) != SR_CCLEND) {
	if (*p == '\\')
	    p++;
        if (*p == '\0') {
	    DEBOUT (("REParseClass expecting more, ERROR\n"));
	    return NULL;
	    }
	c = *p++;
	if (*p == '-') {
	    p++;
	    if (*p == '\\')
		p++;
            if (*p == '\0') {
		DEBOUT (("REParseClass expecting more, ERROR\n"));
		return NULL;
		}
	    (*pAction) (RANGE, u, c, *p);
	    p++;
	    }
	else
	    (*pAction) (RANGE, u, c, c);
	}
    return p + RECharLen (p);
}

char * __stdcall REParseAny (PACT pAction, char *p)
{
    DEBOUT (("REParseAny (%04x, %s)\n", pAction, p));

    (*pAction) (ANY, 0, '\0', '\0');
    return p + RECharLen (p);
}

char * __stdcall REParseBOL (PACT pAction, char *p)
{
    DEBOUT (("REParseBOL (%04x, %s)\n", pAction, p));

    (*pAction) (BOL, 0, '\0', '\0');
    return p + RECharLen (p);
}

char * __stdcall REParsePrev (PACT pAction, char *p)
{
    unsigned int i = *(p + 1) - '0';

    DEBOUT (("REParsePrev (%04x, %s)\n", pAction, p));

    if (i < 1 || i > cArg) {
	DEBOUT (("REParsePrev invalid previous number, ERROR\n"));
	return NULL;
	}

    (*pAction) (PREV, i, '\0', '\0');
    return p + RECharLen (p);
}

char * __stdcall REParseEOL (PACT pAction, char *p)
{
    DEBOUT (("REParseEOL (%04x, %s)\n", pAction, p));

    (*pAction) (EOL, 0, '\0', '\0');
    return p + RECharLen (p);
}

char * __stdcall REParseAlt (PACT pAction, char *p)
{
    unsigned u = 0;

    DEBOUT (("REParseAlt (%04x, %s)\n", pAction, p));

    while (RECharType (p) != SR_RIGHTOR) {
	p += RECharLen (p);
        u = (*pAction) (LEFTOR, u, '\0', '\0');
	if ((p = REParseRE (pAction, p, EndAltRE)) == NULL)
	    return NULL;
        u = (*pAction) (ORSIGN, u, '\0', '\0');
	}
    (*pAction) (RIGHTOR, u, '\0', '\0');
    return p + RECharLen (p);
}

char * __stdcall REParseNot (PACT pAction, char *p)
{
    unsigned u;

    DEBOUT (("REParseNot (%04x, %s)\n", pAction, p));

    p += RECharLen (p);
    if (*p == '\0') {
	DEBOUT (("REParseNot expecting more, ERROR\n"));
	return NULL;
	}
    u = (*pAction) (NOTSIGN, 0, '\0', '\0');
    p = REParseSE (pAction, p);
    (*pAction) (NOTSIGN1, u, '\0', '\0');
    return p;
}

char * __stdcall REParseAbbrev (PACT pAction, char *p)
{
    int i;
    flagType fZSTmp;

    DEBOUT (("REParseAbbrev (%04x, %s)\n", pAction, p));

    p += RECharLen (p);

    fZSTmp = fZSyntax;
    fZSyntax = TRUE;
    if (p[-1] == '\0') {
	DEBOUT (("REParseAbbrev expecting abbrev char, ERROR\n"));
	fZSyntax = fZSTmp;
	return NULL;
	}

    for (i = 0; pAbbrev[i]; i++)
	if (p[-1] == *pAbbrev[i])
	    if (REParseSE (pAction, pAbbrev[i] + 1) == NULL) {
		fZSyntax = fZSTmp;
		return NULL;
		}
	    else {
		fZSyntax = fZSTmp;
		return p;
		}
    DEBOUT (("REParseAbbrev found invalid abbrev char %s, ERROR\n", p - 1));
    fZSyntax = fZSTmp;
    return NULL;
}

char * __stdcall REParseChar (PACT pAction, char *p)
{
    DEBOUT (("REParseChar (%04x, %s)\n", pAction, p));

    if (*p == '\\')
	p++;
    if (*p == '\0') {
	DEBOUT (("REParseChar expected more, ERROR\n"));
	return NULL;
	}
    (*pAction) (LETTER, 0, *p, '\0');
    return p+1;
}

char * __stdcall REParseClosure (PACT pAction, char *p)
{
    unsigned u;

    DEBOUT (("REParseaClosure (%04x, %s)\n", pAction, p));

    u = (*pAction) (SMSTAR, 0, '\0', '\0');
    if ((p = REParseSE (pAction, p)) == NULL)
	return NULL;
    (*pAction) (SMSTAR1, u, '\0', '\0');
    return p + REClosureLen (p);
}

char * __stdcall REParseGreedy (PACT pAction, char *p)
{
    unsigned u;

    DEBOUT (("REParseGreedy (%04x, %s)\n", pAction, p));

    u = (*pAction) (STAR, 0, '\0', '\0');
    if ((p = REParseSE (pAction, p)) == NULL)
	return NULL;
    (*pAction) (STAR1, u, '\0', '\0');
    return p + REClosureLen (p);
}

char * __stdcall REParsePower (PACT pAction, char *p)
{
    char *p1;
    int exp;

    DEBOUT (("REParsePower (%04x, %s)\n", pAction, p));

    p1 = REParseSE (NullAction, p);

    if (p1 == NULL)
	return NULL;

    p1 += REClosureLen (p1);

    if (*p1 == '\0') {
	DEBOUT (("REParsePower expecting more, ERROR\n"));
	return NULL;
	}

    if (sscanf (p1, "%d", &exp) != 1) {
	DEBOUT (("REParsePower expecting number, ERROR\n"));
	return NULL;
	}

    p1 = strbskip (p1, digits);

    while (exp--)
	if (REParseSE (pAction, p) == NULL)
	    return NULL;
    return p1;
}

unsigned __stdcall NullAction( unsigned int type, unsigned int u, unsigned char x, unsigned char y )
{
    type; u; x; y;
    return 0;
}

char __stdcall REClosureChar (char *p)
{
    p = REParseSE (NullAction, p);
    if (p == NULL)
        return CCH_ERROR;

    if (fZSyntax)
	switch (*p) {
	case '^':
            return CCH_POWER;
	case '+':
            return CCH_SMPLUS;
	case '#':
            return CCH_PLUS;
	case '*':
            return CCH_SMCLOSURE;
	case '@':
            return CCH_CLOSURE;
	default:
            return CCH_NONE;
	    }
    else
	switch (*p) {
	case '+':
            return CCH_PLUS;
	case '*':
            return CCH_CLOSURE;
	default:
            return CCH_NONE;
	    }
}

struct patType * __stdcall RECompile( char *p, flagType fCase, flagType fZS )
{
    fZSyntax = fZS;

    REEstimate (p);

    DEBOUT (("Length is %04x\n", RESize));

    if (RESize == -1)
	return NULL;

    if ((REPat = (struct patType *)(char *)malloc(RESize)) == NULL)
	return NULL;

    memset ((char *) REPat, -1, RESize);
    memset ((char *) REPat->pArgBeg, 0, sizeof (REPat->pArgBeg));
    memset ((char *) REPat->pArgEnd, 0, sizeof (REPat->pArgEnd));

    REip = REPat->code;
    REArg = 1;
    REPat->fCase = fCase;
    REPat->fUnix = (flagType) !fZS;

    cArg = 0;

    CompileAction (PROLOG, 0, '\0', '\0');

    if (REParseRE (CompileAction, p, NULL) == NULL)
	return NULL;

    CompileAction (EPILOG, 0, '\0', '\0');

#if DEBUG
    REDump (REPat);
#endif
    return REPat;
}

char __stdcall Escaped( char c )
{
    switch (c) {
    case 't':
	return '\t';
    case 'e':
	return 0x1B;
    case 'h':
	return 0x08;
    case 'g':
	return 0x07;
    case 'n':
	return '\n';
    case 'r':
	return '\r';
    case '\\':
	return '\\';
    default:
	return c;
	}
}

flagType __stdcall REGetArg (struct patType *pat, int i, char *p)
{
    int l = 0;

    if (i > MAXPATARG)
	return FALSE;
    else
    if (pat->pArgBeg[i] != (char *)-1)
	memmove ((char *)p, (char *)pat->pArgBeg[i], l = RELength (pat, i));
    p[l] = '\0';
    return TRUE;
}

flagType __stdcall RETranslate (struct patType *buf, char *src, char *dst)
{
    int i, w;
    char *work;
    char chArg = (char) (buf->fUnix ? '\\' : '$');

    work = (char *)malloc(MAXLINELEN);
    if (work == NULL)
	return FALSE;

    *dst = '\0';

    while (*src != '\0') {
	if (*src == chArg && (isdigit (src[1]) || src[1] == '(')) {
	    w = 0;

	    src += 2;

	    if (isdigit (src[-1]))
		i = src[-1] - '0';
	    else {
		i = atoi (src);

		if (*src == '-')
		    src++;
		src = strbskip (src, digits);

		if (*src == ',') {
		    w = i;
		    i = atoi (++src);
		    src = strbskip (src, digits);
		    }

		if (*src++ != ')') {
		    free (work);
		    return FALSE;
		    }
		}
	    if (!REGetArg (buf, i, work)) {
		free (work);
		return FALSE;
		}
	    sprintf (dst, "%*s", w, work);
	    dst += strlen (dst);
	    }
	else
	if (*src == '\\') {
	    src++;
	    if (!*src) {
		free (work);
		return FALSE;
		}
	    *dst++ = Escaped (*src++);
	    }
	else
	if (*src == chArg && src[1] == chArg) {
	    *dst++ = chArg;
	    src += 2;
	    }
	else
	    *dst++ = *src++;
	}
    *dst = '\0';
    free (work);
    return TRUE;
}

int __stdcall RETranslateLength (struct patType *buf, char *src)
{
    int i, w;
    int length = 0;
    char chArg = (char) (buf->fUnix ? '\\' : '$');

    while (*src != '\0') {
	if (*src == chArg && (isdigit (src[1]) || src[1] == '(')) {
	    w = 0;
	    src += 2;
	    if (isdigit (src[-1]))
		i = src[-1] - '0';
	    else {
		i = atoi (src);
		if (*src == '-')
		    src++;
		src = strbskip (src, digits);
		if (*src == ',') {
		    w = i;
		    i = atoi (++src);
		    src = strbskip (src, digits);
		    }
		if (*src++ != ')')
		    return -1;
		}
	    i = RELength (buf, i);
	    length += max (i, abs(w));
	    }
	else
	if (*src == '\\') {
	    src += 2;
	    length++;
	    }
	else
	if (*src == chArg && src[1] == chArg) {
	    src += 2;
	    length++;
	    }
	else {
	    length++;
	    src++;
	    }
	}
    return length;
}

int __stdcall RELength (struct patType *pat, int i)
{
    if (i > MAXPATARG)
	return -1;
    else
    if (pat->pArgBeg[i] == (char *)-1)
	return 0;
    else
	return pat->pArgEnd[i] - pat->pArgBeg[i];
}

char * __stdcall REStart (struct patType *pat)
{
    return pat->pArgBeg[0] == (char *)-1 ? NULL : pat->pArgBeg[0];
}
