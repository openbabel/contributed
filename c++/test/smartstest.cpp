 /**********************************************************************
 smartstest.cpp - Test SMARTS algorithms and atom typing.

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

#include <fstream>

#include <openbabel/mol.h>
#include <openbabel/obconversion.h>


using namespace std;
using namespace OpenBabel;

void smarts_test()
{
  ostringstream os;

#ifdef TESTDATADIR
  string testdatadir = TESTDATADIR;
  string smarts_file = testdatadir + "smartstest.txt";
  string results_file = testdatadir + "smartsresults.txt";
  string smilestypes_file = testdatadir + "attype.00.smi";
#else
   string smarts_file = "files/smartstest.txt";
   string results_file = "files/smartsresults.txt";
   string smilestypes_file = "files/attype.00.smi";
#endif

  cout << endl << "# Testing SMARTS...  \n";

  std::ifstream ifs;
  os << "Bail out! Cannot read test data " << smarts_file;
  BOOST_REQUIRE_MESSAGE( SafeOpen(ifs, smarts_file.c_str()), os.str().c_str() );

  //read in the SMARTS test patterns
  char buffer[BUFF_SIZE];
  vector<OBSmartsPattern*> vsp;
  for (;ifs.getline(buffer,BUFF_SIZE);)
    {
      if (buffer[0] == '#') // skip comment line
        continue;

      OBSmartsPattern *sp = new OBSmartsPattern;

      if (sp->Init(buffer))
        vsp.push_back(sp);
      else
        delete sp;
    }
  ifs.close();

  std::ifstream rifs;
  os.str("");
  os << "Bail out! Cannot read results file " << results_file;
  BOOST_REQUIRE_MESSAGE( SafeOpen(rifs, results_file.c_str()), os.str().c_str() );
  
  unsigned int npats;
  rifs.getline(buffer,BUFF_SIZE);
  sscanf(buffer,"%d %*s",&npats);

  //make sure the number of SMARTS patterns is the same as in the
  //reference data
  os.str("");
  os << "Bail out! Correct number of patterns not read in" <<
        "Read in " << vsp.size() << " expected " << npats << endl;
  BOOST_REQUIRE_MESSAGE( npats == vsp.size(), os.str().c_str() );
 
  std::ifstream mifs;
  os << "Bail out! Cannot read atom types " << smilestypes_file;
  BOOST_REQUIRE_MESSAGE( SafeOpen(mifs, smilestypes_file.c_str()), os.str().c_str() );

  unsigned int k;
  unsigned int res_line = 0;
  OBMol mol;
  vector<string> vs;
  vector<OBSmartsPattern*>::iterator i;
  vector<vector<int> > mlist;
  unsigned int currentMol = 0; // each molecule is a separate test
  bool molPassed = true;

  OBConversion conv(&mifs, &cout);
  BOOST_REQUIRE_MESSAGE( conv.SetInAndOutFormats("SMI","SMI"), "Bail out! SMILES format is not loaded" );

  //read in molecules, match SMARTS, and compare results to reference data
  for (;mifs;)
    {
      mol.Clear();
      conv.Read(&mol);
      if (mol.Empty())
        continue;

      currentMol++;
      molPassed = true;

      for (i = vsp.begin();i != vsp.end();i++)
        {
          BOOST_REQUIRE_MESSAGE( rifs.getline(buffer,BUFF_SIZE), "Bail out! Error reading reference data" );
          res_line++;

          tokenize(vs,buffer);
          (*i)->Match(mol);
          mlist = (*i)->GetMapList();
 
          if (mlist.size() != vs.size()) {
            os.str("");
            os << "mol " << currentMol
               << " # number of matches different than reference\n";
            os << "# Expected " << vs.size() << " matches, found "
               << mlist.size() << "\n";
            os << "# Error with molecule " << mol.GetTitle();
            os << "#  on pattern " << (*i)->GetSMARTS() << "\n";
            if (mlist.size())
              os << "# First match: atom #" << mlist[0][0] << "\n";
            molPassed = false;
            break;
          }
          BOOST_CHECK_MESSAGE( mlist.size() == vs.size(), os.str().c_str() );

          if (mlist.size())
            {
              for (k = 0;k < vs.size();k++)
                {
                  if (atoi(vs[k].c_str()) != mlist[k][0]) {
                    os.str("");
                    os << "not ok " << currentMol
                       << "# matching atom numbers different than reference\n";
                    os << "# Expected " << vs[k] << " but found "
                       << mlist[k][0] << "\n";
                    os << "# Molecule: " << mol.GetTitle() << "\n";
                    os << "# Pattern: " << (*i)->GetSMARTS() << "\n";
                    molPassed = false;
                    break;
                  }
                  BOOST_CHECK_MESSAGE( atoi(vs[k].c_str()) == mlist[k][0], os.str().c_str() );
                }
              if (k != vs.size())
                {
                  molPassed = false;
                  break;
                }
            }
        }
      os.str("");
      os << "ok " << currentMol << " # molecule passed tests\n";
      BOOST_CHECK_MESSAGE( molPassed, os.str().c_str() );
    }

}
