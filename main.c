#include "stdio.h"
#include "string.h"
#include "stdbool.h"
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlschemastypes.h>
/*
gcc main.c -I/usr/include/libxml2 -lxml2 -std=gnu11 -o chartgen
*/
struct Canvas{
  char * charttitle;
  int width;
  int length;
  char bgcolor [7];
};
struct YaxisElements{
  char *name;
  char *unit;
  char fillcolor [7];
  int values [10];
  bool showvalue;
};
struct Yaxis{
  char *name;
  char color [7];
  int howmanyelements;
  struct YaxisElements elements[10];

};
struct Xaxis{
  char *name;
  int howmanyelements;
  char xelements[10][256];
};
struct Yaxis y_axis;
struct Xaxis x_axis;
struct Canvas canvas;
struct YaxisElements elements[10];
char *inputpath;
char *outputpath;
char *validationpath;

static void printBarChart()
{
  //Doldurulması Gerekli

}
static void printLineChart()
{
//Doldurulması Gerekli
}
static void printPieChart()
{
  //Doldurulmam Gerekli

}
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
            }
          }
        }
        }
        else if(!strcmp(cur_node->name,"Xaxis"))
        {
          xmlNode * asd_node = NULL;
          asd_node = cur_node;
          asd_node = asd_node -> children;
          while (asd_node->type != XML_ELEMENT_NODE) {
            asd_node = asd_node->next;
          }
          x_axis.name = malloc(256*sizeof(char));
          strcpy(x_axis.name, asd_node->children->content);
        }
        else if(!strcmp(cur_node->name,"Yaxis"))
        {
          //y_axis.y_axisElems = malloc(5* sizeof(struct YaxisElements));
          xmlNode * asd_node = NULL;
          for (asd_node = cur_node->children; asd_node; asd_node = asd_node->next) {
             if ( asd_node->type == XML_ELEMENT_NODE) {
                if(!strcmp(asd_node->name,"name"))
                {
                  y_axis.name = malloc(256* sizeof(char));
                  strcpy(y_axis.name,asd_node->children->content);
                }
                else if(!strcmp(asd_node->name,"forecolor"))
                {
                  strcpy(y_axis.color,asd_node->children->content);
                }
              }
          }
        }
        else if(!strcmp(cur_node->name,"Xset"))
        {
          xmlNode * asd_node = NULL;
          int a = 0;
          for (asd_node = cur_node->children; asd_node; asd_node = asd_node->next) {
             if ( asd_node->type == XML_ELEMENT_NODE && !strcmp(asd_node->name,"xdata")) {
               strcpy(x_axis.xelements[a],asd_node->children->content);
               a ++;
             }
           }
           x_axis.howmanyelements = a;
        }
        else if(!strcmp(cur_node->name,"Yset"))
        {
          xmlAttr * NodeAttribute = NULL;
          xmlNode * asd_node = NULL;
          char * attribute = NULL;
          int a = y_axis.howmanyelements;
          y_axis.elements[a].name = malloc(sizeof(char)*256);
          y_axis.elements[a].unit = malloc(sizeof(char)*25);
          y_axis.elements[a].showvalue = false;
          if(cur_node->properties != NULL)
          {
            for (NodeAttribute = cur_node->properties; NodeAttribute;NodeAttribute = NodeAttribute->next) {
              attribute = xmlNodeGetContent((xmlNode*)NodeAttribute);
              if(!strcmp(NodeAttribute->name, "unit"))
              {
                strcpy(y_axis.elements[a].unit , attribute);
              }
              else if(!strcmp(NodeAttribute->name, "name"))
              {
                strcpy(y_axis.elements[a].name , attribute);
              }
              else if(!strcmp(NodeAttribute->name, "showvalue"))
              {
                if(!strcmp(attribute,"yes"))
                {
                  y_axis.elements[a].showvalue = true;
                }
              }
              else if(!strcmp(NodeAttribute->name, "fillcolor"))
              {
                strcpy(y_axis.elements[a].fillcolor, attribute);
              }
            }
          }
          int i = 0;
          for (asd_node = cur_node->children; asd_node; asd_node = asd_node->next) {
             if ( asd_node->type == XML_ELEMENT_NODE) {
               if(!strcmp(asd_node->name, "ydata"))
               {
                 y_axis.elements[a].values[i] = atoi(asd_node->children->content);
                 i ++;
               }

             }
           }
           y_axis.howmanyelements = a +1;
         }
      }
      FillElements(cur_node->children);
  }
}

static void Validate()
{
  xmlDocPtr doc;
	xmlSchemaPtr schema = NULL;
	xmlSchemaParserCtxtPtr ctxt;
	xmlLineNumbersDefault(1);
	ctxt = xmlSchemaNewParserCtxt(validationpath);
	schema = xmlSchemaParse(ctxt);
	xmlSchemaFreeParserCtxt(ctxt);
	doc = xmlReadFile(inputpath, NULL, 0);

  if (doc == NULL)
	{
		fprintf(stderr, "Could not parse %s\n");
	}
	else
	{
		xmlSchemaValidCtxtPtr ctxt;
		int ret ;
		ctxt = xmlSchemaNewValidCtxt(schema);
		ret = xmlSchemaValidateDoc(ctxt, doc);

		if (ret == 0)
		{
			printf("validates\n");
		}
		else if (ret > 0)
		{
			printf("fails to validate\n");
		}
		else
		{
			printf("validation generated an internal error\n");
		}
		xmlSchemaFreeValidCtxt(ctxt);
		xmlFreeDoc(doc);

	}
}

int main(int argc , char *argv[])
{
  int charttype = -1;
  bool validate = false;
  for(int i = 1; i < argc; i++)
  {
    if (!strcmp(argv[i],"-h"))
    {
      printf("-h : help \n-o <outputfilename> : next word is outputname \n-i : <inputfilename> next word is the name of input file \n-t <type> : Type pf the graph must be one of bar, pie, line ");
    }
    else if (!strcmp(argv[i],"-i") && (argv[i + 1]))//eger oyleyse input kismindayiz demektir.
    {
      inputpath = malloc(sizeof(char)* 256);
      strcpy(inputpath, argv[i + 1]);
    }
    else if (!strcmp(argv[i],"-o") && (argv[i + 1]) )//eger oyleyse output kismindayiz demektir.
    {
      outputpath = malloc(sizeof(char)* 256);
      strcpy(outputpath, argv[i + 1]);
    }
    else if (!strcmp(argv[i],"-v") && (argv[i + 1]))//eger oyleyse verification kismindayiz demektir.
    {
      validationpath = malloc(sizeof(char)* 256);
      validate = true;
      strcpy(validationpath, argv[i + 1]);
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
  if(validate)
  {
    Validate();
  }
	xmlDoc         *doc = NULL;
	xmlNode        *root_element = NULL;
	const char     *Filename = inputpath;
	doc = xmlReadFile(Filename, NULL, 0);
	if (doc == NULL)
	{
		printf("Error: File Not Found! %s\n", Filename);
	}
	else
	{
      root_element = xmlDocGetRootElement(doc);
      FillElements(root_element);
      if(charttype == -1)
      {
        printf("Please Enter a Chart Type\n");
      }
      else if(charttype == 1)
      {
        printBarChart();
      }
      else if(charttype == 2)
      {
        printLineChart();
      }
      else
      {
        printPieChart();
      }
		  xmlFreeDoc(doc);
  }
  xmlCleanupParser();

	return (0);
}
/* Recursive function that prints the XML structure */
