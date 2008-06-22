/**********************************************************************
chains.cpp - Test OBChainParser class

This file is part of the Open Babel project.
For more information, see <http://openbabel.sourceforge.net/>

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

#include <boost/test/unit_test.hpp>

#include <openbabel/babelconfig.h>
#include <openbabel/mol.h>
#include <openbabel/obconversion.h>
#include <fstream>

using namespace std;
using namespace OpenBabel;

void chains_test()
{
  string files[] = {"1CRN", "1XAJ", "1FB7", "1LWF", "2RKP"};
  cout << "# Testing OBChainParser..." << endl;

  OBConversion conv;
 
  ostringstream out;
  for (int i = 0; i < 5; ++i) {
    string xyzfile = "files/" + files[i] + ".xyz";
    string pdbfile = "files/" + files[i] + ".pdb";

    cout << "file = " << xyzfile << endl;
    // read xyz & pdb file
    std::ifstream ifsXYZ, ifsPDB;
    out.str("");
    out << "Bail out! Cannot read XYZ file: " << xyzfile;
    BOOST_REQUIRE_MESSAGE( SafeOpen(ifsXYZ, xyzfile.c_str()), out.str().c_str() );
    out.str("");
    out << "Bail out! Cannot read XYZ file: " << pdbfile;
    BOOST_REQUIRE_MESSAGE( SafeOpen(ifsPDB, pdbfile.c_str()), out.str().c_str() );
    
    OBMol xyz, pdb;
    conv.SetInFormat("XYZ");
    conv.Read(&xyz, &ifsXYZ);
    conv.SetInFormat("PDB");
    conv.Read(&pdb, &ifsPDB);
    ifsXYZ.close();
    ifsPDB.close();

    xyz.GetAtom(1)->GetResidue(true);

    // compare number of residues
    BOOST_CHECK_EQUAL( xyz.NumResidues(), pdb.NumResidues() );

    FOR_RESIDUES_OF_MOL (res, xyz) {
      OBResidue *resRef = pdb.GetResidue(res->GetIdx());
      BOOST_CHECK_EQUAL( res->GetName(), resRef->GetName() );
    }


    
  }

}

