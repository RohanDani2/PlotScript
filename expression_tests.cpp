#include "catch.hpp"

#include "expression.hpp"

TEST_CASE( "Test default expression", "[expression]" ) {

  Expression exp;
 
  REQUIRE(!exp.isHeadNumber());
  REQUIRE(!exp.isHeadSymbol());
}

TEST_CASE( "Test double expression", "[expression]" ) {

  Expression exp(6.023);
  
  REQUIRE(exp.isHeadNumber());
  REQUIRE(!exp.isHeadSymbol());

}


TEST_CASE( "Test symbol expression", "[expression]" ) {

  Expression exp(Atom("asymbol"));

  REQUIRE(!exp.isHeadNumber());
  REQUIRE(exp.isHeadSymbol());
}

