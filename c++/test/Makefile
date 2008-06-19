all: test_suite

test_suite: test.cpp atom.cpp bond.cpp mol.cpp
	g++ test.cpp -o test_suite -lopenbabel

check: test_suite
	cp styleline.xml report.xml
	./test_suite --report_format=XML --report_level=detailed 2>> report.xml
