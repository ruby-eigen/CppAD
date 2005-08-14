// BEGIN SHORT COPYRIGHT
/* -----------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-05 Bradley M. Bell

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
------------------------------------------------------------------------ */
// END SHORT COPYRIGHT

/*
$begin Value.cpp$$
$spell
	Cpp
	cstddef
$$

$section Conversion to Base Type: Example and Test$$
$index Value$$
$index base type, value$$
$index example, Value$$
$index test, Value$$

$comment This file is in the Example subdirectory$$ 
$code
$verbatim%Example/Value.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>

bool Value(void)
{	bool ok = true;

	using namespace CppAD;

	// independent variable vector
	CppADvector< AD<double> > X(2);
	X[0] = 3.;
	X[1] = 4.;

	// check value before recording
	ok &= (Value(X[0]) == 3.);
	ok &= (Value(X[1]) == 4.);

	// start recording

	Independent(X);

	// cannot call Value here (tape is recording)

	// dependent variable vector 
	CppADvector< AD<double> > Y(1);
	Y[0] = - X[1];

	// create f: X -> Y and vectors used for derivative calculations
	ADFun<double> f(X, Y);
	CppADvector<double> v( f.Domain() );
	CppADvector<double> w( f.Range() );

	// check value after recording
	ok &= (Value(X[0]) == 3.);
	ok &= (Value(X[1]) == 4.);
	ok &= (Value(Y[0]) == -4.);

	return ok;
}
// END PROGRAM
