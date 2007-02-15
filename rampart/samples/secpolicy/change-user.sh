#!/bin/bash
if [ $# -ne 1 ]
then
    echo "Usage : $0 username"
    exit
fi

for i in '*.xml'
do
echo replacing $i
sed -i 's/user/'$1'/g' $i
done
