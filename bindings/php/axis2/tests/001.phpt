--TEST--
Check for axis2 presence
--SKIPIF--
<?php if (!extension_loaded("axis2")) print "skip"; ?>
--FILE--
<?php 
echo "axis2 extension is available";
/*
	you can add regression tests for your extension here

  the output of your test code has to be equal to the
  text in the --EXPECT-- section below for the tests
  to pass, differences between the output and the
  expected text are interpreted as failure

	see php5/README.TESTING for further information on
  writing regression tests
*/
?>
--EXPECT--
axis2 extension is available
