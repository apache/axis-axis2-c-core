#!/bin/bash
echo 'Running autogen.sh in Util'
cd util
./autogen.sh

echo 'Running autogen.sh in Axiom'
cd ../axiom
./autogen.sh

echo 'Running autogen.sh in Xml_schema'
cd ../xml_schema
./autogen.sh

echo 'Running autogen.sh in Woden'
cd ../woden
./autogen.sh

echo 'Running autogen.sh in Guththila'
cd ../guththila
./autogen.sh

echo 'Running autogen.sh in Rampart'
cd ../rampart
./autogen.sh

echo 'Running autogen.sh in Axis2/C'
cd ..

echo -n 'Running libtoolize...'
if libtoolize --force > /dev/null 2>&1; then
	echo 'done.'
else
	echo 'failed.'
	exit 1
fi

echo -n 'Running aclocal...'
if aclocal > /dev/null 2>&1; then
	echo 'done.'
else
	echo 'failed.'
	exit 1
fi

echo -n 'Running autoheader...'
if autoheader > /dev/null 2>&1; then
	echo 'done.'
else
	echo 'failed.'
	exit 1
fi

echo -n 'Running autoconf...'
if autoconf > /dev/null 2>&1; then
	echo 'done.'
else
	echo 'failed.'
	exit 1
fi

echo -n 'Running automake...'
if automake --add-missing > /dev/null 2>&1; then
	echo 'done.'
else
	echo 'failed.'
	exit 1
fi

echo 'done'
