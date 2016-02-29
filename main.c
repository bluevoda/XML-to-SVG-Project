#include "stdio.h"
#include "string.h"
#include <libxml/parser.h>
#include <libxml/tree.h>
static void print_element_names(xmlNode * a_node)
{
    xmlNode *cur_node = NULL;

     for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
        	printf("Node Type: Element, name: %s \n Its children's type is: %d \n Its children's content is: %s \n", cur_node->name, cur_node->children->type, cur_node->children->content);
        }
	print_element_names(cur_node->children);
    }
}

int main(int argc , char *argv[])
{
  int inputloc = -1;
  int outputloc = -1;
  int verificationloc = -1;
  int charttype = -1;
  for(int i = 1; i < argc; i++)
  {
    if (!strcmp(argv[i],"-h"))
    {
      printf("-h : help \n-o <outputfilename> : next word is outputname \n-i : <inputfilename> next word is the name of input file \n-t <type> : Type pf the graph must be one of bar, pie, line ");
    }
    else if (!strcmp(argv[i],"-i"))//eger oyleyse input kismindayiz demektir.
    {
      inputloc = i + 1;
    }
    else if (!strcmp(argv[i],"-o"))//eger oyleyse output kismindayiz demektir.
    {
      outputloc = i + 1;
          }
    else if (!strcmp(argv[i],"-v"))//eger oyleyse verification kismindayiz demektir.
    {
      verificationloc = i + 1;
    }
    else if (!strcmp(argv[i],"-t"))
    {
      if(!strcmp(argv[i + 1],"bar"))
      {
        charttype = 1;
      }
      else if(!strcmp(argv[i + 1],"line"))
      {
        charttype = 2;
      }
      else if(!strcmp(argv[i + 1],"pie"))
      {
        charttype = 3;
      }
      else{
        printf("Wrong Type Entered! \n");
      }
    }
  }
	xmlDoc         *doc = NULL;
	xmlNode        *root_element = NULL;
	const char     *Filename = argv[inputloc];
	doc = xmlReadFile(Filename, NULL, 0);
	if (doc == NULL)
	{
		printf("Error: File Not FOund! %s\n", Filename);
	}
	else
	{
      root_element = xmlDocGetRootElement(doc);
		  print_element_names(root_element);
		  /*
                   * free the document
                   */
		xmlFreeDoc(doc);;
          }
        /*
         *Free the global variables that may
         *have been allocated by the parser.
         */
        xmlCleanupParser();

	return (0);
}
/* Recursive function that prints the XML structure */
