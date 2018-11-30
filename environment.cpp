#include "environment.hpp"

#include <cassert>
#include <cmath>
#include <complex>
#include <iostream>
#include <ctype.h>

#include "environment.hpp"
#include "semantic_error.hpp"

/***********************************************************************
Helper Functions
**********************************************************************/

// predicate, the number of args is nargs
bool nargs_equal(const std::vector<Expression> & args, unsigned nargs) {
	return args.size() == nargs;
}

/***********************************************************************
Each of the functions below have the signature that corresponds to the
typedef'd Procedure function pointer.
**********************************************************************/



// the default procedure always returns an expresison of type None
Expression default_proc(const std::vector<Expression> & args) {
	args.size(); // make compiler happy we used this parameter
	return Expression();
};

const double PI = std::atan2(0, -1);
const double EXP = std::exp(1);
std::complex<double> i(0.0, 1.0);

Expression add(const std::vector<Expression> & args) {
	// check all arguments are numbers, while adding
	double result = 0.0;
	std::complex<double> complexVal(0, 0);
	std::complex<double> allnumAddition(0, 0);
	bool value = false;
	if (args[0].isHeadNumber() == true || args[0].isHeadComplex() == true) { //adding only real numbers
		for (auto & a : args) {
			if (a.isHeadNumber()) {		//adds within for loop for numbers
				result += a.head().asNumber();
			}
			else if (a.isHeadComplex()) { //adds within for loop for complex nums
				value = true;
				complexVal += a.head().asComplex();
			}
		}
		if (value == false) {
			return Expression(result); //returns double value if both vals are numbers
		}
		else {
			allnumAddition += complexVal;
			allnumAddition += result;
			value = false;
			return Expression(allnumAddition); //returns complex double value if either is complex
		}
	}
	return Expression();
};

Expression mul(const std::vector<Expression> & args) {
	// check all arguments are numbers, while multiplying
	double result = 1.0;
	std::complex<double> complexMultiply(1, 0);
	std::complex<double> allNumMultiply(1, 0);
	bool value = false;
	if (args[0].isHeadNumber() == true || args[0].isHeadComplex() == true) {
		for (auto & a : args) {
			if (a.isHeadNumber()) {  //calculates for real value
				result *= a.head().asNumber();
			}
			else if (a.isHeadComplex()) {  //calculates for imaginary value 
				value = true;
				complexMultiply *= a.head().asComplex();
			}
		}
		if (value == false) {
			return Expression(result); //returns double value if both vals are numbers
		}
		else {
			allNumMultiply = result * complexMultiply;
			value = true;
			return Expression(allNumMultiply); //returns double value if both vals are complex
		}
	}
	return Expression();
};

Expression subneg(const std::vector<Expression> & args) {
	double result = 0.0;
	std::complex<double> complexSubtract(0, 0);
	std::complex<double> allnumSubtract(0, 0);
	// preconditions
	if (nargs_equal(args, 1)) {
		if (args[0].isHeadNumber()) {
			result = -args[0].head().asNumber(); //negates value if result is double
		}
		else if (args[0].isHeadComplex()) {
			complexSubtract = -args[0].head().asComplex(); //negates complex value
		}
	}
	else if (nargs_equal(args, 2)) {
		if ((args[0].isHeadNumber()) && (args[1].isHeadNumber())) {
			result = args[0].head().asNumber() - args[1].head().asNumber();
			return Expression(result); //calculates for two numbers
		}
		else if ((args[0].isHeadComplex()) && (args[1].isHeadComplex())) {
			complexSubtract = args[0].head().asComplex() - args[1].head().asComplex();
			return Expression(complexSubtract); //calculates for two complex
		}
		else if ((args[0].isHeadComplex()) && (args[1].isHeadNumber())) {
			complexSubtract = args[0].head().asComplex() - args[1].head().asNumber();
			return Expression(complexSubtract); //calculates for one complex and one number
		}
		else if ((args[0].isHeadNumber()) && (args[1].isHeadComplex())) {
			complexSubtract = args[0].head().asNumber() - args[1].head().asComplex();
			return Expression(complexSubtract); //calculates for one complex and one number
		}
	}
	else {
		throw SemanticError("Error: in call to subtraction or negation: invalid number of arguments.");
	}
	if (args[0].isHeadNumber()) {
		return Expression(result);
	}
	else if (args[0].isHeadComplex()) {
		return Expression(complexSubtract);
	}
	return Expression();
};

