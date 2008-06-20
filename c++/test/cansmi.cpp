 /**********************************************************************
 cansmi.cpp - Test Canonical SMILES generation -- write and then read

 This file is part of the Open Babel project.
 For more information, see <http://openbabel.sourceforge.net/>

 Copyright (C) 2001-2007 Geoffrey R. Hutchison
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

void cansmi_test()
{
#ifdef TESTDATADIR
  string testdatadir = TESTDATADIR;
  string smilestypes_file = testdatadir + "nci.smi";
#else
   string smilestypes_file = "files/nci.smi";
#endif

  cout << endl << "# Testing Canonical SMILES Generation ...  \n";
  
  char message[BUFF_SIZE];
  
  std::ifstream mifs;
  snprintf( message, BUFF_SIZE, "Bail out! Cannot read test data %s", smilestypes_file.c_str());
  BOOST_REQUIRE_MESSAGE( SafeOpen(mifs, smilestypes_file.c_str()), message );

  OBConversion conv(&mifs, &cout);
  BOOST_REQUIRE_MESSAGE( conv.SetInAndOutFormats("SMI","CAN"), "Bail out! SMILES format is not loaded" );

  unsigned int currentMol = 0;
  OBMol mol, mol2;
  string buffer;

  //read in molecules (as SMI), write as CANSMI, read in again
  while (getline(mifs, buffer))
    {
      mol.Clear();
      
      currentMol++;

      snprintf( message, BUFF_SIZE, "not ok %d # SMILES read failed, buffer was %s", currentMol, buffer.c_str());
      BOOST_CHECK_MESSAGE( conv.ReadString(&mol, buffer), message );
      if (mol.Empty())
        continue;

      currentMol++;
      buffer = conv.WriteString(&mol);

      mol2.Clear();
      snprintf( message, BUFF_SIZE, "not ok %d # SMARTS did not match for molecule %s", currentMol, buffer.c_str());
      BOOST_CHECK_MESSAGE( conv.ReadString(&mol2, buffer), message );

      // Now make sure the molecules are roughly equivalent
      currentMol++;
      snprintf( message, BUFF_SIZE, "not ok %d # number of heavy atoms wrong for molecule %s", currentMol, buffer.c_str());
      BOOST_CHECK_MESSAGE( mol.NumHvyAtoms() == mol2.NumHvyAtoms(), message );
    }

}
