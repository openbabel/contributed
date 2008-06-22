/**********************************************************************
obmmff94validate.cpp - Validate the MMFF94 implementation

  This program assumes that the following files are in the current
  working directory:

    MMFF94_dative.mol2
    MMFF94_opti.log

  Both these files are from the MMFF94 validation suite wich can be
  found here: http://server.ccl.net/cca/data/MMFF94/

  For Each molecule in the .mol2 file the energy is calculated using
  the openbabel MMFF94 implementation. The energy values of indivudual 
  energy terms are also calculated (total bond energy, totral angle 
  energy, ...). These resulults are compared with values from the
  MMFF94_opti.log file. The difference is geven and the relative error.
  The atomtypes are also checked, when succesfull, PASSED will be printed.
  When an atom has a wrong type, XXXX FAILED XXXX will be printed next
  to its type.

Copyright (C) 2006 Tim Vandermeersch
 
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
#include <openbabel/base.h>
#include <openbabel/mol.h>
#include <openbabel/obconversion.h>
#include <openbabel/forcefield.h>
#include <unistd.h>

using namespace std;
using namespace OpenBabel;

void mmff94_validate()
{
  OBForceField* pFF = OBForceField::FindForceField("MMFF94");

  OBConversion conv;
  OBFormat *format_in = conv.FindFormat("mol2");
  vector<string> vs;
  vector<int> types;
  vector<double> fcharges, pcharges;
  vector<double> bond_lengths;
  char buffer[BUFF_SIZE], _logbuf[BUFF_SIZE];
  bool molfound, atomfound, bondfound, fchgfound, pchgfound;
  double etot, ebond, eangle, eoop, estbn, etor, evdw, eeq;
  double termcount; //1=bond, 2=angle, 3=strbnd, 4=torsion, 5=oop
  int n = 0;

  BOOST_REQUIRE_MESSAGE( format_in && conv.SetInFormat(format_in), "Could not set mol2 input format" );

  ifstream ifs, ifs2;
  ofstream ofs;

  ifs.open("MMFF94_dative.mol2");
  BOOST_REQUIRE_MESSAGE( ifs, "Could not open ./MMFF94_dative.mol2" );
 
  ifs2.open("MMFF94_opti.log");
  BOOST_REQUIRE_MESSAGE( ifs2, "Could not open ./MMFF94_opti.log" );
    
  ofs.open("MMFF94_openbabel.log");
  BOOST_REQUIRE_MESSAGE( ofs, "Could not open ./MMFF94_openbabel.log" );
    
  pFF->SetLogFile(&ofs);
  pFF->SetLogLevel(OBFF_LOGLVL_HIGH);
   
  OBMol mol;
  for (unsigned int c=1;; c++) {
    mol.Clear();
    types.clear();
    fcharges.clear();
    pcharges.clear();
    bond_lengths.clear();

    if (!conv.Read(&mol, &ifs))
      break;
    if (mol.Empty())
      break;
     
    BOOST_CHECK_MESSAGE( pFF->Setup(mol), "Could not setup calculations (missing parameters...)" );

    pFF->GetAtomTypes(mol);
    //pFF->GetFormalCharges(mol);
    pFF->GetPartialCharges(mol);

    termcount = 0;
    molfound = false;
    atomfound = false;
    bondfound = false;
    fchgfound = false;
    pchgfound = false;

    // Parse log file for types, charges, energies, ..
    while (ifs2.getline(buffer, 150)) {
      tokenize(vs, buffer);
      if (vs.size() == 0) {
        bondfound = false;
        continue;
      }
	
      string str(buffer);
      if (string::npos != str.find(mol.GetTitle(),0))
        molfound = true;

      // read atom types
      if (atomfound) {
        if (n) {
          types.push_back(atoi(vs[2].c_str()));
          types.push_back(atoi(vs[5].c_str()));
          types.push_back(atoi(vs[8].c_str()));
          types.push_back(atoi(vs[11].c_str()));
        } else {
          if (vs.size() > 2)
            types.push_back(atoi(vs[2].c_str()));
          if (vs.size() > 5)
            types.push_back(atoi(vs[5].c_str()));
          if (vs.size() > 8)
            types.push_back(atoi(vs[8].c_str()));
   
          atomfound = false;
        }
        n--;
      }
        
      // read formal charges
      if (fchgfound) {
        if (n) {
          fcharges.push_back(atof(vs[2].c_str()));
          fcharges.push_back(atof(vs[5].c_str()));
          fcharges.push_back(atof(vs[8].c_str()));
          fcharges.push_back(atof(vs[11].c_str()));
        } else {
          if (vs.size() > 2)
            fcharges.push_back(atof(vs[2].c_str()));
          if (vs.size() > 5)
            fcharges.push_back(atof(vs[5].c_str()));
          if (vs.size() > 8)
            fcharges.push_back(atof(vs[8].c_str()));
   
          fchgfound = false;
        }
        n--;
      }
 
      // read partial charges
      if (pchgfound) {
        if (n) {
          pcharges.push_back(atof(vs[2].c_str()));
          pcharges.push_back(atof(vs[5].c_str()));
          pcharges.push_back(atof(vs[8].c_str()));
          pcharges.push_back(atof(vs[11].c_str()));
        } else {
          if (vs.size() > 2)
            pcharges.push_back(atof(vs[2].c_str()));
          if (vs.size() > 5)
            pcharges.push_back(atof(vs[5].c_str()));
          if (vs.size() > 8)
            pcharges.push_back(atof(vs[8].c_str()));
   
          pchgfound = false;
        }
        n--;
      }
 
      // identify blocks
      if (molfound && EQn(buffer, " ATOM NAME  TYPE", 16)) {
        atomfound = true;
        n = mol.NumAtoms() / 4;
      }
      if (molfound && EQn(buffer, "   ATOM   FCHARGE", 17)) {
        fchgfound = true;
        n = mol.NumAtoms() / 4;
      }
      if (molfound && EQn(buffer, "   ATOM    CHARGE", 17)) {
        pchgfound = true;
        n = mol.NumAtoms() / 4;
      }

      if (bondfound)
        bond_lengths.push_back(atof(vs[7].c_str()));

      // Get the energies
      if (molfound) {
        if (EQn(buffer, " Total ENERGY", 13))
          etot = atof(vs[3].c_str());
        if (EQn(buffer, " Bond Stretching", 16))
          ebond = atof(vs[2].c_str());
        if (EQn(buffer, " Angle Bending", 14))
          eangle = atof(vs[2].c_str());
        if (EQn(buffer, " Out-of-Plane Bending", 21))
          eoop = atof(vs[2].c_str());
        if (EQn(buffer, " Stretch-Bend", 13))
          estbn = atof(vs[1].c_str());
        if (EQn(buffer, "     Total Torsion", 18))
          etor = atof(vs[2].c_str());
        if (EQn(buffer, "     Net vdW", 12))
          evdw = atof(vs[2].c_str());
        if (EQn(buffer, " Electrostatic", 14))
          eeq = atof(vs[1].c_str());
        if (EQn(buffer, " ---------------------", 22) && (termcount == 0)) {
          termcount++;
        bondfound = true;
        }
        if (EQn(buffer, " OPTIMOL>  # read next", 22))
          break;
      }
    } // while (getline)
      
    ostringstream os;
    vector<int>::iterator i;
    vector<double>::iterator di;
    unsigned int ni;
    bool failed;

    cout << "--------------------------------------------------------------------------------" << endl;
    cout << "                                                                                " << endl;
    cout << "  VALIDATE MOLECULE " << c << ": " << mol.GetTitle() << endl;
    cout << "                                                                                " << endl;
    cout << "IDX  HYB  AROM  OB_TYPE  LOG_TYPE       RESULT                                  " << endl;
    cout << "----------------------------------------------                                  " << endl;
 
    // 
    // validate atom types
    //
    ni = 1;
    failed = false;
    for (i = types.begin(); i != types.end();i++) {
      if (ni > mol.NumAtoms())
        continue;

      OBPairData *type = (OBPairData*) mol.GetAtom(ni)->GetData("FFAtomType");
      if (!type)
	continue;

      os.str("");
      os << "In molecule " << mol.GetTitle() << ": Wrong atom type for atom ";
      os << ni << " # found " << type->GetValue() << ", expected " << *i;
      BOOST_CHECK_MESSAGE( atoi(type->GetValue().c_str()) == (*i), os.str().c_str());
      
      if (atoi(type->GetValue().c_str()) == (*i))
        snprintf(_logbuf, BUFF_SIZE, "%2d   %3d  %4d    %3d      %3d          PASSED", 
                  mol.GetAtom(ni)->GetIdx(), mol.GetAtom(ni)->GetHyb(), 
                  mol.GetAtom(ni)->IsAromatic(), atoi(mol.GetAtom(ni)->GetType()), *i);
      else {
        snprintf(_logbuf, BUFF_SIZE, "%2d   %3d  %4d    %3d      %3d      XXX FAILED XXX", 
                  mol.GetAtom(ni)->GetIdx(), mol.GetAtom(ni)->GetHyb(), 
                  mol.GetAtom(ni)->IsAromatic(), atoi(type->GetValue().c_str()), *i);
        failed = true;
      }
      
      cout << _logbuf << endl;
        
      ni++;
    }

    /*
    cout << endl;
    cout << "IDX  OB_FCARGE  LOG_FCHARGE       RESULT" << endl;
    cout << "----------------------------------------" << endl;
            
    // 
    // validate formal charges
    //
    ni = 1;
    for (di = fcharges.begin(); di != fcharges.end(); di++) {
      if (ni > mol.NumAtoms())
        continue;
	
      if (fabs((*di) - mol.GetAtom(ni)->GetPartialCharge()) <= 0.001)
        snprintf(_logbuf, BUFF_SIZE, "%2d   %7.4f     %7.4f          PASSED", mol.GetAtom(ni)->GetIdx(), mol.GetAtom(ni)->GetPartialCharge(), *di);
      else {
        snprintf(_logbuf, BUFF_SIZE, "%2d   %7.4f     %7.4f      XXX FAILED XXX", mol.GetAtom(ni)->GetIdx(), mol.GetAtom(ni)->GetPartialCharge(), *di);
        failed = true;
      }
      
      cout << _logbuf << endl;
        
      ni++;
    }

    */

    cout << endl;
    cout << "IDX  OB_PCARGE  LOG_PCHARGE       RESULT" << endl;
    cout << "----------------------------------------" << endl;
      
    // 
    // validate partial charges
    //
    ni = 1;
    for (di = pcharges.begin(); di != pcharges.end(); di++) {
      if (ni > mol.NumAtoms())
        continue;
	
      OBPairData *chg = (OBPairData*) mol.GetAtom(ni)->GetData("FFPartialCharge");
      if (!chg)
	continue;

      os.str("");
      os << "In molecule " << mol.GetTitle() << ": Wrong partial charge for atom ";
      os << ni << " # found " << chg->GetValue() << ", expected " << *di;
      BOOST_CHECK_MESSAGE( fabs((*di) - atof(chg->GetValue().c_str())) <= 0.001, os.str().c_str());

      if (fabs((*di) - atof(chg->GetValue().c_str())) <= 0.001)
        snprintf(_logbuf, BUFF_SIZE, "%2d   %7.4f     %7.4f          PASSED", 
		  mol.GetAtom(ni)->GetIdx(), atof(chg->GetValue().c_str()), *di);
      else {
        snprintf(_logbuf, BUFF_SIZE, "%2d   %7.4f     %7.4f      XXX FAILED XXX",
                  mol.GetAtom(ni)->GetIdx(), atof(chg->GetValue().c_str()), *di);
        failed = true;
      }
      
      cout << _logbuf << endl;
        
      ni++;
    }

    double ene, delta;
    cout << endl;
    cout << "TERM                     OB ENERGY     LOG ENERGY         DELTA" << endl;
    cout << "---------------------------------------------------------------" << endl;
 
    // 
    // validate energies
    //
    
    // bond stretching
    ene = pFF->E_Bond();
    delta = (ene - ebond);
    
    os.str("");
    os << "In molecule " << mol.GetTitle() << ": Wrong bond stretching energy ";
    os << " # found " << ene << ", expected " << ebond;
    BOOST_CHECK_MESSAGE( delta < 0.005, os.str().c_str());

    snprintf(_logbuf, BUFF_SIZE, "Bond Stretching        %11.5f    %11.5f   %11.5f", ene, ebond, delta);
    cout << _logbuf << endl;
    
    // angle bending
    ene = pFF->E_Angle();
    delta = (ene - eangle);

    os.str("");
    os << "In molecule " << mol.GetTitle() << ": Wrong angle bending energy ";
    os << " # found " << ene << ", expected " << eangle;
    BOOST_CHECK_MESSAGE( delta < 0.005, os.str().c_str());

    snprintf(_logbuf, BUFF_SIZE, "Angle Bending          %11.5f    %11.5f   %11.5f", ene, eangle, delta);
    cout << _logbuf << endl;
    
    // stretch bending
    ene = pFF->E_StrBnd();
    delta = (ene - estbn);

    os.str("");
    os << "In molecule " << mol.GetTitle() << ": Wrong stretch bending energy ";
    os << " # found " << ene << ", expected " << estbn;
    BOOST_CHECK_MESSAGE( delta < 0.005, os.str().c_str());

    snprintf(_logbuf, BUFF_SIZE, "Stretch-Bending        %11.5f    %11.5f   %11.5f", ene, estbn, delta);
    cout << _logbuf << endl;
    
    // OOP
    ene = pFF->E_OOP();
    delta = (ene - eoop);

    os.str("");
    os << "In molecule " << mol.GetTitle() << ": Wrong out-of-plane bending energy ";
    os << " # found " << ene << ", expected " << eoop;
    BOOST_CHECK_MESSAGE( delta < 0.005, os.str().c_str());

    snprintf(_logbuf, BUFF_SIZE, "Out-Of-Plane Bending   %11.5f    %11.5f   %11.5f", ene, eoop, delta);
    cout << _logbuf << endl;
    
    // Torsional
    ene = pFF->E_Torsion();
    delta = (ene - etor);

    os.str("");
    os << "In molecule " << mol.GetTitle() << ": Wrong torsional energy ";
    os << " # found " << ene << ", expected " << etor;
    BOOST_CHECK_MESSAGE( delta < 0.005, os.str().c_str());
 
    snprintf(_logbuf, BUFF_SIZE, "Torsional              %11.5f    %11.5f   %11.5f", ene, etor, delta);
    cout << _logbuf << endl;
    
    // VDW
    ene = pFF->E_VDW();
    delta = (ene - evdw);

    os.str("");
    os << "In molecule " << mol.GetTitle() << ": Wrong van der waals energy ";
    os << " # found " << ene << ", expected " << evdw;
    BOOST_CHECK_MESSAGE( delta < 0.005, os.str().c_str());
 
    snprintf(_logbuf, BUFF_SIZE, "Van der Waals          %11.5f    %11.5f   %11.5f", ene, evdw, delta);
    cout << _logbuf << endl;
      
    // Electrostatic 
    ene = pFF->E_Electrostatic();
    delta = (ene - eeq);

    os << "In molecule " << mol.GetTitle() << ": Wrong electrostatic energy ";
    os << " # found " << ene << ", expected " << eeq;
    BOOST_CHECK_MESSAGE( delta < 0.005, os.str().c_str());
 
    snprintf(_logbuf, BUFF_SIZE, "Electrostatic          %11.5f    %11.5f   %11.5f", ene, eeq, delta);
    cout << _logbuf << endl;

    cout << endl;
    ene = pFF->Energy();
    delta = (ene - etot);

    os.str("");
    os << "In molecule " << mol.GetTitle() << ": Wrong total energy ";
    os << " # found " << ene << ", expected " << etot;
    BOOST_CHECK_MESSAGE( delta < 0.005, os.str().c_str());
 
    snprintf(_logbuf, BUFF_SIZE, "Total ENERGY           %11.5f    %11.5f   %11.5f", ene, etot, delta);
    cout << _logbuf << endl;

  } // for (unsigned int c;; c++ )
   
  if (ifs)
    ifs.close();
  if (ifs2)
    ifs2.close();
  if (ofs)
    ofs.close();
}
