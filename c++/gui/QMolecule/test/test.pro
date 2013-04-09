######################################################################
# Author: Diego Saldana Miranda
# LinkedIn: http://lnkd.in/b2kTzS
######################################################################

TEMPLATE = app
TARGET =
LIBS += ~/Downloads/openbabel-2.3.1/build/libopenbabel.a
DEPENDPATH += ../src/
DEPENDPATH += ../include/
DEPENDPATH += ../test/
INCLUDEPATH += /usr/local/include/openbabel-2.0/
INCLUDEPATH += ../include/
OBJECTS_DIR = ../obj/
MOC_DIR = ../moc/

# Input
HEADERS += QMolecule.h

SOURCES += QOBPainter.cpp
SOURCES += QMolecule.cpp
SOURCES += test.cpp
