/**********************************************************************
dock.h - OBDock class.
 
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

#ifndef OB_DOCK_H
#define OB_DOCK_H

#include <vector>
#include <string>
#include <map>

#include <list>
#include <set>
#include <openbabel/base.h>
#include <openbabel/mol.h>

namespace OpenBabel
{
  // score functions
#define OBDOCK_CHEMSCORE 	(1 << 0)   //!< score the poses using the ChemScore empirical scoring function
  
  //! \class OBDock
  //! \brief Docking Class
  class OBDock {
    public:
      //! constructor
      OBDock();
      //! destructor
      ~OBDock();
      
      //////////////////////////////////////////////////////////////////////////
      //
      //  Setup
      //
      //////////////////////////////////////////////////////////////////////////
      //! \name Setup
      //@{
      /*! Set the protein
       *  \param protein OBMol object that contains the protein
       */
      void SetProtein(OBMol &protein) 
      {
        _protein = protein;
      }
      /*! Set the ligand
       *  \param ligand OBMol object that contains the ligand
       */
      void SetLigand(OBMol &ligand) 
      {
        _ligand = ligand;
      }
      /*! Create a binding pocket from all atoms within radius from any atom of the ligand. 
       *  Call this function after SetProtein() and SetLigand() to trim the protein to a
       *  binding pocket, this improves performance. All atoms which do not belong to the 
       *  pocket will be deleted from the protein OBMol object. Finally, all hydrogen, 
       *  oxygen, nitrogen and carbon atoms with a valence of 0 will also be deleted. Use
       *  CreateResiduePocketFromLigand() if you want to use the pocket later with MMFF94.
       *
       *  Both CreateAtomPocketFromLigand() and CreateResiduePocketFromLigand() assume that
       *  the ligand is allready posed in the binding pocket. (e.g. When you have the 
       *  crystal structure for the complex, output from another docking program, ...)
       *
       *  \param radius radius in A
       *  \return the number of atoms for the binding pocket
       */
      int CreateAtomPocketFromLigand(double radius);
      /*! Create a binding pocket from all residues that have at least one atom within 
       *  radius from any atom of the ligand. Call this function after SetProtein() and 
       *  SetLigand() to trim the protein to a binding pocket, this improves performance. 
       *  All atoms which do not belong to the pocket will be deleted from the protein 
       *  OBMol object. By selecting full residues for the binding pocket, it will be 
       *  possible to assign correct MMFF94 atom types when you use the binding pocket
       *  as input structure for the OBForcefieldMMFF94 Class. (MMFF94 describes 
       *  protein-ligand better than the other available force fields. OpenBabel's MMFF94
       *  implementation has a quick way of assigning MMFF atom types based on amino acid 
       *  residue. It's not recommended and tested to use other force fields in the 
       *  same way)
       *
       *  Both CreateAtomPocketFromLigand() and CreateResiduePocketFromLigand() assume that
       *  the ligand is allready posed in the binding pocket. (e.g. When you have the 
       *  crystal structure for the complex, output from another docking program, ...)
       *
       *  \param radius radius in A
       *  \return the number of atoms for the binding pocket
       */
      int CreateResiduePocketFromLigand(double radius);
      /*! Create a binding pocket from all atoms within radius from point. 
       *  Call this function after SetProtein() and SetLigand() to trim the protein to a
       *  binding pocket, this improves performance. All atoms which do not belong to the 
       *  pocket will be deleted from the protein OBMol object. Finally, all hydrogen, 
       *  oxygen, nitrogen and carbon atoms with a valence of 0 will also be deleted. Use
       *  CreateResiduePocketFromPoint() if you want to use the pocket later with MMFF94.
       *
       *  Both CreateAtomPocketFromPoint() and CreateResiduePocketFromPoint() do not 
       *  assume that the ligand is allready posed in the binding pocket. (e.g. When 
       *  you have the identified a possible binding site in the protein, but do not 
       *  have a structure with a ligand in this binding pocket)
       *
       *  \param radius radius in A
       *  \return the number of atoms for the binding pocket
       */
      int CreateAtomPocketFromPoint(double radius, vector3 &point);
      /*! Create a binding pocket from all residues that have at least one atom within 
       *  radius from point. Call this function after SetProtein() and 
       *  SetLigand() to trim the protein to a binding pocket, this improves performance. 
       *  All atoms which do not belong to the pocket will be deleted from the protein 
       *  OBMol object. By selecting full residues for the binding pocket, it will be 
       *  possible to assign correct MMFF94 atom types when you use the binding pocket
       *  as input structure for the OBForcefieldMMFF94 Class. (MMFF94 describes 
       *  protein-ligand better than the other available force fields. OpenBabel's MMFF94
       *  implementation has a quick way of assigning MMFF atom types based on amino acid 
       *  residue. It's not recommended and tested to use other force fields in the 
       *  same way)
       *
       *  Both CreateAtomPocketFromPoint() and CreateResiduePocketFromPoint() do not 
       *  assume that the ligand is allready posed in the binding pocket. (e.g. When 
       *  you have the identified a possible binding site in the protein, but do not 
       *  have a structure with a ligand in this binding pocket)
       *
       *  \param radius radius in A
       *  \return the number of atoms for the binding pocket
       */
      int CreateResiduePocketFromPoint(double radius, vector3 &point);
      
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
      //! \name ChemScore
      //@{
      /*! Score the current protein-ligand complex
       *
       *  \return the ChemScore for the current protein-ligand complex in kJ/mol
       */
      double ChemScore();
      int ChemScoreAtomType(OBAtom *atom);
      double ChemScoreBlockFunction(double x, double x1, double x2);
      double ChemScoreHBondTerm();
      double ChemScoreMetalTerm();
      double ChemScoreLipophilicTerm();
      double ChemScoreRotationTerm();
      //@}
      
      //////////////////////////////////////////////////////////////////////////
      //
      //  Grid docking
      //
      //////////////////////////////////////////////////////////////////////////
      //! \name Grid Docking
      //@{
      /*! Initialize a grid docking for the ligand in the binding pocket. 
       *
       *  \param scoring the scoring function (e.g. OBDOCK_CHEMSCORE)
       *  \param translate_step the translate step size
       *  \param rotate_step the rotate step size
       */
      bool GridDockInitialize(int scoring, double translate_step, double rotate_step);
      /*! Generate the next pose, assign a score and store it. 
       *  \return true if there are more poses
       */
      bool GridDockNextPose();
      //@}
      
      //////////////////////////////////////////////////////////////////////////
      //
      //  Get informeration on poses
      //
      //////////////////////////////////////////////////////////////////////////
      //! \name Grid Docking
      //@{
      //! \return the number of generated poses.
      int NumPoses();
      //! \return the score for the pose
      double GetPose(OBMol &pose, int index);
      double GetCurrentPose(OBMol &pose);
      /*! Get the protein
       *  \param protein OBMol object to which the protein wil be copied
       */
      void GetProtein(OBMol &protein) 
      {
        protein = _protein;
      }
      /*! Get the ligand
       *  \param ligand OBMol object to which the ligand wil be copied
       */
      void GetLigand(OBMol &ligand) 
      {
        ligand = _ligand;
      }
      /*! Get the complex (= protein + ligand)
       *  \param mol OBMol object to which the ligand and protein wil be copied
       */
      void GetComplex(OBMol &mol) 
      {
        OBMol theComplex = _protein;
	theComplex += _ligand;
	mol = theComplex;
      }
      
      //@}
 
    private:
      OBMol 			_protein; //!< OBMol object that holds the atoms for the protein
      OBMol 			_ligand; //!< OBMol object that holds the atoms for the ligand
      vector3 			_pocket_dimentions; //!< pocket dimentions, constant
      vector3 			_ligand_dimentions; //!< ligand dimentions, conformer and rotation specific
      int			_scoring; //!< scoring function (e.g. OBDOCK_CHEMSCORE)
      double			_translate_step; //!< translate step size in A
      double			_rotate_step; //!< rotate step size in degrees

      // iterate variables
      vector3			_translate; //!< hold current translation
      vector3			_rotate; //!< hold the current rotation
      int  			_conformer; //!< hold the current conformer
      
      //////////////////////////////////////////////////////////////////////////
      //
      //  Privare method for internal use
      //
      //////////////////////////////////////////////////////////////////////////
      //! \name Private methods for internal use
      //@{
      /*! Get the dimentions for the mol.
       *  \return (Xmax-Xmin, Ymax-Ymin, Zmax-Zmin)
       */
      vector3 GetDimentions(OBMol &mol);
      /*! Get the minimum X, Y and Z value for the given OBMol object.
       *  \return (Xmin, Ymin, Zmin)
       */
      vector3 GetMinimum(OBMol &mol);
      /*! Get the maximum X, Y and Z value for the given OBMol object.
       *  \return (Xmax, Ymax, Zmax)
       */
      vector3 GetMaximum(OBMol &mol);
      //@}
 
  }; // class OBDock

}// namespace OpenBabel

#endif   // OB_DOCK_H

//! \file dock.h
//! \brief Docking Class
