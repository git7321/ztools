/* fexpunge.c - remove all deleted objects from the index */

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include <stdio.h>
#include <windows.h>
#include <tools.h>
#include <rm.h>
#include <string.h>
#include <time.h>
#include <direct.h>
#include <malloc.h>

__int64 __stdcall fexpunge (char *pDir, FILE *list)
{
    int fhidx;
    char *dir;
    char *szRec;
    char *idx;
    char *file;
    __int64 totbytes;
    struct _stati64 statbuf;

    totbytes = 0i64;
    dir = idx = file = szRec = NULL;
    if ((dir = (char *)malloc(MAX_PATH)) == NULL ||
	    (idx = (char *)malloc(MAX_PATH)) == NULL ||
	    (file = (char *)malloc(MAX_PATH)) == NULL ||
	    (szRec = (char *)malloc(MAX_PATH)) == NULL) {
	if (list)
	    fprintf (list, "Unable to allocate internal storage\n");
	goto done;
    }

    strcpy (dir, pDir);
    pathcat (dir, RM_DIR);
    strcpy (idx, dir);
    pathcat (idx, RM_IDX);
    if ((fhidx = _open (idx, _O_RDWR | _O_BINARY)) != -1) {
	if (list)
	    fprintf (list, "Expunging files in %s\n", pDir);

	readIdxRec (fhidx, szRec);
	if (fIdxHdr (szRec))
	    if (!readNewIdxRec (fhidx, szRec, MAX_PATH))
		goto done;
	do {
            if (szRec[0] != '\0') {
		sprintf (file, "%s\\deleted.%03x", dir, (_lseeki64(fhidx, 0i64, SEEK_CUR)
			 - strlen (szRec)) / RM_RECLEN);

                if (_stati64 (file, &statbuf) == -1) {
		    if (list)
			fprintf (list, " (%s - %s)\n", file, error ());
		}
		else {
		    _unlink (file);
		    totbytes += statbuf.st_size;
		    if (list) {
			char *pTime = ctime (&statbuf.st_mtime);

			*(pTime + 24) = '\0';
			upd (dir, szRec, file);
			fprintf (list, "%8ld %s  %s\n", statbuf.st_size, pTime,
				 file);
			fflush (list);
		    }
		}
	    }
	} while (readNewIdxRec (fhidx, szRec, MAX_PATH));

	_close (fhidx);
	_unlink (idx);
	if (_rmdir (dir))
	    fprintf (list, "ERROR: Unable to remove directory %s - %s\n", dir, error ());
	if (list)
	    fprintf (list, "%ld bytes freed\n", totbytes);
    }
    else
	if (!_stati64 (dir, &statbuf))
	    fprintf (list, "Warning: Cannot open %s - %s\n", idx, error ());
done:
    if (dir)
	free (dir);
    if (idx)
	free (idx);
    if (file)
	free (file);
    if (szRec)
	free (szRec);
    return totbytes;
}
