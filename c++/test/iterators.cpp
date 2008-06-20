 /**********************************************************************
 iterators.cpp - tests for iterators

 This file is part of the Open Babel project.
 For more information, see <http://openbabel.sourceforge.net/>

 Copyright (C) 1998-2001 by OpenEye Scientific Software, Inc.
 Some portions Copyright (C) 2001-2005 Geoffrey R. Hutchison
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
#include <openbabel/obiter.h>
#include <openbabel/obconversion.h>
#include <fstream>

using namespace std;
using namespace OpenBabel;

void iterator_test()
{
#ifdef TESTDATADIR
  string testdatadir = TESTDATADIR;
  string smilestypes_file = testdatadir + "attype.00.smi";
#else
   string smilestypes_file = "files/attype.00.smi";
#endif

  cout << endl << "# Testing iterators...  \n";

  char message[BUFF_SIZE];
  
  std::ifstream mifs;

  snprintf( message, BUFF_SIZE, "Bail out! Cannot read test data %s", smilestypes_file.c_str());
  BOOST_REQUIRE_MESSAGE( SafeOpen(mifs, smilestypes_file.c_str()), message );

  OBConversion conv(&mifs, &cout);
  BOOST_REQUIRE_MESSAGE( conv.SetInAndOutFormats("SMI","SMI"), "Bail out! SMILES format is not loaded" );

  OBMol mol;

  unsigned int currentTest = 0;
  unsigned int counter = 0;

  // run through atom and bond iterators
  while(mifs.peek() != EOF && mifs.good())
    {
      mol.Clear();
      conv.Read(&mol);
      if (mol.Empty())
        continue;

      counter = 0;
      currentTest++;
      FOR_ATOMS_OF_MOL(atom, mol)
        ++counter;
      snprintf( message, BUFF_SIZE, "not ok %d # atom iterator failed: expected %d "
		                    "but found %d", currentTest, mol.NumAtoms(), counter);
      BOOST_CHECK_MESSAGE(counter == mol.NumAtoms(), message);

      counter = 0;
      currentTest++;
      FOR_DFS_OF_MOL(atom, mol)
        ++counter;
      snprintf( message, BUFF_SIZE, "not ok %d # depth-first iterator failed: expected %d "
		                    "but found %d", currentTest, mol.NumAtoms(), counter);
      BOOST_CHECK_MESSAGE(counter == mol.NumAtoms(), message);

      counter = 0;
      currentTest++;
      FOR_BFS_OF_MOL(atom, mol)
        ++counter;
      snprintf( message, BUFF_SIZE, "not ok %d # breadth-first iterator failed: expected %d "
		                    "but found %d", currentTest, mol.NumAtoms(), counter);
      BOOST_CHECK_MESSAGE(counter == mol.NumAtoms(), message);

      counter = 0;
      currentTest++;
      FOR_BONDS_OF_MOL(bond, mol)
        ++counter;
      snprintf( message, BUFF_SIZE, "not ok %d # bond iterator failed: expected %d "
		                    "but found %d", currentTest, mol.NumBonds(), counter);
      BOOST_CHECK_MESSAGE(counter == mol.NumBonds(), message);

    }

}
