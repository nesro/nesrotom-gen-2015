#include <list>
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

	void print(const char *file_name, bool printComments);

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
class Storage {
public:
	static const int gp = 5;
	static const int mp = 6;
	static const int pc = 7;
	static const int reg_stack_size = 5;

	int reg_top;
	int reg_cnt;
	int mem_top;

	/* push register and get its number */
	int push(void);

	/* pop register and get its number */
	int pop(void);

	/* just get the stack's top number */
	int top(void);

	void print(void);

	Storage();
	~Storage();
};

#endif /* SRC_CODE_H_ */
