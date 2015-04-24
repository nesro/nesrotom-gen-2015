
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
	char *comment_str;

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

	TmSource(void);
	~TmSource(void);
};

#endif /* SRC_CODE_H_ */
