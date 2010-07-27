#!/usr/bin/sh
# There is a Jira issue AXIS2C-1437 as following
#==================================
#On AIX, the dynamic are correctly built, but they are not installed by "make install". I only have .a and .la in $deploy_dir/lib. No .so. It looks like that the dynamic libraries are build with a ".so.0" extension instead of just ".so" 
#==================================
#A workaround script is provided under the same jira to manually copy .so.0 to .so. This is it

[[ -z "$deploy_dir" ]] && exit 2

cd ${1:-axis2c-src-1.6.0}

cp $(find . -name '*.so.0') $deploy_dir/lib
cd $deploy_dir/lib
for f in *.so.0
do
	x=$(basename $f .0)
	[[ -h $x ]] || ln -s -f $f $x
done
mv libaxis2_mod_addr.* ../modules/addressing
mv libaxis2_mod_log.* ../modules/logging
