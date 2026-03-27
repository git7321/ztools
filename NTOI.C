/* convert an arbitrary based number to an integer */

#include <ctype.h>
#include <stdio.h>
#include <windows.h>
#include <tools.h>

int __stdcall ntoi (char *p, int base)
{
    int i, c;
    flagType fFound;

    if (base < 2 || base > 16)
	return -1;
    i = 0;
    fFound = FALSE;
    while ((c = *p++)!=-1) {
	c = tolower (c);
	if (!isxdigit (c))
	    break;
	if (c <= '9')
	    c -= '0';
	else
	    c -= 'a'-10;
	if (c >= base)
	    break;
	i = i * base + c;
	fFound = TRUE;
	}
    if (fFound)
	return i;
    else
	return -1;
}
