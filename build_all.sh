set -e
AXIS2C_HOME=`pwd`/deploy
AXIS2C=`pwd`
export AXIS2C_HOME AXIS2C
if [ `uname -s` = Darwin ]
then
  export DYLD_LIBRARY_PATH=${AXIS2C_HOME}/lib
else
  export LD_LIBRARY_PATH=${AXIS2C_HOME}/lib
fi

cd util
sh build.sh

cd ../axiom
sh build.sh

cd ../xml_schema
sh build.sh

cd ../woden
sh build.sh

cd ../
sh build.sh

