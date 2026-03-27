/* fdelete.c - perform undeleteable delete */

#include <fcntl.h>
#include <sys\types.h>
#include <sys\stat.h>
#include <stdio.h>
#include <windows.h>
#include <tools.h>
#include <rm.h>
#include <io.h>
#include <string.h>
#include <direct.h>
#include <malloc.h>

char rm_header[RM_RECLEN] = { RM_NULL RM_MAGIC RM_VER};

int __stdcall fdelete(char *p)
{
    char *dir;
    char *idx;
    char *szRec;
    int attr, fhidx;
    int erc;

    dir = idx = szRec = NULL;
    fhidx = -1;
    if ((dir = (char *)malloc(MAX_PATH)) == NULL ||
        (idx = (char *)malloc(MAX_PATH)) == NULL ||
        (szRec = (char *)malloc(MAX_PATH)) == NULL) {
        erc = 3;
        goto cleanup;
    }

    if ( ( attr = GetFileAttributes( p ) ) == -1) {
        erc = 1;
        goto cleanup;
    }

    if (TESTFLAG (attr, FILE_ATTRIBUTE_READONLY)) {
        erc = 2;
        goto cleanup;
    }

    pname (p);

    upd (p, RM_DIR, dir);

    strcpy (idx, dir);
    pathcat (idx, RM_IDX);

    if ( _mkdir (dir) == 0 )
        SetFileAttributes(dir, FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM);

    fileext (p, szRec);

    if ((fhidx = _open (idx, _O_CREAT | _O_RDWR | _O_BINARY,
                        _S_IWRITE | _S_IREAD)) == -1) {
        erc = 3;
        goto cleanup;
    }

    if (!convertIdxFile (fhidx, dir)) {
        erc = 3;
        goto cleanup;
    }

    sprintf (strend (dir), "\\deleted.%03x",
             _lseek (fhidx, 0L, SEEK_END) / RM_RECLEN);

    _unlink (dir);

    if (rename(p, dir) == -1) {
        erc = 2;
        goto cleanup;
    }

    if (!writeNewIdxRec (fhidx, szRec)) {
        rename( dir, p );
        erc = 2;
        goto cleanup;
    }
    erc = 0;
    cleanup:
    if (fhidx != -1)
        _close(fhidx);
    if (dir != NULL)
        free (dir);
    if (idx != NULL)
        free (idx);
    if (szRec != NULL)
        free (szRec);
    return erc;
}

int __stdcall writeIdxRec (int fhIdx, char *rec)
{
    return _write (fhIdx, rec, RM_RECLEN) == RM_RECLEN;
}

int __stdcall readIdxRec (int fhIdx, char *rec)
{
    return _read (fhIdx, rec, RM_RECLEN) == RM_RECLEN;
}

int __stdcall convertIdxFile (int fhIdx, char *dir)
{
    char firstRec[RM_RECLEN];
    int iRetCode = TRUE;
    char *oldName, *newName;

    oldName = newName = NULL;
    if ((oldName = (char *)malloc(MAX_PATH)) == NULL ||
        (newName = (char *)malloc(MAX_PATH)) == NULL) {
        iRetCode = FALSE;
        goto cleanup;
    }

    if (_lseek (fhIdx, 0L, SEEK_END) == 0L)
        writeIdxHdr (fhIdx);
    else {
        if (_lseek (fhIdx, 0L, SEEK_SET) == -1) goto cleanup;

        if (!readIdxRec (fhIdx, firstRec))
            goto cleanup;
        if (fIdxHdr (firstRec))
            goto cleanup;
        else {
            if (!writeIdxHdr (fhIdx)) {
                iRetCode = FALSE;
                goto cleanup;
            }
            strcpy (oldName, dir);
            strcpy (newName, dir);
            pathcat (oldName, "\\deleted.000");
            sprintf (strend (newName), "\\deleted.%03x",
                     _lseek (fhIdx, 0L, SEEK_END) / RM_RECLEN);
            if ( rename(oldName, newName) || !writeIdxRec (fhIdx, firstRec)) {
                iRetCode = FALSE;
                goto cleanup;
            }
        }
    }
    cleanup:
    if (oldName != NULL)
        free (oldName);
    if (newName != NULL)
        free (newName);
    return iRetCode;
}

flagType __stdcall fIdxHdr (char *rec)
{
    return (flagType)(rec[0] == RM_SIG
                      && !strncmp(rec+1, RM_MAGIC, strlen(RM_MAGIC)));
}

int __stdcall writeIdxHdr (int fhIdx)
{
    if (_lseek (fhIdx, 0L, SEEK_SET) == -1)
        return 0;

    return writeIdxRec (fhIdx, rm_header);
}

int __stdcall writeNewIdxRec (int fhIdx, char *szRec)
{
    char rec[RM_RECLEN];
    int cbLen;

    cbLen = strlen(szRec) + 1;
    while (cbLen > 0) {
        memset(rec, 0, RM_RECLEN);
        strncat (rec, szRec, RM_RECLEN-1);
        szRec += RM_RECLEN;
        if (!writeIdxRec (fhIdx, rec))
            return FALSE;
        cbLen -= RM_RECLEN;
    }
    return TRUE;
}

int __stdcall readNewIdxRec (
                  int fhIdx,
                  char *szRec,
                  unsigned int cbMax
                  ) {
    char rec[RM_RECLEN];
    unsigned int cb = 0;

    do {
        if (!readIdxRec (fhIdx, rec))
            return FALSE;
        strncpy (szRec, rec, RM_RECLEN);
        szRec += RM_RECLEN;
        cb += RM_RECLEN;
    } while (!memchr (rec, '\0', RM_RECLEN) && (cb < cbMax));

    return TRUE;
}
