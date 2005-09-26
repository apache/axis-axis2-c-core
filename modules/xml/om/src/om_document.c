/*
 * Copyright 2004,2005 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "om_document.h"
#include "stdlib.h"

om_document_t *create_om_document()
{
	om_document_t *doc=(om_document_t *)malloc(sizeof(om_document_t));
	if(!doc)
	{
	
		//error handling should go hear
		
	}
	doc->first_child = NULL;
	doc->last_child  = NULL;
	doc->root_element= NULL;
	return doc;

}

