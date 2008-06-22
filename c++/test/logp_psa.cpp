/**********************************************************************
logppsa.cpp - Unit tests for Open Babel OBLogP and OBPSA class

Copyright (C) 2008 Tim Vandermmeersch
 
This file is part of the Open Babel project.
For more information, see <http://openbabel.sourceforge.net/>

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
#include <openbabel/groupcontrib.h>

#include <stdio.h>
#include <iostream>

using namespace std;
using namespace OpenBabel;

void logp_psa_test()
{
  cout << "# Unit tests for OBLogP and OBPSA \n";

  OBConversion obConversion;
  obConversion.SetInAndOutFormats("smi", "mdl");
  OBMol obMol;
  
  OBDescriptor* obLogP = OBDescriptor::FindType("logP");
  BOOST_REQUIRE_MESSAGE( obLogP, "Could not find logP descriptor" );
  OBDescriptor* obPSA  = OBDescriptor::FindType("TPSA");
  BOOST_REQUIRE_MESSAGE( obPSA, "Could not find logP descriptor" );
  double logP, psa;

  // Oc1ccccc1OC
  obConversion.ReadString(&obMol, "Oc1ccccc1OC");
  obMol.AddHydrogens();
  
  logP = obLogP->Predict(&obMol);
  BOOST_CHECK_MESSAGE( IsNear(logP , 1.4008), "logP for molecule Oc1ccccc1OC" ); // value from JOELib2
  
  psa = obPSA->Predict(&obMol);
  BOOST_CHECK_MESSAGE( IsNear(psa , 29.46), "PSA for molecule Oc1ccccc1OC" ); // value from JOELib2

  // c1ccccc1CBr
  obConversion.ReadString(&obMol, "c1ccccc1CBr");
  obMol.AddHydrogens();
  
  logP = obLogP->Predict(&obMol);
  BOOST_CHECK_MESSAGE( IsNear(logP , 2.5815), "logP for molecule c1ccccc1CBr" ); // value from JOELib2
  
  psa = obPSA->Predict(&obMol);
  BOOST_CHECK_MESSAGE( IsNear(psa , 0.0), "PSA for molecule c1ccccc1CBr" ); // value from JOELib2

  // Cc1ccccc1NC(=O)C
  obConversion.ReadString(&obMol, "Cc1ccccc1NC(=O)C");
  obMol.AddHydrogens();
  
  logP = obLogP->Predict(&obMol);
  BOOST_CHECK_MESSAGE( IsNear(logP , 2.0264), "logP for molecule Cc1ccccc1NC(=O)C" ); // JOELib2 = 1.9534, more H added on N
  
  psa = obPSA->Predict(&obMol);
  BOOST_CHECK_MESSAGE( IsNear(psa , 29.1), "PSA for molecule Cc1ccccc1NC(=O)C" ); // JOELib2 = 1.9534, more H added on N

}


