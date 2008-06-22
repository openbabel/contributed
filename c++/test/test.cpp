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
#include "mmff94validate.cpp"
#include "internalcoord.cpp"
#include "logp_psa.cpp"
#include "ringtest.cpp"
#include "math.cpp"
#include "residue.cpp"
#include "invalidsmiles.cpp"
#include "invalidsmarts.cpp"
#include "smartstest.cpp"
#include "smilesmatch.cpp"
//#include "inchiwrite.cpp"
#include "unitcell.cpp"
#include "forcefield.cpp"
#include "chains.cpp"

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
    suite->add(BOOST_TEST_CASE(forcefield_test));
    suite->add(BOOST_TEST_CASE(formalcharge_test));
    suite->add(BOOST_TEST_CASE(format_test));
    suite->add(BOOST_TEST_CASE(formula_test));
    suite->add(BOOST_TEST_CASE(ghemical_test));
    //suite->add(BOOST_TEST_CASE(inchi_write_test));
    suite->add(BOOST_TEST_CASE(internalcoord_test));
    suite->add(BOOST_TEST_CASE(invalid_smarts_test));
    suite->add(BOOST_TEST_CASE(invalid_smiles_test));
    suite->add(BOOST_TEST_CASE(iterator_test));
    suite->add(BOOST_TEST_CASE(logp_psa_test));
    suite->add(BOOST_TEST_CASE(math_test));
    suite->add(BOOST_TEST_CASE(mmff94_test));
    suite->add(BOOST_TEST_CASE(mol_test));
    suite->add(BOOST_TEST_CASE(phmodel_test), 2); // 2 = expected failures
    suite->add(BOOST_TEST_CASE(smarts_test)); 
    suite->add(BOOST_TEST_CASE(smiles_match_test)); 
    suite->add(BOOST_TEST_CASE(residue_test)); 
    suite->add(BOOST_TEST_CASE(ring_test)); 
    suite->add(BOOST_TEST_CASE(uff_test));
    suite->add(BOOST_TEST_CASE(unitcell_test));
    
    suite->add(BOOST_TEST_CASE(mmff94_validate), 143);

    suite->add(BOOST_TEST_CASE(chains_test));


    return suite;
}
