SIMPLE SIMILARITY SEARCH SERVER (S4)
====================================

S4 provides a simple RESTful Web Service interface to OpenBabel's similarity search tools.

--- Build S4

To build S4 use Maven:

cd S4
mvn2 package

You will then find a build of the S4 JAR file in the Maven target directory, e.g. S4/target/S4-1.0.0-SNAPSHOT.jar

--- Run S4

To run the S4 service copy the S4 JAR file built in the previous step.

S4 depends on the Simple Framework http://simpleframework.org.

Download simple-5.0.7.jar and copy it to the same directory as the S4 jar (or copy it from your local Maven
repository after the build, e.g. ~/.m2/repository/org/simpleframework/simple/5.0.7/simple-5.0.7.jar).

To launch S4 you need to define three properties passed on the command-line:

s4.babel.exe.path.name
  Full path name of the OpenBabel executable.

s4.index.file.name
  Full path to the OpenBabel Fast Search index file to search.

s4.server.port.number (optional; default: 8080)
  The HTTP port number for the server to listen to.

Example:

java -cp S4.jar:simple-5.0.4.jar                \
    -Ds4.babel.exe.path.name=/usr/bin/babel     \
    -Ds4.index.file.name=/path/to/index_file.fs \
    -Ds4.server.port.number=8888                \
    com.syngenta.ciu.s4.Server

A template script file for defining the above properties and launching S4 is provided in src/main/scripts/runS4.sh.

--- Use S4

You can search by requesting simsearch on the S4 server with parameters smiles and cutoff (the latter is optional;
default is 0.8), e.g.

http://server.name:8888/simsearch?smiles=O=C%28[C@H]%28CC1=CC=CC=C1%29NC%28=O%29[C@H]%28CC%28=O%29O%29N%29OCC&cutoff=0.9

It is important to remember that SMILES values must be URL encoded. A simple way to to do this is to enter the
following command into the address bar of your browser:

javascript:encodeURIComponent("O=C1C2=C(N=CN2C)N(C(=O)N1C)C")

--- How S4 uses OpenBabel

The simsearch service uses the BabelSimilaritySearch class to run the OpenBabel executable.

1. The query SMILES is written to a temporary file smiles.query.file.
2. A second temporary file, temporary.results.file, is created to hold intermediate results.
3. The OpenBabel executable is run to perform a similarity search of the Fast Search index file, s4.index.file.name,
using the specified tanimoto.cutoff:

babel <s4.index.file.name> <temporary.results.file> -S <smiles.query.file> -at <tanimoto.cutoff>

This writes the search results (structure id and SMILES) to temporary.results.file.

4. The OpenBabel executable is run a second time to determine the Tanimoto similarity coefficient between the query
structure in smiles.query.file and each of the structures in temporary.results.file:

babel <smiles.query.file> <temporary.results.file> -ofpt

5. The output of this command is parse to produce a mapping between structure id and Tanimoto coefficient.

6. The temporary.results.file is then parsed with the structure id used to lookup the Tanimoto coefficient using the
map produced in step 5. Then the structure id, SMILES and Tanimoto coefficient are separated by TABs and appended to
the output of simesearch.