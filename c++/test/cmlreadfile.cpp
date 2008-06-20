/**********************************************************************
mol.cpp - Unit tests for Open Babel OBMol class

Copyright (C) 2005-2006 Geoffrey R. Hutchison
Copyright (C) 2008 Tim Vandermeersch
 
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

#include <stdio.h>
#include <iostream>

using namespace std;
using namespace OpenBabel;

string cmlfile = "cmltest/cs2a.cml";
string cmlfile_multi = "files/3d.head.2.cml";

void cmlreadfile_test()
{
  cout << "# Unit tests for OBMol \n";

  cout << "ok 1\n"; // for loading tests

  OBConversion obconv_first;
  BOOST_REQUIRE_MESSAGE( obconv_first.SetInFormat("CML"), "Bail out! Fail format isn't loaded!" );

  // Test using ReadFile to read from CML
  OpenBabel::OBMol obmol_first;
  BOOST_CHECK( obconv_first.ReadFile(&obmol_first, cmlfile) );
  
  OBConversion obconv;
  BOOST_REQUIRE_MESSAGE( obconv.SetInFormat("CML"), "Bail out! Fail format isn't loaded!" );
  
  // Test using ReadFile to read from multimol CML
  OpenBabel::OBMol obmol;
  BOOST_CHECK( obconv.ReadFile(&obmol, cmlfile_multi) );

  // Test reading the second and final molecule using Read
  BOOST_CHECK( obconv.Read(&obmol) );
  BOOST_CHECK_EQUAL( obmol.NumAtoms(), 28 );
 
  // Test that there are no remaining molecules
  // (this test fails on Linux)
  // if (!obconv.Read(&obmol))
  //        cout << "ok 6!" << endl;
  // else
  //        cout << "not ok 6" << endl;

}
