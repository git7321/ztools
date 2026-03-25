/* parse.h - support tops-20 comnd jsys on MSDOS */
#include <setjmp.h>

#define PINVIS  0x4000
#define PABBREV 0x2000

extern struct tbPairType *valParse;
extern jmp_buf ParseAC;
extern jmp_buf PromptAC;
extern char bufField[MAXARG];

#define SETPROMPT(p,b) setjmp(PromptAC);initParse(b,p);setjmp(ParseAC)
