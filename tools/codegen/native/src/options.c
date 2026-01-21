/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "wsdl2c_native.h"
#include "schema_loader.h"
#include <string.h>

/* Note: wsdl2c_operation_t is now defined in wsdl2c_native.h */

/* Message structures (internal to parser/options) */
typedef struct wsdl2c_message {
    axis2_char_t *name;
    axutil_array_list_t *parts;
} wsdl2c_message_t;

typedef struct wsdl2c_message_part {
    axis2_char_t *name;
    axis2_char_t *type;
    axis2_char_t *element;
} wsdl2c_message_part_t;

/* Context creation and destruction */
AXIS2_EXTERN wsdl2c_context_t* AXIS2_CALL
wsdl2c_context_create(const axutil_env_t *env)
{
    wsdl2c_context_t *context = NULL;

    AXIS2_PARAM_CHECK(env->error, env, NULL);

    context = AXIS2_MALLOC(env->allocator, sizeof(wsdl2c_context_t));
    if (!context) {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    memset(context, 0, sizeof(wsdl2c_context_t));
    context->env = env;

    /* Initialize options with defaults */
    context->options = AXIS2_MALLOC(env->allocator, sizeof(wsdl2c_options_t));
    if (!context->options) {
        AXIS2_FREE(env->allocator, context);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    memset(context->options, 0, sizeof(wsdl2c_options_t));

    /* Set default values */
    context->options->language = axutil_strdup(env, "c");
    context->options->databinding = axutil_strdup(env, "adb");
    context->options->output_dir = axutil_strdup(env, ".");
    context->options->server_side = AXIS2_FALSE;
    context->options->service_descriptor = AXIS2_FALSE;
    context->options->generate_test = AXIS2_FALSE;
    context->options->unwrap = AXIS2_FALSE;
    context->options->generate_all = AXIS2_FALSE;
    context->options->async_only = AXIS2_FALSE;
    context->options->sync_only = AXIS2_FALSE;

    return context;
}

AXIS2_EXTERN void AXIS2_CALL
wsdl2c_context_free(wsdl2c_context_t *context, const axutil_env_t *env)
{
    if (!context) {
        return;
    }

    if (context->options) {
        if (context->options->wsdl_uri) {
            AXIS2_FREE(env->allocator, context->options->wsdl_uri);
        }
        if (context->options->output_dir) {
            AXIS2_FREE(env->allocator, context->options->output_dir);
        }
        if (context->options->package_name) {
            AXIS2_FREE(env->allocator, context->options->package_name);
        }
        if (context->options->language) {
            AXIS2_FREE(env->allocator, context->options->language);
        }
        if (context->options->databinding) {
            AXIS2_FREE(env->allocator, context->options->databinding);
        }
        if (context->options->prefix) {
            AXIS2_FREE(env->allocator, context->options->prefix);
        }
        AXIS2_FREE(env->allocator, context->options);
    }

    if (context->wsdl) {
        /* Free WSDL structure fields */
        if (context->wsdl->target_namespace) {
            AXIS2_FREE(env->allocator, context->wsdl->target_namespace);
        }
        if (context->wsdl->service_name) {
            AXIS2_FREE(env->allocator, context->wsdl->service_name);
        }
        if (context->wsdl->port_type_name) {
            AXIS2_FREE(env->allocator, context->wsdl->port_type_name);
        }
        if (context->wsdl->binding_name) {
            AXIS2_FREE(env->allocator, context->wsdl->binding_name);
        }

        /* Free operations array and its contents */
        if (context->wsdl->operations) {
            int i, size = axutil_array_list_size(context->wsdl->operations, env);
            for (i = 0; i < size; i++) {
                wsdl2c_operation_t* operation = (wsdl2c_operation_t*)axutil_array_list_get(context->wsdl->operations, env, i);
                if (operation) {
                    if (operation->name) {
                        AXIS2_FREE(env->allocator, operation->name);
                    }
                    if (operation->input_message) {
                        AXIS2_FREE(env->allocator, operation->input_message);
                    }
                    if (operation->output_message) {
                        AXIS2_FREE(env->allocator, operation->output_message);
                    }
                    if (operation->soap_action) {
                        AXIS2_FREE(env->allocator, operation->soap_action);
                    }
                    AXIS2_FREE(env->allocator, operation);
                }
            }
            axutil_array_list_free(context->wsdl->operations, env);
        }

        /* Free messages array and its contents */
        if (context->wsdl->messages) {
            int i, size = axutil_array_list_size(context->wsdl->messages, env);
            for (i = 0; i < size; i++) {
                wsdl2c_message_t* message = (wsdl2c_message_t*)axutil_array_list_get(context->wsdl->messages, env, i);
                if (message) {
                    if (message->name) {
                        AXIS2_FREE(env->allocator, message->name);
                    }

                    /* Free message parts array and its contents */
                    if (message->parts) {
                        int j, parts_size = axutil_array_list_size(message->parts, env);
                        for (j = 0; j < parts_size; j++) {
                            wsdl2c_message_part_t* part = (wsdl2c_message_part_t*)axutil_array_list_get(message->parts, env, j);
                            if (part) {
                                if (part->name) {
                                    AXIS2_FREE(env->allocator, part->name);
                                }
                                if (part->type) {
                                    AXIS2_FREE(env->allocator, part->type);
                                }
                                if (part->element) {
                                    AXIS2_FREE(env->allocator, part->element);
                                }
                                AXIS2_FREE(env->allocator, part);
                            }
                        }
                        axutil_array_list_free(message->parts, env);
                    }
                    AXIS2_FREE(env->allocator, message);
                }
            }
            axutil_array_list_free(context->wsdl->messages, env);
        }

        /* AXIS2C-1197 FIX: Free schema_elements array and its contents */
        if (context->wsdl->schema_elements) {
            int i, size = axutil_array_list_size(context->wsdl->schema_elements, env);
            for (i = 0; i < size; i++) {
                wsdl2c_schema_element_t* element = (wsdl2c_schema_element_t*)axutil_array_list_get(
                    context->wsdl->schema_elements, env, i);
                if (element) {
                    if (element->name) {
                        AXIS2_FREE(env->allocator, element->name);
                    }
                    if (element->c_name) {
                        AXIS2_FREE(env->allocator, element->c_name);
                    }
                    if (element->type) {
                        AXIS2_FREE(env->allocator, element->type);
                    }
                    if (element->c_type) {
                        AXIS2_FREE(env->allocator, element->c_type);
                    }
                    if (element->namespace_uri) {
                        AXIS2_FREE(env->allocator, element->namespace_uri);
                    }
                    AXIS2_FREE(env->allocator, element);
                }
            }
            axutil_array_list_free(context->wsdl->schema_elements, env);
        }

        /* AXIS2C-1197 FIX: Free complex_types array and its contents */
        if (context->wsdl->complex_types) {
            int i, size = axutil_array_list_size(context->wsdl->complex_types, env);
            for (i = 0; i < size; i++) {
                wsdl2c_complex_type_t* complex_type = (wsdl2c_complex_type_t*)axutil_array_list_get(
                    context->wsdl->complex_types, env, i);
                if (complex_type) {
                    if (complex_type->name) {
                        AXIS2_FREE(env->allocator, complex_type->name);
                    }
                    if (complex_type->c_name) {
                        AXIS2_FREE(env->allocator, complex_type->c_name);
                    }
                    if (complex_type->base_type) {
                        AXIS2_FREE(env->allocator, complex_type->base_type);
                    }
                    /* Free elements within complex type */
                    if (complex_type->elements) {
                        int j, elem_size = axutil_array_list_size(complex_type->elements, env);
                        for (j = 0; j < elem_size; j++) {
                            wsdl2c_schema_element_t* elem = (wsdl2c_schema_element_t*)axutil_array_list_get(
                                complex_type->elements, env, j);
                            if (elem) {
                                if (elem->name) {
                                    AXIS2_FREE(env->allocator, elem->name);
                                }
                                if (elem->c_name) {
                                    AXIS2_FREE(env->allocator, elem->c_name);
                                }
                                if (elem->type) {
                                    AXIS2_FREE(env->allocator, elem->type);
                                }
                                if (elem->c_type) {
                                    AXIS2_FREE(env->allocator, elem->c_type);
                                }
                                if (elem->namespace_uri) {
                                    AXIS2_FREE(env->allocator, elem->namespace_uri);
                                }
                                AXIS2_FREE(env->allocator, elem);
                            }
                        }
                        axutil_array_list_free(complex_type->elements, env);
                    }
                    AXIS2_FREE(env->allocator, complex_type);
                }
            }
            axutil_array_list_free(context->wsdl->complex_types, env);
        }

        /* Note: schema_node cleanup would depend on its actual type */
        /* Free the WSDL structure itself */
        AXIS2_FREE(env->allocator, context->wsdl);
    }

    /* Free the schema registry (external schema import support) */
    if (context->schema_registry) {
        wsdl2c_schema_registry_free(context->schema_registry, env);
    }

    if (context->doc) {
        xmlFreeDoc(context->doc);
    }

    AXIS2_FREE(env->allocator, context);
}

/* Print usage information */
AXIS2_EXTERN void AXIS2_CALL
wsdl2c_print_usage(void)
{
    printf("Usage: wsdl2c-native [options] -uri <url or path>\n");
    printf("\nwhere [options] include:\n");
    printf("  -o <path>                Specify a directory path for the generated code.\n");
    printf("  -a                       Generate async style code only (Default: off).\n");
    printf("  -s                       Generate sync style code only (Default: off). Takes precedence over -a.\n");
    printf("  -p <pkg1>                Specify a custom package name for the generated code.\n");
    printf("  -prefix <string>         Prefix for all generated filenames and identifiers (AXIS2C-1330).\n");
    printf("                           Use this to avoid name clashes when generating code from\n");
    printf("                           multiple WSDLs that have identical type names.\n");
    printf("                           Example: -prefix Mapping_ generates Mapping_adb_Coordinate.c\n");
    printf("  -l <language>            Valid languages are c (Default: c).\n");
    printf("  -t                       Generate a test case for the generated code.\n");
    printf("  -ss                      Generate server side code (i.e. skeletons) (Default: off).\n");
    printf("  -sd                      Generate service descriptor (i.e. services.xml). (Default: off). Valid with -ss.\n");
    printf("  -d <databinding>         Valid databinding(s) are adb, none (Default: adb).\n");
    printf("  -g                       Generates all the classes. It generates clases for both client side and server side.\n");
    printf("  -u                       Unpacks the databinding classes / Unwrap parameters.\n");
    printf("  -uri <url or path>       A url or path to a WSDL (Required).\n");
    printf("  -h, --help               Show this help message.\n");
    printf("  -v, --version            Show version information.\n");
    printf("\nThis is the native C implementation of WSDL2C, targeting compatibility with Axis2/Java 2.0.1-SNAPSHOT output.\n");
}

/* Parse command line options */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
wsdl2c_parse_options(wsdl2c_context_t *context, int argc, char **argv, const axutil_env_t *env)
{
    AXIS2_PARAM_CHECK(env->error, context, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, argv, AXIS2_FAILURE);

    /* Manual argument parsing to handle non-standard options like -uri, -ss, -sd */
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-uri") == 0 && i + 1 < argc) {
            context->options->wsdl_uri = axutil_strdup(env, argv[i + 1]);
            i++; /* Skip the URI argument */
        } else if (strcmp(argv[i], "-ss") == 0) {
            context->options->server_side = AXIS2_TRUE;
        } else if (strcmp(argv[i], "-sd") == 0) {
            context->options->service_descriptor = AXIS2_TRUE;
        } else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            if (context->options->output_dir) {
                AXIS2_FREE(env->allocator, context->options->output_dir);
            }
            context->options->output_dir = axutil_strdup(env, argv[i + 1]);
            i++; /* Skip the argument */
        } else if (strcmp(argv[i], "-d") == 0 && i + 1 < argc) {
            if (context->options->databinding) {
                AXIS2_FREE(env->allocator, context->options->databinding);
            }
            context->options->databinding = axutil_strdup(env, argv[i + 1]);
            if (strcmp(argv[i + 1], "adb") != 0 && strcmp(argv[i + 1], "none") != 0) {
                fprintf(stderr, "Error: Only 'adb' and 'none' databinding are supported\n");
                return AXIS2_FAILURE;
            }
            i++; /* Skip the argument */
        } else if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            if (context->options->package_name) {
                AXIS2_FREE(env->allocator, context->options->package_name);
            }
            context->options->package_name = axutil_strdup(env, argv[i + 1]);
            i++; /* Skip the argument */
        } else if (strcmp(argv[i], "-prefix") == 0 && i + 1 < argc) {
            /* AXIS2C-1330 FIX: Prefix option to prevent name clashes */
            if (context->options->prefix) {
                AXIS2_FREE(env->allocator, context->options->prefix);
            }
            context->options->prefix = axutil_strdup(env, argv[i + 1]);
            i++; /* Skip the argument */
        } else if (strcmp(argv[i], "-l") == 0 && i + 1 < argc) {
            if (context->options->language) {
                AXIS2_FREE(env->allocator, context->options->language);
            }
            context->options->language = axutil_strdup(env, argv[i + 1]);
            if (strcmp(argv[i + 1], "c") != 0) {
                fprintf(stderr, "Error: Only 'c' language is supported by this native implementation\n");
                return AXIS2_FAILURE;
            }
            i++; /* Skip the argument */
        } else if (strcmp(argv[i], "-u") == 0) {
            context->options->unwrap = AXIS2_TRUE;
        } else if (strcmp(argv[i], "-a") == 0) {
            context->options->async_only = AXIS2_TRUE;
        } else if (strcmp(argv[i], "-s") == 0) {
            context->options->sync_only = AXIS2_TRUE;
        } else if (strcmp(argv[i], "-t") == 0) {
            context->options->generate_test = AXIS2_TRUE;
        } else if (strcmp(argv[i], "-g") == 0) {
            context->options->generate_all = AXIS2_TRUE;
        } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            wsdl2c_print_usage();
            return AXIS2_FAILURE; /* Exit after showing help */
        } else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
            printf("wsdl2c-native version 1.0.0\n");
            printf("Native C implementation of WSDL2C code generator\n");
            printf("Compatible with Axis2/Java 2.0.1-SNAPSHOT output format\n");
            return AXIS2_FAILURE; /* Exit after showing version */
        } else if (argv[i][0] == '-') {
            fprintf(stderr, "Unknown option: %s. Use -h for help.\n", argv[i]);
            return AXIS2_FAILURE;
        }
    }


    /* Validate required options */
    if (!context->options->wsdl_uri) {
        fprintf(stderr, "Error: WSDL URI is required. Use -uri <path_to_wsdl>\n");
        return AXIS2_FAILURE;
    }

    /* Validate option combinations */
    if (context->options->service_descriptor && !context->options->server_side) {
        fprintf(stderr, "Warning: -sd option is only valid with -ss (server side generation)\n");
        context->options->service_descriptor = AXIS2_FALSE;
    }

    if (context->options->sync_only && context->options->async_only) {
        fprintf(stderr, "Warning: -s takes precedence over -a\n");
        context->options->async_only = AXIS2_FALSE;
    }

    return AXIS2_SUCCESS;
}

