#include "catch.hpp"
//
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cmath>

#include "semantic_error.hpp"
#include "interpreter.hpp"
#include "expression.hpp"

Expression run(const std::string & program){  
  std::istringstream iss(program);
    
  Interpreter interp;
  
  bool ok = interp.parseStream(iss);
  if(!ok){
    std::cerr << "failed to parse: " << program << std::endl; 
  }
  REQUIRE(ok == true);

  Expression result;

  REQUIRE_NOTHROW(result = interp.evaluate());

  return result;
}

TEST_CASE( "Test Interpreter parser with expected input", "[interpreter]" ) {

  std::string program = "(begin (define r 10) (* pi (* r r)))";

  std::istringstream iss(program);
 
  Interpreter interp;

  bool ok = interp.parseStream(iss);

  REQUIRE(ok == true);
}

TEST_CASE( "Test Interpreter parser with numerical literals", "[interpreter]" ) {

  std::vector<std::string> programs = {"(1)", "(+1)", "(+1e+0)", "(1e-0)"};
  
  for(auto program : programs){
    std::istringstream iss(program);
 
    Interpreter interp;

    bool ok = interp.parseStream(iss);

    REQUIRE(ok == true);
  }

  {
    std::istringstream iss("(define x 1abc)");
    
    Interpreter interp;

    bool ok = interp.parseStream(iss);

    REQUIRE(ok == false);
  }
}

TEST_CASE( "Test Interpreter parser with truncated input", "[interpreter]" ) {

  {
    std::string program = "(f";
    std::istringstream iss(program);
  
    Interpreter interp;
    bool ok = interp.parseStream(iss);
    REQUIRE(ok == false);
  }
  
  {
    std::string program = "(begin (define r 10) (* pi (* r r";
    std::istringstream iss(program);

    Interpreter interp;
    bool ok = interp.parseStream(iss);
    REQUIRE(ok == false);
  }
}

TEST_CASE( "Test Interpreter parser with extra input", "[interpreter]" ) {

  std::string program = "(begin (define r 10) (* pi (* r r))) )";
  std::istringstream iss(program);

  Interpreter interp;

  bool ok = interp.parseStream(iss);

  REQUIRE(ok == false);
}

TEST_CASE( "Test Interpreter parser with single non-keyword", "[interpreter]" ) {

  std::string program = "(\"hello\")";
  std::istringstream iss(program);
  
  Interpreter interp;

  bool ok = interp.parseStream(iss);

  REQUIRE(ok == true);
}

TEST_CASE( "Test Interpreter parser with empty input", "[interpreter]" ) {

  std::string program;
  std::istringstream iss(program);
  
  Interpreter interp;

  bool ok = interp.parseStream(iss);

  REQUIRE(ok == false);
}

TEST_CASE( "Test Interpreter parser with empty expression", "[interpreter]" ) {

  std::string program = "( )";
  std::istringstream iss(program);
  
  Interpreter interp;

  bool ok = interp.parseStream(iss);

  REQUIRE(ok == false);
}

TEST_CASE( "Test Interpreter parser with bad number string", "[interpreter]" ) {

  std::string program = "(1abc)";
  std::istringstream iss(program);
  
  Interpreter interp;

  bool ok = interp.parseStream(iss);

  REQUIRE(ok == false);
}

TEST_CASE( "test interpreter parser with incorrect input. regression test", "[interpreter]" ) {

  std::string program = "(+ 1 2) (+ 3 4)";
  std::istringstream iss(program);
  
  Interpreter interp;

  bool ok = interp.parseStream(iss);

  REQUIRE(ok == false);
}

