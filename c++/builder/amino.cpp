
#include <openbabel/mol.h>
#include <openbabel/obconversion.h>

#include <iostream>

using namespace std;
using namespace OpenBabel;

void writeXYZ(OBAtom*, char*);

int main(int argc,char *argv[])
{
  // turn off slow sync with C-style output (we don't use it anyway).
  std::ios::sync_with_stdio(false);

  OBMol obMol;
  OBConversion obConversion;
  OBFormat *inFormat;
  OBSmartsPattern smarts;
  string amino = "[#7][#6][#6](=O)O";
  smarts.Init(amino);

  string filename;
  vector< vector <int> > maplist;
  vector< vector <int> >::iterator matches;

  obMol.Clear();
  filename = argv[1];
  inFormat = obConversion.FormatFromExt(filename.c_str());
  obConversion.SetInFormat(inFormat);
  obConversion.ReadFile(&obMol, filename);

  char buffer[BUFF_SIZE];
  OBAtom *atom;
  
  if (smarts.Match(obMol))
    {
      maplist = smarts.GetUMapList();
      matches = maplist.begin();

      snprintf(buffer, BUFF_SIZE, "%d\n", obMol.NumAtoms());
      cout << buffer;
      cout << obMol.GetTitle() << '\n';

      for (unsigned int i = 0; i < 5; ++i)
        {
          atom = obMol.GetAtom((*matches)[i]);
          writeXYZ(atom, buffer);
        }

      for (unsigned int i = 1; i <= obMol.NumAtoms(); ++i)
        {
          if (i == (*matches)[0] || i == (*matches)[1] ||
              i == (*matches)[2] || i == (*matches)[3] ||
              i == (*matches)[4])
            continue;

          atom = obMol.GetAtom(i);
          writeXYZ(atom, buffer);
        }

    } // matched smarts
    
  return 0;
}

void writeXYZ(OBAtom *atom, char *buffer)
{
      snprintf(buffer, BUFF_SIZE, "%-3s%15.5f%15.5f%15.5f\n",
               etab.GetSymbol(atom->GetAtomicNum()),
               atom->GetX(),
               atom->GetY(),
               atom->GetZ());
      cout << buffer;
}
