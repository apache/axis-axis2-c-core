<?php
$writer = new Axis2XMLWriter(NULL,1,0);
$output = new Axis2OMOutput($writer);
$ns1= new Axis2OMNamespace("http://test.org","testprefix");
$element = new Axis2OMElement(NULL,"ThisIsATest",$ns1);
$attr1 = new Axis2OMAttribute("key","value",NULL);
$element1 = new Axis2OMElement(NULL,"Test1",NULL);
$element1->addAttribute($attr1);
$text = new Axis2OMText(NULL,"This is text");
$element->addChild($text);


$element->addChild($element1);
$element->serialize($output);
echo $writer->getXML();


?>