#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
revert_list='
'
move_list='
'
move_sed='s|list_files.sh|ls_files.sh|'
#
cat << EOF > junk.sed
#
/\$codep [*]\//! b two
: one
N
/\/[*] *\$\\\$/! b one
s|\$codep [*]/|\$srccode%cpp% */|
s|/[*] *\$\\\$|/* %\$\$|
#
: two
/\$verbatim/! b end
/\$\\\$/! N
/\$\\\$/! b two
/\.[ch]pp%/s|\$verbatim|\$srcfile|
#
: end
EOF
# -----------------------------------------------------------------------------
if [ $0 != "bin/batch_edit.sh" ]
then
	echo "bin/batch_edit.sh: must be executed from its parent directory"
	exit 1
fi
# -----------------------------------------------------------------------------
# bash function that echos and executes a command
echo_eval() {
	echo $*
	eval $*
}
# -----------------------------------------------------------------------------
cp bin/batch_edit.sh $HOME/trash/batch_edit.sh
git reset --hard
# ---------------------------------------------------------------------------
list_all=`bin/ls_files.sh`
for file in $list_all
do
	if [ "$file" != 'bin/batch_edit.sh' ]
	then
		echo_eval sed -f junk.sed -i $file
	fi
done
# ----------------------------------------------------------------------------
for old in $move_list
do
	new=`echo $old | sed -e "$move_sed"`
	echo_eval git mv $old $new
done
# ----------------------------------------------------------------------------
# files that were hand edited and cached using 'git_new.sh to'
if [ -e new ]
then
	echo_eval git_new.sh from
fi
# ----------------------------------------------------------------------------
for file in $revert_list
do
	echo_eval git checkout $file
done
# ----------------------------------------------------------------------------
cp $HOME/trash/batch_edit.sh bin/batch_edit.sh
echo "$0: OK"
exit 0
