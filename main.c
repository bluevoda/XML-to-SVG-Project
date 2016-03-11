#include "stdio.h"
#include "string.h"
#include <libxml/parser.h>
#include <libxml/tree.h>
/*
gcc main.c -I/usr/include/libxml2 -lxml2 -std=gnu11 -o chartgen
*/
struct Canvas{
  char * charttitle;
  int width;
  int length;
  char bgcolor [6];
};
struct Yaxis{
  char *name;
  char color [6];
  int *YaxisElements;
};
struct YaxisElements{
  char *name;
  char *unit;
  char color [7];
  int *values;
};
struct Xaxis{
  char *name;
  char **xelements;
};

struct Yaxis y_axis;
struct Xaxis x_axis;
struct Canvas canvas;

/*
static void print_element_names(xmlNode * a_node)
{

}
*/
static void FillElements(xmlNode * a_node)
{
  xmlNode *cur_node =NULL;
   for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
      if (cur_node->type == XML_ELEMENT_NODE) {
        if(!strcmp(cur_node->name,"charttitle"))
        {
          canvas.charttitle = malloc(250 * sizeof(char));
          strcpy(canvas.charttitle,cur_node->children->content);
        }
        else if(!strcmp(cur_node->name,"canvas"))
        {
          xmlNode * asd_node = NULL;
          for (asd_node = cur_node->children; asd_node; asd_node = asd_node->next) {
             if ( asd_node->type == XML_ELEMENT_NODE) {

            if(!strcmp(asd_node->name,"length"))
            {
              canvas.length= atoi(asd_node->children->content);
              //printf("%s\n",canvas.charttitle );
            }
            else if(!strcmp(asd_node->name,"width"))
            {
              canvas.width= atoi(asd_node->children->content);
            }
            else if(!strcmp(asd_node->name,"backcolor"))
            {
              strcpy(canvas.bgcolor,asd_node->children->content);

                printf(" 1");
            }
          }
        }
        }
        else if(!strcmp(cur_node->name,"Xaxis"))
        {
                        printf(" 2");
          cur_node = cur_node->children;
          while (cur_node->type != XML_ELEMENT_NODE) {
            cur_node = cur_node->next;
          }
            printf("%s\n", cur_node->children->content);
          x_axis.name = malloc(256*sizeof(char));
          strcpy(x_axis.name, cur_node->children->content);
          printf("%s\n",x_axis.name);

        }
        else if(!strcmp(cur_node->name,"Yaxis"))
        {

        }
        else if(!strcmp(cur_node->name,"Xset"))
        {

        }
        else if(!strcmp(cur_node->name,"Yset"))
        {

        }
      }
FillElements(cur_node->children);
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
      //print_element_names(root_element);
      FillElements(root_element);
		  xmlFreeDoc(doc);;
    }
        xmlCleanupParser();

	return (0);
}
/* Recursive function that prints the XML structure */
