/**********************************************************************
formula.cpp - Test molecular formula, weight & exact mass

This file is part of the Open Babel project.
For more information, see <http://openbabel.sourceforge.net/>

Some portions Copyright (C) 2005-2006 Geoffrey R. Hutchison
Copyright (C) 2008 Tim Vandermeersch
 
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation version 2 of the License.
 
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
***********************************************************************/

// used to set import/export for Cygwin DLLs
#ifdef WIN32
#define USING_OBDLL
#endif

#include <openbabel/babelconfig.h>

#include <fstream>

#include <openbabel/mol.h>
#include <openbabel/obconversion.h>

using namespace std;
using namespace OpenBabel;

void GenerateFormulaReference();

void formula_test()
{
#ifdef TESTDATADIR
  string testdatadir = TESTDATADIR;
  string results_file = testdatadir + "formularesults.txt";
  string smilestypes_file = testdatadir + "attype.00.smi";
#else
  string results_file = "files/formularesults.txt";
  string smilestypes_file = "files/attype.00.smi";
#endif

  cout << "# Testing molecular formulas..." << endl;

  std::ifstream mifs;
  BOOST_REQUIRE_MESSAGE( SafeOpen(mifs, smilestypes_file.c_str()), "Bail out! Cannot read file " );

  std::ifstream rifs;
  BOOST_REQUIRE_MESSAGE( SafeOpen(rifs, smilestypes_file.c_str()), "Bail out! Cannot read file " );

  char buffer[BUFF_SIZE];
  char message[BUFF_SIZE];
  vector<string> vs;
  OBMol mol;
  OBConversion conv(&mifs, &cout);
  unsigned int currentTest = 0;
  // double mass;

  BOOST_REQUIRE_MESSAGE( conv.SetInAndOutFormats("SMI","SMI"), "Bail out! SMILES format is not loaded" );

  for (;mifs;)
    {
      mol.Clear();
      conv.Read(&mol);
      if (mol.Empty())
        continue;
      BOOST_REQUIRE_MESSAGE( rifs.getline(buffer,BUFF_SIZE), "Bail out! error reading reference data" );

      tokenize(vs,buffer);
      BOOST_REQUIRE_MESSAGE( vs.size() == 3, "Bail out! Reference data has incorrect format" );

      currentTest++;
      snprintf(message, BUFF_SIZE, "not ok %d # molecular formula incorrect for molecule %s"
		                   " # Expected %s, found %s", currentTest, mol.GetTitle(),
				   vs[0].c_str(), mol.GetFormula().c_str());
      BOOST_CHECK_MESSAGE(vs[0] == mol.GetFormula(), message );

      currentTest++;
      snprintf(message, BUFF_SIZE, "not ok %d # molecular weight incorrect for molecule %s"
		                   " # Expected %f, found %f ", currentTest, mol.GetTitle(),
				   atof(vs[1].c_str()), mol.GetMolWt());
      BOOST_CHECK_MESSAGE( fabs(atof(vs[1].c_str()) - mol.GetMolWt() ) < 1.0e-3, message );

      currentTest++;
      snprintf(message, BUFF_SIZE, "not ok %d # exact mass incorrect for molecule %s"
		                   " # Expected %f, found %f ", currentTest, mol.GetTitle(),
				   atof(vs[2].c_str()), mol.GetExactMass());
      BOOST_CHECK_MESSAGE( fabs(atof(vs[2].c_str()) - mol.GetExactMass() ) < 1.0e-3, message );

      // now after adding explict hydrogens -- should be identical
      //  since we'll add hydrogens that were implicit before

      // PR#1485580
      BOOST_CHECK( mol.AddHydrogens() );

      currentTest++;
      snprintf(message, BUFF_SIZE, "not ok %d # molecular formula incorrect for "
		                   "hydrogen-added molecule %s", currentTest, mol.GetTitle());
      BOOST_CHECK_MESSAGE(vs[0] == mol.GetFormula(), message);

      currentTest++;
      snprintf(message, BUFF_SIZE, "not ok %d # molecular weight incorrect for hydrogen-added "
		                   "molecule %s # Expected %f, found %f ", currentTest, mol.GetTitle(),
				   atof(vs[1].c_str()), mol.GetMolWt());
      BOOST_CHECK_MESSAGE( fabs(atof(vs[1].c_str()) - mol.GetMolWt() ) < 1.0e-3, message);

      currentTest++;
      snprintf(message, BUFF_SIZE, "not ok %d # exact mass  incorrect for hydrogen-added "
		                   "molecule %s # Expected %f, found %f ", currentTest, mol.GetTitle(),
				   atof(vs[2].c_str()), mol.GetExactMass());
      BOOST_CHECK_MESSAGE( fabs(atof(vs[2].c_str()) - mol.GetExactMass() ) < 1.0e-3, message);

    }

}
