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

//#define BOOST_TEST_MODULE Mol
#include <boost/test/unit_test.hpp>

#include <openbabel/babelconfig.h>
#include <openbabel/mol.h>
#include <openbabel/obconversion.h>

#include <stdio.h>
#include <iostream>

using namespace std;
using namespace OpenBabel;

#ifdef TESTDATADIR
  string testdatadir = TESTDATADIR;
  string d2file = testdatadir + "test2d.xyz";
  string d3file = testdatadir + "test3d.xyz";
#else
  string d2file = "files/test2d.xyz";
  string d3file = "files/test3d.xyz";
#endif

//BOOST_AUTO_TEST_CASE( mol_test )
void mol_test()
{
  // turn off slow sync with C-style output (we don't use it anyway).
  std::ios::sync_with_stdio(false);

  cout << "# Unit tests for OBMol \n";

  OBMol emptyMol, testMol1;

  testMol1.ReserveAtoms(-1);
  testMol1.ReserveAtoms(0);
  testMol1.ReserveAtoms(2);

  // atom component tests

  BOOST_CHECK_EQUAL( testMol1.NumAtoms(), 0 );

  testMol1.NewAtom();
  BOOST_CHECK_EQUAL( testMol1.NumAtoms(), 1 );

  testMol1.NewAtom();
  testMol1.AddBond(1, 2, 1);
  BOOST_CHECK_EQUAL( testMol1.NumBonds(), 1 );

  testMol1.Clear();
  BOOST_CHECK_EQUAL( testMol1.NumAtoms(), 0 );

  ifstream ifs1(d3file.c_str());
  BOOST_REQUIRE_MESSAGE( ifs1, "Bail out! Cannot read input file!" );
  
  OBConversion conv(&ifs1, &cout);
  OBFormat* pFormat;
  
  pFormat = conv.FindFormat("XYZ");
  BOOST_REQUIRE_MESSAGE( pFormat != NULL , "Bail out! Cannot read file format!" );
  
  BOOST_REQUIRE_MESSAGE( conv.SetInAndOutFormats(pFormat, pFormat), "Bail out! File format isn't loaded" );

  OBMol testMol2D, testMol3D;
  BOOST_CHECK( conv.Read(&testMol3D) );
  testMol3D.Center();
  
  // test bond insertion (PR#1665649)
  OBMol doubleBondMol;
  OBAtom *a1, *a2, *a3;
  OBBond *b;
  doubleBondMol.BeginModify();
  a1 = doubleBondMol.NewAtom();
  a1->SetVector(0.0, 0.0, 0.0);
  a1->SetAtomicNum(6);
  a2 = doubleBondMol.NewAtom();
  a2->SetVector(1.6, 0.0, 0.0);
  a2->SetAtomicNum(6);
  b = doubleBondMol.NewBond();
  b->SetBegin(a1);
  b->SetEnd(a2);
  a1->AddBond(b);
  a2->AddBond(b);
  doubleBondMol.EndModify();

  // test AddHydrogens
  OBMol testMolH;
  testMolH.BeginModify();
  OBAtom *testAtom = testMolH.NewAtom();
  testAtom->SetVector(0.5f, 0.5f, 0.5f);
  testAtom->SetAtomicNum(6);
  testMolH.EndModify();
  testMolH.AddHydrogens();
  BOOST_CHECK_EQUAL( testMolH.NumAtoms(), 5 );

  // test AddHydrogens (pr #1665519)
  OBMol testMolH2;
  OBAtom *testAtom2 = testMolH2.NewAtom();
  testAtom2->SetVector(0.5f, 0.5f, 0.5f);
  testAtom2->SetAtomicNum(6);
  testMolH2.AddHydrogens();
  BOOST_CHECK_EQUAL( testMolH2.NumAtoms(), 5 );
  
}
