/* strom.cpp */

#include <vector>
#include <map>
#include <iostream>

#include "debug.h"
#include "strom.h"
#include "tabsym.h"

#include "code.h"

extern TmSource *g_ts;
extern Storage *g_s;

/******************************************************************************/
/* TODO FIXME */
/* TODO FIXME */
/* TODO FIXME */
/* TODO FIXME */

/* note: if the order doesn't matter, do to the deeper part first please */
int get_bop_depth(Bop *a) {
	_fn();
	int size = 1;
	Bop *l = dynamic_cast<Bop *>(a->left);
	Bop *r = dynamic_cast<Bop *>(a->right);
	if (l) {
		size += get_bop_depth(l);
	}
	if (r) {
		size += get_bop_depth(r);
	}
	_return(size);
}

bool cmp_numbvar(Expr *a, Expr *b) {
	_fn();
	Numb *numb_a = dynamic_cast<Numb *>(a);
	Numb *numb_b = dynamic_cast<Numb *>(b);
	Var *var_a = dynamic_cast<Var *>(a);
	Var *var_b = dynamic_cast<Var *>(b);
	Bop *bop_a = dynamic_cast<Bop *>(a);
	Bop *bop_b = dynamic_cast<Bop *>(b);

	_debug("na=%p, nb=%p, va=%p, vb=%p, ba=%p, bb=%p\n", (void * )numb_a,
			(void * )numb_b, (void * )var_a, (void * )var_b, (void * )bop_a,
			(void * )bop_b);

	assert(!bop_a);
	assert(!bop_b);
	assert(numb_a || var_a);
	assert(numb_b || var_b);
	if (numb_a && numb_b) {
		_return(numb_a->Value() == numb_b->Value());
	}
	if (var_a && var_b) {
		_return(var_a->addr == var_b->addr);
	}
	_return(false);
}

bool cmp_bop(Bop *l, Bop *r) {
	_fn();
	_debug("left bop=%p, op=%d\n", (void * )l, l ? l->op : 0);
	_debug("right bop=%p, op=%d\n", (void * )r, r ? r->op : 0);

	if (l->op != r->op) {
		_return(false);
	}
	Bop *ll = dynamic_cast<Bop *>(l->left);
	Bop *lr = dynamic_cast<Bop *>(l->right);
	Bop *rl = dynamic_cast<Bop *>(r->left);
	Bop *rr = dynamic_cast<Bop *>(r->right);
	_debug("ll=%p, lr=%p, rl=%p, rr=%p\n", (void * )ll, (void * )lr,
			(void * )rl, (void * )rr);
	if ((ll && !rl) || (!ll && rl) || (lr && !rr) || (!lr && rr)) {
		_return(false);
	}
	if (!ll && !rl && !cmp_numbvar(l->left, r->left)) {
		_return(false);
	}
	if (!lr && !rr && !cmp_numbvar(l->right, r->right)) {
		_return(false);
	}
	if (ll && rl && !cmp_bop(ll, rl)) {
		_return(false);
	}
	if (lr && rr && !cmp_bop(lr, rr)) {
		_return(false);
	}
	_return(true);
}

