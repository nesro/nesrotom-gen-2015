#ifndef _STROM_
#define _STROM_

#include <vector>

#define _DO_YOU_WANT_ZASPOC_ 0

enum Operator {
	Plus = 0, //
	Minus = 1, //
	Times = 2, //
	Divide = 3, //
	Eq = 4, //
	NotEq = 5, //
	Less = 6, //
	Greater = 7, //
	LessOrEq = 8, //
	GreaterOrEq = 9, //
	Error = 10, //
};

class Node {
public:
	virtual Node *Optimize() {
		return this;
	}
	virtual int Translate() = 0;
	virtual ~Node() {
	}
	virtual void print() = 0;
};

class Expr: public Node {
};

class Statm: public Node {
};

class Var: public Expr {
public:
	int addr;
	bool rvalue;
	const char *name;
//	int value;
//	bool value_is_set;
	Var(int, bool, const char *);
	virtual int Translate();
	virtual void print();
};

class Numb: public Expr {
public:
	int value;
	Numb(int);
	virtual int Translate();
	int Value();
	virtual void print();
};

class Bop: public Expr {
public:
	Operator op;
	Expr *left, *right;
	int tmp_var;
	std::vector<Bop *> dup_childs;
	Bop *dup_parent;
	Bop(Operator, Expr*, Expr*);
	virtual ~Bop();
	virtual Node *Optimize();
	virtual int Translate();
	virtual void print();
};

class UnMinus: public Expr {
public:
	Expr *expr;
	UnMinus(Expr *e);
	virtual ~UnMinus();
	virtual Node *Optimize();
	virtual int Translate();
	virtual void print();
};

class Assign: public Statm {
public:
	Var *var;
	Expr *expr;
	Assign(Var*, Expr*);
	virtual ~Assign();
	virtual Node *Optimize();
	virtual int Translate();
	virtual void print();
};

class Read: public Statm {
public:
	Var *var;
	Read(Var *);
	virtual ~Read();
	virtual Node *Optimize();
	virtual int Translate();
	virtual void print();
};

class Write: public Statm {
public:
	Expr *expr;
	Write(Expr*);
	virtual ~Write();
	virtual Node *Optimize();
	virtual int Translate();
	virtual void print();
};

class If: public Statm {
public:
	Expr *cond;
	Statm *thenstm;
	Statm *elsestm;
	If(Expr*, Statm*, Statm*);
	virtual ~If();
	virtual Node *Optimize();
	virtual int Translate();
	virtual void print();
};

class While: public Statm {
public:
	Expr *cond;
	Statm *body;
	While(Expr*, Statm*);
	virtual ~While();
	virtual Node *Optimize();
	virtual int Translate();
	virtual void print();
};

class StatmList: public Statm {
public:
	Statm *statm;
	StatmList *next;
	StatmList(Statm*, StatmList*);
	virtual ~StatmList();
	virtual Node *Optimize();
	virtual int Translate();
	virtual void print();
};

class Empty: public Statm {
	virtual int Translate() {
		_fn();
		_return(0);
	}
	virtual void print();
};

class Prog: public Node {
public:
	StatmList *stm;
	Prog(StatmList*);
	virtual ~Prog();
	virtual Node *Optimize();
	virtual int Translate();
	virtual void print();
};

Expr *VarOrConst(char*);

#endif
