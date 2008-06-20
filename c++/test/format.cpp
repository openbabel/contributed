/**********************************************************************
format.cpp - Unit tests for Open Babel OBFormat class

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

void format_test()
{
  cout << "# Unit tests for OBFormat \n";

  OBConversion obConversion;
  BOOST_CHECK( obConversion.SetInAndOutFormats("smi", "mdl") );

  OBFormat *inFormat = obConversion.GetInFormat();
  BOOST_CHECK( inFormat );

  OBFormat *outFormat = obConversion.GetOutFormat();
  BOOST_CHECK( outFormat );

}

