/**********************************************************************
bond.cpp - Unit tests for Open Babel OBBond class

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

//#define BOOST_TEST_MODULE Bond
#include <boost/test/unit_test.hpp>

#include <openbabel/babelconfig.h>
#include <openbabel/mol.h>
#include <openbabel/obconversion.h>

#include <stdio.h>
#include <iostream>

using namespace std;
using namespace OpenBabel;

//BOOST_AUTO_TEST_CASE( bond_test )
void bond_test()
{
  // turn off slow sync with C-style output (we don't use it anyway).
  std::ios::sync_with_stdio(false);

  cout << "# Unit tests for OBBond \n";
  
  // OBBond isolation tests (no connection to residue, molecule...)

  OBAtom emptyAtom, begin1, end1;
  OBBond emptyBond, bond1;

  bond1.SetBegin(&begin1);
  bond1.SetEnd(&end1);
}
