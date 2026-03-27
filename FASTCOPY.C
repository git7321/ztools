/*  fastcopy - use multiple threads to whack data from one file to another
 *
 *  Modifications:
 *      18-Oct-1990 w-barry Removed 'dead' code.
 *      21-Nov-1990 w-barry Updated API's to the Win32 set.
 */
#define INCL_DOSPROCESS
#define INCL_DOSSEMAPHORES

#include <stdio.h>
#include <process.h>
#include <windows.h>
#include <tools.h>
#include <malloc.h>

#define BUFSIZE     0xFE00
#define STACKSIZE   256

typedef struct BUF BUF;

struct BUF {
    BOOL  flag;
    DWORD cbBuf;
    BUF  *fpbufNext;
    BYTE  ach[BUFSIZE];
    };

#define LAST    TRUE
#define NOTLAST FALSE

static HANDLE            hevQNotEmpty;
static CRITICAL_SECTION  hcrtQLock;
static BUF              *fpbufHead = NULL;
static BUF              *fpbufTail = NULL;
static HANDLE            hfSrc, hfDst;
static HANDLE            hThread;
static BOOLEAN           fAbort;

LPSTR __stdcall writer( void );
DWORD __stdcall reader( void );
BUF * __stdcall dequeue( void );
void __stdcall enqueue( BUF *fpbuf );
char * __stdcall fastcopy( HANDLE hfSrcParm, HANDLE hfDstParm );

LPSTR __stdcall writer ()
{
    BUF *fpbuf;
    DWORD cbBytesOut;
    BOOL f = !LAST;
    LPSTR npsz = NULL;

    while (f != LAST && npsz == NULL) {
        fpbuf = dequeue ();
        if ((f = fpbuf->flag) != LAST) {
            if( !WriteFile( hfDst, fpbuf->ach, fpbuf->cbBuf, &cbBytesOut, NULL) ) {
                npsz = "WriteFile: error";
            } else if( cbBytesOut != ( DWORD )fpbuf->cbBuf ) {
                npsz = "WriteFile: out-of-space";
            }
        } else {
            npsz = *(LPSTR *)fpbuf->ach;
        }
        LocalFree(fpbuf);
    }
    if ( f != LAST ) {
        fAbort = TRUE;
    }
    WaitForSingleObject( hThread, (DWORD)-1 );
    CloseHandle( hThread );
    CloseHandle(hevQNotEmpty);
    DeleteCriticalSection(&hcrtQLock);
    return npsz;
}

DWORD __stdcall reader()
{
    BUF *fpbuf;
    BOOL f = !LAST;

    while ( !fAbort && f != LAST) {
        if ( (fpbuf = LocalAlloc(LMEM_FIXED,sizeof(BUF)) ) == 0) {
            printf ("LocalAlloc error %ld\n",GetLastError());
            exit (1);
        }
        f = fpbuf->flag = NOTLAST;
        if ( !ReadFile( hfSrc, fpbuf->ach, BUFSIZE, &fpbuf->cbBuf, NULL) || fpbuf->cbBuf == 0) {
            f = fpbuf->flag = LAST;
            *(LPSTR *)fpbuf->ach = NULL;
        }
        enqueue (fpbuf);
    }
    return( 0 );
}

BUF * __stdcall dequeue( void )
{
    BUF *fpbuf=NULL;

    while (TRUE) {

        if (fpbufHead != NULL) {
            EnterCriticalSection( &hcrtQLock );
            fpbufHead = (fpbuf = fpbufHead)->fpbufNext;
            if( fpbufTail == fpbuf ) {
                fpbufTail = NULL;
            }
            LeaveCriticalSection( &hcrtQLock );
            break;
        }

        WaitForSingleObject( hevQNotEmpty, (DWORD)-1 );
    }
    return fpbuf;
}

void __stdcall enqueue( BUF *fpbuf )
{
    fpbuf->fpbufNext = NULL;

    EnterCriticalSection( &hcrtQLock );

    if( fpbufTail == NULL ) {
        fpbufHead = fpbuf;
    } else {
        fpbufTail->fpbufNext = fpbuf;
    }
    fpbufTail = fpbuf;
    LeaveCriticalSection( &hcrtQLock );

    SetEvent( hevQNotEmpty );
}

char * __stdcall fastcopy( HANDLE hfSrcParm, HANDLE hfDstParm)
{
    DWORD dwReader;

    hfSrc = hfSrcParm;
    hfDst = hfDstParm;

    hevQNotEmpty = CreateEvent( NULL, (BOOL)FALSE, (BOOL)FALSE,NULL );
    if ( hevQNotEmpty == INVALID_HANDLE_VALUE ) {
        return NULL;
    }
    InitializeCriticalSection( &hcrtQLock );

    fAbort = FALSE;
    hThread = CreateThread( 0, STACKSIZE, (LPTHREAD_START_ROUTINE)reader, 0, 0, &dwReader );
    if( hThread == INVALID_HANDLE_VALUE ) {
        return "can't create thread";
    }
    return( writer() );
}
