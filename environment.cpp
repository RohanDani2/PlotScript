#include "environment.hpp"

#include <cassert>
#include <cmath>
#include <complex>
#include <iostream>

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
	double result = 0;
	std::complex<double> complexVal(0, 0);
	std::complex<double> allnumAddition(0, 0);
	if (args[0].isHeadNumber() == true || args[0].isHeadComplex() == true) { //adding only real numbers
		for (auto & a : args) {
			if (a.isHeadNumber()) {
				result += a.head().asNumber();
			}
			else if (a.isHeadComplex()) {
				complexVal += a.head().asComplex();
			}
		}
		if (args[0].isHeadNumber() == true && args[1].isHeadNumber() == true) {
			return Expression(result);
		}
		else {
			allnumAddition += complexVal;
			allnumAddition += result;
			return Expression(allnumAddition);
		}
	}
	else {
		throw SemanticError("Error in call to add, argument not a number");
	}
};

Expression mul(const std::vector<Expression> & args) {

	// check all arguments are numbers, while multiplying
	double result = 1;
	std::complex<double> complexMultiply(1, 0);
	std::complex<double> allNumMultiply(1, 0);
	if (args[0].isHeadNumber() == true || args[0].isHeadComplex() == true) {
		for (auto & a : args) {
			if (a.isHeadNumber()) {
				result *= a.head().asNumber();
			}
			else if (a.isHeadComplex()) {
				complexMultiply *= a.head().asComplex();
			}
		}
		if (args[0].isHeadNumber() == true && args[1].isHeadNumber() == true) {
			return Expression(result);
		}
		else {
			allNumMultiply = result * complexMultiply;
			return Expression(allNumMultiply);
		}
	}
	else {
		throw SemanticError("Error in call to mul, argument not a number");
	}
};

Expression subneg(const std::vector<Expression> & args) {

	double result = 0;
	std::complex<double> complexSubtract(0, 0);
	std::complex<double> allnumSubtract(0, 0);
	// preconditions
	if (nargs_equal(args, 1)) {
		if (args[0].isHeadNumber()) {
			result = -args[0].head().asNumber();
		}
		else if (args[0].isHeadComplex()) {
			complexSubtract = -args[0].head().asComplex();
		}
		else if (args[0].isHeadSymbol()) {
			if (args[0].head() == i) {
				complexSubtract = -i;
			}
			else if (args[0].head() == EXP) {
				result = -EXP;
			}
			else if (args[0].head() == PI) {
				result = -PI;
			}
		}
 		else {
			throw SemanticError("Error in call to negate: invalid argument.");
		}
	}
	else if (nargs_equal(args, 2)) {
		if ((args[0].isHeadNumber()) && (args[1].isHeadNumber())) {
			result = args[0].head().asNumber() - args[1].head().asNumber();
			return Expression(result);
		}
		else if ((args[0].isHeadComplex()) && (args[1].isHeadComplex())) {
			complexSubtract = args[0].head().asComplex() - args[1].head().asComplex();
			return Expression(complexSubtract);
		}
		else if ((args[0].isHeadComplex()) && (args[1].isHeadNumber())) {
			complexSubtract = args[0].head().asComplex() - args[1].head().asNumber();
			return Expression(complexSubtract);
		}
		else if ((args[0].isHeadNumber()) && (args[1].isHeadComplex())) {
			complexSubtract = args[0].head().asNumber() - args[1].head().asComplex();
			return Expression(complexSubtract);
		}
		else {
			throw SemanticError("Error in call to subtraction: invalid argument.");
		}
	}
	else {
		throw SemanticError("Error in call to subtraction or negation: invalid number of arguments.");
	}
	if (args[0].isHeadNumber()) {
		return Expression(result);
	}
	else if (args[0].isHeadComplex()) {
		return Expression(complexSubtract);
	}
};