Expression div(const std::vector<Expression> & args) {
	double result = 0.0;
	std::complex<double> complexDivide(0, 0);
	if (nargs_equal(args, 2)) {
		if ((args[0].isHeadNumber()) && (args[1].isHeadNumber())) {
			result = args[0].head().asNumber() / args[1].head().asNumber();
			return Expression(result); //calculates for two numbers
		}
		else if ((args[0].isHeadComplex() && (args[1].isHeadComplex()))) {
			complexDivide = args[0].head().asComplex() / args[1].head().asComplex();
			return Expression(complexDivide); //calculates for two complex
		}
		else if ((args[0].isHeadComplex() && (args[1].isHeadNumber()))) {
			complexDivide = args[0].head().asComplex() / args[1].head().asNumber();
			return Expression(complexDivide); //calculates for one complex and one number
		}
		else if ((args[0].isHeadNumber() && (args[1].isHeadComplex()))) {
			complexDivide = args[0].head().asNumber() / args[1].head().asComplex();
			return Expression(complexDivide); //calculates for one complex and one number
		}
	}
	else if (nargs_equal(args, 1)) {
		if ((args[0].isHeadNumber())) {
			if (args[0].head().asNumber() == 0) {
				throw SemanticError("Error: in call to division: invalid argument.");
			}
			result = 1 / args[0].head().asNumber();
			return Expression(result);
		}
		else if ((args[0].isHeadComplex())) {
			complexDivide = std::pow(args[0].head().asComplex(), -1);
			return Expression(complexDivide);
		}
	}
	else {
		throw SemanticError("Error: in call to division: invalid number of arguments.");
	}
	return Expression();

};

Expression exponent(const std::vector<Expression> & args) { //exponent
	double result = 0.0;
	std::complex<double> complexExponent(0, 0);
	if (nargs_equal(args, 2)) {
		if ((args[0].isHeadNumber()) && (args[1].isHeadNumber())) {
			result = pow(args[0].head().asNumber(), args[1].head().asNumber());
			return Expression(result); //calculates for two numbers
		}
		else if ((args[0].isHeadComplex()) && (args[1].isHeadComplex())) {
			complexExponent = pow(args[0].head().asComplex(), args[1].head().asComplex());
			return Expression(complexExponent); //calculates for two complex
		}
		else if ((args[0].isHeadComplex()) && (args[1].isHeadNumber())) {
			complexExponent = pow(args[0].head().asComplex(), args[1].head().asNumber());
			return Expression(complexExponent); //calculates for one complex and one number
		}
		else if ((args[0].isHeadNumber()) && (args[1].isHeadComplex())) {
			complexExponent = pow(args[0].head().asNumber(), args[1].head().asComplex());
			return Expression(complexExponent); //calculates for one complex and one number
		}
	}
	else {
		throw SemanticError("Error: in call to exponent: invalid number of arguments.");
	}
	return Expression();
}

Expression squareroot(const std::vector<Expression> & args) { //square root
	double result = 0.0;
	std::complex<double> complexSqrt(0, 0);
	if (nargs_equal(args, 1)) {
		if (args[0].isHeadNumber()) {
			if (args[0].isHeadNumber()) {
				result = sqrt(args[0].head().asNumber());
				if (std::isnan(result)) { //check for negative square root
					std::complex<double> negativeSqrt(0, sqrt(std::abs(args[0].head().asNumber())));
					return Expression(negativeSqrt);
				}
				return Expression(result);
			}
		}
		else if (args[0].isHeadComplex()) {
			complexSqrt = sqrt(args[0].head().asComplex()); //check for complex
			return Expression(complexSqrt);
		}
	}
	else {
		throw SemanticError("Error: in call to square root: invalid number of arguments.");
	}
	return Expression();
}

Expression naturalLog(const std::vector<Expression> & args) { //natural Log
	double result = 0.0;
	if (nargs_equal(args, 1)) {
		if (args[0].isHeadNumber()) {
			result = log(args[0].head().asNumber()); //calculates log of number
		}
	}
	else {
		throw SemanticError("Error: in call to natural log: invalid number of arguments.");
	}
	return Expression(result);
}

Expression sine(const std::vector<Expression> & args) { //sine
	double result = 0.0;
	std::complex<double> sineComplex(0, 0);
	if (nargs_equal(args, 1)) {
		if (args[0].isHeadNumber()) { //checks sine for number value
			result = sin(args[0].head().asNumber());
		}
		else if (args[0].isHeadComplex()) {
			sineComplex = sin(args[0].head().asComplex()); //check sine for complex
			return Expression(sineComplex);
		}
	}
	else {
		throw SemanticError("Error: in call to sine: invalid number of arguments.");
	}
	return Expression(result);
}

