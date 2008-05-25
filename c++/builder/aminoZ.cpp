
#include <openbabel/mol.h>
#include <openbabel/obconversion.h>

#include <iostream>

using namespace std;
using namespace OpenBabel;

int main(int argc,char *argv[])
{
  // turn off slow sync with C-style output (we don't use it anyway).
  std::ios::sync_with_stdio(false);

  OBMol mol;
  OBConversion obConversion;
  OBFormat *inFormat;
  OBSmartsPattern smarts;
  // we set up the input files with amide on each end -- but the N is atom 1
  // (i.e., we build from N to C)
  string amino = "[#1][#6](=O)[#7][#6][#6](=O)[#7]";
  smarts.Init(amino);

  string filename;

  mol.Clear();
  filename = argv[1];
  inFormat = obConversion.FormatFromExt(filename.c_str());
  obConversion.SetInFormat(inFormat);
  obConversion.ReadFile(&mol, filename);

  if (!smarts.Match(mol))
    return 0;

  vector< vector <int> > maplist = smarts.GetUMapList();
  vector< vector <int> >::iterator matches = maplist.begin();

  // delete some atoms we don't want in the final zmatrix build file
  // "[#1][#6](=O)[#7][#6][#6](=O)[#7]"
  OBAtom *extraC = mol.GetAtom((*matches)[1]); // HC=O
  OBAtom *extraCO = mol.GetAtom((*matches)[2]);
  OBAtom *extraN = mol.GetAtom((*matches)[7]); // -NH2
  mol.DeleteHydrogens(extraC);
  mol.DeleteAtom(extraC);
  mol.DeleteAtom(extraCO);
  mol.DeleteHydrogens(extraN);
  mol.DeleteAtom(extraN);

  char buffer[BUFF_SIZE];
  
  OBAtom *a,*b,*c;//, *atom;
  
  vector<OBInternalCoord*> vic;
  vic.push_back((OBInternalCoord*)NULL);
  FOR_ATOMS_OF_MOL(atom, mol)
    vic.push_back(new OBInternalCoord);

  CartesianToInternal(vic,mol);

  double r,w,t;
  char type[4];
  FOR_ATOMS_OF_MOL (atom, mol) {
    a = vic[atom->GetIdx()]->_a;
    b = vic[atom->GetIdx()]->_b;
    c = vic[atom->GetIdx()]->_c;
    r = vic[atom->GetIdx()]->_dst;
    w = vic[atom->GetIdx()]->_ang;
    t = vic[atom->GetIdx()]->_tor;
    
    strncpy(type, etab.GetSymbol(atom->GetAtomicNum()), 4);
    type[3] = '\0';
    
    if (t < 0)
      t += 360;
    snprintf(buffer, BUFF_SIZE, "%-2s %10.6f  %10.6f  %10.6f  ", 
             type, r, w, t);
    cout << buffer;

    if (atom->GetIdx() == 1) 
      snprintf(buffer, BUFF_SIZE, "%4d%4d%4d", 0, 0, 0);
    if (atom->GetIdx() == 2) 
      snprintf(buffer, BUFF_SIZE, "%4d%4d%4d", a->GetIdx(), 0, 0);
    if (atom->GetIdx() == 3) 
      snprintf(buffer, BUFF_SIZE, "%4d%4d%4d", a->GetIdx(), b->GetIdx(), 0);
    if (atom->GetIdx() >= 4) 
      snprintf(buffer, BUFF_SIZE, "%4d%4d%4d", a->GetIdx(), b->GetIdx(), c->GetIdx());
    cout << buffer;

    snprintf(buffer, BUFF_SIZE, "  %10.6f\n", atom->GetPartialCharge());
    cout << buffer;
  }
  
  return 0;
}
