#!/bin/bash
# Licensed to the Apache Software Foundation (ASF) under one or more
# contributor license agreements.  See the NOTICE file distributed with
# this work for additional information regarding copyright ownership.
# The ASF licenses this file to You under the Apache License, Version 2.0
# (the "License"); you may not use this file except in compliance with
# the License.  You may obtain a copy of the License at
# 
#   http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
echo 'Running autogen.sh in Util'
cd util
sh autogen.sh

echo 'Running autogen.sh in Axiom'
cd ../axiom
sh autogen.sh

echo 'Running autogen.sh in Guththila'
cd ../guththila
sh autogen.sh

echo 'Running autogen.sh in Neethi'
cd ../neethi
sh autogen.sh

echo 'Running autogen.sh in Axis2/C'
cd ..

# --copy on libtoolize and automake below is deliberate; do not drop it.
#
# Without it both tools install their helper scripts -- ltmain.sh, test-driver,
# compile, missing, install-sh, config.guess, config.sub, depcomp -- as symlinks
# into /usr/share/automake-X.Y and /usr/share/libtool. The version is in that
# path, so the next automake upgrade deletes the directory the links point at
# and every one of them dangles. The tree then fails in ways that do not name
# the cause: configure reports "cannot find required auxiliary files", make
# check dies with "test-driver: No such file or directory", and rsync refuses
# the tree with "symlink has no referent". A distribution that upgrades on a
# six-month cadence hits this roughly every six months.
#
# With --copy they are real files, so the tree keeps working across upgrades and
# survives being copied to another machine.
echo -n 'Running libtoolize...'
if [ `uname -s` = Darwin ]
then
    LIBTOOLIZE=glibtoolize
else
    LIBTOOLIZE=libtoolize
fi

if $LIBTOOLIZE --force --copy > /dev/null 2>&1; then

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
if automake --add-missing --copy > /dev/null 2>&1; then
	echo 'done.'
else
	echo 'failed.'
	exit 1
fi

echo 'done'
