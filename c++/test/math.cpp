/**********************************************************************
math.cpp - Unit tests for the math code in OpenBabel

Copyright (C) 1998-2001 by OpenEye Scientific Software, Inc.
Copyright (C) 2001-2006 Geoffrey R. Hutchison
Copyright (C) 2006 Benoit Jacob
Copyright (C) 2008 Tim Vandermeersch
 
This file is part of the Open Babel project.
For more information, see <http://openbabel.sourceforge.net/>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation version 2 of the License.
 
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
***********************************************************************/

// used to set import/export for Cygwin DLLs
#ifdef WIN32
#define USING_OBDLL
#endif

#include <openbabel/babelconfig.h>
#include <openbabel/math/matrix3x3.h>
#include <openbabel/obutil.h>

#include <iostream>
#include <stdlib.h>
#include <string.h>

#define REPEAT 1000

using namespace std;
using namespace OpenBabel;

OBRandom randomizer;
int testCount = 0;
int failedCount = 0;
char currentFunc [256];
int tmp;

void pickRandom( double & d )
{
  d = randomizer.NextFloat() * 2.0 - 1.0;
}

void pickRandom( vector3 & v )
{
  pickRandom( v.x() );
  pickRandom( v.y() );
  pickRandom( v.z() );
}

void pickRandom( matrix3x3 & m )
{
  for( int row = 0; row < 3; row++ )
    for( int column = 0; column < 3; column++ )
      pickRandom( m( row, column ) );
}

bool compare( const double & d1, const double & d2, double precision = 1e-6 )
{
  return IsApprox( d1, d2, precision );
}

bool compare( const vector3 & v1, const vector3 & v2, double precision = 1e-6 )
{
  return v1.IsApprox( v2, precision );
}

bool compare( const matrix3x3 & m1, const matrix3x3 & m2 )
{
  bool ret = true;
  for( int i = 0; i < 3; i++ )
    ret &= compare( m1.GetColumn(i), m2.GetColumn(i) );
  return ret;
}

// tests the constructors, accessors, mutators of class vector3.
void testBasics_vector3()
{
  double d[3];
  vector3 v1, v2, v3;

  // test compare() itself, assuming Set() works
  v1.Set( 1.0, 0.0, 0.0 );
  v2.Set( 1.0, 1e-20, 0.0 );
  v3.Set( 1.0, 0.1, 0.0 );
  BOOST_CHECK( compare( v1, v2 ) );
  BOOST_CHECK( ! compare( v2, v3 ) );

  // test constructors and operator=
  pickRandom( d[0] );
  pickRandom( d[1] );
  pickRandom( d[2] );
  v1 = vector3( d[0], d[1], d[2] );
  v2 = vector3( v1 );
  v3 = v1;
  BOOST_CHECK( compare( v1, v2 ) );
  BOOST_CHECK( compare( v1, v3 ) );

  // test constant operator()
  BOOST_CHECK( compare( static_cast<const vector3>(v1).x(), d[0] ) );
  BOOST_CHECK( compare( static_cast<const vector3>(v1).y(), d[1] ) );
  BOOST_CHECK( compare( static_cast<const vector3>(v1).z(), d[2] ) );

  // test Get
  v1.Get( d );
  BOOST_CHECK( compare( v1.x(), d[0] ) );
  BOOST_CHECK( compare( v1.y(), d[1] ) );
  BOOST_CHECK( compare( v1.z(), d[2] ) );

  // test non-constant operator(), Set, SetX, SetY, SetZ
  pickRandom( d[0] );
  pickRandom( d[1] );
  pickRandom( d[2] );
  v1.x() = d[0];
  v1.y() = d[1];
  v1.z() = d[2];
  BOOST_CHECK( compare( v1.x(), d[0] ) );
  BOOST_CHECK( compare( v1.y(), d[1] ) );
  BOOST_CHECK( compare( v1.z(), d[2] ) );
  v2.Set( d );
  BOOST_CHECK( compare( v1, v2 ) );
  v3.SetX( d[0] );
  v3.SetY( d[1] );
  v3.SetZ( d[2] );
  BOOST_CHECK( compare( v1, v3 ) );
}

