/**********************************************************************
phmodel.cpp - Unit tests for Open Babel OBPhModel class

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

#include <openbabel/mol.h>
#include <openbabel/obconversion.h>
#include <openbabel/forcefield.h>
#include <openbabel/phmodel.h>

using namespace std;
using namespace OpenBabel;

void phmodel_test()
{
  OBMol mol;
  OBConversion conv;
  conv.SetInFormat("smi");
  conv.SetOutFormat("smi");

  unsigned int test = 0;

  // amine acid COOH pKa = 4.0 (carboxylic acid entry in phmodel.txt)
  // amino acid NH3+ pKa = 10.0 (amine entry in phmodel.txt)

  // 
  // Aspartic acid (sidechain COOH pKa = 3.8)
  //
  conv.ReadString(&mol, "NC(CC(O)=O)C(O)=O");
  mol.SetAutomaticFormalCharge(true);
  mol.AddHydrogens(false, true, 1.0); // NH3+ COOH COOH
  //conv.Write(&mol, &cout);
  
  BOOST_CHECK_MESSAGE( mol.NumAtoms() == 17, "Aspartic acid pH 1.0" );

  conv.ReadString(&mol, "NC(CC(O)=O)C(O)=O");
  mol.SetAutomaticFormalCharge(true);
  mol.AddHydrogens(false, true, 3.9); // NH3+ COOH COO-
  //conv.Write(&mol, &cout);
 
  BOOST_CHECK_MESSAGE( mol.NumAtoms() == 16, "Aspartic acid pH 3.9" );

  conv.ReadString(&mol, "NC(CC(O)=O)C(O)=O");
  mol.SetAutomaticFormalCharge(true);
  mol.AddHydrogens(false, true, 7.4); // NH3+ COO- COO-
  //conv.Write(&mol, &cout);
 
  BOOST_CHECK_MESSAGE( mol.NumAtoms() == 15, "Aspartic acid pH 7.4" );

  conv.ReadString(&mol, "NC(CC(O)=O)C(O)=O");
  mol.SetAutomaticFormalCharge(true);
  mol.AddHydrogens(false, true, 13.0); // NH2 COO- COO-
  //conv.Write(&mol, &cout);

  BOOST_CHECK_MESSAGE( mol.NumAtoms() == 14, "Aspartic acid pH 13.0" );

  // 
  // Glutamic acid (sidechain COOH pKa = 4.3)
  //
  conv.ReadString(&mol, "NC(CCC(O)=O)C(O)=O");
  mol.SetAutomaticFormalCharge(true);
  mol.AddHydrogens(false, true, 1.0); // NH3+ COOH COOH
  //conv.Write(&mol, &cout);
  
  BOOST_CHECK_MESSAGE( mol.NumAtoms() == 20, "Glutamic acid pH 1.0" );

  conv.ReadString(&mol, "NC(CCC(O)=O)C(O)=O");
  mol.SetAutomaticFormalCharge(true);
  mol.AddHydrogens(false, true, 4.15); // NH3+ COOH COO-
  //conv.Write(&mol, &cout);
 
  // known bug
  BOOST_CHECK_MESSAGE( mol.NumAtoms() == 19, "Glutamic acid pH 4.15" );

  conv.ReadString(&mol, "NC(CCC(O)=O)C(O)=O");
  mol.SetAutomaticFormalCharge(true);
  mol.AddHydrogens(false, true, 7.4); // NH3+ COO- COO-
  //conv.Write(&mol, &cout);
 
  BOOST_CHECK_MESSAGE( mol.NumAtoms() == 18, "Glutamic acid pH 7.4" );

  conv.ReadString(&mol, "NC(CCC(O)=O)C(O)=O");
  mol.SetAutomaticFormalCharge(true);
  mol.AddHydrogens(false, true, 13.0); // NH2 COO- COO-
  //conv.Write(&mol, &cout);

  BOOST_CHECK_MESSAGE( mol.NumAtoms() == 17, "Glutamic acid pH 13.0" );

  // 
  // Histidine (sidechain nH+ pKa = 6.08)
  //
  conv.ReadString(&mol, "NC(Cc1ncnc1)C(O)=O");
  mol.SetAutomaticFormalCharge(true);
  mol.AddHydrogens(false, true, 1.0); // NH3+ COOH nH+
  //conv.Write(&mol, &cout);
  
  BOOST_CHECK_MESSAGE( mol.NumAtoms() == 22, "Histidine pH 1.0" );
  
  conv.ReadString(&mol, "NC(Cc1ncnc1)C(O)=O");
  mol.SetAutomaticFormalCharge(true);
  mol.AddHydrogens(false, true, 5.0); // NH3+ COO- nH+
  //conv.Write(&mol, &cout);
 
  BOOST_CHECK_MESSAGE( mol.NumAtoms() == 21, "Histidine pH 5.0" );

  conv.ReadString(&mol, "NC(Cc1ncnc1)C(O)=O");
  mol.SetAutomaticFormalCharge(true);
  mol.AddHydrogens(false, true, 7.4); // NH3+ COO- n:
  //conv.Write(&mol, &cout);
 
  BOOST_CHECK_MESSAGE( mol.NumAtoms() == 20, "Histidine pH 7.4" );

  conv.ReadString(&mol, "NC(Cc1ncnc1)C(O)=O");
  mol.SetAutomaticFormalCharge(true);
  mol.AddHydrogens(false, true, 13.0); // NH2 COO- n:
  //conv.Write(&mol, &cout);

  BOOST_CHECK_MESSAGE( mol.NumAtoms() == 19, "Histidine pH 13.0" );

  // 
  // Lysine (sidechain NH3+ pKa = 8.28)
  //
  conv.ReadString(&mol, "NC(CCCCN)C(O)=O");
  mol.SetAutomaticFormalCharge(true);
  mol.AddHydrogens(false, true, 1.0); // NH3+ COOH NH3+
  //conv.Write(&mol, &cout);
  
  BOOST_CHECK_MESSAGE( mol.NumAtoms() == 26, "Lysine pH 1.0" );

  conv.ReadString(&mol, "NC(CCCCN)C(O)=O");
  mol.SetAutomaticFormalCharge(true);
  mol.AddHydrogens(false, true, 7.4); // NH3+ COO- NH3+
  //conv.Write(&mol, &cout);
 
  BOOST_CHECK_MESSAGE( mol.NumAtoms() == 25, "Lysine pH 7.4" );

  conv.ReadString(&mol, "NC(CCCCN)C(O)=O");
  mol.SetAutomaticFormalCharge(true);
  mol.AddHydrogens(false, true, 9.0); // NH3+ COO- NH2
  //conv.Write(&mol, &cout);
  
  // known bug
  BOOST_CHECK_MESSAGE( mol.NumAtoms() == 24, "Lysine pH 9.0" );

  conv.ReadString(&mol, "NC(CCCCN)C(O)=O");
  mol.SetAutomaticFormalCharge(true);
  mol.AddHydrogens(false, true, 13.0); // NH2 COO- NH2
  //conv.Write(&mol, &cout);

  BOOST_CHECK_MESSAGE( mol.NumAtoms() == 23, "Lysine pH 13.0" );

  // 
  // Tyrosine (sidechain OH pKa = 10.1)
  //
  conv.ReadString(&mol, "NC(Cc1ccc(O)cc1)C(O)=O");
  mol.SetAutomaticFormalCharge(true);
  mol.AddHydrogens(false, true, 1.0); // NH3+ COOH NH3+
  //conv.Write(&mol, &cout);
  
  BOOST_CHECK_MESSAGE( mol.NumAtoms() == 25, "Tyrosine pH 1.0" );

  conv.ReadString(&mol, "NC(Cc1ccc(O)cc1)C(O)=O");
  mol.SetAutomaticFormalCharge(true);
  mol.AddHydrogens(false, true, 7.4); // NH3+ COO- NH3+
  //conv.Write(&mol, &cout);
 
  BOOST_CHECK_MESSAGE( mol.NumAtoms() == 24, "Tyrosine pH 7.4" );

  conv.ReadString(&mol, "NC(Cc1ccc(O)cc1)C(O)=O");
  mol.SetAutomaticFormalCharge(true);
  mol.AddHydrogens(false, true, 10.05); // NH3+ COO- NH2
  //conv.Write(&mol, &cout);
 
  BOOST_CHECK_MESSAGE( mol.NumAtoms() == 23, "Tyrosine pH 10.05" );

  conv.ReadString(&mol, "NC(Cc1ccc(O)cc1)C(O)=O");
  mol.SetAutomaticFormalCharge(true);
  mol.AddHydrogens(false, true, 13.0); // NH2 COO- NH2
  //conv.Write(&mol, &cout);

  BOOST_CHECK_MESSAGE( mol.NumAtoms() == 22, "Tyrosine pH 13.0" );

  // 
  // Arginine (sidechain =NH2+ pKa = 12.0)
  //
  conv.ReadString(&mol, "NC(CCCNC(N)=N)C(O)=O");
  mol.SetAutomaticFormalCharge(true);
  mol.AddHydrogens(false, true, 1.0); // NH3+ COOH =NH2+
  //conv.Write(&mol, &cout);
  
  BOOST_CHECK_MESSAGE( mol.NumAtoms() == 28, "Arginine pH 1.0" );

  conv.ReadString(&mol, "NC(CCCNC(N)=N)C(O)=O");
  mol.SetAutomaticFormalCharge(true);
  mol.AddHydrogens(false, true, 7.4); // NH3+ COO- NH3+
  //conv.Write(&mol, &cout);
 
  BOOST_CHECK_MESSAGE( mol.NumAtoms() == 27, "Arginine pH 7.4" );

  conv.ReadString(&mol, "NC(CCCNC(N)=N)C(O)=O");
  mol.SetAutomaticFormalCharge(true);
  mol.AddHydrogens(false, true, 11.0); // NH3+ COO- NH2
  //conv.Write(&mol, &cout);
 
  BOOST_CHECK_MESSAGE( mol.NumAtoms() == 26, "Arginine pH 11.0" );

  conv.ReadString(&mol, "NC(CCCNC(N)=N)C(O)=O");
  mol.SetAutomaticFormalCharge(true);
  mol.AddHydrogens(false, true, 13.0); // NH2 COO- NH2
  //conv.Write(&mol, &cout);

  BOOST_CHECK_MESSAGE( mol.NumAtoms() == 25, "Arginine pH 13.0" );

  // 
  // Gly-Gly (terminal NH3+, COOH and the amide bond)
  //
  conv.ReadString(&mol, "NCC(=O)NCC(=O)O");
  mol.SetAutomaticFormalCharge(true);
  mol.AddHydrogens(false, true, 1.0); // NH3+ COOH =NH2+
  //conv.Write(&mol, &cout);
  
  BOOST_CHECK_MESSAGE( mol.NumAtoms() == 18, "Gly-Gly pH 1.0" );

  conv.ReadString(&mol, "NCC(=O)NCC(=O)O");
  mol.SetAutomaticFormalCharge(true);
  mol.AddHydrogens(false, true, 7.4); // NH3+ COO- NH3+
  //conv.Write(&mol, &cout);
 
  BOOST_CHECK_MESSAGE( mol.NumAtoms() == 17, "Gly-Gly pH 7.4" );

  conv.ReadString(&mol, "NCC(=O)NCC(=O)O");
  mol.SetAutomaticFormalCharge(true);
  mol.AddHydrogens(false, true, 13.0); // NH2 COO- NH2
  //conv.Write(&mol, &cout);

  BOOST_CHECK_MESSAGE( mol.NumAtoms() == 16, "Gly-Gly pH 13.0" );

}
