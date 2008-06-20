#include <boost/test/included/unit_test.hpp>
using boost::unit_test_framework::test_suite;

// the test classes
#include "atom.cpp"
#include "bond.cpp"
#include "mol.cpp"
#include "aromatest.cpp"
#include "iterators.cpp"
#include "cansmi.cpp"
#include "cmlreadfile.cpp"
#include "conversion.cpp"
#include "ffghemical.cpp"
#include "ffmmff94.cpp"
#include "ffuff.cpp"
#include "data.cpp"
#include "formalcharge.cpp"
#include "format.cpp"
#include "formula.cpp"
#include "phmodel.cpp"


test_suite *init_unit_test_suite( int argc, char* argv[] )
{
    test_suite* suite = BOOST_TEST_SUITE( "OpenBabelTestSuite" );

    // turn off slow sync with C-style output (we don't use it anyway).
    std::ios::sync_with_stdio(false);

    suite->add(BOOST_TEST_CASE(aromatic_test));
    suite->add(BOOST_TEST_CASE(atom_test));
    suite->add(BOOST_TEST_CASE(bond_test));
    suite->add(BOOST_TEST_CASE(cansmi_test));
    suite->add(BOOST_TEST_CASE(cmlreadfile_test));
    suite->add(BOOST_TEST_CASE(conversion_test));
    suite->add(BOOST_TEST_CASE(data_test));
    suite->add(BOOST_TEST_CASE(formalcharge_test));
    suite->add(BOOST_TEST_CASE(format_test));
    suite->add(BOOST_TEST_CASE(formula_test));
    suite->add(BOOST_TEST_CASE(ghemical_test));
    suite->add(BOOST_TEST_CASE(iterator_test));
    suite->add(BOOST_TEST_CASE(mmff94_test));
    suite->add(BOOST_TEST_CASE(mol_test));
    suite->add(BOOST_TEST_CASE(phmodel_test), 2); // 2 = expected failures
    suite->add(BOOST_TEST_CASE(uff_test));
    
    return suite;
}
