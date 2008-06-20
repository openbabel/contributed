all: test_suite

test_suite: test.cpp atom.cpp bond.cpp mol.cpp ffmmff94.cpp ffuff.cpp \
		ffghemical.cpp aromatest.cpp iterators.cpp cansmi.cpp \
		cmlreadfile.cpp conversion.cpp data.cpp formalcharge.cpp \
		formula.cpp phmodel.cpp
	g++ test.cpp -o test_suite -lopenbabel

report: test_suite report.xsl
	cp reportline.xml report.xml
	./test_suite --report_format=XML --report_level=detailed 2>> report.xml

log: test_suite log.xsl
	cp logline.xml log.xml
	./test_suite --log_format=XML --log_level=all >> log.xml

check: report log
