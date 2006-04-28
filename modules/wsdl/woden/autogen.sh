#!/bin/bash
  
for i in libtoolize aclocal autoconf autoheader
do
    echo -n "Running $i..."
    $i || exit 1
    echo 'done.'
done
  
echo -n 'Running automake...'
automake --add-missing
echo 'done.'
exit 0

