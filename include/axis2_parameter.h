#ifndef AXIS2_PARAMETER_H
#define AXIS2_PARAMETER_H

#include <stdio.h>
#include <stdlib.h>
#include <axis2_om_element.h>
#include <axis2.h>
#include <axis2_errno.h>

/**
 * @file axis2_parameter.h
 * @brief Parameter
 */
typedef enum axis2_parameter_types_t {
    AXIS2_PARAMETER_INVALID = -1,
    AXIS2_PARAMETER_DOM = 10,
    AXIS2_PARAMETER_TEXT = 20,
    
} axis2_parameter_types_t;

/**
  * Paramter can be any thing it can be XML element with number of child elements
  */
typedef struct
{
    char *name;
    void *value;
    axis2_om_element_t *param_element;
    int type;
    boolean_t is_locked;
} axis2_parameter_t;

axis2_parameter_t *axis2_parameter_create ();

/*
 *	Create a param with name and value
 */
axis2_parameter_t *axis2_parameter_create_with_name_value (const char *name, char *value);

/* Memory deallocation */
void axis2_parameter_free (axis2_parameter_t * param);

void axis2_parameter_set_name (axis2_parameter_t * param, char *name);

char *axis2_parameter_get_name (axis2_parameter_t * param);

void axis2_parameter_set_value (axis2_parameter_t * param, void *value);

void *axis2_parameter_get_value (axis2_parameter_t * param);

boolean_t axis2_parameter_is_locked (axis2_parameter_t * param);

void axis2_parameter_set_locked (axis2_parameter_t * param,
                                  boolean_t locked);

void axis2_parameter_set_param_element (axis2_parameter_t * param,
                                         axis2_om_element_t * om_element);

axis2_parameter_t *axis2_parameter_get_param_element (axis2_parameter_t *
                                                        param);

void axis2_parameter_set_type (axis2_parameter_t * param, int type);

int axis2_parameter_get_type (axis2_parameter_t * param);

#endif /* AXIS2_PARAMETER_H */
