/*  re.h - common include files for regular expression compilers
 */
#define INTERNAL

RE_OPCODE *REip;
struct patType *REPat;
int REArg;

#define ACTIONMIN   0

#define PROLOG      0
#define LEFTARG     1
#define RIGHTARG    2
#define SMSTAR      3
#define SMSTAR1     4
#define STAR        5
#define STAR1       6
#define ANY         7
#define BOL         8
#define EOL         9
#define NOTSIGN     10
#define NOTSIGN1    11
#define LETTER      12
#define LEFTOR      13
#define ORSIGN      14
#define RIGHTOR     15
#define CCLBEG      16
#define CCLNOT      17
#define RANGE       18
#define EPILOG      19
#define PREV        20

#define ACTIONMAX   20

int __stdcall REMatch(struct patType  *,char  *,char  *,RE_OPCODE *[], unsigned, flagType );
struct patType * __stdcall RECompile (char *, flagType, flagType);
char __stdcall REGetArg (struct patType *,int ,char *);
char __stdcall RETranslate (struct patType *,char *,char *);
int  __stdcall RETranslateLength (struct patType *,char *);
int  __stdcall RELength (struct patType *,int );
char * __stdcall REStart (struct patType *);

typedef unsigned __stdcall ACT (unsigned int, unsigned int, unsigned char, unsigned char);
typedef ACT * PACT;

unsigned __stdcall CompileAction(unsigned int, unsigned int, unsigned char, unsigned char);
unsigned __stdcall EstimateAction(unsigned int, unsigned int, unsigned char, unsigned char);
unsigned __stdcall NullAction(unsigned int, unsigned int, unsigned char, unsigned char);

int __stdcall RECharType (char *);
int __stdcall RECharLen (char *);
int __stdcall REClosureLen (char *);
char * __stdcall REParseRE (PACT, char *,int *);
char * __stdcall REParseE (PACT,char *);
char * __stdcall REParseSE (PACT,char *);
char * __stdcall REParseClass (PACT,char *);
char * __stdcall REParseAny (PACT,char *);
char * __stdcall REParseBOL (PACT,char *);
char * __stdcall REParsePrev (PACT, char *);
char * __stdcall REParseEOL (PACT,char *);
char * __stdcall REParseAlt (PACT,char *);
char * __stdcall REParseNot (PACT,char *);
char * __stdcall REParseAbbrev (PACT,char *);
char * __stdcall REParseChar (PACT,char *);
char * __stdcall REParseClosure (PACT,char *);
char * __stdcall REParseGreedy (PACT,char *);
char * __stdcall REParsePower (PACT,char *);
char __stdcall REClosureChar (char *);
char __stdcall Escaped (char );

void __stdcall REStackOverflow (void);
void __stdcall REEstimate (char *);

#ifdef DEBUG
void __stdcall REDump (struct patType *p);
#endif
