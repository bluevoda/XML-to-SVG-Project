#include "stdio.h"
#include "string.h"
#include "stdbool.h"
#include <math.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlschemastypes.h>

/*
gcc main.c -I/usr/include/libxml2 -lxml2 -std=gnu11 -o chartgen -lm
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
int textsize;

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
  xmlDocPtr doc = NULL; /* document pointer */
  xmlNodePtr root_node = NULL, node = NULL, pathnode = NULL, node1 = NULL, gnode = NULL;/* node pointers */
  xmlDtdPtr dtd = NULL; /* DTD pointer */
  char buff[256];
  int i, j;

  doc = xmlNewDoc(BAD_CAST "2.0");
  root_node = xmlNewNode(NULL, BAD_CAST "html");
  xmlDocSetRootElement(doc, root_node);
  node1 = xmlNewChild(root_node, NULL, BAD_CAST "body", NULL);
  node = xmlNewChild(node1, NULL, BAD_CAST "svg", NULL);
  char height[9];
  char width[9];
  sprintf(height,"%d",canvas.length * y_axis.howmanyelements + 20);
  sprintf(width, "%d",canvas.width * 2);
  xmlNewProp(node, BAD_CAST "height", BAD_CAST height);
  xmlNewProp(node, BAD_CAST "width", BAD_CAST width);
  xmlNewProp(node, BAD_CAST "encoding", BAD_CAST "UNICODE");
