#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "code.h"

int CodeLine::last_line_number = 0;

type_t CodeLine::get_type(instruction_t instr) {
	switch (instr) {
	case HALT:
	case IN:
	case OUT:
	case ADD:
	case INC:
	case DEC:
	case SHR:
	case SHL:
	case SUB:
	case MUL:
	case DIV:
		return (RR);
		break;
	case LD:
	case ST:
		return (RM);
		break;
	case LDA:
	case LDC:
	case JLT:
	case JLE:
	case JGT:
	case JGE:
	case JEQ:
	case JNE:
		return (RA);
		break;
	case BLANK:
		return (BLANK_T);
		break;
	case COMMENT:
		return (COMMENT_T);
		break;
	default:
		assert(0);
		break;
	}
}

CodeLine::CodeLine(void) {
	this->instruction = BLANK;
	this->op[0] = 0;
	this->op[1] = 1;
	this->op[2] = 2;
	this->comment_str = NULL;
	this->type = this->get_type(this->instruction);
	this->line_number = this->last_line_number++;
}

CodeLine::CodeLine(instruction_t instr, int op0, int op1, int op2,
		const char *comment) {
	this->instruction = instr;
	this->op[0] = op0;
	this->op[1] = op1;
	this->op[2] = op2;
	this->comment_str = comment;
	this->type = this->get_type(this->instruction);

	if (instr != COMMENT) {
		this->line_number = this->last_line_number++;
	} else {
		this->line_number = this->last_line_number;
	}
}

CodeLine::~CodeLine(void) {
	// FIXME
	//free(this->comment_str);
}

/*****************************************************************************/

int TmSource::addComment(const char *comment) {
	CodeLine *cl = new CodeLine(COMMENT, 0, 0, 0, comment);
	this->lines.push_back(cl);
	return cl->line_number;
}

int TmSource::addBlank(void) {
	CodeLine *cl = new CodeLine(BLANK, 0, 0, 0, NULL);
	this->lines.push_back(cl);
	return cl->line_number;
}

int TmSource::addInstr(instruction_t instr, int op0, int op1, int op2) {
	CodeLine *cl = new CodeLine(instr, op0, op1, op2, NULL);
	this->lines.push_back(cl);
	return cl->line_number;
}

int TmSource::addInstr(instruction_t instr, int op0, int op1, int op2,
		const char *comment) {
	CodeLine *cl = new CodeLine(instr, op0, op1, op2, comment);
	this->lines.push_back(cl);
	return cl->line_number;
}

void TmSource::print(bool printComments) {
	for (std::vector<int>::size_type i = 0; i != this->lines.size(); i++) {
		/* std::cout << someVector[i]; ... */

		static const char *instr_str[] = { "HALT", "IN", "OUT", "ADD", "INC",
				"DEC", "SHR", "SHL", "SUB", "MUL", "DIV", "LD", "ST", "LDA",
				"LDC", "JLT", "JLE", "JGT", "JGE", "JEQ", "JNE", };

		switch (lines[i]->type) {
		case BLANK_T:
			printf("%02d:\tBLANK\n", this->lines[i]->line_number);
			break;
		case COMMENT_T:
			if (!printComments) {
				break;
			}

			printf("// %s\n",
					this->lines[i]->comment_str == NULL ?
							"null" : this->lines[i]->comment_str);
			break;
		case RR:
			printf("%d:\t%s\t%d,%d,%d\n", this->lines[i]->line_number,
					instr_str[this->lines[i]->instruction],
					this->lines[i]->op[0], this->lines[i]->op[1],
					this->lines[i]->op[2]);
			break;
		case RM:
		case RA:
			printf("%d:\t%s\t%d,%d(%d)\n", this->lines[i]->line_number,
					instr_str[this->lines[i]->instruction],
					this->lines[i]->op[0], this->lines[i]->op[1],
					this->lines[i]->op[2]);
			break;
		default:
			assert(0);
			break;
		}
	}
}

TmSource::TmSource(void) {
}

TmSource::~TmSource(void) {
}
