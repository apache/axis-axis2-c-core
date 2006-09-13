#include <stdio.h>
#include <guththila_buffer.h>
#include <guththila_reader.h>
#include <guththila_writer.h>
#include <guththila.h>
int main ()
{
  axis2_allocator_t *allocator = axis2_allocator_init (NULL);
  axis2_env_t *env = axis2_env_create (allocator);
  guththila_t *parser = guththila_create (env, NULL);
  guththila_writer_t *writer = guththila_writer_create_for_memory (env);

  guththila_create_xml_stream_writer_for_memory (env, parser);
  guththila_write_start_document (env, parser);
  guththila_write_start_element_with_prefix_and_namespace (env, parser, "my", "http://localhost", "person");
  guththila_write_start_element_with_prefix_and_namespace (env, parser, "my", "one");
  guththila_write_characters (env, parser, "test");
  guththila_write_end_element (env, parser);
  guththila_write_start_element (env, parser, "two");
  guththila_write_default_namespace (env, parser, "http://another.host.com");
  guththila_write_start_element (env, parser, "two.one");
  guththila_write_end_element (env, parser);
  guththila_write_end_document (env, parser);
  char *t = guththila_writer_get_buffer (env, parser->xsw->writer);
  printf ("%s \n", t);
  guththila_xml_writer_free (env, parser);
  guththila_free (env, parser);
  axis2_allocator_free (allocator);
  axis2_env_free (env);
  return 0;
}
