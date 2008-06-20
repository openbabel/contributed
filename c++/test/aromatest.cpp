/**********************************************************************
aromatest.cpp - Test Open Babel aromaticity perception

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

#if !HAVE_STRNCASECMP
extern "C" int strncasecmp(const char *s1, const char *s2, size_t n);
#endif

#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace std;
using namespace OpenBabel;

void aromatic_test()
{
  cout << endl << "# Testing aromaticity perception...  " << endl;

  string filename = "files/aromatics.smi";
  
  ifstream ifs(filename.c_str());
  BOOST_REQUIRE_MESSAGE( ifs, "Bail out! Cannot read input file!" );
  
  OBConversion conv(&ifs, &cout);
  OBFormat* pFormat;
  
  pFormat = conv.FormatFromExt(filename.c_str());
  BOOST_REQUIRE_MESSAGE( pFormat != NULL, "Bail out! Cannot read file format!" );
  
  // Finally, we can do some work!
  OBMol mol;
  
  unsigned int testCount = 0;

  BOOST_REQUIRE_MESSAGE( conv.SetInAndOutFormats(pFormat, pFormat), "Bail out! File format isn't loaded");
  
  int molCount = 0;
  char message[BUFF_SIZE];
  while(ifs.peek() != EOF && ifs.good())
    {
      mol.Clear();
      conv.Read(&mol);
      molCount++;
      
      FOR_ATOMS_OF_MOL(atom, mol)
        {
          if (atom->IsHydrogen())
            continue;

          testCount++;
          
	  snprintf(message, BUFF_SIZE, "not ok %d # atom isn't aromatic!\n"
		      "# atom idx %d in molecule %d %s", 
		      testCount, atom->GetIdx(), molCount, mol.GetTitle());
          BOOST_CHECK_MESSAGE( atom->IsAromatic(), message );
        }

      FOR_BONDS_OF_MOL(bond, mol)
        {
          if (!bond->IsInRing())
            continue;
          
	  testCount++;
        
	  snprintf(message, BUFF_SIZE, "not ok %d # bond isn't aromatic!\n"
                                       "# begin atom %d, end atom %d, bond order %d in molecule %d %s", 
                                       testCount, bond->GetBeginAtomIdx(), bond->GetEndAtomIdx(),
				       bond->GetBondOrder(), molCount, mol.GetTitle());
          BOOST_CHECK_MESSAGE( bond->IsAromatic(), message );
        }
	
    } // while reading molecules
    
}
