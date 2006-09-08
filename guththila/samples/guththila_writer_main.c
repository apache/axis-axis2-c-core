#include <stdio.h>
#include <guththila_buffer.h>
#include <guththila_reader.h>
#include <guththila_writer.h>
#include <guththila_xml_pull_parser.h>
int main ()
{
  axis2_allocator_t *allocator = axis2_allocator_init (NULL);
  axis2_env_t *env = axis2_env_create (allocator);
  guththila_xml_pull_parser_t *parser = guththila_xml_pull_parser_create (env, NULL);
  guththila_writer_t *writer = guththila_writer_create_for_memory (env);
/*   int c = 0; */
/*   c = guththila_writer_write (env, "test", 0, 5, writer); */
/* char *file = "<Template><Application1 xmlns:xplt=\"http://www.xxxx.com/\" xmlns:xpl=\"http://www.xxxx.com/\">first</Application1><Application2 xmlns:xplt=\"http://www.xxxx.com/\" xmlns:xpl=\"http://www.xxxx.com/\"> second</Application2> </Template>"; */

/*   guththila_writer_write (env, file, 0, strlen (file), writer); */
/*   guththila_writer_write (env, file, 0, strlen (file), writer); */
/*   guththila_writer_write (env, file, 0, strlen (file), writer); */
/*   guththila_writer_write (env, file, 0, strlen (file), writer); */
/*   guththila_writer_write (env, file, 0, strlen (file), writer); */
/*   guththila_writer_write (env, file, 0, strlen (file), writer); */
/*   guththila_writer_write (env, file, 0, strlen (file), writer); */
/*   char *t =  guththila_writer_get_buffer (env, writer); */
/*   printf ("print \t %s \n", t); */
  guththila_xml_pull_parser_create_xml_stream_writer_for_memory (env, parser);
  guththila_xml_pull_parser_write_start_document (env, parser);
  guththila_xml_pull_parser_write_start_element (env, parser, "person");
  guththila_xml_pull_parser_write_start_element (env, parser, "name");
  guththila_xml_pull_parser_write_characters (env, parser, "guththila");
  guththila_xml_pull_parser_write_start_element (env, parser, "escape");
  guththila_xml_pull_parser_write_escape_character (env, parser, "&");
  guththila_xml_pull_parser_write_escape_character (env, parser, ">");
  guththila_xml_pull_parser_write_end_element (env, parser);
  guththila_xml_pull_parser_write_start_element (env, parser, "escape");
  guththila_xml_pull_parser_write_default_namespace (env, parser, "http://www.cse.mrt.ac.lk");
  guththila_xml_pull_parser_write_namespace (env, parser, "a", "http://ws.apache.org");
  guththila_xml_pull_parser_write_start_element (env, parser, "test");
  guththila_xml_pull_parser_write_attribute_with_prefix(env, parser, "a", "web", "false");
  guththila_xml_pull_parser_write_start_element (env, parser, "one");
  guththila_xml_pull_parser_write_start_element_with_prefix (env, parser, "a", "two");
  guththila_xml_pull_parser_write_characters (env, parser, "it works !");
  guththila_xml_pull_parser_write_end_document (env, parser);
  char *t = guththila_writer_get_buffer (env, parser->xsw->writer);
  printf ("%s \n", t);
  guththila_xml_pull_parser_xml_writer_free (env, parser);
  guththila_xml_pull_parser_free (env, parser);
  axis2_allocator_free (allocator);
  axis2_env_free (env);
  return 0;
}
