#!/bin/bash

echo "run libtoolize."          && 
libtoolize                      && 
echo "run aclocal."             && 
aclocal                         && 
echo "run autoconf."            && 
autoconf                        && 
echo "run autoheader."          && 
autoheader                      && 
echo "run automake."            && 
#automake --add-missing --gnu
automake
