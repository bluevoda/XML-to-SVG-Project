#include "stdio.h"
#include "string.h"
#include "stdbool.h"
#include <math.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlschemastypes.h>
#include <libxml/HTMLtree.h>

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
  int values [20];
  bool showvalue;
};
struct Yaxis{
  char *name;
  char color [7];
  int howmanyelements;
  struct YaxisElements elements[20];

};
struct Xaxis{
  char *name;
  int howmanyelements;
  char xelements[20][256];
};
struct Yaxis y_axis;
struct Xaxis x_axis;
struct Canvas canvas;
struct YaxisElements elements[20];
char *inputpath;
char *outputpath;
char *validationpath;
int textsize;

static void printBarChart()
{
    xmlNodePtr root_node = NULL, node1 = NULL, node2 = NULL, node0=NULL, node00=NULL, nodet=NULL, colornode=NULL, nodec=NULL;  /* node pointers */
    xmlDtdPtr dtd = NULL;       /* DTD pointer */
    char buff[512];
long int y;
char num[256];
char el[256];

    xmlDocPtr doc = xmlNewDoc(BAD_CAST "1.0");
    root_node = xmlNewNode(NULL, BAD_CAST "svg");
    xmlNewProp(root_node, BAD_CAST "version", BAD_CAST "1.1");
xmlNewProp(root_node, BAD_CAST "xmlns", BAD_CAST "http://www.w3.org/2000/svg");
xmlNewProp(root_node, BAD_CAST "xmlns:xlink", BAD_CAST "http://www.w3.org/1999/xlink");
xmlNewProp(root_node, BAD_CAST "class", BAD_CAST "chartgen");
sprintf(buff,"%d",canvas.width);
xmlNewProp(root_node, BAD_CAST "width", BAD_CAST buff);
sprintf(buff,"%d",canvas.length);
xmlNewProp(root_node, BAD_CAST "length", BAD_CAST buff);
/*char s0[10];
char s01[10];
sprintf(s0,"%s",canvas.bgcolor);
sprintf(s01,"#%s",s0);
sprintf(buff,"%s",s01);
xmlNewProp(root_node, BAD_CAST "fill", buff);*/
xmlDocSetRootElement(doc, root_node);

//Tittle yazısı
sprintf(buff,"%s",canvas.charttitle);
nodet= xmlNewChild(root_node, NULL, BAD_CAST "text", BAD_CAST buff);
sprintf(buff,"%d",canvas.width/2);
xmlNewProp(nodet, BAD_CAST "x", BAD_CAST buff);
sprintf(buff,"%d",canvas.length/25);
xmlNewProp(nodet, BAD_CAST "y", BAD_CAST buff);
	snprintf(buff, sizeof(buff), "%d", canvas.length/20);
	xmlNewProp(nodet, BAD_CAST "font-size", buff);

//y ekseninin başına "Sales (TL) yazısı
sprintf(buff,"%s",y_axis.name);
node0= xmlNewChild(root_node, NULL, BAD_CAST "text", BAD_CAST buff);
xmlNewProp(node0, BAD_CAST "x", BAD_CAST "0");
sprintf(buff,"%d",canvas.length/25);
xmlNewProp(node0, BAD_CAST "y", BAD_CAST buff);
	snprintf(buff, sizeof(buff), "%d", canvas.length/20);
	xmlNewProp(node0, BAD_CAST "font-size", buff);

//dikey eksen
node1= xmlNewChild(root_node, NULL, BAD_CAST "line", NULL);
sprintf(buff,"%d",canvas.width/8);
xmlNewProp(node1, BAD_CAST "x1", BAD_CAST buff);
sprintf(buff,"%d",canvas.length/25);
xmlNewProp(node1, BAD_CAST "y1", BAD_CAST buff);
sprintf(buff,"%d",canvas.width/8);
xmlNewProp(node1, BAD_CAST "x2", BAD_CAST buff);
sprintf(buff,"%d",canvas.length-(canvas.length/25));
xmlNewProp(node1, BAD_CAST "y2", BAD_CAST buff);
xmlNewProp(node1, BAD_CAST "style", BAD_CAST "stroke:rgb(255,0,0);stroke-width:2");


int k, interval, maxnum=11250; //maxnum değişkenine en büyük değer atandı
//k tüm alanı yatay çizgilere bölerek aradaki mesafeyi hesaplıyor
 k=canvas.length/(x_axis.howmanyelements+1);
 //en büyük elemanı toplam elemana oranlayarak ne kadar arttığını buluyor
interval=maxnum/x_axis.howmanyelements;
int d, newcount;
newcount=interval;
d=canvas.length-(canvas.length/25);
int count=1;
//yatay çizgileri çizme
for(int i=x_axis.howmanyelements+1; i>=1; i--){
xmlNodePtr noden = NULL, noden2 = NULL;
noden= xmlNewChild(root_node, NULL, BAD_CAST "line", NULL);
sprintf(buff,"%d",canvas.width/8);
xmlNewProp(noden, BAD_CAST "x1", BAD_CAST buff);
sprintf(buff,"%d",d);
xmlNewProp(noden, BAD_CAST "y1", BAD_CAST buff);
sprintf(buff,"%d",canvas.width-((canvas.width/8)*2));
xmlNewProp(noden, BAD_CAST "x2", BAD_CAST buff);
sprintf(buff,"%d",d);
xmlNewProp(noden, BAD_CAST "y2", BAD_CAST buff);
xmlNewProp(noden, BAD_CAST "style", BAD_CAST "stroke:rgb(255,0,0);stroke-width:2");

if(i<x_axis.howmanyelements+1){
newcount=interval*count;
sprintf(buff,"%d",newcount);      //sol tarafa değerlerin yazılması
noden2= xmlNewChild(root_node, NULL, BAD_CAST "text", BAD_CAST buff);
xmlNewProp(noden2, BAD_CAST "x", BAD_CAST "0");
sprintf(buff,"%d",d);
xmlNewProp(noden2, BAD_CAST "y", BAD_CAST buff);
snprintf(buff, sizeof(buff), "%d", canvas.length/25);
xmlNewProp(noden2, BAD_CAST "font-size", buff);
count++;
}

//x ekseninin sonuna Months yazdırma
if(i==x_axis.howmanyelements+1){
sprintf(buff,"%s",x_axis.name);
node00= xmlNewChild(root_node, NULL, BAD_CAST "text", BAD_CAST buff);
sprintf(buff,"%d",canvas.width-((canvas.width/8)*2));
xmlNewProp(node00, BAD_CAST "x", BAD_CAST buff);
sprintf(buff,"%d",canvas.length-(canvas.length/25));
xmlNewProp(node00, BAD_CAST "y", BAD_CAST buff);
snprintf(buff, sizeof(buff), "%d", canvas.length/20);
xmlNewProp(node00, BAD_CAST "font-size", buff);
}
d=d-((canvas.length-(canvas.length/25))/(x_axis.howmanyelements+1));
}

  int x_interval, x_count;
x_interval=((canvas.width-((canvas.width/8)*2))-(canvas.width/8))/x_axis.howmanyelements;

int colory=canvas.length/20;
//bar chartların çizimi
for(int j=1; j<=x_axis.howmanyelements; j++){
//değerleri oranlayarak bar'ların bitiş noktaları belirlendi
xmlNodePtr node3 = NULL, node4 = NULL, node5 = NULL, node6 = NULL, node7 = NULL;

node3=xmlNewChild(root_node, NULL, BAD_CAST "g", NULL);
xmlNewProp(node3, BAD_CAST "class", BAD_CAST "bar");

//Months değerlerinin yazdırılması
x_count=(x_interval*(j-1))+(canvas.width/8);
sprintf(el,"%s",x_axis.xelements[j-1]);
node5=xmlNewChild(node3, NULL, BAD_CAST "text", BAD_CAST el);
sprintf(buff,"%d",x_count);
xmlNewProp(node5, BAD_CAST "x", BAD_CAST buff);
sprintf(buff,"%d",canvas.length);
xmlNewProp(node5, BAD_CAST "y", BAD_CAST buff);
snprintf(buff, sizeof(buff), "%d", canvas.length/40);
xmlNewProp(node5, BAD_CAST "font-size", buff);

for(int m=0; m<y_axis.howmanyelements; m++){
maxnum=11250;
y=(canvas.length*maxnum)-((canvas.length-k)*y_axis.elements[m].values[j-1]);
y=y/maxnum;
node4=xmlNewChild(node3, NULL, BAD_CAST "rect", NULL);
char s3[10];
char s4[10];
sprintf(s3,"%s",y_axis.elements[m].fillcolor);
sprintf(s4,"#%s",s3);
sprintf(el,"%s",s4);
xmlNewProp(node4, BAD_CAST "fill", BAD_CAST el);
sprintf(num,"%d",x_count);
xmlNewProp(node4, BAD_CAST "x", BAD_CAST num);
sprintf(num,"%d",y);
xmlNewProp(node4, BAD_CAST "y", BAD_CAST num);
sprintf(num,"%d",(canvas.length-(canvas.length/25))-y);
xmlNewProp(node4, BAD_CAST "height", BAD_CAST num);
sprintf(num,"%d",canvas.width/20);
xmlNewProp(node4, BAD_CAST "width", BAD_CAST num);

if(j==1){
colornode=xmlNewChild(node3, NULL, BAD_CAST "rect", NULL);
sprintf(el,"%s",s4);
xmlNewProp(colornode, BAD_CAST "fill", BAD_CAST el);
sprintf(num,"%d",canvas.width-((canvas.width/8)*2));
xmlNewProp(colornode, BAD_CAST "x", BAD_CAST num);
sprintf(num,"%d",colory);
xmlNewProp(colornode, BAD_CAST "y", BAD_CAST num);
sprintf(num,"%d",canvas.length/20);
xmlNewProp(colornode, BAD_CAST "height", BAD_CAST num);
sprintf(num,"%d",canvas.width/20);
xmlNewProp(colornode, BAD_CAST "width", BAD_CAST num);
colory=colory+(canvas.length/20);
sprintf(buff,"%s",y_axis.elements[m].name);
node7= xmlNewChild(node3, NULL, BAD_CAST "text", BAD_CAST buff);
sprintf(buff,"%d",canvas.width-((canvas.width/8)*2)+(canvas.width/20));
xmlNewProp(node7, BAD_CAST "x", BAD_CAST buff);
sprintf(buff,"%d",colory);
xmlNewProp(node7, BAD_CAST "y", BAD_CAST buff);
snprintf(buff, sizeof(buff), "%d", canvas.length/25);
xmlNewProp(node7, BAD_CAST "font-size", buff);
colory=colory+(canvas.length/20);
}

//Sales değerlerinin dikey olarak yazdırılması
if(y_axis.elements[m].showvalue){
sprintf(el,"%d",y_axis.elements[m].values[j-1]);
node6=xmlNewChild(node3, NULL, BAD_CAST "text", BAD_CAST el);
sprintf(el,"%d",x_count);
xmlNewProp(node6, BAD_CAST "x", BAD_CAST el);
sprintf(el,"%d",y);
xmlNewProp(node6, BAD_CAST "y", BAD_CAST el);
//xmlNewProp(node6, BAD_CAST "transform", BAD_CAST "translate(27,190)rotate(90 90,90)");
snprintf(buff, sizeof(buff), "%d", canvas.length/40);
xmlNewProp(node6, BAD_CAST "font-size", buff);
}
else{
x_count=x_count+canvas.width/20;
}
x_count=x_count+canvas.width/20;
}
//x_count=x_count+canvas.width/20;
}
  htmlSaveFile(outputpath, doc);
  xmlFreeDoc(doc);
  xmlCleanupParser();
  xmlMemoryDump();
}

