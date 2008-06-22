/**********************************************************************
forcefield.cpp - Test OBForceField class

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

#include <openbabel/babelconfig.h>
#include <openbabel/mol.h>
#include <openbabel/obconversion.h>
#include <openbabel/builder.h>
#include <openbabel/forcefield.h>
#include <fstream>

using namespace std;
using namespace OpenBabel;

void forcefield_test()
{

#ifdef TESTDATADIR
  string testdatadir = TESTDATADIR;
  string molecules_file = testdatadir + "forcefield.sdf";
#else
  string molecules_file = "files/forcefield.sdf";
#endif

  cout << "# Testing OBForceField..." << endl;

  std::ifstream mifs;
  BOOST_REQUIRE_MESSAGE( SafeOpen(mifs, molecules_file.c_str()), "Bail out! Cannot read file " );

  OBMol mol;
  OBConversion conv(&mifs, &cout);

  BOOST_REQUIRE_MESSAGE( conv.SetInAndOutFormats("SDF","SDF"), "Bail out! SDF format is not loaded" );
    
  OBForceField* pFF = OBForceField::FindForceField("MMFF94");

  BOOST_REQUIRE_MESSAGE(pFF != NULL, "Bail out! Cannot load force field!" );

  pFF->SetLogFile(&cout);
  pFF->SetLogLevel(OBFF_LOGLVL_NONE);

  double energy;
  while(mifs) {
    mol.Clear();
    conv.Read(&mol);
    if (mol.Empty())
      continue;
    if (string(mol.GetTitle()) != "/Volumes/Home/ghutchis/Desktop/UFF/clopidogrel.gpr")
      continue;

    // use OBBuilder to create molecules, they are not optimized    
    OBBuilder builder;
    builder.Build(mol);
    BOOST_REQUIRE_MESSAGE( pFF->Setup(mol), "Bail out! could not setup force field on " );
    BOOST_CHECK_MESSAGE( fabs(pFF->Energy(false) - 1026.59) < 1.0, 
		    "Output from OBBuilder::Build() has wrong energy" );
 
    // test SteepestDescent
    pFF->SteepestDescent(100);
    BOOST_CHECK_MESSAGE( fabs(pFF->Energy(false) - 104.177) < 1.0,
		   "Energy after SteepestDescent(100) is wrong" );

    // test SetCoordinates
    pFF->SetCoordinates(mol);
    BOOST_CHECK_MESSAGE( fabs(pFF->Energy(false) - 1026.59) < 1.0, 
		    "SetCoordinates() doesn't reset coordinates in _mol" );

    // test ConjugateGradients
    pFF->ConjugateGradients(100);
    BOOST_CHECK_MESSAGE( fabs(pFF->Energy(false) - 106.496) < 1.0,
		   "Energy after ConjugateGradients(100) is wrong" );

    // test Setup on resetting coordinates
    pFF->Setup(mol);
    BOOST_CHECK_MESSAGE( fabs(pFF->Energy(false) - 1026.59) < 1.0, 
		    "Setup() doesn't reset coordinates in _mol" );


  }

}
