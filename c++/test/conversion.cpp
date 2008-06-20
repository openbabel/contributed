/**********************************************************************
conversion.cpp - Unit tests for Open Babel OBConversion class

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

void conversion_test()
{
  cout << "# Unit tests for OBConversion \n";

  OBMol obMol;
  OBConversion obConversion;
  BOOST_CHECK( obConversion.SetInAndOutFormats("smi", "mdl") );

  BOOST_CHECK( obConversion.ReadString(&obMol, "C1=CC=CS1") );

  BOOST_CHECK_EQUAL( obMol.NumAtoms(), 5 );

  BOOST_CHECK( obMol.AddHydrogens() );
  BOOST_CHECK_EQUAL( obMol.NumAtoms(), 9 ); 

  BOOST_CHECK( (obConversion.WriteString(&obMol)).length() );

  // PR#1474265
  BOOST_CHECK( obConversion.WriteFile(&obMol, "test.mdl") );
  ifstream ifs("test.mdl");
  BOOST_CHECK( ifs.good() );

  // PR#143577
  BOOST_CHECK( obConversion.SetInFormat("mdl") );
  BOOST_CHECK( obConversion.ReadFile(&obMol, "test.mdl") );
  BOOST_CHECK( remove("test.mdl") != -1);
  
  // gzip input
  // gzip output

  // multi-molecule reading
  // PR#1465586
  // aromatics.smi
  // attype.00.smi

  //ReadFile()
  //Read()
  //WriteString()
  // GetOutputIndex()
  // IsLast

  //ReadString()
  //IsFirstInput
  //Read()

  // splitting
  
  // splitting using gzip-input
  // PR#1357705
  
  // size 0 input
  // PR#1250900
  
  // RegisterFormat
  // FindFormat
  // FormatFromExt
  // FormatFromMIME
  // GetNextFormat
  // GetDefaultFormat

  // BatchFileName
  // IncrementedFileName

  // option handling
  // AddOption
  // IsOption
  // RemoveOption
  // IsOption

  // SetOptions
  // IsOption

  // RegisterOptionParam
  // GetOptionParams

  // GetInStream
  // GetOutStream
  // SetInStream
  // SetOutStream

  // nasty tests
  BOOST_CHECK( !obConversion.ReadString(&obMol, "") );
  BOOST_CHECK( !obConversion.Read(&obMol) );
}


