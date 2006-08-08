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
 *
 *   
 */


#ifndef GUTHTHILA_NAMESPACE_H
#define GUTHTHILA_NAMESPACE_H
#include "guththila_token.h"

typedef struct guththila_namespace_s
{
    guththila_char_t *name;
    int length;
    guththila_char_t *uri;
    int lengthuri;
    
} guththila_namespace_t;

#endif /* GUTHTHILA_NAMESPACE_H */
