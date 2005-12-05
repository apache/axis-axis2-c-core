<?php
$reader  = new Axis2XMLReader("much_ado.xml");
$builder = new Axis2OMBuilder($reader);
$doc     = new Axis2OMDocument(NULL,$builder);
$root    = $doc->getRootElement();
if($root)
{
	echo "\n\n\nlocalname \t";
	echo $root->getLocalname();
	$ns = $root->getNamespace();
	echo "\n";
	if($ns)
	{
		echo "uri \n";
		echo $ns->getUri();
		echo "\n";
		echo $ns->getPrefix();
		echo "\n";
	}
}

$node = $doc->buildNext();
do{
	if($node == NULL)
		break;	
	
	if($node->getNodeType() == AXIS2_OM_ELEMENT)
	{
		echo $node->getLocalname();
		echo "\n";
		
	}
	if($node->getNodeType() == AXIS2_OM_TEXT)
	{
		echo "text value  \t";
		echo $node->getValue();
		echo "\n";
	}
	
	$node = $doc->buildNext();
		
 }while($node);

$writer = new Axis2XMLWriter(NULL, 1, 0);
$output = new Axis2OMOutput($writer);
$root->serialize($output);
echo "\n\n__________serialized output of xml file \n\n";
echo $writer->getXML();
echo "\n\n";
?>