/* true, if something has been found */
bool find_subtree_in_bop(std::vector<Bop *> &vb, Bop *haystack, Bop *needle) {
	_fn();

	static int tmp_var_pool = 50; /* TODO FIXME tohle by chtelo nejak vypocitat */

	_debug("haystack %p op=%d, needle %p op=%d\n", (void* )haystack,
			haystack->op, (void * )needle, needle->op);

	if (haystack != needle && needle->dup_parent != haystack
			&& haystack->dup_parent != needle && cmp_bop(needle, haystack)) {

		Bop *orig = needle;
		while (orig->dup_parent && orig->dup_parent != orig) {
			_debug("orig=%p, orig->dup_parent=%p\n", (void* )orig,
					(void* )orig->dup_parent);
			orig = orig->dup_parent;
		}
		if (!needle->dup_parent) {
			_debug("VBPUSH needle %p\n", (void * )needle);
			vb.push_back(needle);
			needle->dup_parent = needle; /* FIXME je sam sobe parentem */
			needle->tmp_var = tmp_var_pool++;
		}
		_debug("ORIGPUSH haystack %p\n", (void * )haystack);
		orig->dup_childs.push_back(haystack);
		haystack->dup_parent = needle;

		assert(haystack->op == needle->op);
		_debug("!!!!!!! op=%d %p and %p are the same!\n", haystack->op,
				(void* )haystack, (void* )needle);

		_return(true);
	}

	Bop *hl = dynamic_cast<Bop *>(haystack->left);
	Bop *hr = dynamic_cast<Bop *>(haystack->right);
	_debug("hl=%p, hr=%p\n", (void * )hl, (void * )hr);
	if (hl && find_subtree_in_bop(vb, hl, needle)) {
//		_return(true);
	}
	if (hr && find_subtree_in_bop(vb, hr, needle)) {
//		_return(true);
	}

	//	if (l) {
	//		_debug("left bop=%p, op=%d\n", (void * )l, l ? l->op : 0);
	//	}
	//	if (r) {
	//		_debug("right bop=%p, op=%d\n", (void * )r, r ? r->op : 0);
	//	}

	_return(false);
}

/* true, if something has been found */
bool local_optimize(std::vector<Bop *> &vb, Bop *h, Bop *n) {
	_fn();
	static int local_optimize_calls = 0;
	_debug("local optimize op=%d calls=%d \n", h->op, local_optimize_calls++);
	Bop *l = dynamic_cast<Bop *>(n->left);
	Bop *r = dynamic_cast<Bop *>(n->right);
	_debug("left %p\n", (void* )l);
	_debug("right %p\n", (void* )r);
	if (l && !l->dup_parent && find_subtree_in_bop(vb, h, l)) {
//		_return(true);
	}
	if (r && !r->dup_parent && find_subtree_in_bop(vb, h, r)) {
//		_return(true);
	}
	if (l && !l->dup_parent && local_optimize(vb, h, l)) {
//		_return(true);
	}
	if (r && !r->dup_parent && local_optimize(vb, h, r)) {
//		_return(true);
	}
	_return(false);
}

//void try2_find_bops(std::vector<Bop *> &bops, Bop *b) {
//	if (b) {
//		bops.push_back(b);
//		Bop *ll = dynamic_cast<Bop *>(b->left);
//		Bop *lr = dynamic_cast<Bop *>(b->right);
//		if (ll) {
//			try2_find_bops(bops, ll);
//		}
//		if (lr) {
//			try2_find_bops(bops, lr);
//		}
//	}
//}

//void try2_optimize(Bop *b) {
//	std::vector<Bop *> bops;
//	try2_find_bops(bops, b);
//
//	for (int l = 0; l < bops.size(); ++l) {
//		for (int r = l + 1; r < bops.size(); ++r) {
//			if (cmp_bop(bops[l], bops[r])) {
//				Bop *bl = bops[l];
//				Bop *br = bops[r];
//
//				br->dup_parent = bl;
//			}
//		}
//	}
//}

/******************************************************************************/

void replaceDups(std::vector<Bop *> &vb, Bop *a) {
	_fn();
	bool bl = false, br = false;
	Bop *l = dynamic_cast<Bop *>(a->left);
	Bop *r = dynamic_cast<Bop *>(a->right);

//	for (auto i = vb.begin(); i != vb.end(); ++i)
//		std::cerr << *i << ' ';
//	std::cerr << std::endl;

	_debug("a=%p, l=%p r=%p\n", (void* )a, (void* )l, (void* )r);
//	for (std::vector<Bop*>::iterator it = vb.begin(); it != vb.end(); ++it) {
	for (unsigned i = 0; i < vb.size(); ++i) {
		_debug("replace dups searching for %p\n", (void* )vb[i]);
		if (l && l->dup_parent == vb[i]) {
//			delete a->left;
			a->left = NULL;
			_debug("REPLACING %p\n", (void* )&(a->left));
			a->left = new Var((vb[i])->tmp_var, true);
			_debug("NOW IT IS %p\n", (void* )&(a->left));
			bl = true;
//			vb.erase(vb.begin() + i);
		}
		if (r && r->dup_parent == vb[i]) {
//			delete a->right;
			a->right = NULL;
			_debug("REPLACING %p\n", (void* )&(a->right));
			a->right = new Var((vb[i])->tmp_var, true);
			_debug("NOW IT IS %p\n", (void* )&(a->right));
//			vb.erase(vb.begin() + i);
			br = true;
		}
	}
	if (l && !bl) {
		replaceDups(vb, l);
	}
	if (r && !br) {
		replaceDups(vb, r);
	}
	_return_void;
}