TEST_CASE( "test interpreter result with literal expressions", "[interpreter]" ) {
  
  { // number
    std::string program = "(4)";
    Expression result = run(program);
	REQUIRE(result == Expression(4.));
  }

  { // symbol
    std::string program = "(pi)";
	Expression result = run(program);
	REQUIRE(result == Expression(atan2(0, -1)));
  }

  { // symbol
	  std::string program = "(- pi)";
	  Expression result = run(program);
	  REQUIRE(result == Expression(-atan2(0, -1)));
  }

  { // symbol
	  std::string program = "(- e)";
	  Expression result = run(program);
	  REQUIRE(result == Expression(-exp(1)));
  }

  { // symbol
	  std::string program = "(- I)";
	  Expression result = run(program);
	  REQUIRE(result.isHeadComplex() == true);
  }

}

TEST_CASE( "Test Interpreter result with simple procedures (add)", "[interpreter]" ) {

  { // add, binary case
    std::string program = "(+ 1 2)";
    INFO(program);
    Expression result = run(program);
    REQUIRE(result == Expression(3.));
  }
  
  { // add, 3-ary case
    std::string program = "(+ 1 2 3)";
    INFO(program);
    Expression result = run(program);
    REQUIRE(result == Expression(6.));
  }

  { // add, 6-ary case
    std::string program = "(+ 1 2 3 4 5 6)";
    INFO(program);
    Expression result = run(program);
    REQUIRE(result == Expression(21.));
  }
}

TEST_CASE("Test trig procedure", "[interpreter]") {
	{
		std::string program = "(sin 0)";
		INFO(program);
		Expression result = run(program);
		REQUIRE(result == Expression(0));
	}
	{
		std::string program = "(cos 0)";
		INFO(program);
		Expression result = run(program);
		REQUIRE(result == Expression(1));
	}
	{
		std::string program = "(tan 0)";
		INFO(program);
		Expression result = run(program);
		REQUIRE(result == Expression(0));
	}
	{
		std::string program = "(sqrt 4)";
		INFO(program);
		Expression result = run(program);
		REQUIRE(result == Expression(2));
	}
	{
		std::string program = "(ln 1)";
		INFO(program);
		Expression result = run(program);
		REQUIRE(result == Expression(0));
	}
	{
		std::string program = "(^ 2 4)";
		INFO(program);
		Expression result = run(program);
		REQUIRE(result == Expression(16));
	}
}

