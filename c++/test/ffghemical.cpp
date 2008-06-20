/**********************************************************************
ffghemical.cpp - Test energy and gradients for Ghemical force field

This file is part of the Open Babel project.
For more information, see <http://openbabel.sourceforge.net/>

Some portions Copyright (C) 2008 Geoffrey R. Hutchison
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
#include <openbabel/forcefield.h>
#include <fstream>

using namespace std;
using namespace OpenBabel;

void ghemical_test()
{
#ifdef TESTDATADIR
  string testdatadir = TESTDATADIR;
  string results_file = testdatadir + "ghemicalresults.txt";
  string molecules_file = testdatadir + "forcefield.sdf";
#else
  string results_file = "files/ghemicalresults.txt";
  string molecules_file = "files/forcefield.sdf";
#endif

  cout << "# Testing Ghemical Force Field..." << endl;

  std::ifstream mifs;
  BOOST_REQUIRE_MESSAGE( SafeOpen(mifs, molecules_file.c_str()), "Bail out! Cannot read file " );

  std::ifstream rifs;
  BOOST_REQUIRE_MESSAGE( SafeOpen(rifs, results_file.c_str()), "Bail out! Cannot read file " );

  char buffer[BUFF_SIZE];
  char message[BUFF_SIZE];
  vector<string> vs;
  OBMol mol;
  OBConversion conv(&mifs, &cout);
  unsigned int currentTest = 0;

  BOOST_REQUIRE_MESSAGE( conv.SetInAndOutFormats("SDF","SDF"), "Bail out! SDF format is not loaded" );
    
  OBForceField* pFF = OBForceField::FindForceField("Ghemical");

  BOOST_REQUIRE_MESSAGE(pFF != NULL, "Bail out! Cannot load force field!" );

  pFF->SetLogFile(&cout);
  pFF->SetLogLevel(OBFF_LOGLVL_NONE);

  double energy;
  while(mifs)
    {
      currentTest++;
      mol.Clear();
      conv.Read(&mol);
      if (mol.Empty())
        continue;
      BOOST_REQUIRE_MESSAGE( rifs.getline(buffer,BUFF_SIZE), "Bail out! error reading reference data" );
        
      BOOST_REQUIRE_MESSAGE( pFF->Setup(mol), "Bail out! could not setup force field on " );

      // compare the calculated energy to our reference data
      energy = pFF->Energy(false);
      snprintf(message, BUFF_SIZE, "not ok %d # calculated energy incorect "
		      " for molecule %s\n # Expected %s, found %f", 
		      currentTest, mol.GetTitle(), buffer, energy);
      BOOST_CHECK_MESSAGE( fabs(atof(buffer) - energy ) < 1.0e-3, message );

      // check that gradients validate too
      snprintf(message, BUFF_SIZE, "not ok %d # gradients do not validate", currentTest);
      BOOST_CHECK_MESSAGE( pFF->ValidateGradients(), message );
    }

}
