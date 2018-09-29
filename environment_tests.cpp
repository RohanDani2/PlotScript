#include "catch.hpp"

#include "environment.hpp"
#include "semantic_error.hpp"

#include <cmath>

TEST_CASE( "Test default constructor", "[environment]" ) {

  Environment env;

  REQUIRE(env.is_known(Atom("pi")));
  REQUIRE(env.is_exp(Atom("pi")));

  REQUIRE(!env.is_known(Atom("hi")));
  REQUIRE(!env.is_exp(Atom("hi")));

  REQUIRE(env.is_proc(Atom("+")));
  REQUIRE(env.is_proc(Atom("-")));
  REQUIRE(env.is_proc(Atom("*")));
  REQUIRE(env.is_proc(Atom("/")));
  REQUIRE(!env.is_proc(Atom("op")));
}

TEST_CASE( "Test get expression", "[environment]" ) {
  Environment env;

  REQUIRE(env.get_exp(Atom("pi")) == Expression(std::atan2(0, -1)));
  REQUIRE(env.get_exp(Atom("hi")) == Expression());
}

TEST_CASE( "Test add expression", "[environment]" ) {
  Environment env;

  Expression a(Atom(1.0));
  env.add_exp(Atom("one"), a);
  REQUIRE(env.is_known(Atom("one")));
  REQUIRE(env.is_exp(Atom("one")));
  REQUIRE(env.get_exp(Atom("one")) == a);

  Expression b(Atom("hello"));
  env.add_exp(Atom("hi"), b);
  REQUIRE(env.is_known(Atom("hi")));
  REQUIRE(env.is_exp(Atom("hi")));
  REQUIRE(env.get_exp(Atom("hi")) == b);

  REQUIRE_THROWS_AS(env.add_exp(Atom(1.0), b), SemanticError);
}

TEST_CASE( "Test get built-in procedure", "[environment]" ) {
  Environment env;

  INFO("default procedure")
  Procedure p1 = env.get_proc(Atom("doesnotexist"));
  Procedure p2 = env.get_proc(Atom("alsodoesnotexist"));
  REQUIRE(p1 == p2);

  std::vector<Expression> args;
  std::vector<Expression> complexArgs;
  std::vector<Expression> complexNumArgs;
  std::vector<Expression> numComplexArgs;
  complexArgs.emplace_back(std::complex<double>(5.0, 6.0));
  complexArgs.emplace_back(std::complex<double>(5.0, 6.0));
  complexNumArgs.emplace_back(std::complex<double>(5.0, 6.0));
  complexNumArgs.emplace_back(1.0);
  numComplexArgs.emplace_back(1.0);
  numComplexArgs.emplace_back(std::complex<double>(5.0, 6.0));

  REQUIRE(p1(args) == Expression());
  REQUIRE(p2(args) == Expression());

  INFO("trying add procedure")
  Procedure padd = env.get_proc(Atom("+"));
  args.emplace_back(1.0);
  args.emplace_back(2.0);
  REQUIRE(padd(args) == Expression(3.0));
  REQUIRE(padd(complexArgs) == Expression(std::complex<double>(10.0,12.0)));
  REQUIRE(padd(complexNumArgs) == Expression(std::complex<double>(6.0, 6.0)));

  Procedure pmul = env.get_proc(Atom("*"));
  REQUIRE(pmul(args) == Expression(2.0));
  REQUIRE(pmul(complexArgs) == Expression(std::complex<double>(25.0, 36.0)));
  REQUIRE(pmul(complexNumArgs) == Expression(std::complex<double>(5.0, 6.0)));

  Procedure psub = env.get_proc(Atom("-"));
  REQUIRE(psub(args) == Expression(-1.0));
  REQUIRE(psub(complexArgs) == Expression(std::complex<double>(0.0, 0.0)));
  REQUIRE(psub(complexNumArgs) == Expression(std::complex<double>(4.0, 6.0)));
  REQUIRE(psub(numComplexArgs) == Expression(std::complex<double>(-4.0, 6.0)));

  Procedure pdiv = env.get_proc(Atom("/"));
  REQUIRE(pdiv(args) == Expression(0.5));
  REQUIRE(pdiv(complexArgs) == Expression(std::complex<double>(1.0, 1.0)));
  REQUIRE(pdiv(complexNumArgs) == Expression(std::complex<double>(5.0, 6.0)));
  REQUIRE(pdiv(numComplexArgs) == Expression(std::complex<double>(0.2, 6.0)));

  Procedure pexp = env.get_proc(Atom("^"));
  REQUIRE(pexp(args) == Expression(1.0));
  REQUIRE(pexp(complexArgs) == Expression(std::complex<double>(3125.0, 46656.0)));
  REQUIRE(pexp(complexNumArgs) == Expression(std::complex<double>(5.0, 6.0)));
  REQUIRE(pexp(numComplexArgs) == Expression(std::complex<double>(1.0, 6.0)));
}

TEST_CASE( "Test reset", "[environment]" ) {
  Environment env;

  Expression a(Atom(1.0));
  env.add_exp(Atom("one"), a);
  Expression b(Atom("hello"));
  env.add_exp(Atom("hi"), b);

  env.reset();
  REQUIRE(!env.is_known(Atom("one")));
  REQUIRE(!env.is_exp(Atom("one")));
  REQUIRE(env.get_exp(Atom("one")) == Expression());
  REQUIRE(!env.is_known(Atom("hi")));
  REQUIRE(!env.is_exp(Atom("hi")));
  REQUIRE(env.get_exp(Atom("hi")) == Expression());
}

TEST_CASE( "Test semeantic errors", "[environment]" ) {

  Environment env;

  {
    Expression exp(Atom("begin"));
    
    REQUIRE_THROWS_AS(exp.eval(env), SemanticError);
  }
}

