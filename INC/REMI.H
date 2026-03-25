#define MAXPATARG   10

#define REM_MATCH   0
#define REM_NOMATCH 1
#define REM_UNDEF   2
#define REM_STKOVR  3
#define REM_INVALID 4

typedef unsigned char RE_OPCODE;

struct patType {
    flagType fCase;
    flagType fUnix;
    char *pArgBeg[MAXPATARG];
    char *pArgEnd[MAXPATARG];
    RE_OPCODE code[1];
};

int RESize;

int __stdcall REMatch(struct patType  *,char  *,char  *,RE_OPCODE *[], unsigned, flagType );
struct patType * __stdcall RECompile (char *, flagType, flagType);
char __stdcall REGetArg (struct patType *,int ,char *);
char __stdcall RETranslate (struct patType *,char *,char *);
int  __stdcall RELength (struct patType *,int );
char * __stdcall REStart (struct patType *);
