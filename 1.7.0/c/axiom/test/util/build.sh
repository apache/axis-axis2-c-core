#!/bin/bash
gcc axiom_util_test.c ../../../util/test/util/create_env.c -g -I$AXIS2C_HOME/include/axis2-1.2 -L$AXIS2C_HOME/lib -laxutil -laxis2_axiom -laxis2_parser -o axiom