static void printLineChart()
{

	//reading
	//variables
	char buffer[128];
	char backColor[7];
	strcpy(backColor, canvas.bgcolor);
	int xSize = canvas.length;
	int currentX = 0;
	int ySize = canvas.width;
	int currentY = ySize;
	char xAxisName[128];
	strcpy(xAxisName, x_axis.name);
	char yAxisName[128];
	strcpy(yAxisName, y_axis.name);
	int smallOne = 0;
	if (xSize > ySize)
	{
		smallOne = ySize;
	}
	else
	{
		smallOne = xSize;
	}
	int xOrder = xSize/10;
	int yOrder = ySize/10;
	int howManyValues = ySize/50;
	int maxValue = 0;

	int sales[y_axis.howmanyelements][x_axis.howmanyelements]; //sale values
	for (int i = 0; i < y_axis.howmanyelements; i++)
	{
		for(int j = 0; j < x_axis.howmanyelements; j++)
		{
			sales[i][j] = y_axis.elements[i].values[j];
		}
	}

	char cities[y_axis.howmanyelements][128]; //city names
	for (int i = 0; i < y_axis.howmanyelements; i++)
	{
		strcpy(cities[i], y_axis.elements[i].name);
	}

	char months[x_axis.howmanyelements][128]; //month names
	for (int i = 0; i < x_axis.howmanyelements; i++)
	{
		strcpy(months[i], x_axis.xelements[i]);
	}

	char fillColors[y_axis.howmanyelements][7]; //color codes
	for (int i = 0; i < y_axis.howmanyelements; i++)
	{
		strcpy(fillColors[i], y_axis.elements[i].fillcolor);
	}

	//finding max value
	for (int i = 0; i < sizeof(cities)/sizeof(cities[0]); i++) //for each city
	{
		for (int j = 0;j < sizeof(sales[0])/sizeof(sales[0][0]);j++) //for each month
		{
			if (maxValue < sales[i][j])
			{
				maxValue = sales[i][j];
			}
		}
	}
	int smallify = maxValue/ySize + 3000/ySize;

	//creates root
	xmlDocPtr doc = NULL;       /* document pointer */
	xmlNodePtr root_node = NULL, line = NULL, text = NULL;/* node pointers */
	xmlDtdPtr dtd = NULL;       /* DTD pointer */
	doc = xmlNewDoc(BAD_CAST "2.0");
	root_node = xmlNewNode(NULL, BAD_CAST "svg");
	xmlDocSetRootElement(doc, root_node);
	snprintf(buffer, sizeof(buffer), "%d", xSize);
	xmlNewProp(root_node, BAD_CAST "width", buffer);
	snprintf(buffer, sizeof(buffer), "%d", ySize);
	xmlNewProp(root_node, BAD_CAST "height", buffer);
	snprintf(buffer, sizeof(buffer), "%s", "");
	strcat(buffer, "background:#");
	strcat(buffer, backColor);
	xmlNewProp(root_node, BAD_CAST "style", buffer);

	//reading completed
	for (int i = 0;i < sizeof(cities)/sizeof(cities[0]);i++) //for each city
	{
		currentX = 0;
		currentY = ySize;
		for (int j = 0;j < sizeof(sales[0])/sizeof(sales[0][0]);j++) //for each month
		{
			line = xmlNewChild(root_node, NULL, BAD_CAST "line", NULL);
			if (j != 0)
			{
				//sets x1 and y1
				snprintf(buffer, sizeof(buffer), "%d", currentX + xOrder);
				xmlNewProp(line, BAD_CAST "x1", buffer);
				snprintf(buffer, sizeof(buffer), "%d", currentY - yOrder);
				xmlNewProp(line, BAD_CAST "y1", buffer);
			}
			currentX = currentX + xSize/ (sizeof(sales[0])/sizeof(sales[0][0])+1);
			currentY = ySize-sales[i][j]/smallify;
			if (j != 0)
			{
				if (y_axis.elements[i].showvalue == 1)
				{
					snprintf(buffer, sizeof(buffer), "%d", sales[i][j]);
					text = xmlNewChild(root_node, NULL, BAD_CAST "text", buffer);
					xmlNewProp(text, BAD_CAST "fill", BAD_CAST "blue");
					snprintf(buffer, sizeof(buffer), "%d", smallOne/40);
					xmlNewProp(text, BAD_CAST "font-size", buffer);
				}
				//sets x2 and y2
				snprintf(buffer, sizeof(buffer), "%d", currentX + xOrder);
				xmlNewProp(line, BAD_CAST "x2", buffer);
				if (y_axis.elements[i].showvalue == 1)
				{
					snprintf(buffer, sizeof(buffer), "%d", currentX - xSize/40 + xOrder);
					xmlNewProp(text, BAD_CAST "x", buffer);
				}
				snprintf(buffer, sizeof(buffer), "%d", currentY - yOrder);
				xmlNewProp(line, BAD_CAST "y2", buffer);
				if (y_axis.elements[i].showvalue == 1)
				{
					xmlNewProp(text, BAD_CAST "y", buffer);
				}
				snprintf(buffer, sizeof(buffer), "%s", "");
				strcat(buffer, "stroke:#");
				strcat(buffer, fillColors[i]);
				strcat(buffer, ";stroke-width:2)");
				xmlNewProp(line, BAD_CAST "style", BAD_CAST buffer);
			}

			//writes months
			text = xmlNewChild(root_node, NULL, BAD_CAST "text", BAD_CAST months[j]);
			snprintf(buffer, sizeof(buffer), "%d", currentX + xOrder - xSize/20);
			xmlNewProp(text, BAD_CAST "x", buffer);
			snprintf(buffer, sizeof(buffer), "%d", ySize + ySize/40 - yOrder);
			xmlNewProp(text, BAD_CAST "y", buffer);
			xmlNewProp(text, BAD_CAST "fill", BAD_CAST "blue");
			snprintf(buffer, sizeof(buffer), "%d", smallOne/(8*x_axis.howmanyelements));
			xmlNewProp(text, BAD_CAST "font-size", buffer);
			}

		//writes cities, must be changed into legend
		text = xmlNewChild(root_node, NULL, BAD_CAST "text", BAD_CAST cities[i]);
		snprintf(buffer, sizeof(buffer), "%d", 0);
		xmlNewProp(text, BAD_CAST "x", buffer);
		snprintf(buffer, sizeof(buffer), "%d", ySize-sales[i][0]/smallify - yOrder);
		xmlNewProp(text, BAD_CAST "y", buffer);
		snprintf(buffer, sizeof(buffer), "%s", "");
		strcat(buffer, "#");
		strcat(buffer, fillColors[i]);
		xmlNewProp(text, BAD_CAST "fill", buffer);
		snprintf(buffer, sizeof(buffer), "%d", smallOne/40);
		xmlNewProp(text, BAD_CAST "font-size", buffer);
	}

	for (int i = 1;i <= howManyValues;i++) //values
	{
		snprintf(buffer, sizeof(buffer), "%d", i*maxValue/howManyValues);
		text = xmlNewChild(root_node, NULL, BAD_CAST "text", buffer);
		snprintf(buffer, sizeof(buffer), "%d", 0);
		xmlNewProp(text, BAD_CAST "x", buffer);
		snprintf(buffer, sizeof(buffer), "%d", ySize - i*maxValue/howManyValues/smallify - yOrder);
		xmlNewProp(text, BAD_CAST "y", buffer);
		xmlNewProp(text, BAD_CAST "fill", BAD_CAST "blue");
		snprintf(buffer, sizeof(buffer), "%d", smallOne/40);
		xmlNewProp(text, BAD_CAST "font-size", buffer);
	}

	//x line
	line = xmlNewChild(root_node, NULL, BAD_CAST "line", NULL);
	xmlNewProp(line, BAD_CAST "x1", "0");
	snprintf(buffer, sizeof(buffer), "%d", ySize - yOrder);
	xmlNewProp(line, BAD_CAST "y1", buffer);
	snprintf(buffer, sizeof(buffer), "%d", xSize - xOrder/2);
	xmlNewProp(line, BAD_CAST "x2", buffer);
	snprintf(buffer, sizeof(buffer), "%d", ySize - yOrder);
	xmlNewProp(line, BAD_CAST "y2", buffer);
	xmlNewProp(line, BAD_CAST "style", BAD_CAST "stroke:rgb(0,0,255);stroke-width:2");
	snprintf(buffer, sizeof(buffer), "%s", xAxisName);
	text = xmlNewChild(root_node, NULL, BAD_CAST "text", buffer);
	snprintf(buffer, sizeof(buffer), "%d", 0);
	xmlNewProp(text, BAD_CAST "x", buffer);
	snprintf(buffer, sizeof(buffer), "%d", ySize/40);
	xmlNewProp(text, BAD_CAST "y", buffer);
	xmlNewProp(text, BAD_CAST "fill", BAD_CAST "blue");
	snprintf(buffer, sizeof(buffer), "%d", smallOne/40);
	xmlNewProp(text, BAD_CAST "font-size", buffer);

	//y line
	line = xmlNewChild(root_node, NULL, BAD_CAST "line", NULL);
	snprintf(buffer, sizeof(buffer), "%d", xOrder);
	xmlNewProp(line, BAD_CAST "x1", buffer);
	snprintf(buffer, sizeof(buffer), "%d", ySize);
	xmlNewProp(line, BAD_CAST "y1", buffer);
	snprintf(buffer, sizeof(buffer), "%d", xOrder);
	xmlNewProp(line, BAD_CAST "x2", buffer);
	snprintf(buffer, sizeof(buffer), "%d", yOrder/2);
	xmlNewProp(line, BAD_CAST "y2", buffer);
	xmlNewProp(line, BAD_CAST "style", BAD_CAST "stroke:rgb(0,0,255);stroke-width:2");
	snprintf(buffer, sizeof(buffer), "%s", yAxisName);
	text = xmlNewChild(root_node, NULL, BAD_CAST "text", buffer);
	snprintf(buffer, sizeof(buffer), "%d", xSize - xSize/10);
	xmlNewProp(text, BAD_CAST "x", buffer);
	snprintf(buffer, sizeof(buffer), "%d", ySize - yOrder - ySize/40);
	xmlNewProp(text, BAD_CAST "y", buffer);
	xmlNewProp(text, BAD_CAST "fill", BAD_CAST "blue");
	snprintf(buffer, sizeof(buffer), "%d", smallOne/40);
	xmlNewProp(text, BAD_CAST "font-size", buffer);

	//closing file
	htmlSaveFile(outputpath, doc);
	xmlFreeDoc(doc);
	xmlCleanupParser();
	xmlMemoryDump();
}

