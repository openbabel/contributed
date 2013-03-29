#!/bin/bash

# Full path to the OpenBabel executable.
BABEL_EXE_PATH=/usr/bin/babel

# Full path to the Fast Search index file to search against.
INDEX_FILE_PATH=

# Paths to JAR files.
CLASS_PATH=lib/S4.jar:lib/simple-5.0.7.jar

# Port number for the Web server to listen on.
PORT_NUMBER=8080

# Launch the server.
java -cp ${CLASS_PATH}                                  \
    -Ds4.babel.exe.path.name=${BABEL_EXE_PATH}          \
    -Ds4.index.file.name=${INDEX_FILE_PATH}             \
    -Ds4.server.port.number=${PORT_NUMBER}              \
    com.syngenta.ciu.s4.Server