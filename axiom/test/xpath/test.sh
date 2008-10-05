clear

export AXIS2C_HOME="/usr/local/axis2c"

echo 'Compiling...'

gcc -o test -I$AXIS2C_HOME/include/axis2-1.5.0/ -L$AXIS2C_HOME/lib -I../include -I../src -laxutil -laxis2_axiom -laxis2_parser -laxis2_engine -lpthread -laxis2_http_sender -laxis2_http_receiver test_linux.c ../src/*.c -ldl -Wl,--rpath -Wl,$AXIS2C_HOME/lib

echo 'Running...'

./test
