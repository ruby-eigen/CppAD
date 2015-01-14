/* $Id$ */
# ifndef CPPAD_COMP_OP_INCLUDED
# define CPPAD_COMP_OP_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-15 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */


namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file comp_op.hpp
Zero order forward mode check how many comparisons changed.
*/

/*!
Zero order forward mode execution of op = CompOp.

The C++ source code corresponding to this operation is
\verbatim
	left Cop right
\endverbatim
where Cop is one of the following:
<, <=, == , >=, >, !=.

\tparam Base
base type for the operator; i.e., this operation was recorded
using AD< \a Base > and computations by this routine are done using type 
\a Base.

\param count
If the comparision has the same result as when t operation seqeunce was
recorded, \a count is not affected.
Otherwise it is incremented by one.

\param arg
\n
\a arg[0]
is static cast to size_t from the enum type
\verbatim
	enum CompareOp {
		CompareLt, 
		CompareLe, 
		CompareEq, 
		CompareGe, 
		CompareGt, 
		CompareNe
	}
\endverbatim
for this operation.
\n
\n 
\a arg[1] & 1 
\n
If this expression is true, 
the result of the comparison during taping it true.
Othwise the result if false.
\n
\n
\a arg[1] & 2
\n
if this expression is true, left is a variable, otherwise it is a parameter.
\n
\n
\a arg[1] & 4
\n
if this expression is true, right is a variable, otherwise it is a parameter.
\n

\param num_par
is the lenght of the \a parameter vector.
This value is only used for checking assumptions mentioned below
(and is not used at all when NDEBUG is defined).

\param parameter
Vector of parameters corresponding to the tape.
If left is a parameter, \a parameter[ arg[2] ] is its value.
If right is a parameter, \a parameter[ arg[3] ] is its value.

\param cap_order
number of columns in the matrix containing the Taylor coefficients.

\param taylor
Matrix of Taylor coefficients.
If left is a variable, \a taylor[ arg[2] * cap_order + 0 ] is its value.
If right is a variable, \a taylor[ arg[3] * cap_order + 0 ] is its value.


\par Checked Assertions where op is a binary operator:
\li NumArg(ComOp) == 4
\li NumRes(ComOp) == 0
\li size_t(arg[0]) <= static_cast<size_t>( CompareNe )
\li arg[1] != 0 (either left or right is a variable)
\li if left is a parameter, \a arg[2] < \a num_par
\li if right is a parameter, \a arg[3] < \a num_par

*/
template <class Base>
inline void forward_comp_op_0(
	size_t&        count       ,
	const addr_t*  arg         , 
	size_t         num_par     ,
	const Base*    parameter   ,
	size_t         cap_order   ,
	Base*          taylor      )
{	bool result;
	Base left;
	Base right;

	CPPAD_ASSERT_UNKNOWN( NumArg(ComOp) == 4 );
	CPPAD_ASSERT_UNKNOWN( NumRes(ComOp) == 0 );
	CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) <= static_cast<size_t> (CompareNe) );
	CPPAD_ASSERT_UNKNOWN( arg[1] != 0 );

	// result of comparision during recording
	result = (arg[1] & 1) == 1;

	// value of left operand for this forward sweep
	if( arg[1] & 2 )
		left = taylor[ arg[2] * cap_order + 0 ];
	else
	{	CPPAD_ASSERT_UNKNOWN( size_t(arg[2]) < num_par );
		left = parameter[ arg[2] ];
	}

	// value of right operand for this forward sweep.
	if( arg[1] & 4 )
		right = taylor[ arg[3] * cap_order + 0 ];
	else
	{	CPPAD_ASSERT_UNKNOWN( size_t(arg[3]) < num_par );
		right = parameter[ arg[3] ];
	}
	switch( CompareOp( arg[0] ) )
	{	case CompareLt:
		count += ( result != LessThanZero(left - right) );
		break;

		case CompareLe:
		count += ( result != LessThanOrZero(left - right) );
		break;

		case CompareEq:
		count += ( result != (left == right) );
		break;

		case CompareGe:
		count += ( result != GreaterThanOrZero(left - right) );
		break;

		case CompareGt:
		count += ( result != GreaterThanZero(left - right) );
		break;

		case CompareNe:
		count += ( result != (left != right) );
		break;

		default:
		CPPAD_ASSERT_UNKNOWN(0);
	}
	return;
}

template <class Base>
inline void forward_leqpv_op_0(
	size_t&       count       ,
	const addr_t* arg         ,
	const Base*   parameter   ,
	size_t        cap_order   ,
	Base*         taylor      )
{
	// check assumptions
	CPPAD_ASSERT_UNKNOWN( NumArg(LeqpvOp) == 2 );
	CPPAD_ASSERT_UNKNOWN( NumRes(LeqpvOp) == 0 );

	// Taylor coefficients corresponding to arguments and result
	Base  x = parameter[ arg[0] ];
	Base* y = taylor + arg[1] * cap_order;

	count += GreaterThanZero(x - y[0]);
}

template <class Base>
inline void forward_leqvv_op_0(
	size_t&       count       ,
	const addr_t* arg         ,
	const Base*   parameter   ,
	size_t        cap_order   ,
	Base*         taylor      )
{
	// check assumptions
	CPPAD_ASSERT_UNKNOWN( NumArg(LeqvvOp) == 2 );
	CPPAD_ASSERT_UNKNOWN( NumRes(LeqvvOp) == 0 );

	// Taylor coefficients corresponding to arguments and result
	Base* x = taylor + arg[0] * cap_order;
	Base* y = taylor + arg[1] * cap_order;

	count += GreaterThanZero(x[0] - y[0]);
}

template <class Base>
inline void forward_gtpv_op_0(
	size_t&       count       ,
	const addr_t* arg         ,
	const Base*   parameter   ,
	size_t        cap_order   ,
	Base*         taylor      )
{
	// check assumptions
	CPPAD_ASSERT_UNKNOWN( NumArg(GtpvOp) == 2 );
	CPPAD_ASSERT_UNKNOWN( NumRes(GtpvOp) == 0 );

	// Taylor coefficients corresponding to arguments and result
	Base  x = parameter[ arg[0] ];
	Base* y = taylor + arg[1] * cap_order;

	count += GreaterThanOrZero(y[0] - x);
}

template <class Base>
inline void forward_gtvv_op_0(
	size_t&       count       ,
	const addr_t* arg         ,
	const Base*   parameter   ,
	size_t        cap_order   ,
	Base*         taylor      )
{
	// check assumptions
	CPPAD_ASSERT_UNKNOWN( NumArg(GtvvOp) == 2 );
	CPPAD_ASSERT_UNKNOWN( NumRes(GtvvOp) == 0 );

	// Taylor coefficients corresponding to arguments and result
	Base* x = taylor + arg[0] * cap_order;
	Base* y = taylor + arg[1] * cap_order;

	count += GreaterThanOrZero(y[0] - x[0]);
}

} // END_CPPAD_NAMESPACE
# endif
