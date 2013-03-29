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

---