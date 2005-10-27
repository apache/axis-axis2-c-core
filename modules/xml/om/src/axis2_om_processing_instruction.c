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

#include <axis2_om_processing_instruction.h>

axis2_status_t AXIS2_CALL axis2_om_processing_instruction_impl_free (axis2_environment_t
                                                          * environment,
                                                          axis2_om_processing_instruction_t
                                                          *
                                                          processing_instruction);

axis2_om_processing_instruction_t *
AXIS2_CALL axis2_om_processing_instruction_create (axis2_environment_t * environment,
                                        axis2_om_node_t * parent,
                                        const axis2_char_t * target,
                                        const axis2_char_t * value,
                                        axis2_om_node_t ** node)
{
    axis2_om_processing_instruction_t *processing_instruction = NULL;

    if (!node || !target || !value)
    {
        environment->error->errorno = AXIS2_ERROR_INVALID_NULL_PARAMETER;
        return NULL;
    }

    *node = axis2_om_node_create (environment);
    if (!*node)
    {
        environment->error->errorno = AXIS2_ERROR_NO_MEMORY;
        return NULL;
    }

    processing_instruction =
        (axis2_om_processing_instruction_t *) axis2_malloc (environment->
                                                            allocator,
                                                            sizeof
                                                            (axis2_om_processing_instruction_t));
    if (!processing_instruction)
    {
        axis2_om_node_free (environment, *node);
        environment->error->errorno = AXIS2_ERROR_NO_MEMORY;
        return NULL;
    }

    processing_instruction->value = NULL;

    if (value)
    {
        processing_instruction->value =
            axis2_strdup (environment->string, value);
        if (!processing_instruction->value)
        {
            axis2_free (environment->allocator, processing_instruction);
            axis2_om_node_free (environment, *node);
            environment->error->errorno = AXIS2_ERROR_NO_MEMORY;
            return NULL;
        }
    }

    processing_instruction->target = NULL;

    if (target)
    {
        processing_instruction->target =
            axis2_strdup (environment->string, target);
        if (!processing_instruction->target)
        {
            axis2_free (environment->allocator,
                        processing_instruction->value);
            axis2_free (environment->allocator, processing_instruction);
            axis2_om_node_free (environment, *node);
            environment->error->errorno = AXIS2_ERROR_NO_MEMORY;
            return NULL;
        }
    }

    (*node)->data_element = processing_instruction;
    (*node)->node_type = AXIS2_OM_PROCESSING_INSTRUCTION;

    if (parent)
    {
        (*node)->parent = parent;
        axis2_om_node_add_child (environment, parent, (*node));
    }

    /* operations */
    processing_instruction->ops = NULL;
    processing_instruction->ops =
        (axis2_om_processing_instruction_ops_t *) axis2_malloc (environment->
                                                                allocator,
                                                                sizeof
                                                                (axis2_om_processing_instruction_ops_t));
    if (!processing_instruction->ops)
    {
        axis2_free (environment->allocator, processing_instruction->value);
        axis2_free (environment->allocator, processing_instruction->target);
        axis2_free (environment->allocator, processing_instruction);
        axis2_om_node_free (environment, *node);
        environment->error->errorno = AXIS2_ERROR_NO_MEMORY;
        return NULL;
    }

    processing_instruction->ops->axis2_om_processing_instruction_ops_free =
        axis2_om_processing_instruction_impl_free;

    return processing_instruction;
}

axis2_status_t AXIS2_CALL
axis2_om_processing_instruction_impl_free (axis2_environment_t * environment,
                                           axis2_om_processing_instruction_t *
                                           processing_instruction)
{
    if (processing_instruction)
    {
        if (processing_instruction->value)
        {
            axis2_free (environment->allocator,
                        processing_instruction->value);
        }

        if (processing_instruction->target)
        {
            axis2_free (environment->allocator,
                        processing_instruction->target);
        }

        axis2_free (environment->allocator, processing_instruction);
        return AXIS2_SUCCESS;
    }
    return AXIS2_FAILURE;
}