Expression cosine(const std::vector<Expression> & args) { //cosine
	double result = 0.0;
	std::complex<double> cosineComplex(0, 0);
	if (nargs_equal(args, 1)) {
		for (auto &a : args) {
			if (a.isHeadNumber()) {
				result = cos(a.head().asNumber()); //calculates cosine number value
			}
			else if (a.isHeadComplex()) {
				cosineComplex = cos(a.head().asComplex()); //calculates cosine complex val
				return Expression(cosineComplex);
			}
		}
	}
	else {
		throw SemanticError("Error: in call to cosine: invalid number of arguments.");
	}
	return Expression(result);
}

Expression tangent(const std::vector<Expression> & args) { //tan
	double result = 0.0;
	std::complex<double> tangentComplex(0, 0);
	if (nargs_equal(args, 1)) {
		if (args[0].isHeadNumber()) {
			result = tan(args[0].head().asNumber()); //tangent as number value 
		}
		else if (args[0].isHeadComplex()) {
			tangentComplex = tan(args[0].head().asComplex()); //tangent as complex val
			return Expression(tangentComplex);
		}
	}
	else {
		throw SemanticError("Error: in call to tangent: invalid number of arguments.");
	}
	return Expression(result);
}

Expression realNum(const std::vector<Expression> & args) { //real
	double value = 0.0;
	if (nargs_equal(args, 1)) {
		if (args[0].isHeadComplex()) {
			value = std::real(args[0].head().asComplex()); //find real value for compelx num
		}
		else {
			throw SemanticError("Error: in call to realNum: not complex.");
		}
	}
	else {
		throw SemanticError("Error: in call to realNum: more than one argument.");
	}
	return Expression(value); //returns real double
}

Expression imaginary(const std::vector<Expression> & args) { //imag
	double value = 0.0;
	if (nargs_equal(args, 1)) {
		if (args[0].isHeadComplex()) {
			value = std::imag(args[0].head().asComplex()); //uses complex imaginary lib to get value
		}
		else {
			throw SemanticError("Error: in call to imaginary: not complex.");
		}
	}
	else {
		throw SemanticError("Error: in call to imaginary: more than one argument.");
	}
	return Expression(value); //returns double value
}

Expression absoluteValue(const std::vector<Expression> & args) { //absolute value
	double value = 0.0;
	if (nargs_equal(args, 1)) {
		if (args[0].isHeadComplex()) {
			value = std::abs(args[0].head().asComplex());  //calculates the magnitude value
		}
		else {
			throw SemanticError("Error: in call to absoluteValue: not complex.");
		}
	}
	else {
		throw SemanticError("Error: in call to absoluteValue: not one argument.");
	}
	return Expression(value); //returns the magnitude
}

Expression phaseAngle(const std::vector<Expression> & args) { //phaseAngle
	double value = 0.0;
	if (nargs_equal(args, 1)) {
		if (args[0].isHeadComplex()) {
			value = std::arg(args[0].head().asComplex());  //calculates the phase angle value
		}
		else {
			throw SemanticError("Error: in call to phaseAngle: not complex.");
		}
	}
	else {
		throw SemanticError("Error: in call to phaseAngle: not one argument.");
	}
	return Expression(value); //returns the phase angle value 
}

Expression conjugate(const std::vector<Expression> & args) { //conjugate
	std::complex<double> conjugate(0, 0);
	if (nargs_equal(args, 1)) {
		if (args[0].isHeadComplex()) {
			conjugate = std::conj(args[0].head().asComplex());
			return Expression(conjugate); //returns the conjugate value 
		}
		else {
			throw SemanticError("Error: in call to conjugate: not complex.");
		}

	}
	else {
		throw SemanticError("Error: in call to conjugate: more than 1 argument.");
	}

}

Expression list(const std::vector<Expression> & args) {
	Atom head("list");
	Expression list(head);

	for (auto &a : args) {
		list.append(a);
	}
	return list;
}

