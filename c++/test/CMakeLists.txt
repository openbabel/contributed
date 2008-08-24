project(openbabel_unit_tests)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)


find_package(OpenBabel3 REQUIRED) # find and setup OpenBabel
if(OPENBABEL3_FOUND)
  include_directories( ${OPENBABEL3_INCLUDE_DIR} )
  link_directories( ${OPENBABEL3_LIBRARY_DIRS} )
endif(OPENBABEL3_FOUND)

find_package(Eigen2 REQUIRED)
if(EIGEN2_FOUND)
  include_directories(${EIGEN2_INCLUDE_DIR})
endif(EIGEN2_FOUND)


set(sources 
    atom.cpp 
    bond.cpp 
    mol.cpp 
    ffmmff94.cpp 
    ffuff.cpp 
    ffghemical.cpp 
    aromatest.cpp 
    iterators.cpp 
    cansmi.cpp
    cmlreadfile.cpp 
    conversion.cpp 
    data.cpp 
    formalcharge.cpp 
    formula.cpp 
    phmodel.cpp 
    mmff94validate.cpp 
    internalcoord.cpp 
    logp_psa.cpp 
    ringtest.cpp 
    math.cpp 
    residue.cpp 
    invalidsmiles.cpp 
    invalidsmarts.cpp 
    smartstest.cpp 
    smilesmatch.cpp 
    inchiwrite.cpp 
    unitcell.cpp 
    forcefield.cpp
    chains.cpp
)

#add_executable(test_suite test.cpp ${sources})
add_executable(test_suite test.cpp)
target_link_libraries(test_suite ${OPENBABEL3_LIBRARIES})
#	g++ test.cpp -o test_suite -lopenbabel3 -linchi -I/home/tim/openbabel-git/include -L/home/tim/openbabel-git/src
#	g++ test.cpp -o test_suite -lopenbabel3 -linchi 