/* Utility function implementations for standalone mode */

/* Wrapper functions to match Axis2 allocator signatures */
static void* standalone_malloc(void *allocator, size_t size) {
    (void)allocator; /* Unused in standalone mode */
    return malloc(size);
}

static void* standalone_realloc(void *allocator, void *ptr, size_t size) {
    (void)allocator; /* Unused in standalone mode */
    return realloc(ptr, size);
}

static void standalone_free(void *allocator, void *ptr) {
    (void)allocator; /* Unused in standalone mode */
    free(ptr);
}

AXIS2_EXTERN axutil_env_t* AXIS2_CALL
axutil_env_create_all(const char *log_file, int log_level)
{
    axutil_env_t *env = NULL;
    axutil_allocator_t *allocator = NULL;
    axutil_error_t *error = NULL;
    axutil_log_t *log = NULL;

    (void)log_file; /* Unused parameter */

    /* Simple allocator implementation */
    allocator = malloc(sizeof(axutil_allocator_t));
    if (!allocator) {
        return NULL;
    }
    allocator->malloc_fn = standalone_malloc;
    allocator->realloc_fn = standalone_realloc;
    allocator->free_fn = standalone_free;

    /* Simple error structure */
    error = malloc(sizeof(axutil_error_t));
    if (!error) {
        free(allocator);
        return NULL;
    }
    error->error_number = 0;
    error->status_code = 0;

    /* Simple log structure */
    log = malloc(sizeof(axutil_log_t));
    if (!log) {
        free(error);
        free(allocator);
        return NULL;
    }
    log->level = log_level;

    /* Environment structure */
    env = malloc(sizeof(axutil_env_t));
    if (!env) {
        free(log);
        free(error);
        free(allocator);
        return NULL;
    }
    env->allocator = allocator;
    env->error = error;
    env->log = log;

    return env;
}

