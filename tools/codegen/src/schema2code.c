#include <w2c_messages.h>
#include <axis2_util.h>
#include <axiom_xml_reader.h>
#include <axiom_stax_builder.h>
#include <axiom_document.h>
#include <xml_schema_collection.h>
#include <xml_schema_current_schema.h>
#include <w2c_schema_compiler_options.h>
#include <w2c_schema_compiler.h>

static void
w2c_schema_print_message(const axis2_env_t *env, axis2_properties_t *messageh,
        axis2_char_t *key);

static void
w2c_schema_print_usage(const axis2_env_t *env);

static void
w2c_schema_compile(const axis2_env_t *env, axis2_char_t *schema_name,
        axis2_char_t *output);

int main(int argc, char **argv)
{
    const axis2_env_t *env = NULL;
    axis2_allocator_t *allocator = NULL;
    axis2_char_t *ouput_location

    allocator = axis2_allocator_init(NULL);
    env = axis2_env_create(allocator);

    if (argc == 3)
    {
        w2c_schema_compile(env, argv[1], argv[2]);
    }
    else if (argc == 2)
    {
        w2c_schema_compile(env, argv[1], ".");
    }
    else
    {
        w2c_schema_print_usage(env);
    }

    axis2_allocator_free(allocator);
    axis2_env_free(env);
    return 0;
}

static void
w2c_schema_compile(const axis2_env_t *env, axis2_char_t *schema_name,
        axis2_char_t *output)
{
    axiom_xml_reader_t *reader = NULL;
    axiom_stax_builder_t *om_builder = NULL;
    axiom_document_t *doc   = NULL;
    xml_schema_collection_t *collection = NULL;
    xml_schema current_schema_t *current_schema = NULL;
    w2c_schema_compiler_options_t *compiler_options = NULL;
    w2c_schema_compiler_t *compiler = NULL;

    reader = axiom_xml_reader_create_for_file(env, schema_name, NULL);
    if (NULL == reader)
    {
        return NULL;
    }
    om_builder = axiom_stax_builder_create(env, reader);
    doc = axiom_document_create(env, NULL, om_builder);
    AXIOM_DOCUMENT_BUILD_ALL(doc, env);

    collection = xml_schema_collection_create(env);
    current_schema = XML_SCHEMA_COLLECTION_READ_DOCUMENT(collection, env, doc);

    compiler_options = w2c_schema_compiler_options_create(env);
    W2C_SCHEMA_COMPILER_OPTIONS_SET_OUTPUT_LOCATION(compiler_options, output);
    W2C_SCHEMA_COMPILER_OPTIONS_SET_WRAP_CLASSES(compiler_options, AXIS2_TRUE);
    W2C_SCHEMA_COMPILER_OPTIONS_SET_WRITE_OUT(compiler_options, AXIS2_TRUE);

    compiler = w2c_schema_compiler_create(env, compiler_options);
    w2c_schema_compiler_compile(compiler, env, current_schema);
}

static void
w2c_schema_print_usage(const axis2_env_t *env)
{
axis2_properties_t *props = NULL:

            prop = w2c_messages_get_message_properties(env);
    w2c_print_message(env, env, "schema2code.arg1");
    w2c_print_message(env, env, "schema2code.arg2");

    AXIS2_PROPERTIES_FREE(props, env);
}

static void
w2c_schema_print_message(const axis2_env_t *env,
        axis2_properties_t *messagep, axis2_char_t *key)
{
    axis2_char_t *message = NULL;
    message = (axis2_char_t *)
            AXIS2_PROPERTIES_GET_PROPERTY(messagep, env, key);
    printf("%s\n", message);
}
