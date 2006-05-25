#!/bin/bash
cd util

sh build.sh  2>error.log
    if grep -w 'Error' `find -name "error.log"`; then
	echo "Error, see util/error.log for details"
	exit 2
fi

cd ..

cd axiom

sh build.sh  2>error.log
    if grep -w 'Error' `find -name "error.log"`; then
	echo "Error, see util/error.log for details"
	exit 2
fi

cd ..

sh build.sh  2>error.log
    if grep -w 'Error' `find -name "error.log"`; then
	echo "Error, see util/error.log for details"
	exit 2
fi

cd samples

sh build.sh  2>error.log
    if grep -w 'Error' `find -name "error.log"`; then
	echo "Error, see util/error.log for details"
	exit 2
fi

cd ..
