 /**********************************************************************
 smilesmatch.cpp - Test SMARTS matching (i.e., SMILES matching themselves)

 This file is part of the Open Babel project.
 For more information, see <http://openbabel.sourceforge.net/>

 Copyright (C) 1998-2001 by OpenEye Scientific Software, Inc.
 Some portions Copyright (C) 2001-2005 Geoffrey R. Hutchison
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

void smiles_match_test()
{
  ostringstream os;

#ifdef TESTDATADIR
  string testdatadir = TESTDATADIR;
  string smilestypes_file = testdatadir + "nci.smi";
#else
   string smilestypes_file = "files/nci.smi";
#endif

  cout << endl << "# Testing SMILES self-matching using SMARTS...  \n";
  
  std::ifstream mifs;
  os << "Bail out! Cannot read test data " << smilestypes_file;
  BOOST_REQUIRE_MESSAGE( SafeOpen(mifs, smilestypes_file.c_str()), os.str().c_str() );

  OBConversion conv(&mifs, &cout);
  BOOST_REQUIRE_MESSAGE( conv.SetInAndOutFormats("SMI","SMI"), "Bail out! SMILES format is not loaded" );

  unsigned int currentMol = 0;
  OBSmartsPattern smarts;
  OBMol mol;
  string buffer;

  //read in molecules and see if their SMARTS matches themselves
  while (getline(mifs, buffer))
    {
      mol.Clear();
      conv.ReadString(&mol, buffer);
      if (mol.Empty())
        continue;

      // trim off any title, etc.
      string::size_type pos = buffer.find_first_of(" \t\n\r");
      if (pos != string::npos)
        buffer.erase(pos);

      pos = buffer.find_first_of('.');
      if (pos != string::npos)
        continue;

      smarts.Init(buffer);
      os.str("");
      os << "not ok " << ++currentMol << " # SMARTS did not match"
         << " for molecule " << buffer;
      BOOST_CHECK_MESSAGE( smarts.Match(mol), os.str().c_str() );
    }

}
