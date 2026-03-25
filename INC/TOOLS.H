/*
 *  tools.h - Header file for accessing TOOLS.LIB routines
 *  includes stdio.h and ctype.h
 */
#include "config.h"

#if !defined (FALSE)
#define FALSE 0
#endif

#if !defined (TRUE)
#define TRUE (!FALSE)
#endif

#if MSDOS
#define  PSEPSTR "\\"
#define  PSEPCHR '\\'
#else
#define  PSEPSTR "/"
#define  PSEPCHR '/'
#endif

#if !defined( _FLAGTYPE_DEFINED_ )
typedef char flagType;
#endif
typedef long ptrType;

#define SETFLAG(l,f)   ((l) |= (f))
#define TESTFLAG(v,f)  (((v)&(f))!=0)
#define RSETFLAG(l,f)  ((l) &= ~(f))

#define SHIFT(c,v)  {c--; v++;}

#if !defined(CW)
#define WORD(h,l)      ((LOW((h))<<8)|LOW((l)))
#define DWORD(h,l)     ((DLOW(h)<<16|DLOW(l)))
#if !defined(MAKEWORD)
#define MAKEWORD(l, h) ((LOW((h))<<8)|LOW((l)))
#endif
#if !defined(MAKELONG)
#define MAKELONG(l, h) ((DLOW(h)<<16|DLOW(l)))
#endif
#endif

#define LOW(w)           ((int)(w)&0xFF)
#define HIGH(w)          LOW((int)(w)>>8)
#define DLOW(l)          ((long)(l)&0xFFFF)
#define DHIGH(l)         DLOW((long)(l)>>16)
#define POINTER(seg,off) ((((long)(seg))<<4)+ (long)(off))

#define FNADDR(f)   (f)

#define SELECT      if(FALSE){
#define CASE(x)     }else if((x)){
#define OTHERWISE   }else{
#define ENDSELECT   }

#define MAXARG      128
#define MAXLINELEN  1024
#define MAXPATHLEN  260

#define PIPE_READ   0
#define PIPE_WRITE  1

#define FILE_ATTRIBUTE_VOLUME_LABEL 0x00

struct spawnInfo {
    unsigned PID;
    int inReadHndl;
    int inWriteHndl;
    int outReadHndl;
    int outWriteHndl;
};

struct findType {
    unsigned        type;
    HANDLE          dir_handle;
    long            attr;
    WIN32_FIND_DATA fbuf;
};

#define FT_DONE     0xFF
#define FT_FILE     0x00
#define FT_SERV     0x01
#define FT_SHAR     0x02
#define FT_MASK     0xFF

#define FT_MIX      0x8000

struct DOS3findType {
    char reserved[21];
    char attr;
    unsigned time;
    unsigned date;
    long length;
    char name[13];
};

typedef struct findType FIND;
typedef FIND * NPFIND;

#define A_MOD  ( FILE_ATTRIBUTE_READONLY + FILE_ATTRIBUTE_HIDDEN + FILE_ATTRIBUTE_SYSTEM + FILE_ATTRIBUTE_ARCHIVE)
#define A_ALL  ( FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_VOLUME_LABEL )

#define HASATTR(a,v) TESTFLAG(a,v)

extern char XLTab[], XUTab[];

struct vectorType {
    int vmax;
    int count;
    unsigned elem[1];
};

typedef struct vectorType VECTOR;

#include "parse.h"
#include "exe.h"
#include "fcb.h"
#include "dir.h"
#include "times.h"
#include "ttypes.h"

#define U_DRIVE 0x8
#define U_PATH  0x4
#define U_NAME  0x2
#define U_EXT   0x1

#define REALDRIVE      0x8000
#define ISTMPDRIVE(x)  (((x)&REALDRIVE)==0)
#define TOKTODRV(x)    ((x)&~REALDRIVE)

#define HEAPOK          0
#define HEAPBADBEGIN   -1
#define HEAPBADNODE    -2
