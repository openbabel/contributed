/**********************************************************************
 inchiwrite.cpp - tests for writing InChIs

 This file is part of the Open Babel project.
 For more information, see <http://openbabel.sourceforge.net/>

 Copyright (C) 2007 by Chris Morley

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


#include <iostream>
#include <fstream>
#include <sstream>
#include <openbabel/babelconfig.h>
#include <openbabel/mol.h>
#include <openbabel/obconversion.h>

using namespace std;

namespace OpenBabel{
  extern string GetInChI(istream& is);
}

using namespace OpenBabel;

void inchiWriteTest(string &file, string &resfile)
{
  ostringstream os;
  
  ifstream ifs(file.c_str());
  os << "# Cannot open " << file;
  BOOST_REQUIRE_MESSAGE( ifs, os.str().c_str() );
  
  ifstream results(resfile.c_str());
  os.str("");
  os << "# Cannot open " << resfile;
  BOOST_REQUIRE_MESSAGE( results, os.str().c_str() );

  stringstream ssout;
  OBConversion conv(&ifs, &ssout);
  OBFormat* pFormat = conv.FormatFromExt(resfile.c_str());
  BOOST_REQUIRE_MESSAGE( pFormat, "# Skipped. Did not recognize the format" );
  
  conv.SetInFormat(pFormat);
  BOOST_REQUIRE_MESSAGE( conv.SetOutFormat("inchi"), "# Skipped. Did not find InChI Format" );

  conv.AddOption("w",OBConversion::OUTOPTIONS); //suppress routine warnings
  int count = conv.Convert();
  
  BOOST_REQUIRE_MESSAGE( count > 0, "# Skipped. Did not convert any molecules" );

  //Compare ssout with the correct InChIs
  int nfail=0;
  int n, skipped=0;
  for(n=1;ssout;++n)
  {
    string correct = GetInChI(results);
    string generated;
    ssout >> generated;
    if(correct.empty() || generated.empty())
      break;
    if(correct.size()<9)//skip molecules with an empty InChI
    {
      skipped++;
      cout << "ok " << n << " # ignored empty InChI\n";
      continue;
    }

    os.str("");
    os << "Not ok " << n << " # Mismatch in molecule #" 
       << n << ". generated / correct\n"
       << "# " << generated << "\n# " << correct << '\n';
 
    BOOST_CHECK_MESSAGE( generated == correct, os.str().c_str());
  }

}

void inchi_write_test()
{
#ifdef TESTDATADIR
  string inchidir = TESTDATADIR + "/../inchi";
  string inchi_file1 = inchidir + "Samples.sdf";
  string inchi_results1 = inchidir + "Samples.txt";
  string inchi_file2 = inchidir + "SamplesTechMan.sdf";
  string inchi_results2 = inchidir + "SamplesTechMan.txt";
#else
   string inchi_file1 = "inchi/Samples.sdf";
   string inchi_results1 = "inchi/Samples.txt";
   string inchi_file2 = "inchi/SamplesTechMan.sdf";
   string inchi_results2 = "inchi/SamplesTechMan.txt";
#endif

  inchiWriteTest(inchi_file1, inchi_results1);
  inchiWriteTest(inchi_file2, inchi_results2);
}



