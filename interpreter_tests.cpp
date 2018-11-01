#include "catch.hpp"
//
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

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

TEST_CASE("Test Task 2 functions", "[interpreter]") {
	{
		std::string program = "(first (list 1 2 3))";
		INFO(program);
		Expression result = run(program);
		REQUIRE(result == Expression(1));
	}
	//{
	//	std::string program = "(rest (list 1 2 3))";
	//	INFO(program);
	//	Expression result = run(program);
	//	REQUIRE(result == Expression((2) (3));
	//}
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

	std::vector<std::string> programs = { "(sin 1 2 4)","(tan 1 2 4)", "(cos 1 2 4)", "(real 1 2 4)","(imag 1 2 4)" ,"(mag 1 24 4)"	
					"(conj 1 23 3)" ,"(arg 1 2)","(real (sin 2) 4)","(imag (sin 2) 5)","(mag (sin 2) 6)","(arg (sin 2) 5 6)" ,"(conj (sin 2) 3)"
					"(first (list 1 2)5 5) (list 3 4) 3 4)" ,"(range 3 -1 1 4)","(range 0 5 -1 5)","(join(list 1 2) 10 5)"
					}; // redefine builtin symbol
	for (auto s : programs) {
		Interpreter interp;

		std::istringstream iss(s);

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

    std::string input = R"(
(define a 1 2)
)";

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
