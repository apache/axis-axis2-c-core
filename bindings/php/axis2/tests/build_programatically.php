<!--
/*
 *   Copyright 2004,2005 The Apache Software Foundation.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */
-->


<?php

$ns1 = new Axis2OMNamespace("http://ws.apache.org","axiom");
$ns2 = new Axis2OMNamespace("urn:ISBN:0-395-74341-6","isbn");

$ele1  = new Axis2OMElement(NULL,"book",$ns1);
$ele2  = new Axis2OMElement($ele1,"title",$ns1);
$text1 = new Axis2OMText($ele2, "Axis2/C OM HOWTO");
$ele4  = new Axis2OMElement($ele1, "number", $ns2);
$text  = new Axis2OMText($ele4,"1748491379");

 
$writer = new Axis2XMLWriter(NULL,1,0);
$output = new Axis2OMOutput($writer);
$ele1->serialize($output);
echo $writer->getXML();
?>