Node * Assign::Optimize() {
	_fn();

	expr = (Var *) (expr->Optimize());

//	int tmp_var_location = 50; /* FIXME TODO tohle se da vycist z tabsym ne? */
	std::vector<Bop *> vb; /* vector of bops and its number (for tmp var) */

	Bop *bop = dynamic_cast<Bop *>(this->expr);
	if (bop) {

		/* find the same bops */
		while (local_optimize(vb, bop, bop))
			;

		StatmList *statm = new StatmList(this, NULL);

		/* replace the bops with the tmp var */
		for (std::vector<Bop *>::iterator it = vb.begin(); it != vb.end();
				++it) {

			_debug("bop parent addr=%p\n", (void * )*it);

			/* create new tmp var and add it to the statm list */
			Assign *tmp_var = new Assign(new Var((*it)->tmp_var, false), *it);
			statm = new StatmList(tmp_var, statm);

//			/* replace all bops with this tmp var */
//			for (std::vector<Bop *>::iterator it2 = (*it)->dup_childs.begin();
//					it2 != (*it)->dup_childs.end(); ++it2) {
//
//				_debug("bop child addr=%p\n", (void * )*it2);
//				Bop **child_to_replace = &(*it2);
//				_debug("child_to_replace %p\n", (void * )*child_to_replace);
//				delete *child_to_replace;
//				Var *newVar = new (*child_to_replace) Var(tmp_var_location,
//						false);
//				_debug("newVar %p\n", (void * )newVar);
//
////				Bop **b = &(*it2);
////				**b = (Bop *) (new Var(tmp_var_location, false));
////				_debug("reaplacing child dup %p\n", (void *)**it2);
////				*it2 = (Bop *) (new Var(tmp_var_location, false));
//			}
////			Bop **b2 = &(*it);
////			*b2 = (Bop *) (new Var(tmp_var_location, false));
////			_debug("reaplacing parent dup %p\n", (void * )**it);
//			*it = (Bop *) (new Var(tmp_var_location, false));
//
//			tmp_var_location++;
		}

		replaceDups(vb, bop);

		_return(statm);
	}

	_return(this);
}

Var::Var(int a, bool rv) {
	_fn();
	addr = a;
	rvalue = rv;
	_return_void;
}

Numb::Numb(int v) {
	_fn();
	value = v;
	_return_void;
}

int Numb::Value() {
	_fn();
	_return(value);
}

Bop::Bop(Operator o, Expr *l, Expr *r) {
	_fn();
	op = o;
	left = l;
	right = r;
	dup_parent = NULL;
	tmp_var = -1;
	_return_void;
}

Bop::~Bop() {
	_fn();
	delete left;
	delete right;
	_return_void;
}

UnMinus::UnMinus(Expr * e) {
	_fn();
	expr = e;
	_return_void;
}

UnMinus::~UnMinus() {
	_fn();
	delete expr;
	_return_void;
}

Assign::Assign(Var * v, Expr * e) {
	_fn();
	var = v;
	expr = e;
	_return_void;
}

Assign::~Assign() {
	_fn();
	delete var;
	delete expr;
	_return_void;
}

Write::Write(Expr * e) {
	_fn();
	expr = e;
	_return_void;
}

Write::~Write() {
	_fn();
	delete expr;
	_return_void;
}

Read::Read(Var * v) {
	_fn();
	var = v;
	_return_void;
}

Read::~Read() {
	_fn();
	delete var;
	_return_void;
}

