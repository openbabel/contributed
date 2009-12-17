import re
import os
import sys
import pybel

conv = pybel.ob.OBConversion()
conv.SetOutFormat("smi")
conv.SetOptions("C", conv.OUTOPTIONS)

##elems = ['Cl', 'Se', 'Br', 'Al', 'Si', 'As', 'Li', 'Na',
##         'Mg', 'Ca', 'Cr', 'Mn', 'Fe', 'Co', 'Ni', 'Zn',
##         'Cu', 'B', 'C', 'N', 'O', 'F', 'P', 'S', 'I', 'K']
##aroms = ['si', 'se', 'b', 'c', 'n', 'o', 'p', 's']

def SMILEStoSMARTS(smile):
    """
    >>> SMILEStoSMARTS("CC[Cl]nnn[si][Se]")
    'AAAaaaaA'
    """
    newname = re.sub("\[(Cl|Se|Br|Al|Si|As|Li|Na|Mg|Ca|Cr|Mn|Fe|Co|Ni|Zn|Cu)\]|[BCNOFPSIK]", "A", smile)
    newname = re.sub("\[(si|se)\]|[bcnops]", "a", newname)
    return newname    

def CanonicalSMARTS(smiles):
    mol = pybel.readstring("smi", smiles)
    # Make sure you do at least 100, and then add in an extra 100 for each
    # ring
    N = 100 * len(mol.OBMol.GetSSSR())
    names = [conv.WriteString(mol.OBMol, True) for i in range(N)]
    unames = set(names)
    smarts = [SMILEStoSMARTS(name) for name in unames]
    return min(smarts)

def usage():
    print """python removedups.py rings.xyz freq-sort.txt > freq-nodups.txt

Remove duplicate SMARTS strings from freq-sort.txt to create
freq-nodups.txt.
"""

def analyse(fragfile, freqfile):
    """Get some idea of how many possible SMARTS representations
    there are for a particular molecule"""
    xyz = {}
    for line in open(fragfile, "r"):
        broken = line.split()
        if len(broken) == 4 and line[0] != 'C':
            xyz["\t".join(broken[:3])] = broken[-1].rstrip()

    N = 10000
    seen = set()
    for line in open(freqfile, "r"):
        broken = line.split()
        lookup = "\t".join(broken[2:])
        assert lookup in xyz
        mol = pybel.readstring("smi", xyz[lookup])
        names = set([conv.WriteString(mol.OBMol, True) for i in range(N)])
        smarts = [SMILEStoSMARTS(name) for name in names]
        usmarts = set(smarts)

        print xyz[lookup], len(names), len(usmarts)

def main(fragfile, freqfile):
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

def test():
    import doctest
    doctest.testmod()
        
if __name__ == "__main__":
    if len(sys.argv) != 3 or not os.path.isfile(sys.argv[1]) or not os.path.isfile(sys.argv[2]):
        usage()
        sys.exit(1)
        
    fragfile = sys.argv[1]
    freqfile = sys.argv[2]

    main(fragfile, freqfile)
##    analyse(fragfile, freqfile)
##    test()

        