void testBasics_matrix3x3()
{
  double d[3][3] = { { 1.0, 0.0, 0.0 },
                     { 0.0, 1.0, 0.0 },
                     { 0.0, 0.0, 1.0 } };
  double x;
  matrix3x3 m1, m2, m3;
  vector3 v1, v2;
  int i, j;

  // test constructors and operator=
  m1 = matrix3x3(1.0);
  m2 = matrix3x3(d);
  BOOST_CHECK( compare( m1, m2 ) );

  // test GetArray
  static double e[3][3]; // without "static", compiler optimizations can
                         // produce bugs.
  pickRandom( m2 );
  m2.GetArray( & e[0][0] );
  m3 = matrix3x3(e);
  BOOST_CHECK( compare( m2, m3 ) );

  // test const operator() and Get()
  for( i = 0; i < 3; i++ )
    for( j = 0; j < 3; j++ )
    {
      BOOST_CHECK( compare( static_cast<const matrix3x3>(m2)(i,j), e[i][j] ) );
      x = m2.Get( i, j );
      BOOST_CHECK( compare( x, e[i][j] ) );
    }

  // test non-const operator() and Set()
  for( i = 0; i < 3; i++ )
    for( j = 0; j < 3; j++ )
      m1(i,j) = m2(i,j);
  BOOST_CHECK( compare( m1, m2 ) );
  for( i = 0; i < 3; i++ )
    for( j = 0; j < 3; j++ )
      m3.Set(i,j, m2(i,j) );
  BOOST_CHECK( compare( m3, m2 ) );

  // test SetColumn(), GetColumn(), SetRow(), GetRow(), transpose()
  for( i = 0; i < 3; i++ )
  {
    pickRandom( v1 );
    m1.SetRow( i, v1 );
    v2 = m1.GetRow( i );
    BOOST_CHECK( compare( v1, v2 ) );
    m1.SetColumn( i, v1 );
    v2 = m1.GetColumn( i );
    BOOST_CHECK( compare( v1, v2 ) );
    m1 = m1.transpose();
    v2 = m1.GetRow( i );
    BOOST_CHECK( compare( v1, v2 ) );
  }
}

void testArithmeticOperators()
{
  matrix3x3 mat1(1.0), mat2;
  vector3 vec1 = VZero, vec2;
  pickRandom( mat2 );
  pickRandom( vec2 );

  BOOST_CHECK( compare( mat2, mat1 * mat2 ) );
  BOOST_CHECK( compare( mat2, mat2 * mat1 ) );
  BOOST_CHECK( compare( vec2, mat1 * vec2 ) );
  BOOST_CHECK( compare( vec2, vec1 + vec2 ) );

  matrix3x3 mat3; pickRandom(mat3);
  vector3 vec3; pickRandom(vec3);
  pickRandom(mat1);
  pickRandom(vec1);

  BOOST_CHECK( compare( ( mat1 * mat2 ) * vec1,
                   mat1 * ( mat2 * vec1 )
                 ) );

  BOOST_CHECK( compare( vec1 * -3.0, - vec1 - vec1 - vec1 ) );
  BOOST_CHECK( compare( 2.0 * vec1, vec1 + vec1 ) );

  double a1, a2;
  do pickRandom(a1); while( a1 == 0.0 );
  pickRandom(a2);
  BOOST_CHECK( compare( vec1 * ( a1 + a2 ), vec1 * a1 + vec1 * a2 ) );
  BOOST_CHECK( compare( ( a1 - a2 ) * vec1, a1 * vec1 - a2 * vec1 ) );
  BOOST_CHECK( compare( ( vec1 / a1 ) * a1, vec1, 1e-5 ) );

  vec3 = vec1; vec3 += vec2;
  BOOST_CHECK( compare( vec1 + vec2, vec3 ) );
  vec3 = vec1; vec3 -= vec2;
  BOOST_CHECK( compare( vec1 - vec2, vec3 ) );
  vec3 = vec1; vec3 *= a1;
  BOOST_CHECK( compare( vec1 * a1, vec3 ) );
  vec3 = vec1; vec3 /= a1;
  BOOST_CHECK( compare( vec1 / a1, vec3 ) );
}

void testDistancesAnglesOrthogonality()
{
  vector3 v1, v2, v3;
  do pickRandom( v1 ); while( v1.length() == 0 );
  BOOST_CHECK( compare( v1.length_2(), v1.length() * v1.length() ) );
  v1.createOrthoVector( v2 );
  BOOST_CHECK( compare( v2.length(), 1.0 ) );
  v1.normalize();
  v1.createOrthoVector( v2 );
  BOOST_CHECK( compare( v1.length(), 1.0 ) );
  BOOST_CHECK( IsNegligible( dot( v1, v2 ), 1.0, 1e-6 ) );

  matrix3x3 m1;
  m1.SetColumn( 0, v1 );
  m1.SetColumn( 1, v2 );
  m1.SetColumn( 2, cross( v1, v2 ) );
  BOOST_CHECK( m1.isOrthogonal() );

  pickRandom( v1 );
  pickRandom( v2 );
  double c = dot( v1, v2 ), s = cross( v1, v2 ).length();
  BOOST_CHECK( compare( c, cos( vectorAngle( v1, v2 ) * DEG_TO_RAD ) * v1.length() * v2.length() ) );
  BOOST_CHECK( compare( s, sin( vectorAngle( v1, v2 ) * DEG_TO_RAD ) * v1.length() * v2.length() ) );

  BOOST_CHECK( compare( v1.distSq(v2), (v1-v2).length_2() ) );

  double t;
  pickRandom(t);
  BOOST_CHECK( compare( fabs(t), Point2Plane( v1 + t * VY, v1, v1 + 5.0 * VX - 3.0 * VZ, v1 - VX + VZ ) ) );
}