AXIS2_EXTERN void AXIS2_CALL
axutil_env_free(axutil_env_t *env)
{
    if (env) {
        if (env->log) {
            free(env->log);
        }
        if (env->error) {
            free(env->error);
        }
        if (env->allocator) {
            free(env->allocator);
        }
        free(env);
    }
}

AXIS2_EXTERN char* AXIS2_CALL
axutil_strdup(const axutil_env_t *env, const char *str)
{
    char *result = NULL;
    size_t len;

    if (!str) {
        return NULL;
    }

    len = strlen(str) + 1;
    if (env && env->allocator && env->allocator->malloc_fn) {
        result = env->allocator->malloc_fn(env->allocator, len);
    } else {
        result = malloc(len);
    }

    if (result) {
        strcpy(result, str);
    }

    return result;
}

AXIS2_EXTERN char* AXIS2_CALL
axutil_string_toupper(const char *str, const axutil_env_t *env)
{
    char *result = NULL;
    int i;

    if (!str) {
        return NULL;
    }

    /* Create a copy of the string */
    result = axutil_strdup(env, str);
    if (!result) {
        return NULL;
    }

    /* Convert to uppercase in place */
    for (i = 0; result[i]; i++) {
        if (result[i] >= 'a' && result[i] <= 'z') {
            result[i] = result[i] - 'a' + 'A';
        }
    }

    return result;
}

