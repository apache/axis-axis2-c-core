#!/bin/bash

for i in '*.xml'
do
echo replacing $i
sed -i 's/user/'$1'/g' $i
done