///intitial
int circledim = 0;// smallest of width and height
circledim = (canvas.length > canvas.width ? canvas.width:canvas.length);
circledim = circledim - 20; // r of the circle
circledim = circledim /2;
printf("%d\n",canvas.width);
//printf("Smallest circledim = %d\n",circledim );
textsize = circledim / 10;

  //HEADER


  gnode = xmlNewChild(node, NULL, BAD_CAST "text",canvas.charttitle);
  char * attrib = malloc(250 * sizeof(char));
  xmlNewProp(gnode,BAD_CAST "text-anchor", BAD_CAST "middle");
  sprintf(attrib,"%d",canvas.width);
  xmlNewProp(gnode,BAD_CAST "x", BAD_CAST attrib);
  sprintf(attrib,"%d",textsize * 2);
  xmlNewProp(gnode,BAD_CAST "y", BAD_CAST attrib);
  sprintf(attrib,"%d",textsize * 2);
  xmlNewProp(gnode,BAD_CAST "font-size", BAD_CAST attrib);
  xmlNewProp(gnode,BAD_CAST "font-family", BAD_CAST "Verdana");
  for (int times = 0; times < y_axis.howmanyelements; times++) {
    //Doldurulmam Gerekli

    int centery = canvas.length * times + canvas.length/2 + textsize * 2;
    int centerx = canvas.width/2;
    int startangle = 0;
    int total = 0;
    for (int i = 0; y_axis.elements[times].values[i] && i < 10; i++) {
      //find total values
      total = total + y_axis.elements[times].values[i];
    }
    double angles [10];
    double totalangle = 0;
    int howmanyelements = 0;
    for (int i = 0; y_axis.elements[times].values[i] && i < 10; i++) {
      //find each angles
      angles[i] = ceil(360 * y_axis.elements[times].values[i]/total);
      totalangle = totalangle + angles[i];
      howmanyelements ++;
    }
    double error = (360.0 - totalangle)/2;
    angles[0]+= error;
    angles[howmanyelements -1] += error;//error correction
    double endAngle = 0;
    for (int i = 0; y_axis.elements[times].values[i] && i < 10; i++) {
      char attr[256];
      gnode = xmlNewChild(node, NULL, BAD_CAST "g", NULL);
      char * color = malloc(sizeof(char)*7);
      switch (i)
      {
        case 0:
        strcpy(color, "BCB968");
        break;
        case 1:
        strcpy(color, "8FA8A2");
        break;
        case 2:
        strcpy(color, "CCD4BD");
        break;
        case 3:
        strcpy(color, "9E3B36");
        break;
        case 4:
        strcpy(color, "50553F");
        break;
        case 5:
        strcpy(color, "212735");
        break;
        case 6:
        strcpy(color, "B9CE8B");
        break;
        case 7:
        strcpy(color, "622C3C");
        break;
        case 8:
        strcpy(color, "9D677E");
        break;
        case 9:
        strcpy(color, "E0BAC5");
        break;
      }
      sprintf(attr, "#%s", color);
      xmlNewProp(gnode,BAD_CAST "fill", BAD_CAST attr);
      pathnode = xmlNewChild(gnode, NULL, BAD_CAST "path", NULL);
      //find each points to make a path
      double startAngle = endAngle;
      endAngle = startAngle + angles[i];
      int x1 = (centerx + circledim* cos(M_PI*startAngle/180));
      int y1 = (centery + circledim* sin(M_PI*startAngle/180));
      int x2 = (centerx + circledim* cos(M_PI*endAngle/180));
      int y2 = (centery + circledim* sin(M_PI*endAngle/180));

      sprintf(attr, "M %d,%d  L %d,%d A %d,%d 0 0,1 %d,%d z",centerx,centery,x1, y1, circledim,circledim, x2, y2);
      xmlNewProp(pathnode,BAD_CAST "d", BAD_CAST attr);
      printf("%f\n",endAngle );
    }
    //Circle finished lets start with HEADER TEXT

    //Leged Title
    gnode = xmlNewChild(node, NULL, BAD_CAST "text",y_axis.elements[times].name);
    attrib = malloc(250 * sizeof(char));
    xmlNewProp(gnode,BAD_CAST "text-anchor", BAD_CAST "start");
    sprintf(attrib,"%d",canvas.width * 3 / 2);
    xmlNewProp(gnode,BAD_CAST "x", BAD_CAST attrib);
    sprintf(attrib,"%d",canvas.length * times + textsize * 5);
    xmlNewProp(gnode,BAD_CAST "y", BAD_CAST attrib);
    sprintf(attrib,"%d",textsize);
    xmlNewProp(gnode,BAD_CAST "font-size", BAD_CAST attrib);
    for (int i = 0; y_axis.elements[times].values[i] && i < 10; i++) {
      //Create value instruction
      if(y_axis.elements[times].showvalue)
      {
        char * value = malloc (sizeof(char)* 15);
        sprintf(value, "(%d)", y_axis.elements[times].values[i]);
        gnode = xmlNewChild(node, NULL, BAD_CAST "text", value);
        char * attribin = malloc(250 * sizeof(char));
        xmlNewProp(gnode,BAD_CAST "text-anchor", BAD_CAST "end");
        sprintf(attribin,"%d",canvas.width * 3 / 2);
        xmlNewProp(gnode,BAD_CAST "x", BAD_CAST attribin);
        sprintf(attribin,"%d",canvas.length * times+textsize * (7 + i));
        xmlNewProp(gnode,BAD_CAST "y", BAD_CAST attribin);
        sprintf(attribin,"%d",textsize * 2 / 3);
        xmlNewProp(gnode,BAD_CAST "font-size", BAD_CAST attribin);
      }
      char * value = malloc (sizeof(char)* 15);
      sprintf(value, "(%d)", y_axis.elements[times].values[i]);
      gnode = xmlNewChild(node, NULL, BAD_CAST "rect", NULL);
      char * attribin = malloc(250 * sizeof(char));
      sprintf(attribin,"%d",canvas.width * 3 / 2 + 5);
      xmlNewProp(gnode,BAD_CAST "x", BAD_CAST attribin);
      sprintf(attribin,"%d",canvas.length * times + textsize * (6 + i) + textsize/2);
      xmlNewProp(gnode,BAD_CAST "y", BAD_CAST attribin);
      sprintf(attribin,"%d",textsize * 2);
      xmlNewProp(gnode,BAD_CAST "width", BAD_CAST attribin);
      sprintf(attribin,"%d",textsize - 1);
      xmlNewProp(gnode,BAD_CAST "height", BAD_CAST attribin);
      char * color = malloc(sizeof(char)*7);
      switch (i)
      {
        case 0:
        strcpy(color, "BCB968");
        break;
        case 1:
        strcpy(color, "8FA8A2");
        break;
        case 2:
        strcpy(color, "CCD4BD");
        break;
        case 3:
        strcpy(color, "9E3B36");
        break;
        case 4:
        strcpy(color, "50553F");
        break;
        case 5:
        strcpy(color, "212735");
        break;
        case 6:
        strcpy(color, "B9CE8B");
        break;
        case 7:
        strcpy(color, "622C3C");
        break;
        case 8:
        strcpy(color, "9D677E");
        break;
        case 9:
        strcpy(color, "E0BAC5");
        break;
      }
      sprintf(attribin,"#%s",color);
      xmlNewProp(gnode,BAD_CAST "fill", BAD_CAST attribin);
      //Print Names
      value = malloc (sizeof(char)* 15);
      sprintf(value, "(%s)", x_axis.xelements[i]);
      gnode = xmlNewChild(node, NULL, BAD_CAST "text", value);
      attribin = malloc(250 * sizeof(char));
      xmlNewProp(gnode,BAD_CAST "text-anchor", BAD_CAST "start");
      sprintf(attribin,"%d",canvas.width * 3 / 2 + 3 * textsize);
      xmlNewProp(gnode,BAD_CAST "x", BAD_CAST attribin);
      sprintf(attribin,"%d",canvas.length * times + textsize * (7 + i));
      xmlNewProp(gnode,BAD_CAST "y", BAD_CAST attribin);
      sprintf(attribin,"%d",textsize * 2 / 3);
      xmlNewProp(gnode,BAD_CAST "font-size", BAD_CAST attribin);

    }
  }





  xmlSaveFormatFileEnc( outputpath, doc, "UTF­8", 1);
  xmlFreeDoc(doc);
  xmlCleanupParser();
  xmlMemoryDump();

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
