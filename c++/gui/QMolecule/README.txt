######################################################################
# Author: Diego Saldana Miranda
# LinkedIn: http://lnkd.in/b2kTzS
######################################################################

In order to compile the test app, you should first make sure that you
have working installations of both: (1) Qt4 and (2) OpenBabel (should
work with version 2.3 and above).

Next, go to the 'test' directory and modify the 'test.pro' file by 
changing lines number 8 and 12, specifying the paths to your openbabel
library and include directories, respectively.

Finally, execute the command 'qmake' and then 'make'. The test app
should be compiled and it should appear on the test directory.

The app shows a table with 4 SMILES formulae that the user can click and
the QMolecule widget displays a 2D depiction of the molecule 
corresponding to the formula clicked by the user. 

The user may modify the SMILES formulae but if the formula is not 
recognized, OpenBabel may throw exceptions that are not handled by the 
widget as of yet.
