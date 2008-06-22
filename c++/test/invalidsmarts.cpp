/**********************************************************************
invalid-smarts.cpp - Test SMARTS pattern parsing (rejecting invalid patterns)

This file is part of the Open Babel project.
For more information, see <http://openbabel.sourceforge.net/>

Some portions Copyright (C) 2005-2006 Geoffrey R. Hutchison
Some portions Copyright (C) 2008 Tim Vandermeersch
 
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
#include <fstream>

using namespace std;
using namespace OpenBabel;

void invalid_smarts_test()
{
  ostringstream os;

#ifdef TESTDATADIR
  string testdatadir = TESTDATADIR;
  string invalid_file = testdatadir + "invalid-smarts.txt";
  string random1_file = testdatadir + "random";
  string random2_file = testdatadir + "random2";
  string random3_file = testdatadir + "random3";
#else
  string invalid_file = "files/invalid-smarts.txt";
  string random1_file = "files/random";
  string random2_file = "files/random2";
  string random3_file = "files/random3";
#endif

  cout << "# Testing invalid SMARTS parsing..." << endl;

  // make sure to kill off all error reporting
  obErrorLog.StopLogging();

  unsigned int currentTest = 0;
  OBSmartsPattern smarts;
  string pattern, buffer;

  std::ifstream mifs;
  os << "Bail out! Cannot read test data " << invalid_file.c_str();
  BOOST_REQUIRE_MESSAGE( SafeOpen(mifs, invalid_file.c_str()), os.str().c_str() );

  while (getline(mifs, pattern))
    {
      // each line is a potential test pattern
      currentTest++;
      os.str("");
      os << "test " << currentTest << " # invalid SMARTS was accepted but should have been rejected";
      BOOST_CHECK_MESSAGE(!smarts.Init(pattern) , os.str().c_str() );
    }

  mifs.close();
  mifs.clear();

  // random file#1
  os.str("");
  os << "Bail out! Cannot read test data " << random1_file.c_str();
  BOOST_REQUIRE_MESSAGE( SafeOpen(mifs, random1_file.c_str()), os.str().c_str() );

  pattern.clear();
  while (getline(mifs, buffer))
    pattern += buffer;

  os.str("");
  os << "random data #1 was parsed but should have been rejected # read " << pattern.size();
  BOOST_CHECK_MESSAGE(!smarts.Init(pattern) , os.str().c_str() );
 
  mifs.close();
  mifs.clear();

  // random2
  os << "Bail out! Cannot read test data " << random2_file.c_str();
  BOOST_REQUIRE_MESSAGE( SafeOpen(mifs, random2_file.c_str()), os.str().c_str() );

  pattern.clear();
  while (getline(mifs, buffer))
    pattern += buffer;

  os.str("");
  os << "random data #2 was parsed but should have been rejected # read " << pattern.size();
  BOOST_CHECK_MESSAGE(!smarts.Init(pattern) , os.str().c_str() );
 
  mifs.close();
  mifs.clear();

  // random3
  os << "Bail out! Cannot read test data " << random3_file.c_str();
  BOOST_REQUIRE_MESSAGE( SafeOpen(mifs, random3_file.c_str()), os.str().c_str() );

  pattern.clear();
  while (getline(mifs, buffer))
    pattern += buffer;

  os.str("");
  os << "random data #3 was parsed but should have been rejected # read " << pattern.size();
  BOOST_CHECK_MESSAGE(!smarts.Init(pattern) , os.str().c_str() );

}