TEST_CASE("Test M1/M2 functions", "[interpreter]") {
	{
		std::string program = "(first (list 1 2 3))";
		INFO(program);
		Expression result = run(program);
		REQUIRE(result == Expression(1));
	}
	{
		std::string program = "(first (list 3 4 5 3) 5)";
		Interpreter interp;
		std::istringstream iss(program);
		bool ok = interp.parseStream(iss);
		REQUIRE(ok == true);
	}
	{
		std::string program = "(first (+ 3 4 5 3))";
		Interpreter interp;
		std::istringstream iss(program);
		bool ok = interp.parseStream(iss);
		REQUIRE(ok == true);
	}
	{
		std::string program = "(rest (list 3 4 5 3))";
		INFO(program);
		Expression result = run(program);
		REQUIRE(result.length() == 3);
	}
	{
		std::string program = "(length (list 3 4 5 3))";
		INFO(program);
		Expression result = run(program);
		REQUIRE(result.length() == 0);
	}
	{
		std::string program = "(rest (list 3 4 5 3) 5)";
		Interpreter interp;
		std::istringstream iss(program);
		bool ok = interp.parseStream(iss);
		REQUIRE(ok == true);
	}
	{
		std::string program = "(% 6 4)";
		Interpreter interp;
		std::istringstream iss(program);
		bool ok = interp.parseStream(iss);
		REQUIRE(ok == true);
	}
	{
		std::string program = "(rest (+ 3 4 5 3))";
		Interpreter interp;
		std::istringstream iss(program);
		bool ok = interp.parseStream(iss);
		REQUIRE(ok == true);
	}
	{
		std::string program = "(join (list 1 2) 10)";
		Interpreter interp;
		std::istringstream iss(program);
		bool ok = interp.parseStream(iss);
		REQUIRE(ok == true);
	}


	{
		std::string program = "(/ I)";
		std::string program1 = "(/ I I)";
		std::string program2 = "(/ 2 I)";
		std::string program3 = "(/ I 2)";
		Expression result = run(program);
		REQUIRE(result.head().isComplex() == true);
		Expression result1 = run(program1);
		REQUIRE(result1.head().isComplex() == true);
		Expression result2 = run(program2);
		REQUIRE(result2.head().isComplex() == true);
		Expression result3 = run(program3);
		REQUIRE(result3.head().isComplex() == true);
	}
	{
		std::string program = "(- I)";
		std::string program1 = "(- I I)";
		std::string program2 = "(- 2 I)";
		std::string program3 = "(- I 2)";
		Expression result = run(program);
		REQUIRE(result.head().isComplex() == true);
		Expression result1 = run(program1);
		REQUIRE(result1.head().isComplex() == true);
		Expression result2 = run(program2);
		REQUIRE(result2.head().isComplex() == true);
		Expression result3 = run(program3);
		REQUIRE(result3.head().isComplex() == true);
	}
	{
		std::string program1 = "(^ I I)";
		std::string program2 = "(^ 2 I)";
		std::string program3 = "(^ I 2)";
		Expression result1 = run(program1);
		REQUIRE(result1.head().isComplex() == true);
		Expression result2 = run(program2);
		REQUIRE(result2.head().isComplex() == true);
		Expression result3 = run(program3);
		REQUIRE(result3.head().isComplex() == true);

	}
	{
		std::string program = "(range 0 5 1)";
		INFO(program);
		Expression result = run(program);
		REQUIRE(result.length() == 6);
	}
		std::string program = "(^ e (- (* pi I)))";
		Expression result = run(program);
		REQUIRE(result.isHeadComplex() == true);

	{
		std::string program = "(apply imag (list (+ 1 (* 3 I))))";
		std::string program1 = "(apply real (list (+ 1 (* 3 I))))";
		std::string program2 = "(apply conj (list (+ 1 (* 3 I))))";
		std::string program3 = "(set-property \"number\" \"three\" (3))";
		std::string program4 = "(append (list 3 4) (+ 3 (* 4 I)))";
		std::string program5 = "(/ 1)";
		std::string program6 = "(/ I)";
		std::string program7 = "(arg I)";
		std::string program8 = "(mag I)";
		std::string program9 = "(tan I)";
		std::string program10 = "(cos I)";
		std::string program11 = "(sin I)";
		std::string program12 = "(sqrt I)";
		std::string program13 = "(sqrt -1)";
		std::string program14 = "(map / (list 1 2 4))";
		std::string program15 = "(lambda (x) (* 2 x))";
		std::string program16 = "(begin (define x (list 0 1 2 3)) (define y (list 1 2 4)) (define x (join x y)))";
		Expression result = run(program);
		REQUIRE(result == Expression(3));
		Expression result1 = run(program1);
		REQUIRE(result1 == Expression(1));
		Expression result2 = run(program2);
		REQUIRE(result2.head().isComplex() == true);
		Expression result3 = run(program3);
		REQUIRE(result3.head().isNumber() == true);
		Expression result4 = run(program4);
		REQUIRE(result4.length() == 3);
		Expression result5 = run(program5);
		REQUIRE(result5 == Expression(1));
		Expression result6 = run(program6);
		REQUIRE(result6.head().isComplex() == true);
		Expression result7 = run(program7);
		REQUIRE(result7.head().isNumber() == true);
		Expression result8 = run(program8);
		REQUIRE(result8.head().isNumber() == true);
		Expression result9 = run(program9);
		REQUIRE(result9.head().isComplex() == true);
		Expression result10 = run(program10);
		REQUIRE(result10.isHeadComplex() == true);
		Expression result11 = run(program11);
		REQUIRE(result11.length() == 0);
		Expression result12 = run(program12);
		REQUIRE(result12.isHeadComplex() == true);
		Expression result13 = run(program13);
		REQUIRE(result13.isHeadComplex() == true);
		Expression result14 = run(program14);
		REQUIRE(result14.length() == 3);
		Expression result15 = run(program15);
		REQUIRE(result15.isHeadSymbol() == true);
		Expression result16 = run(program16);
		REQUIRE(result16.length() == 7);
	}
}
  
