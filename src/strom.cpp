/* strom.cpp */

#include <vector>
#include <map>
#include <iostream>
#include <algorithm>

#include "string.h"

#include "debug.h"
#include "strom.h"
#include "tabsym.h"

#include "code.h"

extern int g_optimize_level;
extern TmSource *g_ts;
extern Storage *g_s;

bool g_ImInWhile = false;

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

	static int tmp_var_pool = 100; /* TODO FIXME tohle by chtelo nejak vypocitat */
	static std::vector<Bop *> seen;

	_debug("haystack %p op=%d, needle %p op=%d\n", (void* )haystack,
			haystack->op, (void * )needle, needle->op);

	if (haystack != needle && needle->dup_parent != haystack
			&& haystack->dup_parent != needle && cmp_bop(needle, haystack)) {

		/* neni uz tady nahodou? */
		for (auto b : seen) {
			if (cmp_bop(needle, b)) {
//				assert(0 && "cmp_bop(needle, b)");
				_return(false);
			}
			if (cmp_bop(haystack, b)) {
//				assert(0 && "cmp_bop(haystack, b)");
				_return(false);
			}
			if (needle == b) {
//				assert(0 && "needle == b");
				_return(false);
			}
		}

		Bop *orig = needle;
		while (orig->dup_parent && orig->dup_parent != orig) {
			_debug("orig=%p, orig->dup_parent=%p\n", (void* )orig,
					(void* )orig->dup_parent);
			orig = orig->dup_parent;
		}
		if (!needle->dup_parent) {
			_debug("VBPUSH needle %p\n", (void * )needle);
			vb.push_back(needle);
			seen.push_back(needle);
			needle->dup_parent = needle; /* FIXME je sam sobe parentem */
			needle->tmp_var = tmp_var_pool++;
			_debug(
					"FSIB: needle->tmp_var =%d, needleop=%d," "left=%p, right=%p\n",
					needle->tmp_var, needle->op, (void* )needle->left,
					(void* )needle->right);
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
			a->left = new Var((vb[i])->tmp_var, true, NULL);
			_debug("NOW IT IS %p\n", (void* )&(a->left));
			bl = true;
//			vb.erase(vb.begin() + i);
		}
		if (r && r->dup_parent == vb[i]) {
//			delete a->right;
			a->right = NULL;
			_debug("REPLACING %p\n", (void* )&(a->right));
			a->right = new Var((vb[i])->tmp_var, true, NULL);
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
	_return_void
;}

Var::Var(int a, bool rv, const char *arg_name) {
	_fn();
	this->addr = a;
	this->rvalue = rv;
	this->name = arg_name;
//	this->value = 0;
//	this->value_is_set = false;
	this->usedEver = false;
	_return_void
;}

Numb::Numb(int v) {
	_fn();
	value = v;
	_return_void
;}

int Numb::Value() {
	_fn();
	_return(value);
}

Bop::Bop(Operator o, Expr *l, Expr *r) {
	_fn();
	op = o;
	if (0 < g_optimize_level) {
		Numb *ln = dynamic_cast<Numb *>(l);
		Numb *rn = dynamic_cast<Numb *>(r);
		Var *lv = dynamic_cast<Var *>(l);
		Var *rv = dynamic_cast<Var *>(r);
		Bop *lb = dynamic_cast<Bop *>(l);
		Bop *rb = dynamic_cast<Bop *>(r);
		_debug("ln=%p, rn=%p, lv=%p, rv=%p, lb=%p, rb=%p\n", (void * )ln,
				(void * )rn, (void * ) lv, (void * ) rb, (void * ) lb,
				(void * ) rb);
		if (o == Plus || o == Times) {
			if (ln && rn) { /* two numbers */
				left = l;
				right = r;
			} else if (ln && rv) { /* number and var A */
				left = l;
				right = r;
			} else if (rn && lv) { /* number and var B */
				left = r;
				right = l;
			} else if (lv && rv) { /* two vars */
				_debug("two vars \"%d\" and \"%d\" here\n", lv->addr, rv->addr);
//				if (0 < strcmp(lv->name, rv->name)) {
				if (lv->addr < rv->addr) {
					left = l;
					right = r;
				} else {
					left = r;
					right = l;
				}
			} else if (lb && rb) { /* two bops */
				if (get_bop_depth(lb) < get_bop_depth(rb)) {
					left = r;
					right = l;
				} else {
					left = l;
					right = r;
				}
			} else if (lb && rn) { /* bop and  number A */
				left = l;
				right = r;
			} else if (ln && rb) { /* bop and  number B */
				left = r;
				right = l;
			} else if (lb && rv) { /* bop and  var A */
				left = l;
				right = r;
			} else if (lv && rb) { /* bop and  var B */
				left = r;
				right = l;
			} else {
				assert(0 && "i dont knot how to bop for + and *");
				left = r;
				right = l;
			}
		} else {
			/* FIXME TODO if (o == Greater) {
			 left = r;
			 right = l;
			 o = LessOrEq;
			 } else if (o == GreaterOrEq) {
			 left = r;
			 right = l;
			 o = Less;
			 } else { */
			left = l;
			right = r;
		}
	} else {
		left = l;
		right = r;
	}

	dup_parent = NULL;
	tmp_var = -1;
	_return_void
;}

Bop::~Bop() {
	_fn();
	delete left;
	delete right;
	_return_void
;}

UnMinus::UnMinus(Expr * e) {
	_fn();
	expr = e;
	_return_void
;}

UnMinus::~UnMinus() {
	_fn();
	delete expr;
	_return_void
;}

Assign::Assign(Var * v, Expr * e) {
	_fn();
	var = v;
	expr = e;
	_return_void
;}

Assign::~Assign() {
	_fn();
	delete var;
	delete expr;
	_return_void
;}

Write::Write(Expr * e) {
	_fn();
	expr = e;
	_return_void
;}

Write::~Write() {
	_fn();
	delete expr;
	_return_void
;}

Read::Read(Var * v) {
	_fn();
	var = v;
	_return_void
;}

Read::~Read() {
	_fn();
	delete var;
	_return_void
;}

If::If(Expr * c, Statm * ts, Statm * es) {
	_fn();
	cond = c;
	thenstm = ts;
	elsestm = es;
	_return_void
;}

If::~If() {
	_fn();
	delete cond;

	if (thenstm) {
		delete thenstm;
	}

	if (elsestm) {
		delete elsestm;
	}

	_return_void
;}

While::While(Expr * c, Statm * b) {
	_fn();
	cond = c;
	body = b;
	_return_void
;}

While::~While() {
	_fn();
	delete body;
	_return_void
;}

StatmList::StatmList(Statm * s, StatmList * n) {
	_fn();
	statm = s;
	next = n;
	_return_void
;}

StatmList::~StatmList() {
	_fn();
	delete statm;
	delete next;
	_return_void
;}

Prog::Prog(StatmList * s) {
	_fn();
	stm = s;
	_return_void
;}

Prog::~Prog() {
	_fn();
	delete stm;
	_return_void
;}

/****************************************************************** OPTIMIZE */

Node * Assign::Optimize() {
	_fn();

	expr = (Var *) (expr->Optimize());

//	int tmp_var_location = 50; /* FIXME TODO tohle se da vycist z tabsym ne? */
	std::vector<Bop *> vb; /* vector of bops and its number (for tmp var) */

	Numb *numb = dynamic_cast<Numb *>(this->expr);
	if (numb) {
		_debug("it's a numb %p with val %d. var=%p\n", (void* )numb,
				numb->Value(), (void* )this->var);

		nastav(this->var->addr, numb->value);
//		this->var->value_is_set = true;
//		this->var->value = numb->value;
		_return(this);
	}

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
			char buf[50];
			snprintf(buf, 50, "tmp%d", (*it)->tmp_var);
			Assign *tmp_var = new Assign(new Var((*it)->tmp_var, false, buf),
					*it);
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

Node * Bop::Optimize() {
	_fn();

	_debug("left=%p, right=%p \n", (void* )left, (void* )right);

	left = (Expr *) (left->Optimize());
	right = (Expr *) (right->Optimize());

	assert(left);
	assert(right);

	Numb *l = dynamic_cast<Numb *>(left);
	Numb *r = dynamic_cast<Numb *>(right);

//	Var *usedLv = dynamic_cast<Var *>(left);
//	Var *usedRv = dynamic_cast<Var *>(right);
//	if (usedLv) {
//		_debug("666 Bop::Optimize(): varL %d has been used in bop\n", usedLv->addr);
//		usedLv->usedEver = true;
//	}
//	if (usedRv) {
//		_debug("666 Bop::Optimize(): varR %d has been used in bop\n", usedRv->addr);
//		usedRv->usedEver = true;
//	}

	Var *lv;
	Var *rv;
	if (!g_ImInWhile) {
		lv = dynamic_cast<Var *>(left);
		rv = dynamic_cast<Var *>(right);
	} else {
		lv = NULL;
		rv = NULL;
	}

	_debug("l=%p, r=%p lv=%p, rv=%p\n", (void* )l, (void* )r, (void* )lv,
			(void* )rv);
//	if (lv) {
//		_debug("lv->value_is_set=%d\n", lv->value);
//	}
//	if (rv) {
//		_debug("rv->value_is_set=%d\n", rv->value);
//	}

	bool leftval_set = false;
	bool rightval_set = false;
	int leftval = -666;
	int rightval = -666;

	if (l) {
		leftval = l->Value();
		leftval_set = true;
	}
	if (r) {
		rightval = r->Value();
		rightval_set = true;
	}
	if (lv && je_nastaveno(lv->addr)) {
		leftval = vrat(lv->addr);
		leftval_set = true;
		_debug("left is var with value %d\n", leftval);
	}
	if (rv && je_nastaveno(rv->addr)) {
		rightval = vrat(rv->addr);
		rightval_set = true;
		_debug("right is var with value %d\n", rightval);
	}

	/* multi by 0 is zero */
	if (this->op == Times
			&& ((rightval_set && rightval == 0) || (leftval_set && leftval == 0))) {
		delete this;
		_return(new Numb(0));
	}

	/* multi by 1 */
	if (this->op == Times) {
		if (rightval_set && rightval == 1) {
			if (l) {
				delete this;
				_return(new Numb(l->Value()));
			} else if (lv) {
				if (je_nastaveno(lv->addr)) {
					delete this;
					_return(new Numb(vrat(lv->addr)));
				} else {
					delete this;
//					_return(new Numb(l->Value()));
//					_return(lv);
					_return(new Var(lv->addr, false, NULL));
				}
			} else {
				assert(0);
			}
		}
		if (leftval_set && leftval == 1) {
			if (r) {
				delete this;
				_return(new Numb(r->Value()));
			} else if (rv) {
				if (je_nastaveno(rv->addr)) {
					delete this;
					_return(new Numb(vrat(rv->addr)));
				} else {
					delete this;
//					_return(new Numb(r->Value()));
//					_return(rv);
					_return(new Var(rv->addr, false, NULL));
				}
			} else {
				assert(0);
			}
		}
	}

	/* plus / minus zero does nothin */
	if (this->op == Plus || this->op == Minus) {
		if (rightval_set && rightval == 0) {
			if (l) {
				delete this;
				_return(new Numb(l->Value()));
			} else if (lv) {
				if (je_nastaveno(lv->addr)) {
					delete this;
					_return(new Numb(vrat(lv->addr)));
				} else {
					delete this;
//					_return(new Numb(l->Value()));
//					_return(lv);
					_return(new Var(lv->addr, false, NULL));
				}
			} else {
				assert(0);
			}
		}
		if (leftval_set && leftval == 0) {
			if (r) {
				delete this;
				_return(new Numb(r->Value()));
			} else if (rv) {
				if (je_nastaveno(rv->addr)) {
					delete this;
					_return(new Numb(vrat(rv->addr)));
				} else {
					delete this;
//					_debug("op is %d and leftval is zero. returning right %d\n",
//							this->op, r->Value());
//					_return(new Numb(r->Value()));
					_return(new Var(rv->addr, false, NULL));
				}
			} else {
				assert(0);
			}
		}
	}

	if (!leftval_set || !rightval_set) {
		_debug2("bop doesnt have values to evalute righnow\n");
		_return(this);
	}

	int res;

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

std::vector<int> g_used_vars;
Node * Write::Optimize() {
	_fn();
	expr = (Expr *) (expr->Optimize());

	Var *usedLv = dynamic_cast<Var *>(this->expr);
	_debug("666 Write::Optimize(), usedLv=%p\n", (void* )usedLv);
	if (usedLv) {
		_debug("666 Write::Optimize(): write used variable %d\n", usedLv->addr);
		usedLv->usedEver = true;
		g_used_vars.push_back(usedLv->addr);
	}

	_return(this);
}

Node * Read::Optimize() {
	_fn();
	odnastav(this->var->addr);
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
	g_ImInWhile = true;
	cond = (Expr *) (cond->Optimize());
	body = (Statm *) (body->Optimize());
	Numb *c = dynamic_cast<Numb *>(cond);
	g_ImInWhile = false;
	if (!c) {
		_return(this);
	}

	if (!c->Value()) {
		delete this;
		_return(new Empty);
	}

	_return(this);
}

bool bop_contains_var(Var *v, Bop *b) {
	_fn();
	Var *lv = dynamic_cast<Var *>(b->left);
	Var *rv = dynamic_cast<Var *>(b->right);
	Bop *lb = dynamic_cast<Bop *>(b->left);
	Bop *rb = dynamic_cast<Bop *>(b->right);
	if (lv && lv->addr == v->addr) {
		_return(true);
	}
	if (rv && rv->addr == v->addr) {
		_return(true);
	}
	if (lb && bop_contains_var(v, lb)) {
		_return(true);
	}
	if (rb && bop_contains_var(v, rb)) {
		_return(true);
	}
	_return(false);
}
bool contains_variable(Var *v, std::vector<Bop *> b) {
	_fn();
	for (auto bop : b) {
		if (bop_contains_var(v, bop)) {
			_return(true);
		}
	}
	_return(false);
}

/* return the tmp_vars. change bops in place */
void gloval_optimize(StatmList *prev, StatmList *now, std::vector<Bop *> btd) {
	_fn();

	assert(now);

	/* FIXME - if we don;t know the prev, bail it out (we could get wrong
	 * results */
	if (!prev) {
		_return_void
;	}

	StatmList *statm = NULL;
	std::vector<Bop *> vb;
	for (auto b1 : btd) {
		for (auto b2 : btd) {
			local_optimize(vb, b1, b2);
		}
	}
	for (std::vector<Bop *>::iterator it = vb.begin(); it != vb.end(); ++it) {
		_debug("GO: bop parent addr=%p\n", (void * )*it);
		char buf[50];
		snprintf(buf, 50, "tmp%d", (*it)->tmp_var);
		Assign *tmp_var = new Assign(new Var((*it)->tmp_var, false, buf), *it);
		statm = new StatmList(tmp_var, statm);
	}

	_debug2("GO: <bops to dedup>\n");
	for (auto b : vb) {
		_debug("GO: bop to dedup: %p\n", (void* )b);
	}
	_debug2("GO: </bops to dedup>\n");

	for (auto b1 : btd) {
		_debug("GO: replacing dups of %p\n", (void* )b1);
		replaceDups(vb, b1);
	}

//	assert(0 && "yes");

	_debug2("GO: creating tmp vars\n");
	_debug2("GO: <statm print>\n");
	if (statm) {
		statm->print();
	} else {
		_debug2("GO: statm is NULL\n");
	}
	_debug2("GO: </statm print>\n");

	if (statm) {
		_debug2(
				"GO: creating: prev->next = new StatmList(statm, now); (or now)\n")

		_debug("GO <print BEFORE prev->next (prev=%p)>\n", (void* )prev);
		if (prev) {
			prev->next->print();
		}
		_debug2("GO </print BEFORE prev->next>\n");

		if (prev) {
			_debug2("GO tmp vars goes to prev->next\n");
			prev->next = new StatmList(statm, now);
		} else {
			_debug2("GO tmp vars goes to now\n");
			now = new StatmList(statm, now);
		}

		_debug2("GO <print AFTER prev->next>\n");
		if (prev) {
			prev->next->print();
		}
		_debug2("GO </print AFTER prev->next>\n");

		_debug2("GO <print now>\n");
		now->print();
		_debug2("GO </print now>\n");

	} else {
		_debug2("GO: statm is null, nothing to do\n")
	}

	btd.clear();

//	assert(0&&"GO END");

	_return_void
;	//	_return(statm);
}

Node * StatmList::Optimize() {
	_fn();
	StatmList *sl = this;
	if (0) {
		do {
			sl->statm = (Statm *) (sl->statm->Optimize());
			sl = sl->next;
		} while (sl);
	} else {
		_debug2("STATM OPTIMIZE ===========================================\n");
		StatmList *sl_prev = NULL;
		StatmList *tmpVarsHerePrev = NULL;
		StatmList *tmpVarsHere = NULL;
		std::vector<Bop *> btd; /* bops to dedup */
		do {

			StatmList *thisSl = sl;

			_debug2("<optmize statm>\n");
			sl->statm = (Statm *) (sl->statm->Optimize());
			_debug2("</optmize statm>\n");

			/* dalsi je statmlist */
			StatmList *sll = dynamic_cast<StatmList *>(sl->statm);
			if (sll) {
				_debug("STATM OPTIMIZE: this statm is a StatmList. next=%p\n",
						(void * )sll->next);
				if (!sll->next) {
					_debug2("it has only 1 statm, replacing\n");
					thisSl = sll;
				}
			}

			Read *r = dynamic_cast<Read *>(thisSl->statm);
			if (r) {
				_debug2("STATM OPTIMIZE: this statm is a read\n");
			}

			Assign *a = dynamic_cast<Assign *>(thisSl->statm);
			Bop *ab = NULL;
			if (a) {
				_debug2("STATM OPTIMIZE: this statm is an assign\n");
				ab = dynamic_cast<Bop *>(a->expr);
			}
			if (a) {
				if (contains_variable(a->var, btd) && sl_prev && !btd.empty()) {
					_debug2("STATM OPTIMIZE GLOBAL OPTIMIZE BECAUSE CV\n");
					gloval_optimize(tmpVarsHerePrev, tmpVarsHere, btd);
				} else {
					if (ab) {
						_debug2("SLO: adding ab to btd\n");

						/* pokud je to prvni bop co tam davame, tmp vars
						 * prijdou pred nej */
						if (btd.empty()) {
							_debug2("SLO: BTD is empty, setting up tVHP, tVH\n");
							tmpVarsHerePrev = sl_prev;
							tmpVarsHere = sl;
						}

						btd.push_back(ab);
					}
				}
			} else {
				if (sl_prev && !btd.empty()) {
					_debug2("STATM OPTIMIZE GLOBAL OPTIMIZE NOT ASSIGN\n");
					gloval_optimize(tmpVarsHerePrev, tmpVarsHere, btd);
				}
			}
			sl_prev = sl;
			sl = sl->next;
		} while (sl);
		if (sl_prev && !btd.empty()) {
			_debug2("STATM OPTIMIZE GLOBAL OPTIMIZE BECAUSE END OF STATML\n");
			gloval_optimize(tmpVarsHerePrev, tmpVarsHere, btd);
		}
//
//		if (tmpVars) {
//			_debug2("tmpVars are not NULL!\n");
//
//			_debug2("<SLO: <tmpVars print>\n");
//			tmpVars->print();
//			_debug2("<SLO: </tmpVars print>\n");
//
//			_debug2("<SLO: <this print>\n");
//			this->print();
//			_debug2("<SLO: </this print>\n");
//
//			assert(0 && "tmpvars");
//			_return(new StatmList(tmpVars, this));
//		}

	}

	_debug2("<SLO: <print>");
	this->print();
	_debug2("<SLO: </print>");
//	assert(0 && "end of SLO");
	_return(this);
}

//			Read *r = dynamic_cast<Read *>(s->statm);
//			Write *wr = dynamic_cast<Write *>(s->statm);
//			If *i = dynamic_cast<If *>(s->statm);
//			If *wh = dynamic_cast<While *>(s->statm);

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
		/* (ra)1 < (rb)2
		 * 2 - 1 = 1 -> 1 > 0 -> YES */
		g_ts->addInstr(SUB, rb, ra, rb, "op <");
		g_ts->addInstr(JGT, rb, 2, Storage::pc);
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
		g_ts->addInstr(JLT, rb, 2, Storage::pc);
		g_ts->addInstr(LDC, rb, 0, rb);
		g_ts->addInstr(LDA, Storage::pc, 1, Storage::pc);
		g_ts->addInstr(LDC, rb, 1, rb);
		break;
	case LessOrEq:
		g_ts->addInstr(SUB, rb, ra, rb, "op <=");
		g_ts->addInstr(JGE, rb, 2, Storage::pc);
		g_ts->addInstr(LDC, rb, 0, rb);
		g_ts->addInstr(LDA, Storage::pc, 1, Storage::pc);
		g_ts->addInstr(LDC, rb, 1, rb);
		break;
	case GreaterOrEq:
		g_ts->addInstr(SUB, rb, ra, rb, "op >=");
		g_ts->addInstr(JLE, rb, 2, Storage::pc);
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

	// || this->var->usedEver
	if (!(std::find(g_used_vars.begin(), g_used_vars.end(), this->var->addr)
			!= g_used_vars.end())) {
		_debug("666 Assign::Translate(): var %d has never been used\n",
				this->var->addr);
		g_ts->addComment("tady mela byt promenna\n");
		_return(0);
	}

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
	g_ts->addComment("cond");
	cond->Translate();
	CodeLine *jmpAfterThen = g_ts->addInstr(JEQ, g_s->pop(), -666,
			Storage::zero, "jumpAfterThen");
	thenstm->Translate();
	jmpAfterThen->op[1] = CodeLine::last_line_number + 1;
	if (elsestm) {
		CodeLine *jmpAfterElse = g_ts->addInstr(LDA, Storage::pc, -666,
				Storage::zero, "jumpAfterElse");
		elsestm->Translate();
		jmpAfterElse->op[1] = CodeLine::last_line_number;
	}
	_return(0);
}

/* TODO FIXME
 *
 var i,j,k;
 begin
 j := 10;
 i := 1;
 while j >= 0 do
 begin
 i := i + 1;
 j := j - 1;
 end;
 write i;
 end
 *
 * */
int While::Translate() {
	_fn();
	int firstOfCond = CodeLine::last_line_number;
	cond->Translate();
	CodeLine *jmpAfterBody = g_ts->addInstr(JEQ, g_s->pop(), -666,
			Storage::zero, "jumpAfterWhileBodyCauseCondFalse");
	body->Translate();
	g_ts->addInstr(LDA, Storage::pc, firstOfCond, Storage::zero,
			"jumpAtBeginnning of wthile");
	jmpAfterBody->op[1] = CodeLine::last_line_number;
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
		_return(new Var(v, true, id));
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
	_return_void
;}

void Numb::print() {
	_fn();
	_debug2("(numb)\n");
	_return_void
;}

void Bop::print() {
	_fn();
	_debug2("<BOP>\n");
	left->print();
	_debug("<op=%d>\n", this->op);
	right->print();
	_debug2("<BOP>\n");
	_return_void
;}

void UnMinus::print() {
	_fn();
	_debug2("<UnMinus>\n");
	expr->print();
	_debug2("</UnMinus>\n");
	_return_void
;}

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
	_return_void
;}

void Write::print() {
	_fn();
	_debug2("<write>\n");
	assert(expr);
	expr->print();
	_debug2("</write>\n");
	_return_void
;}

void Read::print() {
	_fn();
	_debug2("<read>\n");
	_return_void
;}

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
	_return_void
;}

void While::print() {
	_fn();
	_debug2("While=(");
	cond->print();
	_debug2(") {\n");
	body->print();
	_debug2("}=While\n");
	_return_void
;}

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
	_return_void
;}

void Empty::print() {
	_fn();
	_debug2("<Empty>\n");
	_return_void
;}

void Prog::print() {
	_fn();
	_debug2("<Prog>\n");
	stm->print();
	_return_void
;}

