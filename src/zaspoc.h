/* zaspoc.h */

#define _DO_YOU_WANT_ZASPOC_ 0

#ifndef _ZASPOC_
#define _ZASPOC_

enum Operator {
   Plus, Minus, Times, Divide,
   Eq, NotEq, Less, Greater, LessOrEq, GreaterOrEq,
   Error
};

#if _DO_YOU_WANT_ZASPOC_

enum TypInstr {
   TA, TC, BOP, UNM, DR, ST, IFJ, JU, WRT, DUP, STOP, READ
};


int Gener(TypInstr, int = 0);
void GenTR(char*);
void PutIC(int);
int GetIC();
void Print();
void Run();

#endif

#endif
