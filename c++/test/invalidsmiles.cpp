/**********************************************************************
invalid-smiles.cpp - Test SMILES pattern parsing (rejecting invalid patterns)

This file is part of the Open Babel project.
For more information, see <http://openbabel.sourceforge.net/>

Some portions Copyright (C) 2005-2006 Geoffrey R. Hutchison
Some portions Copyright (C) 2008 Tim Vandermeersch
 
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
#include <openbabel/mol.h>
#include <openbabel/obconversion.h>

#include <fstream>

using namespace std;
using namespace OpenBabel;

void invalid_smiles_test()
{
  ostringstream os;

#ifdef TESTDATADIR
  string testdatadir = TESTDATADIR;
  string invalid_file = testdatadir + "invalid-smiles.txt";
  string random1_file = testdatadir + "random";
  string random2_file = testdatadir + "random2";
  string random3_file = testdatadir + "random3";
#else
  string invalid_file = "files/invalid-smiles.txt";
  string random1_file = "files/random";
  string random2_file = "files/random2";
  string random3_file = "files/random3";
#endif

  cout << "# Testing invalid SMILES parsing..." << endl;

  // make sure to kill off all error reporting
  obErrorLog.StopLogging();

  std::ifstream mifs;
  os << "Bail out! Cannot read test data " << invalid_file.c_str();
  BOOST_REQUIRE_MESSAGE( SafeOpen(mifs, invalid_file.c_str()), os.str().c_str() );

  unsigned int currentTest = 0;
  OBMol mol;
  OBConversion conv(&mifs, &cout);
  BOOST_REQUIRE_MESSAGE( conv.SetInAndOutFormats("SMI","SMI"), "Bail out! SMILES format is not loaded" );

  while(mifs.good())
    {
      currentTest++;
      mol.Clear();
      os.str("");
      os << "test " << currentTest << " # invalid SMILES was parsed but should have been rejected";
      BOOST_CHECK_MESSAGE(!conv.Read(&mol) , os.str().c_str() );
    }

  mifs.close();
  mifs.clear();

  // A known bug in 2.1.1 -- we hang on random SMILES garbage. This will be
  // fixed in the next release. PR#1730132
//   // random file#1
//   if (!SafeOpen(mifs, random1_file.c_str()))
//     {
//       cout << "Bail out! Cannot read file " << random1_file << endl;
//       return -1; // test failed
//     }

//   mol.Clear();
//   if (conv.Read(&mol, &mifs))
//     cout << "not ok " << ++currentTest << " # random data was parsed "
//          << " but should have been rejected\n";
//   else
//     cout << "ok " << ++currentTest << " # random data 1\n";
  
//   mifs.close();
//   mifs.clear();

//   // random2
//   if (!SafeOpen(mifs, random2_file.c_str()))
//     {
//       cout << "Bail out! Cannot read file " << random2_file << endl;
//       return -1; // test failed
//     }

//   mol.Clear();
//   if (conv.Read(&mol, &mifs))
//     cout << "not ok " << ++currentTest << " # random data #2 was parsed "
//          << " but should have been rejected\n";
//   else
//     cout << "ok " << ++currentTest << " # random data 2\n";

//   mifs.close();
//   mifs.clear();

//   // random3
//   if (!SafeOpen(mifs, random3_file.c_str()))
//     {
//       cout << "Bail out! Cannot read file " << random3_file << endl;
//       return -1; // test failed
//     }

//   mol.Clear();
//   if (conv.Read(&mol, &mifs))
//     cout << "not ok " << ++currentTest << " # random data #3 was parsed "
//          << " but should have been rejected\n";
//   else
//     cout << "ok " << ++currentTest << " # random data 3\n";

}
