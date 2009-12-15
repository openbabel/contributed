Instructions on use:

./obfragment *.sdf >rings.xyz 2>frequencies.txt &
grep INDEX frequencies.txt | sort -rn >freq-sort.txt
/usr/bin/emacs freq-sort.txt # cut down to size
python removedups.py rings.xyz freq-sort.txt > freq-nodups.txt
./obsort rings.xyz freq-nodups.txt >ring-2500.xyz
