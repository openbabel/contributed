/**********************************************************************
ringtest.cpp - Test ring perception algorithms.

This file is part of the Open Babel project.
For more information, see <http://openbabel.sourceforge.net/>

Copyright (C) 1998-2001 by OpenEye Scientific Software, Inc.
Some portions Copyright (C) 2001-2005 Geoffrey R. Hutchison
 
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

void ring_test()
{
  ostringstream os;

#ifdef TESTDATADIR
  string testdatadir = TESTDATADIR;
  string results_file = testdatadir + "ringresults.txt";
  string smilestypes_file = testdatadir + "attype.00.smi";
#else
  string results_file = "files/ringresults.txt";
  string smilestypes_file = "files/attype.00.smi";
#endif

  cout << "# Testing ring perception..." << endl;

  std::ifstream mifs;
  os << "Bail out! Cannot read test data " << smilestypes_file.c_str();
  BOOST_REQUIRE_MESSAGE( SafeOpen(mifs, smilestypes_file.c_str()), os.str().c_str() );

  std::ifstream rifs;
  os.str("");
  os << "Bail out! Cannot read test data " << results_file.c_str();
  BOOST_REQUIRE_MESSAGE( SafeOpen(rifs, results_file.c_str()), os.str().c_str() );

  unsigned int size;
  OBBond *bond;
  OBAtom *atom;
  int count;
  char buffer[BUFF_SIZE];
  vector<string> vs;
  vector<OBRing*> vr;
  vector<bool> vb;
  vector<int> vi;
  OBMol mol;
  vector<string>::iterator i;
  vector<OBEdgeBase*>::iterator j;
  vector<OBNodeBase*>::iterator k;
  vector<OBRing*>::iterator m;
  OBConversion conv(&mifs, &cout);
  unsigned int currentTest = 0;

  BOOST_REQUIRE_MESSAGE( conv.SetInAndOutFormats("SMI","SMI"), "Bail out! SMILES format is not loaded" );

  for (;mifs;)
    {
      mol.Clear();
      conv.Read(&mol);
      if (mol.Empty())
        continue;
      
      BOOST_REQUIRE_MESSAGE( rifs.getline(buffer,BUFF_SIZE), "Bail out! error reading reference data" );

      vb.clear();
      vb.resize(mol.NumBonds(),false);
      //check ring bonds
      tokenize(vs,buffer);
      for (i = vs.begin();i != vs.end();i++)
        vb[atoi(i->c_str())] = true;

      for (bond = mol.BeginBond(j);bond;bond = mol.NextBond(j))
        {
          os.str("");
	  os << "ring bond data different than reference # Molecule: " << mol.GetTitle();
          BOOST_CHECK_MESSAGE( vb[bond->GetIdx()] == bond->IsInRing(), os.str().c_str() );
        }

      vr = mol.GetSSSR();
      BOOST_REQUIRE_MESSAGE( rifs.getline(buffer,BUFF_SIZE), "Bail out! error reading reference data" );
      sscanf(buffer,"%d",&size);

      os.str("");
      os << "SSSR size different than reference # Molecule: " << mol.GetTitle();
      BOOST_CHECK_MESSAGE( vr.size() == size, os.str().c_str() ); //check SSSR size

      BOOST_REQUIRE_MESSAGE( rifs.getline(buffer,BUFF_SIZE), "Bail out! error reading reference data" );

      tokenize(vs,buffer);
      i = vs.begin();
      for (atom = mol.BeginAtom(k);atom;atom = mol.NextAtom(k))
        {
          os.str("");
          os << "error in SSSR count # Molecule: " << mol.GetTitle();
          BOOST_CHECK_MESSAGE( i != vs.end(), os.str().c_str() ); //check SSSR size

          count = 0;
          for (m = vr.begin();m != vr.end();m++)
            if ((*m)->_pathset[atom->GetIdx()])
              count++;

	  os.str("");
          os << "ring membership test failed # Molecule: " << mol.GetTitle();
          BOOST_CHECK_MESSAGE( atoi(i->c_str()) == count, os.str().c_str() ); 

          i++;
        }
    }

}

