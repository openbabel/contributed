import os
import sys
import pybel

conv = pybel.ob.OBConversion()
conv.SetOutFormat("smi")
conv.SetOptions("C", conv.OUTOPTIONS)

elems = ['Cl', 'Se', 'Br', 'Al', 'Si', 'As', 'Li', 'Na',
         'Mg', 'Ca', 'Cr', 'Mn', 'Fe', 'Co', 'Ni', 'Zn',
         'Cu', 'B', 'C', 'N', 'O', 'F', 'P', 'S', 'I', 'K']
aroms = ['si', 'se', 'b', 'c', 'n', 'o', 'p', 's']

data = ((elems, 'A'), (aroms, 'a'))

def CanonicalSMARTS(smiles):
    mol = pybel.readstring("smi", smiles)
    names = [conv.WriteString(mol.OBMol, True) for i in range(100)]

    smarts = []
    for name in names:
        newname = name
        for elems, smart in data:
            for elem in elems:
                if len(elem) == 2:
                    txt = "[%s]" % elem
                else:
                    txt = elem
                newname = newname.replace(txt, smart)
        smarts.append(newname)

    return min(smarts)

def usage():
    print """python removedups.py rings.xyz freq-sort.txt > freq-nodups.txt

Remove duplicate SMARTS strings from freq-sort.txt to create
freq-nodups.txt.
"""
        
if __name__ == "__main__":
    if len(sys.argv) != 3 or not os.path.isfile(sys.argv[1]) or not os.path.isfile(sys.argv[2]):
        usage()
        sys.exit(1)
        

    fragfile = sys.argv[1]
    freqfile = sys.argv[2]

    
    xyz = {}
    for line in open(fragfile, "r"):
        broken = line.split()
        if len(broken) == 4 and line[0] != 'C':
            xyz["\t".join(broken[:3])] = broken[-1].rstrip()

    seen = set()
    for line in open(freqfile, "r"):
        broken = line.split()
        lookup = "\t".join(broken[2:])
        assert lookup in xyz
        cansmarts = CanonicalSMARTS(xyz[lookup])
        if cansmarts not in seen:
            print line.rstrip()
            seen.add(cansmarts)
        
        
