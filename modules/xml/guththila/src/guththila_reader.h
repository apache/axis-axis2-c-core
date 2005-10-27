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
 * @author Dinesh Premalal (xydinesh@gmail.com, premalwd@cse.mrt.ac.lk)	
 */


#ifndef GUTHTHILA_READER_H
#define GUTHTHILA_READER_H

#include <stdio.h>
#include <stdlib.h>
#include "guththila_buffer.h"
#include "guththila_environment.h"
#include "guththila_defines.h"

typedef struct guththila_reader_s
{
    FILE *fp;
} guththila_reader_t;

GUTHTHILA_DECLARE (guththila_reader_t *)
guththila_reader_create (guththila_environment_t * environment, FILE * fp);
GUTHTHILA_DECLARE (int)
guththila_reader_read (guththila_environment_t * environment,
                       guththila_char_t * buffer, int offset, int length,
                       guththila_reader_t * r);
GUTHTHILA_DECLARE (int)
guththila_reader_set_input_stream (guththila_environment_t * environment,
                                   guththila_reader_t * r, FILE * fp);
GUTHTHILA_DECLARE (void)
guththila_reader_free (guththila_environment_t * environment,
                       guththila_reader_t * r);

#endif /* GUTHTHILA_READER_H */
