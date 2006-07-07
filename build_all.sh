AXIS2C_HOME=`pwd`/deploy
AXIS2C=`pwd`
export AXIS2C_HOME AXIS2C

cd util

sh build.sh
if [ "$?" -ne 0 ]
then
exit 1
fi

cd ../axiom

sh build.sh
if [ "$?" -ne 0 ]
then
exit 1
fi

cd ../xml_schema

sh build.sh
if [ "$?" -ne 0 ]
then
exit 1
fi

cd ../woden
sh build.sh
if [ "$?" -ne 0 ]
then
exit 1
fi

cd ../
sh build.sh
if [ "$?" -ne 0 ]
then
exit 1
fi

cd samples

sh build.sh
if [ "$?" -ne 0 ]
then
exit 1
fi

cd ..
