#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "code.h"
#include "debug.h"

int CodeLine::last_line_number = 0;

extern TmSource *g_ts;

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

void TmSource::print(const char *file_name, bool printComments) {
	FILE *f = NULL;

	if (file_name != NULL) {
		f = fopen(file_name, "w");

		if (f == NULL) {
			fprintf(stderr, "Error: opening file %s\n", file_name);
		}
	} else {
		f = stdout;
	}

	for (std::vector<int>::size_type i = 0; i != this->lines.size(); i++) {
		/* std::cout << someVector[i]; ... */

		static const char *instr_str[] = { "HALT", "IN", "OUT", "ADD", "INC",
				"DEC", "SHR", "SHL", "SUB", "MUL", "DIV", "LD", "ST", "LDA",
				"LDC", "JLT", "JLE", "JGT", "JGE", "JEQ", "JNE", };

		switch (lines[i]->type) {
		case BLANK_T:
			fprintf(f, "%02d: BLANK\n", this->lines[i]->line_number);
			break;
		case COMMENT_T:
			if (!printComments) {
				break;
			}

			fprintf(f, "* %s\n",
					this->lines[i]->comment_str == NULL ?
							"null" : this->lines[i]->comment_str);
			break;
		case RR:
			if (printComments && this->lines[i]->comment_str != NULL) {
				fprintf(f, "* %s\n", this->lines[i]->comment_str);
			}
			fprintf(f, "%02d:%10s%10d,%d,%d\n", this->lines[i]->line_number,
					instr_str[this->lines[i]->instruction],
					this->lines[i]->op[0], this->lines[i]->op[1],
					this->lines[i]->op[2]);
			break;
		case RM:
		case RA:
			if (printComments && this->lines[i]->comment_str != NULL) {
				fprintf(f, "* %s\n", this->lines[i]->comment_str);
			}
			fprintf(f, "%02d:%10s%10d,%d(%d)\n", this->lines[i]->line_number,
					instr_str[this->lines[i]->instruction],
					this->lines[i]->op[0], this->lines[i]->op[1],
					this->lines[i]->op[2]);
			break;
		default:
			assert(0);
			break;
		}
	}

	if (f != NULL) {
		fclose(f);
	}
}

TmSource::TmSource(void) {
}

TmSource::~TmSource(void) {
}

/*****************************************************************************/

int Storage::push(void) {
	_fn();
	if (g_debug_level > 5)
		this->print();

	if (this->reg_cnt == this->reg_stack_size) {
		/* ST r,d(s) = mem(d + reg(s)) = reg(r) */

		if (g_debug_level > 5) {
			print_debug_depth();
			fprintf(stderr, "ST %d\n",
					(this->reg_top + 1) % this->reg_stack_size);
		}

		g_ts->addInstr(ST, (this->reg_top + 1) % this->reg_stack_size,
				(++this->mem_top), Storage::zero);
	} else {
		this->reg_cnt++;
	}

	this->reg_top++;
	this->reg_top %= this->reg_stack_size;

	if (g_debug_level > 5)
		this->print();

	assert(0 <= this->reg_top);
	_return(this->reg_top);
}

int Storage::pop(void) {
	_fn();
	int ret;

	if (g_debug_level > 5)
		this->print();

	ret = this->reg_top;

	if (this->reg_cnt == 0) {
		assert(0 <= this->mem_top);
		if (g_debug_level > 5) {
			print_debug_depth();
			fprintf(stderr, "LD %d\n", this->mem_top);
		}
		/* LD r,d(s) = reg(r) = mem(d + reg(s)) */
		g_ts->addInstr(LD, --this->reg_top, (this->mem_top--), Storage::zero,
				"pop");
	} else {
		this->reg_cnt--;
		this->reg_top--;
	}

	if (this->reg_top == -1) {
		this->reg_top = this->reg_stack_size - 1;
	}

	if (g_debug_level > 5) {
		this->print();
		print_debug_depth();
		fprintf(stderr, "ret=%d\n", ret);
	}

	_return(ret);
}

int Storage::top(void) {
	_fn();
	if (g_debug_level > 5)
		this->print();

	/* kdyz jsme popli posledni registr a ptame se na top, ktery je ale v mem */
	if (this->reg_cnt == 0) {
		assert(0 <= this->mem_top);
		if (g_debug_level > 5) {
			print_debug_depth();
			fprintf(stderr, "LD %d\n", this->mem_top);
		}
		/* LD r,d(s) = reg(r) = mem(d + reg(s)) */
		g_ts->addInstr(LD, this->reg_top, (this->mem_top--), Storage::zero,
				"top");

		if (this->reg_top == -1) {
			this->reg_top = this->reg_stack_size - 1;
		}

		this->reg_cnt++;
	}

	assert(0 <= this->reg_cnt);
	assert(0 <= this->reg_top);
	_return(this->reg_top);
}

void Storage::print(void) {
	print_debug_depth();
	fprintf(stderr, "reg_top=%d, reg_cnt=%d, mem_top=%d\n", this->reg_top,
			this->reg_cnt, this->mem_top);
}

Storage::Storage() {
	_fn();

	this->reg_cnt = 0;
	this->reg_top = -1;
	this->mem_top = 100;

	_return_void;
}

Storage::~Storage() {
	_fn();
	_return_void;
}
