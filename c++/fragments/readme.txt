Instructions on use:

./obfragment *.sdf >rings.xyz 2>frequencies.txt &
grep INDEX frequencies.txt | sort -rn >freq-sort.txt
/usr/bin/emacs freq-sort.txt # cut down to size
./obsort rings.xyz freq-sort.txt >ring-2500.xyz
