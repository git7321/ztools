/* ttypes.h - type definitions for tools library */

#include <time.h>

#define lmax(x,y)  max(x,y)
#define lmin(x,y)  min(x,y)

flagType __stdcall int25(char, char far *, unsigned int, unsigned int);
flagType __stdcall int26(char, char far *, unsigned int, unsigned int);
void __stdcall cursor(int, int);
void __stdcall Move(void far *, void far *, unsigned int);
void __stdcall Fill(char far *, char, unsigned int);
char * __stdcall strbscan(char const *, char const *);
char * __stdcall strbskip(char const *, char const *);
char * __stdcall strncrlfend(char *, int);
flagType __stdcall strpre(char *, char *);
char * __stdcall fcopy(char *, char *);
long __stdcall getlpos();
void __stdcall getlinit(char far *, int, int);
int __stdcall getl(char *, int);

#define lower(x)   (_strlwr(x))
#define upper(x)   (_strupr(x))
#define MakeStr(x) (_strdup(x))
#define strend(x)  ((x)+strlen(x))

char * __stdcall error(void);
__int64 __stdcall fexpunge(char *,FILE *);
char * __cdecl fgetl(char *,int ,FILE *);
int __stdcall fputl(char *,int ,FILE *);
int __stdcall ffirst(char *,int ,struct findType *);
int __stdcall fnext(struct findType *);
void __stdcall findclose(struct findType *);

typedef flagType(__stdcall * __action_routine__)(char *, va_list);

flagType __cdecl forsemi(char *p, __action_routine__ proc, ...);

__int64 __stdcall freespac(int);
__int64 __stdcall sizeround(__int64 ,int);
char * __stdcall fastcopy(HANDLE hfSrcParm, HANDLE hfDstParm);
int __stdcall mapenv(char *,char *);
char * __stdcall ismark(char *);
FILE * __stdcall swopen(char *,char *);
int __stdcall swclose(FILE *);
int __stdcall swread(char *,int ,FILE *);
char * __stdcall swfind(char *,FILE *,char *);
char * __stdcall getenvini(char *,char *);
char __stdcall fPathChr(int);
char __stdcall fSwitChr(int);
flagType __stdcall fPFind(char *,  va_list);
flagType __stdcall findpath(char *,char *, flagType);
FILE * __stdcall pathopen(char *,char *,char *);
int __stdcall forfile(char *, int ,void(__stdcall *)(char *, struct findType *, void *), void *);
int __stdcall rootpath(char *,char *);
int __stdcall sti(char *,int);
int __stdcall ntoi(char *,int);
int __cdecl strcmps(const char *, const char *);
int __cdecl strcmpis(const char *, const char *);
int __stdcall upd(char *,char *,char *);
int __stdcall drive(char *,char *);
int __stdcall extention(char *,char *);
int __stdcall filename(char *,char *);
int __stdcall filenamx(char *,char *);
int __stdcall fileext(char *, char *);
int __stdcall path(char *,char *);
int __stdcall curdir(char *, BYTE);
int __stdcall getattr(char *);
char * __stdcall fmove(char *, char *);
char * __stdcall fappend(char *, HANDLE);
time_t __stdcall ctime2l(char *);
struct tm * __stdcall ctime2tm(char *);
time_t __stdcall date2l(int, int, int, int, int, int);
VECTOR * __stdcall VectorAlloc(int);
flagType __stdcall fAppendVector(VECTOR **, void *);
int __stdcall pipe(int []);
int __stdcall pgetl(char *, int, int);
enum exeKind __stdcall exeType(char *);
char * __stdcall strExeType(enum exeKind);
flagType __stdcall fMatch(char *, char *);

int __stdcall Connect(char *path, char *con, char *sub);
flagType __stdcall fDisconnect(int drive);
char * __stdcall pathcat(char *pDst, char *pSrc);
int __stdcall setattr(char *pname, int attr);

flagType __stdcall swchng(char *strSwFile, char *strTag, char *strLHS, char *strRHS, flagType fNoUndel);
int __stdcall swchnglhs(char *strSwFile, char *strTag, char *strLHS, char *strRHS);

int __stdcall heapdump(FILE *fp, int iFormat);

int __stdcall heapinfo(void);

char * __stdcall pname(char *);
unsigned short __stdcall IsMixedCaseSupported(char *);

void __stdcall ConvertAppToOem(unsigned argc, char* argv[]);
char* __stdcall getenvOem(char* p);
int __stdcall putenvOem(char* p);