Expression div(const std::vector<Expression> & args) {

	double result = 0;
	std::complex<double> complexDivide(0, 0);
	if (nargs_equal(args, 2)) {
		if ((args[0].isHeadNumber()) && (args[1].isHeadNumber())) {
			result = args[0].head().asNumber() / args[1].head().asNumber();
			return Expression(result);
		}
		else if ((args[0].isHeadComplex() && (args[1].isHeadComplex()))) {
			complexDivide = args[0].head().asComplex() / args[1].head().asComplex();
			return Expression(complexDivide);
		}
		else if ((args[0].isHeadComplex() && (args[1].isHeadNumber()))) {
			complexDivide = args[0].head().asComplex() / args[1].head().asNumber();
			return Expression(complexDivide);
		}
		else if ((args[0].isHeadNumber() && (args[1].isHeadComplex()))) {
			complexDivide = args[0].head().asNumber() / args[1].head().asComplex();
			return Expression(complexDivide);
		}
		else {
			throw SemanticError("Error in call to division: invalid argument.");
		}
	}
	else {
		throw SemanticError("Error in call to division: invalid number of arguments.");
	}

};

Expression exponent(const std::vector<Expression> & args) { //exponent
	double result = 0;
	std::complex<double> complexExponent(0, 0);
	if (nargs_equal(args, 2)) {
		if ((args[0].isHeadNumber()) && (args[1].isHeadNumber())) {
			result = pow(args[0].head().asNumber(), args[1].head().asNumber());
			return Expression(result);
		}
		else if ((args[0].isHeadComplex()) && (args[1].isHeadComplex())) {
			complexExponent = pow(args[0].head().asComplex(), args[1].head().asComplex());
			return Expression(complexExponent);
		}
		else if ((args[0].isHeadComplex()) && (args[1].isHeadNumber())) {
			complexExponent = pow(args[0].head().asComplex(), args[1].head().asNumber());
			return Expression(complexExponent);
		}
		else if ((args[0].isHeadNumber()) && (args[1].isHeadComplex())) {
			complexExponent = pow(args[0].head().asNumber(), args[1].head().asComplex());
			return Expression(complexExponent);
		}
		else {
			throw SemanticError("Error in call to exponent: invalid argument.");
		}
	}
	else {
		throw SemanticError("Error in call to exponent: invalid number of arguments.");
	}
}

Expression squareroot(const std::vector<Expression> & args) { //square root
	double result = 0;
	std::complex<double> complexSqrt(0, 0);
	if (nargs_equal(args, 1)) {
		if (args[0].isHeadNumber()) {
			if (args[0].isHeadNumber()) {
				result = sqrt(args[0].head().asNumber());
				if (std::isnan(result)) {
					std::complex<double> negativeSqrt(0, sqrt(std::abs(args[0].head().asNumber())));
					return Expression(negativeSqrt);
				}
				else {
					return Expression(result);
				}
			}
		}
		else if (args[0].isHeadComplex()) {
			complexSqrt = sqrt(args[0].head().asComplex());
			return Expression(complexSqrt);
		}
		else {
			throw SemanticError("Error in call to square root: invalid argument.");
		}
	}
	else {
		throw SemanticError("Error in call to square root: invalid number of arguments.");
	}
}

Expression naturalLog(const std::vector<Expression> & args) { //natural Log
	double result = 0;
	if (nargs_equal(args, 1)) {
		if (args[0].isHeadNumber()) {
			result = log(args[0].head().asNumber());
		}
		else {
			throw SemanticError("Error in call to natural log: invalid argument.");
		}
	}
	else {
		throw SemanticError("Error in call to natural log: invalid number of arguments.");
	}
	return Expression(result);
}

Expression sine(const std::vector<Expression> & args) { //sine
	double result = 0;
	std::complex<double> sineComplex(0, 0);
	if (nargs_equal(args, 1)) {
		if (args[0].isHeadNumber()) {
			result = sin(args[0].head().asNumber());
		}
		else if (args[0].isHeadComplex()) {
			sineComplex = sin(args[0].head().asComplex());
			return Expression(sineComplex);
		}
		else {
			throw SemanticError("Error in call to sine: invalid argument.");
		}
	}
	else {
		throw SemanticError("Error in call to sine: invalid number of arguments.");
	}
	return Expression(result);
}

Expression cosine(const std::vector<Expression> & args) { //cosine
	double result = 0;
	std::complex<double> cosineComplex(0,0);
	if (nargs_equal(args, 1)) {
		for (auto &a : args) {
			if (a.isHeadNumber()) {
				result = cos(a.head().asNumber());
				return Expression(result);
			}
			else if (a.isHeadComplex()) {
				cosineComplex = cos(a.head().asComplex());
				return Expression(cosineComplex);
			}
			else {
				throw SemanticError("Error in call to cosine: invalid argument.");
			}
		}
	}
	else {
		throw SemanticError("Error in call to cosine: invalid number of arguments.");
	}
}

