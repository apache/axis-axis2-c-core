#include <stdio.h>
#include <guththila_buffer.h>
#include <guththila_reader.h>
#include <guththila_xml_pull_parser.h>
int main ()
{
  axis2_allocator_t *allocator = axis2_allocator_init (NULL);
  axis2_env_t *env = axis2_env_create (allocator);
  guththila_reader_t *reader = guththila_reader_create_for_file (env, "x.xml");
  guththila_xml_pull_parser_t *parser = guththila_xml_pull_parser_create (env, reader);
  guththila_xml_pull_parser_createXmlStreamWriter (env, parser, "w.xml");
  guththila_xml_pull_parser_writeStartDocument (env, parser);
  guththila_xml_pull_parser_writeStartElement (env, parser, "person");
  guththila_xml_pull_parser_writeStartElement (env, parser, "name");
  guththila_xml_pull_parser_writeCharacters (env, parser, "guththila");
  guththila_xml_pull_parser_writeStartElement (env, parser, "escape");
  guththila_xml_pull_parser_writeEscapeCharacter (env, parser, "&");
  guththila_xml_pull_parser_writeEscapeCharacter (env, parser, ">");
  guththila_xml_pull_parser_writeEndElement (env, parser);
  guththila_xml_pull_parser_writeStartElement (env, parser, "escape");
  guththila_xml_pull_parser_writeDefaultNamespace (env, parser, "http://www.cse.mrt.ac.lk");
  guththila_xml_pull_parser_writeNamespace (env, parser, "a", "http://ws.apache.org");
  guththila_xml_pull_parser_writeAttribute_with_prefix(env, parser, "a", "web", "false");
  guththila_xml_pull_parser_writeStartElement (env, parser, "one");
  guththila_xml_pull_parser_writeStartElement (env, parser, "two");
  guththila_xml_pull_parser_writeCharacters (env, parser, "it works !");
  guththila_xml_pull_parser_writeEndElement (env,parser);
  guththila_xml_pull_parser_writeEndDocument (env, parser);
  guththila_reader_free (env, reader);
  guththila_xml_pull_parser_free (env, parser);
  axis2_allocator_free (allocator);
  axis2_env_free (env);
  return 0;
}
