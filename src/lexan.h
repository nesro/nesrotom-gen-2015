/* lexan.h */

typedef enum {
	IDENT = 0, //
	NUMB, //
	PLUS, //
	MINUS, //
	TIMES, //
	DIVIDE, //
	EQ, //
	NEQ, //
	LT, //
	GT, //
	LTE, // 10
	GTE, //
	LPAR, //
	RPAR, //
	ASSGN, //
	COMMA, //
	SEMICOLON, //
	kwVAR, //
	kwCONST, //
	kwBEGIN, //
	kwEND, // 20
	kwIF, //
	kwTHEN, //
	kwELSE, //
	kwWHILE, //
	kwDO, //
	kwWRITE, //
	kwREAD, // 27
	EOI
} LexSymbol;

enum {
	MaxLenIdent = 32
};

extern LexSymbol Symb;
extern char Ident[MaxLenIdent]; /* atribut symbolu IDENT */
extern int Cislo; /* atribut symbolu NUMB */

void CtiSymb(void);
void InitLexan(char*);
void Chyba(const char*);
void ChybaSrovnani(LexSymbol);