Expression tangent(const std::vector<Expression> & args) { //tan
	double result = 0;
	std::complex<double> tangentComplex(0, 0);
	if (nargs_equal(args, 1)) {
		if (args[0].isHeadNumber()) {
			result = tan(args[0].head().asNumber());
		}
		else if (args[0].isHeadComplex()) {
			tangentComplex = tan(args[0].head().asComplex());
			return Expression(tangentComplex);
		}
		else {
			throw SemanticError("Error in call to tangent: invalid argument.");
		}
	}
	else {
		throw SemanticError("Error in call to tangent: invalid number of arguments.");
	}
	return Expression(result);
}

Expression realNum(const std::vector<Expression> & args) { //real
	std::complex<double> realNum(0, 0);
	if (nargs_equal(args, 1)) {
		if (args[0].isHeadComplex()) {
			realNum = std::real(args[0].head().asComplex());
			return Expression(realNum);
		}
		else if (args[1].isHeadComplex()) {
			realNum = std::real(args[1].head().asComplex());
			return Expression(realNum);
		}
		else {
			throw SemanticError("Error in call to real: wrong argument.");
		}
	}
	else {
		throw SemanticError("Error in call to real: wrong argument.");
	}

}

Expression imaginary(const std::vector<Expression> & args) { //imag
	std::complex<double> imagNum(0, 0);
	if (nargs_equal(args, 1)) {
		if (args[0].isHeadComplex()) {
			imagNum = std::imag(args[0].head().asComplex());
			return Expression(imagNum);
		}
		else if (args[1].isHeadComplex()) {
			imagNum = std::imag(args[1].head().asComplex());
			return Expression(imagNum);
		}
		else {
			throw SemanticError("Error in call to imag: wrong argument.");
		}
	}
	else {
		throw SemanticError("Error in call to imag: wrong argument.");
	}

}

Expression absoluteValue(const std::vector<Expression> & args) { //absolute value
	std::complex<double> absNum(0, 0);
	if (nargs_equal(args, 1)) {
		if (args[0].isHeadComplex()) {
			absNum = std::abs(args[0].head().asComplex());
			return Expression(absNum);
		}
		else if (args[1].isHeadComplex()) {
			absNum = std::abs(args[1].head().asComplex());
			return Expression(absNum);
		}
		else {
			throw SemanticError("Error in call to absolute value: wrong argument.");
		}
	}
	else {
		throw SemanticError("Error in call to absolute value: wrong argument.");
	}

}

Expression phaseAngle(const std::vector<Expression> & args) { //phaseAngle
	std::complex<double> phaseAngle(0, 0);
	if (nargs_equal(args, 1)) {
		if (args[0].isHeadComplex()) {
			phaseAngle = std::arg(args[0].head().asComplex());
			return Expression(phaseAngle);
		}
		else if (args[1].isHeadComplex()) {
			phaseAngle = std::arg(args[1].head().asComplex());
			return Expression(phaseAngle);
		}
		else {
			throw SemanticError("Error in call to phaseAngle: wrong argument.");
		}
	}
	else {
		throw SemanticError("Error in call to phaseAngle: wrong argument.");
	}

}

Expression conjugate(const std::vector<Expression> & args) { //conjugate
	std::complex<double> conjugate(0, 0);
	if (nargs_equal(args, 1)) {
		if (args[0].isHeadComplex()) {
			conjugate = std::conj(args[0].head().asComplex());
			return Expression(conjugate);
		}
		else if (args[1].isHeadComplex()) {
			conjugate = std::conj(args[1].head().asComplex());
			return Expression(conjugate);
		}
		else {
			throw SemanticError("Error in call to conjugate: wrong argument.");
		}

	}
	else {
		throw SemanticError("Error in call to conjugate: wrong argument.");
	}

}

Environment::Environment() {

	reset();
}

bool Environment::is_known(const Atom & sym) const {
	if (!sym.isSymbol()) return false;

	return envmap.find(sym.asSymbol()) != envmap.end();
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

	// error if overwriting symbol map
	if (envmap.find(sym.asSymbol()) != envmap.end()) {
		throw SemanticError("Attempt to overwrite symbol in environemnt");
	}

	envmap.emplace(sym.asSymbol(), EnvResult(ExpressionType, exp));
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

}
