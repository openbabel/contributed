#include <boost/test/included/unit_test.hpp>
using boost::unit_test_framework::test_suite;

// the test classes
#include "atom.cpp"
#include "bond.cpp"
#include "mol.cpp"


test_suite *init_unit_test_suite( int argc, char* argv[] )
{
    test_suite* test = BOOST_TEST_SUITE( "OpenBabel test suite" );

    return test;
}