If::If(Expr * c, Statm * ts, Statm * es) {
	_fn();
	cond = c;
	thenstm = ts;
	elsestm = es;
	_return_void;
}

If::~If() {
	_fn();
	delete cond;

	if (thenstm) {
		delete thenstm;
	}

	if (elsestm) {
		delete elsestm;
	}

	_return_void;
}

While::While(Expr * c, Statm * b) {
	_fn();
	cond = c;
	body = b;
	_return_void;
}

While::~While() {
	_fn();
	delete body;
	_return_void;
}

StatmList::StatmList(Statm * s, StatmList * n) {
	_fn();
	statm = s;
	next = n;
	_return_void;
}

StatmList::~StatmList() {
	_fn();
	delete statm;
	delete next;
	_return_void;
}

Prog::Prog(StatmList * s) {
	_fn();
	stm = s;
	_return_void;
}

Prog::~Prog() {
	_fn();
	delete stm;
	_return_void;
}

/****************************************************************** OPTIMIZE */

Node * Bop::Optimize() {
	_fn();

	left = (Expr *) (left->Optimize());
	right = (Expr *) (right->Optimize());
	Numb *l = dynamic_cast<Numb *>(left);
	Numb *r = dynamic_cast<Numb *>(right);

	if (!l || !r) {
		return this;
	}

	int res;
	int leftval = l->Value();
	int rightval = r->Value();

	switch (op) {
	case Plus:
		res = leftval + rightval;
		break;

	case Minus:
		res = leftval - rightval;
		break;

	case Times:
		res = leftval * rightval;
		break;

	case Divide:
		res = leftval / rightval;
		break;

	case Eq:
		res = leftval == rightval;
		break;

	case NotEq:
		res = leftval != rightval;
		break;

	case Less:
		res = leftval < rightval;
		break;

	case Greater:
		res = leftval > rightval;
		break;

	case LessOrEq:
		res = leftval <= rightval;
		break;

	case GreaterOrEq:
		res = leftval >= rightval;
		break;

	case Error: //nenastane
	default:
		res = 0;
		assert(0);
		break;
	}

	delete this;
	_return(new Numb(res));
}

Node * UnMinus::Optimize() {
	_fn();
	expr = (Expr *) expr->Optimize();
	Numb *e = dynamic_cast<Numb *>(expr);

	if (!e) {
		_return(this);
	}

	e = new Numb(-e->Value());
	delete this;
	_return(e);
}

Node * Write::Optimize() {
	_fn();
	expr = (Expr *) (expr->Optimize());
	_return(this);
}

Node * Read::Optimize() {
	_fn();
//FIXME?
	_return(this);
}

Node * If::Optimize() {
	_fn();
	cond = (Expr *) (cond->Optimize());
	thenstm = (Statm *) (thenstm->Optimize());

	if (elsestm) {
		elsestm = (Statm *) (elsestm->Optimize());
	}

	Numb *c = dynamic_cast<Numb *>(cond);

	if (!c) {
		_return(this);
	}

	Node *res;

	if (c->Value()) {
		res = thenstm;
		thenstm = 0;
	} else {
		res = elsestm;
		elsestm = 0;
	}

	delete this;
	_return(res);
}

Node * While::Optimize() {
	_fn();
	cond = (Expr *) (cond->Optimize());
	body = (Statm *) (body->Optimize());
	Numb *c = dynamic_cast<Numb *>(cond);

	if (!c) {
		_return(this);
	}

	if (!c->Value()) {
		delete this;
		_return(new Empty);
	}

	_return(this);
}

Node * StatmList::Optimize() {
	_fn();
	StatmList *s = this;

	do {
		s->statm = (Statm *) (s->statm->Optimize());
		s = s->next;
	} while (s);

	_return(this);
}

Node * Prog::Optimize() {
	_fn();

	_debug("-------------- optimize %d\n", 0);

	stm = (StatmList *) (stm->Optimize());
	_return(this);
}

/****************************************************************** TRANSLATE */

/******************************************************************************/

