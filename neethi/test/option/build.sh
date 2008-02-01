#!/bin/bash

gcc options_test.c ../../../util/test/util/create_env.c -g -I$AXIS2C_HOME/include/axis2-1.2 -L$AXIS2C_HOME/lib -lneethi_util -laxutil -laxis2_axiom -laxis2_parser -o options_test 
