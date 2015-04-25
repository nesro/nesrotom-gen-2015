#include <vector>

#ifndef SRC_CODE_H_
#define SRC_CODE_H_
	typedef enum type {
		RR, //
		RM, //
		RA, //
		BLANK_T, //
		COMMENT_T, //
	} type_t;

	typedef enum instruction {
		HALT, //
		IN, //
		OUT, //
		ADD, //
		INC, //
		DEC, //
		SHR, //
		SHL, //
		SUB, //
		MUL, //
		DIV, //
		LD, //
		ST, //
		LDA, //
		LDC, //
		JLT, //
		JLE, //
		JGT, //
		JGE, //
		JEQ, //
		JNE, //
		BLANK, //
		COMMENT, //
	} instruction_t;

class CodeLine {
public:
	type_t type;
	instruction_t instruction;
	int op[3];
	const char *comment_str;

	static int last_line_number;
	int line_number;

	CodeLine(void);
	CodeLine(instruction_t, int, int, int, const char *);
	~CodeLine(void);
	void print(void);
	type_t get_type(instruction_t);
};

/*****************************************************************************/

class TmSource {
public:
	std::vector<CodeLine *> lines;

	int addComment(const char *);
	int addBlank(void);
	int addInstr(instruction_t, int, int, int);
	int addInstr(instruction_t, int, int, int, const char *);

	void print(bool printComments);

	TmSource(void);
	~TmSource(void);
};

/*****************************************************************************/

/*
 * r0 - stack
 * r1 - stack
 * r2 - stack
 * r3 - stack
 * r4 - stack
 * r5 - gp - global pointer (variable storage)
 * r6 - mp - memory pointer (temp storage)
 * r7 - pc
 */
/*
 * budu si drzet co mam ulozeny naposled a v pripade potreby provedu rotaci
 * kdyz pudu do novyho vyrazu, podivam se, jestli to nemam v registrech
 * kdyz skocim pryc, nebudu si pamatovat nic a zacnu znova
 */
class Stack {
public:
	int srp[5]; /* stack registers permutation */
	int size;

	int pop(void);
	void push(int);

	Stack();
	~Stack();
};

#endif /* SRC_CODE_H_ */
