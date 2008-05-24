
#include <openbabel/mol.h>
#include <openbabel/obconversion.h>

#include <iostream>

using namespace std;
using namespace OpenBabel;

void AddResidue(string residue, bool lStereo, 
                OBMol &mol, vector<OBInternalCoord*> &vic,
                const char chain);

int main(int argc,char *argv[])
{
  OBMol mol;
  vector<OBInternalCoord*> vic;
  vic.push_back((OBInternalCoord*)NULL);
  OBInternalCoord* ic;
  int lastN, lastCa, lastCac; // backbone atoms
  lastN = lastCa = lastCac = 0;
  int newN, newCa, newCac;
  int lastAtom = 0; // last atom read from previous residue

  double amideLength = 1.34;
  double bondAngle = 120.0;
  // Parameters for an alpha helix
  // should set this from an interface
  //    double phi = -60.0;
  //    double psi = -45.0;
  // For now, leaving this as straight-chain
  double phi = 179.9;
  double psi = 179.9;
  double omega = 179.9;

  // Should have some way of getting this
  // i.e., incremement every time we assemble a new chain
  char chain = 'A';

  // Read in our sequence, e.g. Ala-Lys-Gly
  string sequence(argv[1]);
  vector<string> vs;
  tokenize(vs, sequence, "-");

  mol.BeginModify();
  for(unsigned int res = 0; res < vs.size(); ++res)
    {
      AddResidue(vs[res], true, mol, vic, chain);
      newN = lastAtom + 1;
      newCa = lastAtom + 2;
      newCac = lastAtom + 3;

      if (lastAtom != 0) {
        // set the peptide bond
        // first the nitrogen
        ic = vic[newN];
        ic->_a = mol.GetAtom(lastCac);
        ic->_dst = amideLength;
        ic->_b = mol.GetAtom(lastCa);
        ic->_ang = bondAngle;
        ic->_c = mol.GetAtom(lastN);
        ic->_tor = psi;

        // now the Calpha
        ic = vic[newCa];
        ic->_b = mol.GetAtom(lastCac);
        ic->_ang = bondAngle;
        ic->_c = mol.GetAtom(lastCa);
        ic->_tor = omega;

        // now the new C=O
        ic = vic[newCac];
        ic->_c = mol.GetAtom(lastCac);
        ic->_tor = phi;

        // add the peptide bond
        mol.AddBond(lastCac, newN, 1);
      }

      // add the known backbone bonds
      mol.AddBond(newN, newCa, 1);
      mol.AddBond(newCa, newCac, 1);
      mol.AddBond(newCac, newCac + 1, 2); // C=O

      lastN = newN;
      lastCa = newCa;
      lastCac = newCac;
      lastAtom = mol.NumAtoms();
    }

  mol.EndModify();

  InternalToCartesian(vic,mol);
  // "End group modification"
  // add the extra H to get NH2
  // need to add the new OH to get COOH

  mol.ConnectTheDots();
  mol.PerceiveBondOrders();
  mol.SetPartialChargesPerceived();

  OBConversion conv;
  OBFormat *pFormat = conv.FindFormat("cml");
  conv.SetOutFormat(pFormat);

  conv.Write(&mol, &cout);
  
  return 0;
}

string Uppercase(const string &residue)
{
  string upperCase;
  const unsigned int length = residue.length();
  for (unsigned int i = 0; i < length; ++i)
    upperCase[i] = toupper(residue[i]);

  return upperCase;
}

void AddResidue(string residue, bool lStereo, 
                OBMol &mol, vector<OBInternalCoord*> &vic,
                const char chain)
{
  string filename;
  if (residue != "Gly") {
    if (lStereo)
      filename = "L-";
    else // D stereo
      filename = "D-";
  }
  filename += residue + ".zmat";
  
  ifstream ifs;
  ifs.open(filename.c_str());

  if (!ifs) // file doesn't exist
    return;

  // Offset:
  //  When we add the internal coordinates, we have to increment
  //  based on the size of the molecule so far
  unsigned int offset = mol.NumAtoms();

  // setup the parent residue
  // TODO -- this doesn't seem to set all the fields properly
  // (maybe we need to set atom types?)
  int prevRes = mol.NumResidues() + 1;
  OBResidue *res = mol.NewResidue();
  res->SetNum(prevRes);
  res->SetChain(chain);
  // needs to be in uppercase
  res->SetName(residue);

  // Read in an amino z-matrix
  // similar to MOPAC zmat format
  char buffer[BUFF_SIZE];
  vector<string> vs;
  OBAtom *atom;

  while (ifs.getline(buffer, BUFF_SIZE)) {
    tokenize(vs, buffer);

    atom = mol.NewAtom();
    atom->SetAtomicNum(etab.GetAtomicNum(vs[0].c_str()));
    atom->SetPartialCharge(atof(vs[7].c_str()));
    res->AddAtom(atom);

    OBInternalCoord *coord = new OBInternalCoord;
    coord->_dst = atof(vs[1].c_str());
    coord->_ang = atof(vs[2].c_str());
    coord->_tor = atof(vs[3].c_str());

    int index;
    // Set _a
    index = atoi(vs[4].c_str());
    if (index > 0 && index <= mol.NumAtoms())
      coord->_a = mol.GetAtom(index + offset);
    else
      coord->_a = NULL;
    // Set _b
    index = atoi(vs[5].c_str());
    if (index > 0 && index <= mol.NumAtoms())
      coord->_b = mol.GetAtom(index + offset);
    else
      coord->_b = NULL;
    // Set _c
    index = atoi(vs[6].c_str());    
    if (index > 0 && index <= mol.NumAtoms())
      coord->_c = mol.GetAtom(index + offset);
    else
      coord->_c = NULL;

    vic.push_back(coord);
  }
}
