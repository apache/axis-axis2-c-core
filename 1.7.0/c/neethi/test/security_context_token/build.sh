#!/bin/bash

gcc security_context_token_test.c ../../../util/test/util/create_env.c -Werror -g -I$AXIS2C_HOME/include/axis2-1.2 -L$AXIS2C_HOME/lib -lneethi -laxutil -laxis2_axiom -laxis2_parser -o security_context_token 
