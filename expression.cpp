#include "expression.hpp"

#include <sstream>
#include <list>

#include "environment.hpp"
#include "semantic_error.hpp"

Expression::Expression() {}

Expression::Expression(const Atom & a) {
	m_head = a;
}

Expression::Expression(const std::vector<Expression> results) {
	for (size_t i = 0; i < results.size(); i++) {
		m_tail.push_back(results[i]);
	}
}

// recursive copy
Expression::Expression(const Expression & a) {

	m_head = a.m_head;
	for (auto e : a.m_tail) {
		m_tail.push_back(e);
	}
}

Expression & Expression::operator=(const Expression & a) {

	// prevent self-assignment
	if (this != &a) {
		m_head = a.m_head;
		m_tail.clear();
		for (auto e : a.m_tail) {
			m_tail.push_back(e);
		}
	}

	return *this;
}


Atom & Expression::head() {
	return m_head;
}

const Atom & Expression::head() const {
	return m_head;
}

bool Expression::isHeadNumber() const noexcept {
	return m_head.isNumber();
}

bool Expression::isHeadSymbol() const noexcept {
	return m_head.isSymbol();
}

bool Expression::isHeadComplex() const noexcept {
	return m_head.isComplex();
}

void Expression::append(const Atom & a) {
	m_tail.emplace_back(a);
}

void Expression::append(const Expression result) {
	m_tail.emplace_back(result);
}

void Expression::remove(const Atom & a) {
	m_tail.erase(m_tail.begin());
}


Expression * Expression::tail() {
	Expression * ptr = nullptr;

	if (m_tail.size() > 0) {
		ptr = &m_tail.back();
	}

	return ptr;
}

Expression::ConstIteratorType Expression::tailConstBegin() const noexcept {
	return m_tail.cbegin();
}

Expression::ConstIteratorType Expression::tailConstEnd() const noexcept {
	return m_tail.cend();
}

Expression apply(const Atom & op, const std::vector<Expression> & args, const Environment & env) {

	// head must be a symbol
	if (!op.isSymbol()) {
		throw SemanticError("Error during evaluation: procedure name not symbol");
	}

	// must map to a proc
	if (!env.is_proc(op)) {
		throw SemanticError("Error during evaluation: symbol does not name a procedure");
	}

	// map from symbol to proc
	Procedure proc = env.get_proc(op);

	// call proc with args
	return proc(args);
}

Expression Expression::handle_lookup(const Atom & head, const Environment & env) {
	if (head.isSymbol()) { // if symbol is in env return value
		if (env.is_exp(head)) {
			return env.get_exp(head);
		}
		else if (head.asSymbol() == "list" && m_tail.empty()) {
			return Expression(head);
		}
		else {
			throw SemanticError("Error during evaluation: unknown symbol");
		}
	}
	else if (head.isNumber()) {
		return Expression(head);
	}
	else if (head.isComplex()) {
		return Expression(head);
	}
	else {
		throw SemanticError("Error during evaluation: Invalid type in terminal expression");
	}
}

Expression Expression::handle_list(Environment & env) {
	Expression result = m_head;
	for (Expression::IteratorType it = m_tail.begin(); it != m_tail.end(); ++it) {
		result.m_tail.push_back(it->eval(env));
	}
	return result;
}

Expression Expression::handle_lambda(Environment & env)
{
	return Expression();
}

Expression Expression::handle_begin(Environment & env) {

	if (m_tail.size() == 0) {
		throw SemanticError("Error during evaluation: zero arguments to begin");
	}

	// evaluate each arg from tail, return the last
	Expression result;
	for (Expression::IteratorType it = m_tail.begin(); it != m_tail.end(); ++it) {
		result = it->eval(env);
	}

	return result;
}


Expression Expression::handle_define(Environment & env) {
	// but tail[0] must not be a special-form or procedure
	std::string s = m_tail[0].head().asSymbol();

	// tail must have size 3 or error
	if (m_tail.size() != 2 && (s != "list")) {
		throw SemanticError("Error during evaluation: invalid number of arguments to define");
	}

	// tail[0] must be symbol
	if (!m_tail[0].isHeadSymbol()) {
		throw SemanticError("Error during evaluation: first argument to define not symbol");
	}

	if ((s == "define") || (s == "begin")) {
		throw SemanticError("Error during evaluation: attempt to redefine a special-form");
	}

	if (env.is_proc(m_head)) {
		throw SemanticError("Error during evaluation: attempt to redefine a built-in procedure");
	}

	// eval tail[1]
	Expression result = m_tail[1].eval(env);

	if (env.is_exp(m_head)) {
		throw SemanticError("Error during evaluation: attempt to redefine a previously defined symbol");
	}

	//and add to env
	env.add_exp(m_tail[0].head(), result);

	return result;
}


// this is a simple recursive version. the iterative version is more
// difficult with the ast data structure used (no parent pointer).
// this limits the practical depth of our AST
Expression Expression::eval(Environment & env) {

	if (m_tail.empty()) {
		return handle_lookup(m_head, env);
	}
	// handle begin special-form
	else if (m_head.isSymbol() && m_head.asSymbol() == "begin") {
		return handle_begin(env);
	}
	// handle define special-form
	else if (m_head.isSymbol() && m_head.asSymbol() == "define") {
		return handle_define(env);
	}
	else if (m_head.isSymbol() && m_head.asSymbol() == "list") {
		return handle_list(env);
	}
	else if (m_head.isSymbol() && m_head.asSymbol() == "lambda") {
		//
	}
	// else attempt to treat as procedure
	else {
		std::vector<Expression> results;
		for (Expression::IteratorType it = m_tail.begin(); it != m_tail.end(); ++it) {
			results.push_back(it->eval(env));
		}
		return apply(m_head, results, env);
	}
}


std::ostream & operator<<(std::ostream & out, const Expression & exp) {
	if ((exp.isHeadNumber() == true || exp.isHeadSymbol() == true)) {
		out << "(";
	}
	if ((exp.head().isSymbol() && exp.head().asSymbol() != "list") || (exp.head().isNumber() || exp.head().isComplex())) {
		out << exp.head();
	}

	for (auto e = exp.tailConstBegin(); e != exp.tailConstEnd(); ++e) {
		out << *e;
		if (e != exp.tailConstEnd() - 1) {
			out << " ";
		}
	}

	if (exp.isHeadNumber() == true || exp.isHeadSymbol() == true) {
		out << ")";
	}

	return out;
}

bool Expression::operator==(const Expression & exp) const noexcept {

	bool result = (m_head == exp.m_head);

	result = result && (m_tail.size() == exp.m_tail.size());

	if (result) {
		for (auto lefte = m_tail.begin(), righte = exp.m_tail.begin();
			(lefte != m_tail.end()) && (righte != exp.m_tail.end());
			++lefte, ++righte) {
			result = result && (*lefte == *righte);
		}
	}

	return result;
}

bool operator!=(const Expression & left, const Expression & right) noexcept {

	return !(left == right);
}