#ifdef STANDALONE_BUILD
/* Simple array list implementation for standalone mode */
axutil_array_list_t* axutil_array_list_create(const axutil_env_t *env, int initial_capacity)
{
    axutil_array_list_t *list = NULL;

    /* Parameter 'env' maintains API compatibility with full Axis2/C library
     * In full build: used for error handling and memory allocation tracking
     * In standalone: unused but keeps same function signature for consistency */
    (void)env;

    if (initial_capacity <= 0) {
        initial_capacity = 10;
    }

    list = malloc(sizeof(axutil_array_list_t));
    if (!list) {
        return NULL;
    }

    list->data = malloc(sizeof(void*) * initial_capacity);
    if (!list->data) {
        free(list);
        return NULL;
    }

    list->size = 0;
    list->capacity = initial_capacity;

    return list;
}

axis2_status_t axutil_array_list_add(axutil_array_list_t *list, const axutil_env_t *env, void *element)
{
    /* Parameter 'env' maintains API compatibility with full Axis2/C library
     * In full build: used for error handling and logging operations
     * In standalone: unused but preserves identical calling convention */
    (void)env;

    if (!list) {
        return AXIS2_FAILURE;
    }

    if (list->size >= list->capacity) {
        /* Expand array */
        int new_capacity = list->capacity * 2;
        void **new_data = realloc(list->data, sizeof(void*) * new_capacity);
        if (!new_data) {
            return AXIS2_FAILURE;
        }
        list->data = new_data;
        list->capacity = new_capacity;
    }

    list->data[list->size] = element;
    list->size++;

    return AXIS2_SUCCESS;
}

void axutil_array_list_free(axutil_array_list_t *list, const axutil_env_t *env)
{
    /* Parameter 'env' maintains API compatibility with full Axis2/C library
     * In full build: used for proper memory deallocation through env->allocator
     * In standalone: uses standard free() but keeps same function signature */
    (void)env;

    if (list) {
        if (list->data) {
            free(list->data);
        }
        free(list);
    }
}

int axutil_array_list_size(axutil_array_list_t *list, const axutil_env_t *env)
{
    /* Parameter 'env' maintains API compatibility with full Axis2/C library
     * In full build: used for parameter validation and error reporting
     * In standalone: simple bounds check but same function signature */
    (void)env;

    if (!list) {
        return 0;
    }

    return list->size;
}

void* axutil_array_list_get(axutil_array_list_t *list, const axutil_env_t *env, int index)
{
    /* Parameter 'env' maintains API compatibility with full Axis2/C library
     * In full build: used for bounds checking and error logging
     * In standalone: basic validation but identical function interface */
    (void)env;

    if (!list || index < 0 || index >= list->size) {
        return NULL;
    }

    return list->data[index];
}
#endif