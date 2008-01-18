#!/bin/bash

gcc uri_test.c ../util/create_env.c -g -I$WSFC_HOME/include/axis2-1.2 -L$WSFC_HOME/lib -laxutil -laxis2_axiom -laxis2_parser -o uri_test 
