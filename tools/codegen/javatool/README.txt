WSDL2C.sh/bat
=========

This file is to simplify the C code generation using WSDL2C java tool. 

How to use
----------
1. Set AXIS2_HOME environment variable to your axis2 java installation directory . 
   eg: 'export AXIS2_HOME=/home/axis2java' 
3. run WSDL2C.sh giving WSDL2C command line arguments as the arguments to the shell script.

Note: Same for the bat file as wel. 


Examples
--------

To generate a server skeleton in C:
	sh WSDL2C.sh -uri interoptestdoclitparameters.wsdl -ss -sd -d adb -u
    or  WSDL2C.bat -uri interoptestdoclitparameters.wsdl -ss -sd -d adb -u

To generate a client stub in C: 

	sh WSDL2C.sh -uri interoptestdoclitparameters.wsdl -d adb -u 
    or  WSDL2C.bat -uri interoptestdoclitparameters.wsdl -d adb -u 

