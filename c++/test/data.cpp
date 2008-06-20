/**********************************************************************
data.cpp - Unit tests for Open Babel data classes

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
#include <openbabel/data.h>
#include <math.h>
#include <stdio.h>
#include <iostream>

using namespace std;
using namespace OpenBabel;

void data_test()
{
  cout << "# Unit tests for data tables \n";

  //double mass = etab.GetMass(2);
  BOOST_CHECK( fabs(etab.GetMass(2) - 4.0026 ) < 2e-3 );

}
