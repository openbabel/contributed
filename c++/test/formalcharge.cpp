/**********************************************************************
formalcharge.cpp - Test molecular formal charge perception

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
#include <openbabel/mol.h>
#include <openbabel/obconversion.h>
#include <fstream>

using namespace std;
using namespace OpenBabel;

void formalcharge_test()
{
#ifdef TESTDATADIR
  string testdatadir = TESTDATADIR;
  string results_file = testdatadir + "formalchargeresults.txt";
  string smilestypes_file = testdatadir + "attype.00.smi";
#else
  string results_file = "files/formalchargeresults.txt";
  string smilestypes_file = "files/attype.00.smi";
#endif

  cout << "# Testing molecular formal charges..." << endl;

  std::ifstream mifs;
  BOOST_REQUIRE_MESSAGE( SafeOpen(mifs, smilestypes_file.c_str()), "Bail out! Cannot read file " );

  std::ifstream rifs;
  BOOST_REQUIRE_MESSAGE( SafeOpen(rifs, results_file.c_str()), "Bail out! Cannot read file " );

  char buffer[BUFF_SIZE];
  char message[BUFF_SIZE];
  vector<string> vs;
  OBMol mol;
  OBConversion conv(&mifs, &cout);
  unsigned int currentTest = 1;

  BOOST_REQUIRE_MESSAGE( conv.SetInAndOutFormats("SMI","SMI"), "Bail out! SMILES format is not loaded" );

  for (;mifs;)
    {
      mol.Clear();
      conv.Read(&mol);
      if (mol.Empty())
        continue;
      BOOST_REQUIRE_MESSAGE( rifs.getline(buffer,BUFF_SIZE), "Bail out! error reading reference data" );

      tokenize(vs,buffer);
      // check charges
    }

}
