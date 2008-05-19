/**********************************************************************
dock.cpp - Docking Class.

Copyright (C) 2007 by Tim Vandermeersch <tim.vandermeersch@gmail.com>
 
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
#include <openbabel/babelconfig.h>

#include <openbabel/dock.h>

#include <openbabel/mol.h>
#include <openbabel/atom.h>
#include <openbabel/bond.h>
#include <openbabel/obiter.h>
#include <openbabel/math/matrix3x3.h>
#include <openbabel/rotamer.h>
#include <openbabel/rotor.h>
#include <openbabel/obconversion.h>
#include <openbabel/builder.h>

using namespace std;

namespace OpenBabel
{
  /** \class OBDock dock.h <openbabel/dock.h>
      \brief Docking Class

      The OBDock class is used for docking.

      Below is and example which explain the basics. 
      
      \code
      OBDock dock;
      dock.SpherePocket(mol, );
      \endcode
  **/


  OBDock::OBDock() 
  {
  }
 
  OBDock::~OBDock() 
  {
  }
  
  //////////////////////////////////////////////////////////////////////////
  //
  //  Setup
  //
  //////////////////////////////////////////////////////////////////////////
 
  int OBDock::CreateAtomPocketFromLigand(double radius)
  {
    bool deleteAtom = true;
    
    while (deleteAtom) {
      FOR_ATOMS_OF_MOL (pck_atom, _protein) {
        deleteAtom = true;
        FOR_ATOMS_OF_MOL (lig_atom, _ligand) {
	  if (pck_atom->GetDistance(&*lig_atom) <= radius)
	    deleteAtom = false;
        }
        
	if (deleteAtom) {
          _protein.DeleteAtom(&*pck_atom);
          break;
	}
      }
    }

    // remove all H, C, N and O atoms with a valence of 0
    deleteAtom = true;
    while (deleteAtom) {
      FOR_ATOMS_OF_MOL (atom, _protein) {
        if (atom->GetValence() == 0) {
          if (atom->GetAtomicNum() == 1) // H
            deleteAtom = true;
          else if (atom->GetAtomicNum() == 6) // C
            deleteAtom = true;
          else if (atom->GetAtomicNum() == 7) // N
            deleteAtom = true;
          else if (atom->GetAtomicNum() == 8) // O
            deleteAtom = true;
        } else
          deleteAtom = false;
        
	if (deleteAtom) {
          _protein.DeleteAtom(&*atom);
          break;
	}
      }
    }

    return _protein.NumAtoms();
  }
  
  int OBDock::CreateResiduePocketFromLigand(double radius)
  {
    bool deleteResidue = true;
    
    FOR_RESIDUES_OF_MOL (residue, _protein) {
      deleteResidue = true;
      FOR_ATOMS_OF_RESIDUE (pck_atom, &*residue) {
        FOR_ATOMS_OF_MOL (lig_atom, _ligand) {
          if (pck_atom->GetDistance(&*lig_atom) <= radius)
            deleteResidue = false;
        }
      }

      if (deleteResidue) {
        vector<OBAtom*> vatoms = residue->GetAtoms();
        vector<OBAtom*>::iterator i;
	
	for (i = vatoms.begin(); i != vatoms.end(); ++i)
	  _protein.DeleteAtom(*i);
      }
    }
	
    return _protein.NumAtoms();
  }
  
  int OBDock::CreateAtomPocketFromPoint(double radius, vector3 &point)
  {
    OBAtom *dummy_atom = new OBAtom;
    dummy_atom->SetVector(point);
    bool deleteAtom = true;
    
    while (deleteAtom) {
      FOR_ATOMS_OF_MOL (pck_atom, _protein) {
        if (pck_atom->GetDistance(dummy_atom) <= radius)
	  deleteAtom = false;
        else 
	  deleteAtom = true;
        
	if (deleteAtom) {
          _protein.DeleteAtom(&*pck_atom);
          break;
	}
      }
    }

    // remove all H, C, N and O atoms with a valence of 0
    deleteAtom = true;
    while (deleteAtom) {
      FOR_ATOMS_OF_MOL (atom, _protein) {
        if (atom->GetValence() == 0) {
          if (atom->GetAtomicNum() == 1) // H
            deleteAtom = true;
          else if (atom->GetAtomicNum() == 6) // C
            deleteAtom = true;
          else if (atom->GetAtomicNum() == 7) // N
            deleteAtom = true;
          else if (atom->GetAtomicNum() == 8) // O
            deleteAtom = true;
        } else
          deleteAtom = false;
        
	if (deleteAtom) {
          _protein.DeleteAtom(&*atom);
          break;
	}
      }
    }

    return _protein.NumAtoms();
  
  }
  
  int OBDock::CreateResiduePocketFromPoint(double radius, vector3 &point)
  {
    OBAtom *dummy_atom = new OBAtom;
    dummy_atom->SetVector(point);
    bool deleteResidue = true;
    
    FOR_RESIDUES_OF_MOL (residue, _protein) {
      deleteResidue = true;
      FOR_ATOMS_OF_RESIDUE (pck_atom, &*residue) {
        if (pck_atom->GetDistance(dummy_atom) <= radius)
          deleteResidue = false;
      }

      if (deleteResidue) {
        vector<OBAtom*> vatoms = residue->GetAtoms();
        vector<OBAtom*>::iterator i;
	
	for (i = vatoms.begin(); i != vatoms.end(); ++i)
	  _protein.DeleteAtom(*i);
      }
    }
	
    return _protein.NumAtoms();
  }
 
  //////////////////////////////////////////////////////////////////////////
  //
  //  Grid docking
  //
  //////////////////////////////////////////////////////////////////////////
 
  bool OBDock::GridDockInitialize(int scoring, double translate_step, double rotate_step)
  {
    _scoring = scoring;
    _translate_step = translate_step;
    _rotate_step = rotate_step;
    
    if (!_ligand.NumAtoms() || !_protein.NumAtoms())
      return false;

    _ligand.SetConformer(0);
    _ligand_dimentions = GetDimentions(_ligand);
    _pocket_dimentions = GetDimentions(_protein);

    cout << "_ligand_dimention = " << _ligand_dimentions << endl;
    cout << "_pocket_dimention = " << _pocket_dimentions << endl;

    vector3 vdiff = _pocket_dimentions - _ligand_dimentions;
    if ((vdiff.x() <= 0.0) || (vdiff.y() <= 0.0) || (vdiff.z() <= 0.0))
      return false;

    return true;
  }
  
  bool OBDock::GridDockNextPose()
  {
    return true;
  }
  
  //////////////////////////////////////////////////////////////////////////
  //
  //  ChemScore
  //
  //  reference:
  //    Empirical scoring functions: I. The development of a
  //    fast empirical scoring function to estimate the
  //    binding affinity of ligands in receptor complexes
  //
  //    Journal of Computer-Aided Molecular Design, 11 (1997) 425–445.
  //
  //////////////////////////////////////////////////////////////////////////
  
  double OBDock::ChemScore()
  {
    double score;

    score = -5.48;
    score += -3.34 * ChemScoreHBondTerm();
    score += -6.03 * ChemScoreMetalTerm();
    score += -0.117 * ChemScoreLipophilicTerm();
    score += 2.56 * ChemScoreRotationTerm();

    return score;
  }
  
  int OBDock::ChemScoreAtomType(OBAtom *atom)
  {
    // Cl, Br, I, S, C, N, O, H, P, F
    switch (atom->GetAtomicNum()) {
      case 1: // H
      case 6: // C
      case 7: // N
      case 8: // O
      case 9: // F
      case 15: // P
      case 16: // S
      case 17: // Cl
      case 35: // Br
      case 53: // I
        break;
    }
  }

  // f(x)
  //      |
  //    1 +-------------+.
  //      |               \.
  //      |                 \.
  //      |                   \
  //    0 +-------------+------+------->
  //                 x1     x2
  //
  //           { 1                              if x <= x1 
  // f1( x ) = { 1 - (x - x1) / (x2 - x1)       if x1 < x <= x2
  //           { 0                              if x > x2
  //
  double OBDock::ChemScoreBlockFunction(double x, double x1, double x2)
  {
    if (x <= x1)
      return 1.0;

    if (x > x2)
      return 0.0;

    double score = 1.0 - ((x - x1) / (x2 - x1));

    return score;
  }
  
  // 
  //                           ----
  // /\G_hbond = coefficient * \     f1( /\r, 0.25, 0.65 ) * g2( /\angle, 30, 80 )
  //                           /
  //                           ---- iI
  //
  //             { 1                              if /\r <= 0.25 A
  // g1( /\r ) = { 1 - (/\r - 0.25) / 0.4         if 0.25 A < /\r <= 0.65 A
  //             { 0                              if /\r > 0.65 A
  //
  //                 { 1                          if /\alpha <= 30°
  // g2( /\angle ) = { 1 - (/\angle - 30) / 50    if 30° < /\alpha <= 80°
  //                 { 0                          if /\alpha > 80°
  // 
  // coefficient = -3.34 kJ/mol
  // /\r = delta_r = deviation of the H...O/N hydrogen bond from 1.85 A
  // /\angle = delta_angle = deviation of the O/N-H...O/N hydrogen bond 
  //           angle from 180°
  //
  // The sum is taken over all hydrogen bonds between ligand atoms i, and 
  // protein atoms I.
  //
  double OBDock::ChemScoreHBondTerm()
  {
    double delta_r, delta_alpha, score, tscore = 0.0;
    
    FOR_ATOMS_OF_MOL (pck_atom, _protein) {
      FOR_ATOMS_OF_MOL (lig_atom, _ligand) {
        if (pck_atom->IsHbondDonorH() && lig_atom->IsHbondAcceptor()) {
	    delta_alpha = 0.0; // default, if no neighbours on H
	    delta_r = fabs(1.85 - pck_atom->GetDistance(&*lig_atom));
            
	    score = ChemScoreBlockFunction(delta_r, 0.25, 0.65);
	    
	    if (score != 0.0) {
	     FOR_NBORS_OF_ATOM (nbr, &*pck_atom)
               delta_alpha = fabs(180.0 - nbr->GetAngle(&*pck_atom, &*lig_atom));
	     score *= ChemScoreBlockFunction(delta_alpha, 30.0, 80.0);
	     tscore += score;
	    }
        } else if (lig_atom->IsHbondDonorH() && pck_atom->IsHbondAcceptor()) {
            delta_alpha = 0.0; // default, if no neighbours on H
	    delta_r = fabs(1.85 - pck_atom->GetDistance(&*lig_atom));
            
	    score = ChemScoreBlockFunction(delta_r, 0.25, 0.65);
	    
	    if (score != 0.0) {
	     FOR_NBORS_OF_ATOM (nbr, &*lig_atom)
               delta_alpha = fabs(180.0 - nbr->GetAngle(&*lig_atom, &*pck_atom));
	     score *= ChemScoreBlockFunction(delta_alpha, 30.0, 80.0);
	     tscore += score;
	    }
        }
      }
    }
 
    return tscore;
  }
  
  double OBDock::ChemScoreMetalTerm()
  {
  }

  double OBDock::ChemScoreLipophilicTerm()
  {
  }

  double OBDock::ChemScoreRotationTerm()
  {
  }

  //////////////////////////////////////////////////////////////////////////
  //
  //  Get informeration on poses
  //
  //////////////////////////////////////////////////////////////////////////
  
  int OBDock::NumPoses()
  {
  }
  
  //////////////////////////////////////////////////////////////////////////
  //
  //  Private method for internal use
  //
  //////////////////////////////////////////////////////////////////////////
 
  vector3 OBDock::GetMinimum(OBMol &mol)
  {
    vector3 min = vector3(0.0, 0.0, 0.0);
    
    FOR_ATOMS_OF_MOL (atom, mol) { 
      vector3 pos = atom->GetVector();
      if (pos.x() < min.x())
        min.SetX(pos.x());
      if (pos.y() < min.y())
        min.SetY(pos.y());
      if (pos.z() < min.z())
        min.SetZ(pos.z());
    } 
    
    return min;
  }
  
  vector3 OBDock::GetMaximum(OBMol &mol)
  {
    vector3 max = vector3(0.0, 0.0, 0.0);
    
    FOR_ATOMS_OF_MOL (atom, mol) { 
      vector3 pos = atom->GetVector();
      if (pos.x() > max.x())
        max.SetX(pos.x());
      if (pos.y() > max.y())
        max.SetY(pos.y());
      if (pos.z() > max.z())
        max.SetZ(pos.z());
    } 
    
    return max;
  }
  
  vector3 OBDock::GetDimentions(OBMol &mol)
  {
    vector3 min, max;
    
    min = GetMinimum(mol);
    max = GetMaximum(mol);
    
    return (max - min);
  }
  
} // end namespace OpenBabel


//! \file dock.cpp
//! \brief Handle OBDock class
