#include <boost/test/included/unit_test.hpp>
using boost::unit_test_framework::test_suite;

// the test classes
#include "atom.cpp"
#include "bond.cpp"
#include "mol.cpp"
#include "ffmmff94.cpp"


test_suite *init_unit_test_suite( int argc, char* argv[] )
{
    test_suite* suite = BOOST_TEST_SUITE( "OpenBabel Test Suite" );

    suite->add(BOOST_TEST_CASE(atom_test));
    suite->add(BOOST_TEST_CASE(bond_test));
    suite->add(BOOST_TEST_CASE(mol_test));
    
    return suite;
}