int Var::Translate() {
	_fn();
	g_ts->addInstr(LD, g_s->push(), addr, Storage::zero);
	_return(0);
}

int Numb::Translate() {
	_fn();
	if (g_debug_level > 5)
		fprintf(stderr, "numb=%d\n", value);
	g_ts->addInstr(LDC, g_s->push(), value, 0);
	_return(0);
}

/* vysledek bopu na vrcholu zasobniku */
int Bop::Translate() {
	_fn();

	if (g_debug_level > 5)
		fprintf(stderr, "left translate\n");
	left->Translate(); //result of the left is on the top

	if (g_debug_level > 5)
		fprintf(stderr, "right translate\n");
	right->Translate(); // result of the right is on the top

	int ra = g_s->pop();
	int rb = g_s->top();

	switch (op) {
	case Plus:
		g_ts->addInstr(ADD, rb, rb, ra, "op +");
		break;
	case Minus:
		g_ts->addInstr(SUB, rb, rb, ra, "op -");
		break;
	case Times:
		g_ts->addInstr(MUL, rb, rb, ra, "op *");
		break;
	case Divide:
		g_ts->addInstr(DIV, rb, rb, ra, "op /");
		break;
	case Less:
		g_ts->addInstr(SUB, rb, ra, rb, "op <");
		g_ts->addInstr(JLT, rb, 2, Storage::pc);
		g_ts->addInstr(LDC, rb, 0, rb);
		g_ts->addInstr(LDA, Storage::pc, 1, Storage::pc);
		g_ts->addInstr(LDC, rb, 1, rb);
		break;
	case Eq:
		g_ts->addInstr(SUB, rb, ra, rb, "op ==");
		g_ts->addInstr(JEQ, rb, 2, Storage::pc);
		g_ts->addInstr(LDC, rb, 0, rb);
		g_ts->addInstr(LDA, Storage::pc, 1, Storage::pc);
		g_ts->addInstr(LDC, rb, 1, rb);
		break;
	case NotEq:
		g_ts->addInstr(SUB, rb, ra, rb, "op !=");
		g_ts->addInstr(JNE, rb, 2, Storage::pc);
		g_ts->addInstr(LDC, rb, 0, rb);
		g_ts->addInstr(LDA, Storage::pc, 1, Storage::pc);
		g_ts->addInstr(LDC, rb, 1, rb);
		break;
	case Greater:
		g_ts->addInstr(SUB, rb, ra, rb, "op >");
		g_ts->addInstr(JGT, rb, 2, Storage::pc);
		g_ts->addInstr(LDC, rb, 0, rb);
		g_ts->addInstr(LDA, Storage::pc, 1, Storage::pc);
		g_ts->addInstr(LDC, rb, 1, rb);
		break;
	case LessOrEq:
		g_ts->addInstr(SUB, rb, ra, rb, "op <=");
		g_ts->addInstr(JLE, rb, 2, Storage::pc);
		g_ts->addInstr(LDC, rb, 0, rb);
		g_ts->addInstr(LDA, Storage::pc, 1, Storage::pc);
		g_ts->addInstr(LDC, rb, 1, rb);
		break;
	case GreaterOrEq:
		g_ts->addInstr(SUB, rb, ra, rb, "op >=");
		g_ts->addInstr(JGE, rb, 2, Storage::pc);
		g_ts->addInstr(LDC, rb, 0, rb);
		g_ts->addInstr(LDA, Storage::pc, 1, Storage::pc);
		g_ts->addInstr(LDC, rb, 1, rb);
		break;
	default:
		break;
	}

	_return(0);
}

int UnMinus::Translate() {
	_fn();
	int p;
	int t;
	expr->Translate();
	g_ts->addInstr(LDC, g_s->push(), 0, 0);
	p = g_s->pop();
	t = g_s->top();
	g_ts->addInstr(SUB, t, p, t, "op -");
	_return(0);
}

int Assign::Translate() {
	_fn();

	expr->Translate(); // pushes result on top of the stack
	g_ts->addInstr(ST, g_s->top(), var->addr, Storage::zero);
	(void) g_s->pop();
	_return(0);
}

