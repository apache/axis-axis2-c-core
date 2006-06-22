AXIS2C_HOME=`pwd`/deploy
AXIS2C=`pwd`
export AXIS2C_HOME AXIS2C

cd util

./autogen.sh
if [ "$?" -ne 0 ]
then
exit 1
fi

./configure --prefix=${AXIS2C_HOME} --enable-static=no
if [ "$?" -ne 0 ]
then
exit 1
fi

make
if [ "$?" -ne 0 ]
then
exit 1
fi

make install
if [ "$?" -ne 0 ]
then
exit 1
fi

cd ../axiom

./autogen.sh
if [ "$?" -ne 0 ]
then
exit 1
fi

./configure --prefix=${AXIS2C_HOME} --enable-static=no --with-axis2_util=${AXIS2C_HOME}/include --enable-tests=yes
if [ "$?" -ne 0 ]
then
exit 1
fi

make
if [ "$?" -ne 0 ]
then
exit 1
fi

make install
if [ "$?" -ne 0 ]
then
exit 1
fi

cd ../xml_schema

./autogen.sh
if [ "$?" -ne 0 ]
then
exit 1
fi

./configure --prefix=${AXIS2C_HOME} --enable-static=no --with-axis2_util=${AXIS2C_HOME}/include --with-axiom=${AXIS2C_HOME}/include
if [ "$?" -ne 0 ]
then
exit 1
fi

make
if [ "$?" -ne 0 ]
then
exit 1
fi

make install
if [ "$?" -ne 0 ]
then
exit 1
fi

cd ..

./autogen.sh
if [ "$?" -ne 0 ]
then
exit 1
fi

./configure --prefix=${AXIS2C_HOME} --enable-tests=yes --enable-trace=yes  --with-apache2=/usr/include/apache2 --with-apr=/usr/include/apr-0 --enable-diclient=no --enable-static=no --enable-openssl=no --with-axis2_util=${AXIS2C}/util/include --with-axiom=${AXIS2C}/axiom/include
if [ "$?" -ne 0 ]
then
exit 1
fi

make
if [ "$?" -ne 0 ]
then
exit 1
fi

make install
if [ "$?" -ne 0 ]
then
exit 1
fi

cd samples

./autogen.sh
if [ "$?" -ne 0 ]
then
exit 1
fi

./configure --prefix=${AXIS2C_HOME} --with-axis2_util=${AXIS2C_HOME}/include --with-axiom=${AXIS2C_HOME}/include
if [ "$?" -ne 0 ]
then
exit 1
fi

make
if [ "$?" -ne 0 ]
then
exit 1
fi

make install
if [ "$?" -ne 0 ]
then
exit
fi

cd ..