Expression first(const std::vector<Expression> & args) { //first

	int count = 0;
	std::vector<Expression> firstVal;
	if (nargs_equal(args, 1)) {
		if (args[0].isHeadSymbol() && args[0].head().asSymbol() == "list") {
			for (auto a = args[0].tailConstBegin(); a != args[0].tailConstEnd(); ++a) {
				++count;
				if (count > 0) {
					firstVal.push_back(*a);
					break;
				}
			}
		}
		else {
			throw SemanticError("Error: argument to first is not a list");
		}
	}
	else {
		throw SemanticError("Error: more than one argument in call to first.");
	}
	if (firstVal.size() == 0) {
		throw SemanticError("Error: argument to first is an empty list.");
	}
	return Expression(args[0].first());
}

Expression rest(const std::vector<Expression> & args) { //first
	std::vector<Expression> restStore = args;
	std::vector<Expression> tempStore;

	Atom head("list");
	Expression list(head);

	for (auto b = args[0].tailConstBegin(); b != args[0].tailConstEnd(); ++b) {
		tempStore.push_back(*b);
	}
	if (tempStore.empty()) {
		throw SemanticError("Error: argument to rest is an empty list.");
	}
	if (nargs_equal(args, 1)) {
		if (args[0].isHeadSymbol() && args[0].head().asSymbol() == "list") {
			for (auto a = args[0].tailConstBegin() + 1; a != args[0].tailConstEnd(); ++a) {
				list.append(*a);
			}
			if (restStore.empty()) {
				return Expression(args[0].head());

			}
		}
	}
	else {
		throw SemanticError("Error: more than one argument in call to rest.");
	}
	return Expression(list);
}

Expression length(const std::vector<Expression> & args) { //length
	double count = 0;
	if (nargs_equal(args, 1)) {
		if (args[0].isHeadSymbol() && args[0].head().asSymbol() == "list") {
			for (auto a = args[0].tailConstBegin(); a != args[0].tailConstEnd(); ++a) {
				count++;
			}
		}
		else {
			throw SemanticError("Error: argument to length is not a list.");
		}
	}
	return Expression(count);
}

Expression appendVal(const std::vector<Expression> & args) { //append
	Expression result = args[0];
	if (args[0].isHeadSymbol() && args[0].head().asSymbol() == "list") {
		result.append(args[1]);
	}
	else {
		throw SemanticError("Error: argument to length is not a list.");
	}
	return Expression(result);
}

Expression join(const std::vector<Expression> & args) { //join
	Expression result = args[0];
	if ((args[0].isHeadSymbol() && args[0].head().asSymbol() == "list")
		&& (args[1].isHeadSymbol() && args[1].head().asSymbol() == "list")) {
		for (auto a = args[1].tailConstBegin(); a != args[1].tailConstEnd(); ++a) {
			result.append(*a);
		}
	}
	else {
		throw SemanticError("Error: first argument to join not a list.");
	}
	return Expression(result);
}

Expression range(const std::vector<Expression> & args) { //range
	double sumVal = 0;
	auto secondVal = args[1];
	auto incrementer = args[2];

	Atom head("list");
	Expression list(head);

	list.append(args[0].head().asNumber());

	sumVal = args[0].head().asNumber() + args[2].head().asNumber();

	if (args[2].head().asNumber() < 0) {
		throw SemanticError("Error: negative or zero increment in range.");
	}
	if (args[0].isHeadNumber() && args[1].isHeadNumber() && args[2].isHeadNumber()) {
		if (args[0].head().asNumber() > args[1].head().asNumber()) {
			throw SemanticError("Error: begin greater than end in range.");
		}
		while (sumVal <= args[1].head().asNumber()) { //for decimals
			list.append(sumVal);
			sumVal = sumVal + args[2].head().asNumber();
		}

	}
	return Expression(list);
}

Expression setProperty(const std::vector<Expression> & args) { //set Property
	if (!args[0].head().isSymbol() && !args[0].head().isString()) {
		throw SemanticError("Error: first argument to set-property not a string");
	}
	Expression setProperty = args[2];
	setProperty.setProp(args[0].head().asSymbol(), args[1]);
	return setProperty;
}

Expression getProperty(const std::vector<Expression> & args) { //set Property
	Expression getValue = args[1];
	return getValue.getProp(args[0].head().asSymbol());
}


Environment::Environment() {
	reset();
}

bool Environment::is_known(const Atom & sym) const {
	if (!sym.isSymbol()) return false;

	return envmap.find(sym.asSymbol()) != envmap.end(); //checks if the symbol is found
}

bool Environment::is_exp(const Atom & sym) const {
	if (!sym.isSymbol()) return false;

	auto result = envmap.find(sym.asSymbol());
	return (result != envmap.end()) && (result->second.type == ExpressionType);
}