int Write::Translate() {
	_fn();
	expr->Translate();
	g_ts->addInstr(OUT, g_s->pop(), 0, 0);
	_return(0);
}

int Read::Translate() {
	_fn();
	int reg = g_s->push();
	g_ts->addInstr(IN, reg, 0, 0);
	g_ts->addInstr(ST, reg, var->addr, Storage::zero);
	(void) g_s->pop();
	_return(0);
}

int If::Translate() {
	_fn();

	int a1 = 0;
	int a2 = 0;

	a1 = a2;
	a2 = a1;

	cond->Translate();

	thenstm->Translate();

	if (elsestm) {
		elsestm->Translate();
	} else {
		//
	}

	_return(0);
}

int While::Translate() {
	_fn();
	int a1 = 0;
	int a2 = 0;

	a1 = a2;
	a2 = a1;

	cond->Translate();
	body->Translate();

	_return(0);
}

int StatmList::Translate() {
	_fn();
	StatmList *s = this;

	do {
		s->statm->Translate();
		s = s->next;
	} while (s);

	_return(0);
}

int Prog::Translate() {
	_fn();
	if (0 < g_debug_level)
		fprintf(stderr,
				"-----------------------------------------TRANSLATE XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
//TODO: kontrola, jestli mam dost pameti - pridat do readme
//	g_ts->addInstr(LD, 6, 0, 0);
//	g_ts->addInstr(ST, 0, 0, 0);
	stm->Translate();
	g_ts->addInstr(HALT, 0, 0, 0);
	_return(0);
}

/************************************************************** TRANSLATE END */

Expr *VarOrConst(char *id) {
	_fn();
	int v;
	DruhId druh = idPromKonst(id, &v);

	switch (druh) {
	case IdProm:
		_return(new Var(v, true));
		break;

	case IdKonst:
		_return(new Numb(v));
		break;

	case Nedef:
	default:
		_return(0);
		break;
	}
}

/************************************************************************ print */

FILE *print_loc = stderr;

void Var::print() {
	_fn();
	_debug("(var %d)\n", this->addr);
	_return_void;
}

void Numb::print() {
	_fn();
	_debug2("(numb)\n");
	_return_void;
}

void Bop::print() {
	_fn();
	_debug2("<BOP>\n");
	left->print();
	_debug("<op=%d>\n", this->op);
	right->print();
	_debug2("<BOP>\n");
	_return_void;
}

void UnMinus::print() {
	_fn();
	_debug2("<UnMinus>\n");
	expr->print();
	_debug2("</UnMinus>\n");
	_return_void;
}

void Assign::print() {
	_fn();
	_debug2("<Assign>\n");
	_debug2("\t<var>\n");
	var->print();
	_debug2("\t</var>\n");
	_debug2("\t<expr>\n");
	expr->print();
	_debug2("\t</expr>\n");
	_debug2("</Assign>\n");
	_return_void;
}

void Write::print() {
	_fn();
	_debug2("<write>\n");
	expr->print();
	_debug2("</write>\n");
	_return_void;
}

void Read::print() {
	_fn();
	_debug2("<read>k\n");
	_return_void;
}

void If::print() {
	_fn();
	_debug2("If (");
	cond->print();
	_debug2(") {\n");
	thenstm->print();
	_debug2("}\n");
	if (elsestm) {
		_debug2("else{\n");
		elsestm->print();
		_debug2("}\n");
	}
	_return_void;
}

void While::print() {
	_fn();
	_debug2("While=(");
	cond->print();
	_debug2(") {\n");
	body->print();
	_debug2("}=While\n");
	_return_void;
}

void StatmList::print() {
	_fn();
	_debug2("<StatmList>\n");
	StatmList *s = this;
	do {
		if (s->statm != NULL) {
			s->statm->print();
		}
		s = s->next;
	} while (s);
	_debug2("</StatmList>\n");
	_return_void;
}

void Empty::print() {
	_fn();
	_debug2("<Empty>\n");
	_return_void;
}

void Prog::print() {
	_fn();
	_debug2("<Prog>\n");
	stm->print();
	_return_void;
}

