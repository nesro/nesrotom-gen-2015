
#ifndef _STROM_
#define _STROM_

#define _DO_YOU_WANT_ZASPOC_ 0

enum Operator {
   Plus, //
   Minus, //
   Times, //
   Divide, //
   Eq, //
   NotEq, //
   Less, //
   Greater, //
   LessOrEq, //
   GreaterOrEq, //
   Error, //
};

class Node {
public:
   virtual Node *Optimize() {return this;}
   virtual void Translate() = 0;
   virtual ~Node() {}
};

class Expr : public Node {
};

class Statm : public Node {
};

class Var : public Expr {
public:
   int addr;
   bool rvalue;
   Var(int, bool);
   virtual void Translate();
};

class Numb : public Expr {
public:
   int value;
   Numb(int);
   virtual void Translate();
   int Value();
};

class Bop : public Expr {
public:
   Operator op;
   Expr *left, *right;
   Bop(Operator, Expr*, Expr*);
   virtual ~Bop();
   virtual Node *Optimize();
   virtual void Translate();
};

class UnMinus : public Expr {
public:
   Expr *expr;
   UnMinus(Expr *e);
   virtual ~UnMinus();
   virtual Node *Optimize();
   virtual void Translate();
};

class Assign : public Statm {
public:
   Var *var;
   Expr *expr;
   Assign(Var*, Expr*);
   virtual ~Assign();
   virtual Node *Optimize();
   virtual void Translate();
};

class Write : public Statm {
public:
   Expr *expr;
   Write(Expr*);
   virtual ~Write();
   virtual Node *Optimize();
   virtual void Translate();
};

class Read : public Statm {
public:
   Expr *expr;
   Read(Expr*);
   virtual ~Read();
   virtual Node *Optimize();
   virtual void Translate();
};

class  If : public Statm {
public:
   Expr *cond;
   Statm *thenstm;
   Statm *elsestm;
   If(Expr*, Statm*, Statm*);
   virtual ~If();
   virtual Node *Optimize();
   virtual void Translate();
};

class While : public Statm {
public:
   Expr *cond;
   Statm *body;
   While(Expr*, Statm*);
   virtual ~While();
   virtual Node *Optimize();
   virtual void Translate();
};

class  StatmList : public Statm {
public:
   Statm *statm;
   StatmList *next;
   StatmList(Statm*, StatmList*);
   virtual ~StatmList();
   virtual Node *Optimize();
   virtual  void Translate();
};

class Empty : public Statm {
   virtual void Translate() {}
};

class Prog : public Node {
public:
   StatmList *stm;
   Prog(StatmList*);
   virtual ~Prog();
   virtual Node *Optimize();
   virtual void Translate();
};

Expr *VarOrConst(char*);

#endif