Expression Environment::get_exp(const Atom & sym) const {

	Expression exp;

	if (sym.isSymbol()) {
		auto result = envmap.find(sym.asSymbol());
		if ((result != envmap.end()) && (result->second.type == ExpressionType)) {
			exp = result->second.exp;
		}
	}

	return exp;
}

void Environment::add_exp(const Atom & sym, const Expression & exp) {
	if (!sym.isSymbol()) {
		throw SemanticError("Attempt to add non-symbol to environment");
	}
	/*
	// error if overwriting symbol map
	if (envmap.find(sym.asSymbol()) != envmap.end()) {
	throw SemanticError("Attempt to overwrite symbol in environemnt");
	}*/
	if (is_exp(sym)) {
		auto it = envmap.find(sym.asSymbol());
		it->second = EnvResult(ExpressionType, exp);
	}
	else {
		envmap.emplace(sym.asSymbol(), EnvResult(ExpressionType, exp));
	}
}

bool Environment::is_proc(const Atom & sym) const {
	if (!sym.isSymbol()) return false;

	auto result = envmap.find(sym.asSymbol());
	return (result != envmap.end()) && (result->second.type == ProcedureType);
}

Procedure Environment::get_proc(const Atom & sym) const {

	//Procedure proc = default_proc;

	if (sym.isSymbol()) {
		auto result = envmap.find(sym.asSymbol());
		if ((result != envmap.end()) && (result->second.type == ProcedureType)) {
			return result->second.proc;
		}
	}

	return default_proc;
}

/*
Reset the environment to the default state. First remove all entries and
then re-add the default ones.
*/
void Environment::reset() {

	envmap.clear();

	// Built-In value of pi
	envmap.emplace("pi", EnvResult(ExpressionType, Expression(PI)));

	// Built-In value of e
	envmap.emplace("e", EnvResult(ExpressionType, Expression(EXP)));

	// Built-In value of i
	envmap.emplace("I", EnvResult(ExpressionType, Expression(i)));

	// Procedure: add;
	envmap.emplace("+", EnvResult(ProcedureType, add));

	// Procedure: subneg;
	envmap.emplace("-", EnvResult(ProcedureType, subneg));

	// Procedure: mul;
	envmap.emplace("*", EnvResult(ProcedureType, mul));

	// Procedure: div;
	envmap.emplace("/", EnvResult(ProcedureType, div));

	// Procedure: exponent;
	envmap.emplace("^", EnvResult(ProcedureType, exponent));

	// Procedure: square root;
	envmap.emplace("sqrt", EnvResult(ProcedureType, squareroot));

	// Procedure: natural log;
	envmap.emplace("ln", EnvResult(ProcedureType, naturalLog));

	// Procedure: sine;
	envmap.emplace("sin", EnvResult(ProcedureType, sine));

	// Procedure: cosine;
	envmap.emplace("cos", EnvResult(ProcedureType, cosine));

	// Procedure: tangent;
	envmap.emplace("tan", EnvResult(ProcedureType, tangent));

	// Procedure: real;
	envmap.emplace("real", EnvResult(ProcedureType, realNum));

	// Procedure: imag;
	envmap.emplace("imag", EnvResult(ProcedureType, imaginary));

	// Procedure: Absolute value
	envmap.emplace("mag", EnvResult(ProcedureType, absoluteValue));

	// Procedure: phase angle
	envmap.emplace("arg", EnvResult(ProcedureType, phaseAngle));

	// Procedure: conjugate
	envmap.emplace("conj", EnvResult(ProcedureType, conjugate));

	//Procedure: first;
	envmap.emplace("list", EnvResult(ProcedureType, list));

	//Procedure: first;
	envmap.emplace("first", EnvResult(ProcedureType, first));

	//Procedure: rest;
	envmap.emplace("rest", EnvResult(ProcedureType, rest));

	//Procedure: length;
	envmap.emplace("length", EnvResult(ProcedureType, length));

	//Procedure: append;
	envmap.emplace("append", EnvResult(ProcedureType, appendVal));

	//Procedure: join;
	envmap.emplace("join", EnvResult(ProcedureType, join));

	//Procedure: range;
	envmap.emplace("range", EnvResult(ProcedureType, range));

	envmap.emplace("set-property", EnvResult(ProcedureType, setProperty));

	envmap.emplace("get-property", EnvResult(ProcedureType, getProperty));
}

