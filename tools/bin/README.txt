WSDL2C.sh
=========

This file is to simplify the C code generation using WSDL2C java tool. 

How to use
----------
1. Open WSDL2C.sh
2. Set your axis2_java home directory in the line 'export AXIS2_HOME=/home/axis2java' and save it.
3. run WSDL2C.sh giving WSDL2C command line arguments as the arguments to the shell script.


Examples
--------

To generate a server skeleton in C:
	sh WSDL2C.sh -uri interoptestdoclitparameters.wsdl -ss -sd -d adb -u

To generate a client stub in C: 

	sh WSDL2C.sh -l c -uri interoptestdoclitparameters.wsdl -d adb -u 