// Test the inversion of matrices and the matrix product. Get an
// invertible random matrix, invert it, multiply and check if the
// resulting matrix is the unit matrix.
void testInversion()
{
  matrix3x3 rnd;
  do pickRandom( rnd ); while( rnd.determinant() < 1e-3 );
  matrix3x3 result = rnd * rnd.inverse();
  BOOST_CHECK( result.isUnitMatrix() );
}

// Test the eigenvalue finder. Set up a diagonal matrix and conjugate
// by a rotation. That way we obtain a symmetric matrix that can be
// diagonalized. Check if the eigenvalue finder reveals the original
// diagonal elements.
void testEigenvalues()
{
  matrix3x3 Diagonal;
  for(int i=0; i<3; i++)
    for(int j=0; j<3; j++)
      Diagonal.Set(i, j, 0.0);
  Diagonal.Set(0, 0, randomizer.NextFloat());
  Diagonal.Set(1, 1, Diagonal.Get(0,0)+fabs(randomizer.NextFloat()));
  Diagonal.Set(2, 2, Diagonal.Get(1,1)+fabs(randomizer.NextFloat()));

  // test the isDiagonal() method
  BOOST_CHECK( Diagonal.isDiagonal() );

  matrix3x3 rndRotation;
  rndRotation.randomRotation(randomizer);

  // check that rndRotation is really a rotation, i.e. that randomRotation() works
  BOOST_CHECK( rndRotation.isOrthogonal() );
  
  matrix3x3 toDiagonalize = rndRotation * Diagonal * rndRotation.inverse();
  BOOST_CHECK( toDiagonalize.isSymmetric() );
  
  vector3 eigenvals;
  toDiagonalize.findEigenvectorsIfSymmetric(eigenvals);
  
  for(unsigned int j=0; j<3; j++)
    BOOST_CHECK( IsNegligible( eigenvals[j] - Diagonal.Get(j,j), Diagonal.Get(2,2) ) );
  
  BOOST_CHECK( eigenvals[0] < eigenvals[1] &&  eigenvals[1] < eigenvals[2] );
}

// Test the eigenvector finder. Set up a symmetric diagonal matrix and
// diagonalize it. The that conjugation with the computed eigenvectors
// really gives a diagonal matrix. Calculate the matrix-vector
// products directly to see if the vectors found are eigenvectors
// indeed, and if the computed eigenvalues are correct.
void testEigenvectors()
{
  matrix3x3 rnd;
  pickRandom( rnd );
  rnd.Set(0,1, rnd.Get(1,0));
  rnd.Set(0,2, rnd.Get(2,0));
  rnd.Set(1,2, rnd.Get(2,1));
  vector3 eigenvals;
  matrix3x3 eigenvects = rnd.findEigenvectorsIfSymmetric(eigenvals);
  
  // Check if the eigenvects are normalized and mutually orthogonal
  BOOST_CHECK( eigenvects.isOrthogonal() );
  
  // for an orthogonal matrix, the inverse should equal the transpose
  BOOST_CHECK( compare( eigenvects.inverse(), eigenvects.transpose() ) );

  matrix3x3 shouldBeDiagonal = eigenvects.inverse() * rnd * eigenvects;
  BOOST_CHECK( shouldBeDiagonal.isDiagonal() );
  
  for(unsigned int j=0; j<3; j++)
    BOOST_CHECK( compare( shouldBeDiagonal.Get(j,j), eigenvals[j] ) );
  
  for(unsigned int i=0; i<3; i++ )
    {
      vector3 EV = eigenvects.GetColumn(i);
      BOOST_CHECK( compare( rnd*EV, eigenvals[i]*EV ) );
    }
}

void math_test()
{
  cout << "# math: repeating each test " << REPEAT << " times" << endl;
  
  randomizer.TimeSeed();

  testBasics_vector3();
  testBasics_matrix3x3();
  testArithmeticOperators();
  testDistancesAnglesOrthogonality();
  testInversion();
  testEigenvalues();
  testEigenvectors();

}
