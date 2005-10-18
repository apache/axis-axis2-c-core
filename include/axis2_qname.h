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

#ifndef AXIS2_QNAME_H
#define AXIS2_QNAME_H

/**
 * @file axis2_qname.h
 * @brief represents a qualified name
 */

#include <axis2_defines.h>
#include <axis2_environment.h>


static const axis2_char_t XML_NAMESPACE_URI[] ="http://www.w3c.org/XML/1998/namespace";

struct axis2_qname;
struct axis2_qname_ops;

typedef struct axis2_qname_ops
{
    /**
     *  Free a qname struct
     *  @return Status code
     */ 
     axis2_status_t (*axis2_qname_ops_free)(axis2_environment_t *environment,struct axis2_qname *qname);
     
     /** 
      * Compare two qnames
      * prefix is ignored when comparing
      * If ns_uri and localpart of qname1 and qname2 is equal returns true
      * @return true if qname1 equals qname2, false otherwise 
      */

     axis2_bool_t (*axis2_qname_ops_equals)(axis2_environment_t *environment,struct axis2_qname *qname1,struct axis2_qname *qname2);

}axis2_qname_ops_t;

typedef struct axis2_qname
{
    /** operations related to qname */
     axis2_qname_ops_t *ops;
     
    /** localpart of qname is mandatory */
    
    axis2_char_t *localpart;
    
    /** namespace uri is optional */
    axis2_char_t *namespace_uri;
    
    /**  prefix mandatory */
    axis2_char_t *prefix;
   
}axis2_qname_t;

/**
 *	creates a qname struct
 *  returns a pointer to a qname struct
 *  @localpart   mandatory
 *  @prefix      mandatory  
 *  @ns_uri      optional
 *  The prefix. Must not be null. Use "" (empty string) to indicate that no namespace URI is present or the namespace URI is not relevant
 *  if null is passed for prefix and uri , "'(empty string ) will be assinged to those fields
 * @return a pointer to newly created qname struct
 */

axis2_qname_t *axis2_qname_create(axis2_environment_t *environment ,const axis2_char_t *localpart,const axis2_char_t *namespace_uri, const axis2_char_t *prefix);


#define axis2_qname_free(environment,qname) ((qname)->ops->axis2_qname_ops_free(environment,qname))
#define axis2_qname_equals(environment,qname1,qname2) ((qname1)->ops->axis2_qname_ops_equals(environment,qname1,qname2))

#endif /* AXIS2_QNAME_H */
