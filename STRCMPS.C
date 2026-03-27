/* strcmps - compare strings and ignore spaces */

#include <ctype.h>

#include <stdio.h>
#include <windows.h>
#include <tools.h>

__cdecl strcmps (const char *p1, const char *p2)
{
    while (TRUE) {
        while (isspace (*p1))
            p1++;
        while (isspace (*p2))
            p2++;
        if (*p1 == *p2)
            if (*p1++ == 0)
                return 0;
            else
                p2++;
        else
            return *p1-*p2;
        }
}

__cdecl strcmpis (const char *p1, const char *p2)
{
    while (TRUE) {
        while (isspace (*p1))
            p1++;
        while (isspace (*p2))
            p2++;
        if (toupper (*p1) == toupper (*p2))
            if (*p1++ == 0)
                return 0;
            else
                p2++;
        else
            return *p1-*p2;
        }
}