TEST_CASE( "Test Interpreter special forms: begin and define", "[interpreter]" ) {

  {
    std::string program = "(define answer 42)";
    INFO(program);
    Expression result = run(program);
    REQUIRE(result == Expression(42.));
  }

  {
    std::string program = "(begin (define answer 42)\n(answer))";
    INFO(program);
    Expression result = run(program);
    REQUIRE(result == Expression(42.));
  }
  
  {
    std::string program = "(begin (define answer (+ 9 11)) (answer))";
    INFO(program);
    Expression result = run(program);
    REQUIRE(result == Expression(20.));
  }

  {
    std::string program = "(begin (define a 1) (define b 1) (+ a b))";
    INFO(program);
    Expression result = run(program);
    REQUIRE(result == Expression(2.));
  }
}

TEST_CASE( "Test a medium-sized expression", "[interpreter]" ) {
  {
    std::string program = "(+ (+ 10 1) (+ 30 (+ 1 1)))";
    Expression result = run(program);
    REQUIRE(result == Expression(43.));
  }
}

TEST_CASE( "Test arithmetic procedures", "[interpreter]" ) {

  {
    std::vector<std::string> programs = {"(+ 1 -2)","(+ -3 1 1)","(- 1)","(- 1 2)",
					 "(* 1 -1)","(* 1 1 -1)", "(/ -1 1)","(/ 1 -1)"};
    for(auto s : programs){
      Expression result = run(s);
      REQUIRE(result == Expression(-1.));
    }
  }
}

TEST_CASE("Test imaginary/real procedures", "[interpreter]") {

	{
		std::vector<std::string> programs = { "(+ 1 -2)","(+ -3 1 1)","(- 1)","(- 1 2)",
			"(* 1 -1)","(* 1 1 -1)","(/ -1 1)","(/ 1 -1)" };

		for (auto s : programs) {
			Expression result = run(s);
			REQUIRE(result == Expression(-1.));
		}
	}
}


TEST_CASE("Test for invalid expressions ", "[interpreter]") {

	std::vector<std::string> programs = { "(sin 1 2 3)","(tan 1 2 3)", "(cos 1 2 3)", 
		"(sqrt 1 2 5)","(ln 1 2)","(real 1 2)","(imag 1 2)","(abs 1 2)","(mag 1 2)","(^ 1 2 2)",
		"(/ 1 2 5)","(- 1 2 6)"
					}; // redefine builtin symbol
	for (auto s : programs) {
		Interpreter interp;

		std::istringstream iss(s);
		bool ok = interp.parseStream(iss);
		REQUIRE(ok == true);
		REQUIRE_THROWS_AS(interp.evaluate(), SemanticError);
	}
}

TEST_CASE( "Test for exceptions from semantically incorrect input", "[interpreter]" ) {

  std::string input = R"((+ 1 a))";

  Interpreter interp;
  
  std::istringstream iss(input);
  
  bool ok = interp.parseStream(iss);
  REQUIRE(ok == true);
  
  REQUIRE_THROWS_AS(interp.evaluate(), SemanticError);
}

TEST_CASE( "Test malformed define", "[interpreter]" ) {

    std::string input = R"((define a 1 2))";

  Interpreter interp;
  
  std::istringstream iss(input);
  
  bool ok = interp.parseStream(iss);
  REQUIRE(ok == true);
  
  REQUIRE_THROWS_AS(interp.evaluate(), SemanticError);
}

TEST_CASE( "Test using number as procedure", "[interpreter]" ) {
    std::string input = R"((1 2 3))";

  Interpreter interp;
  
  std::istringstream iss(input);
  
  bool ok = interp.parseStream(iss);
  REQUIRE(ok == true);
  
  REQUIRE_THROWS_AS(interp.evaluate(), SemanticError);
}