static void printPieChart()
{
  xmlDocPtr doc = NULL; /* document pointer */
  xmlNodePtr root_node = NULL, node = NULL, pathnode = NULL, node1 = NULL, gnode = NULL, anode = NULL, bnode = NULL;/* node pointers */
  xmlDtdPtr dtd = NULL; /* DTD pointer */
  char buff[256];
  int i, j;

  doc = xmlNewDoc(BAD_CAST "1.0");
  node = xmlNewNode(NULL, BAD_CAST "svg");
  xmlDocSetRootElement(doc, node);
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

    //animation
    anode = xmlNewChild(node, NULL, BAD_CAST "g",NULL);
    bnode = xmlNewChild(anode, NULL, BAD_CAST "animateTransform",NULL);
    xmlNewProp(bnode,BAD_CAST "attributeName", BAD_CAST "Transform");
    xmlNewProp(bnode,BAD_CAST "begin", BAD_CAST "0s");
    xmlNewProp(bnode,BAD_CAST "dur", BAD_CAST "20s");
    xmlNewProp(bnode,BAD_CAST "type", BAD_CAST "rotate");
    char * animatribute ;
    sprintf(animatribute, "%d %d %d" ,0,centerx,centery);
    xmlNewProp(bnode,BAD_CAST "from", BAD_CAST animatribute);
    sprintf(animatribute, "%d %d %d" ,360,centerx,centery);
    xmlNewProp(bnode,BAD_CAST "to", BAD_CAST animatribute);
    xmlNewProp(bnode,BAD_CAST "repeatCount", BAD_CAST "indefinite");






    int startangle = 0;
    int total = 0;
    for (int i = 0; y_axis.elements[times].values[i] && i < 20; i++) {
      //find total values
      total = total + y_axis.elements[times].values[i];
    }
    double angles [20];
    double totalangle = 0;
    int howmanyelements = 0;
    for (int i = 0; y_axis.elements[times].values[i] && i < 20; i++) {
      //find each angles
      angles[i] = 360 * y_axis.elements[times].values[i]/total;
      totalangle = totalangle + angles[i];
      howmanyelements ++;
    }
    double error = (360.0 - totalangle)/2;
    angles[0]+= error;
    angles[howmanyelements -1] += error;//error correction
    double endAngle = 0;
    for (int i = 0; y_axis.elements[times].values[i] && i < 20; i++) {
      char attr[256];
      gnode = xmlNewChild(anode, NULL, BAD_CAST "g", NULL);
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
        case 10:
        strcpy(color, "1B325F");
        break;
        case 11:
        strcpy(color, "9CC4E4");
        break;
        case 12:
        strcpy(color, "E9F2F9");
        break;
        case 13:
        strcpy(color, "3A89C9");
        break;
        case 14:
        strcpy(color, "F26C4F");
        break;
        case 15:
        strcpy(color, "9A8230");
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
    for (int i = 0; y_axis.elements[times].values[i] && i < 20; i++) {
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
        case 10:
        strcpy(color, "1B325F");
        break;
        case 11:
        strcpy(color, "9CC4E4");
        break;
        case 12:
        strcpy(color, "E9F2F9");
        break;
        case 13:
        strcpy(color, "3A89C9");
        break;
        case 14:
        strcpy(color, "F26C4F");
        break;
        case 15:
        strcpy(color, "9A8230");
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





  htmlSaveFile(outputpath, doc);
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
