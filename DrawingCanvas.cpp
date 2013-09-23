////////////////////////////////////////////////////////////////////////////////////        
// Michael Meluso
// CSC 350 - Computer Graphics
// Assignment 3
// DrawingCanvas.cpp
//
// This program allows the user to draw simple shapes on a canvas.
//
// Interaction:
// Left click on a box on the left to select a primitive.
// Then left click on the drawing area: once for point, twice for line or rectangle.
// Right click for menu options.
//
// Modified from canvas.cpp by Sumanta Guha.
//////////////////////////////////////////////////////////////////////////////////// 

#include "stdafx.h"
#include <cstdlib>
#include <vector>
#include <iostream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

using namespace std;

#define PI 3.14159265358979324
#define INACTIVE 0
#define POINT 1
#define LINE 2
#define RECTANGLE 3
#define MULTILINE 4
#define CIRCLE 5
#define NUMBERPRIMITIVES 5

// Use the STL extension of C++.
using namespace std;

// Globals.
static GLsizei width, height; // OpenGL window size.
static float pointSize = 3.0; // Size of point
static int primitive = INACTIVE; // Current drawing primitive.
static int pointCount = 0; // Number of  specified points.
static int tempX, tempY; // Co-ordinates of clicked point.
static int isGrid = 1; // Is there grid?
float currentColor[3] = {0.0, 0.0, 0.0}; // Current color being used
bool fillShape = false; // Filled or wire shapes?
bool stillDraw = true; // Keep adding segments to multisegment line?

// Point class.
class Point
{
public:
   Point(){};
   Point(int xVal, int yVal)
   {
	  x = xVal; y = yVal;
	  color[0] = currentColor[0];
	  color[1] = currentColor[1];
	  color[2] = currentColor[2];
   }
   void drawPoint(void); // Function to draw a point.
   friend class MultisegmentLine; // Allow MultisegmentLine access to data of Point objects associated with it
private:
   int x, y; // x and y co-ordinates of point.
   static float size; // Size of point.
   float color[3];
};

float Point::size = pointSize; // Set point size.

// Function to draw a point.
void Point::drawPoint()
{ 
   glColor3f(color[0], color[1], color[2]);
   glPointSize(size);
   glBegin(GL_POINTS);
      glVertex3f(x, y, 0.0);
   glEnd();   
}

// Vector of points.
vector<Point> points;

// Iterator to traverse a Point array.
vector<Point>::iterator pointsIterator; 

// Function to draw all points in the points array.
void drawPoints(void)
{
   // Loop through the points array drawing each point.
   pointsIterator = points.begin();
   while(pointsIterator != points.end() )
   {
      pointsIterator->drawPoint();
	  pointsIterator++;
   }
}

// Line class.
class Line
{
public:
   Line(int x1Val, int y1Val, int x2Val, int y2Val)
   {
	  x1 = x1Val; y1 = y1Val; x2 = x2Val; y2 = y2Val;
	  color[0] = currentColor[0];
	  color[1] = currentColor[1];
	  color[2] = currentColor[2];
   }
   void drawLine();
private:
   int x1, y1, x2, y2; // x and y co-ordinates of endpoints.
   float color[3];
};

// Function to draw a line.
void Line::drawLine()
{
   glColor3f(color[0], color[1], color[2]);
   glBegin(GL_LINES);
      glVertex3f(x1, y1, 0.0);
      glVertex3f(x2, y2, 0.0);
   glEnd();
}

// Vector of lines.
vector<Line> lines;

// Iterator to traverse a Line array.
vector<Line>::iterator linesIterator;

// Function to draw all lines in the lines array.
void drawLines(void)
{
   // Loop through the lines array drawing each line.
   linesIterator = lines.begin();
   while(linesIterator != lines.end() )
   {
      linesIterator->drawLine();
	  linesIterator++;
   }
}

// Rectangle class.
class Rectangle
{
public:
   Rectangle(int x1Val, int y1Val, int x2Val, int y2Val)
   {
	  x1 = x1Val; y1 = y1Val; x2 = x2Val; y2 = y2Val;
	  full = fillShape;
	  color[0] = currentColor[0];
	  color[1] = currentColor[1];
	  color[2] = currentColor[2];
   }
   void drawRectangle();
private:
   int x1, y1, x2, y2; // x and y co-ordinates of diagonally opposite vertices.
   bool full;
   float color[3];
};

// Function to draw a rectangle.
void Rectangle::drawRectangle()
{
   glColor3f(color[0], color[1], color[2]);
   if(!full) {glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);}
   else {glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);}
   glRectf(x1, y1, x2, y2);
}

// Vector of rectangles.
vector<Rectangle> rectangles;

// Iterator to traverse a Rectangle array.
vector<Rectangle>::iterator rectanglesIterator;

// Function to draw all rectangles in the rectangles array.
void drawRectangles(void)
{
   // Loop through the rectangles array drawing each rectangle.
   rectanglesIterator = rectangles.begin();
   while(rectanglesIterator != rectangles.end() )
   {
      rectanglesIterator->drawRectangle();
	  rectanglesIterator++;
   }
}

// Multisegment line class
class MultisegmentLine 
{
public:
	MultisegmentLine(int sX, int sY, int x, int y)
	{
		numPoints = 0;
		addPoint(sX, sY);
		addPoint(x, y);
		color[0] = currentColor[0];
	    color[1] = currentColor[1];
	    color[2] = currentColor[2];
	}
	void drawMultisegmentLine();
	void addPoint(int, int);
private:
	Point msl[100];
	int numPoints;
	float color[3];
};

void MultisegmentLine::addPoint(int x, int y)
{
	if (stillDraw)
	{
	   msl[numPoints] = Point(x, y);
	   numPoints++;
	}
}

void MultisegmentLine::drawMultisegmentLine()
{
   if (numPoints != 0)
   {
      for (int i = 1; i < numPoints; i++)
      {
	     glColor3f(color[0], color[1], color[2]);
         glBegin(GL_LINES);
            glVertex3f(msl[numPoints-1].x, msl[numPoints-1].y, 0.0);
            glVertex3f(msl[numPoints].x, msl[numPoints].y, 0.0);
         glEnd();
         i++;
      }
   }
}

// Vector of multisegment lines.
vector<MultisegmentLine> multisegmentLines;

// Iterator to traverse a MultisegmentLine array.
vector<MultisegmentLine>::iterator multisegmentLinesIterator;

// Function to draw all multisegment lines in the multiline segment array
void drawMultilines(void)
{
	// Loop through the multisegmentlines array drawing each multisegmentline.
   multisegmentLinesIterator = multisegmentLines.begin();
   while(multisegmentLinesIterator != multisegmentLines.end() )
   {
      multisegmentLinesIterator->drawMultisegmentLine();
	  multisegmentLinesIterator++;
   }
}

// Circle class
class Circle
{
public:
	Circle(int tempX, int tempY, int x, int y)
	{
		cX = tempX; cY = tempY; rad = ((abs(x - tempX))^2 + (abs(y-tempY))^2);
		full = fillShape;
	    color[0] = currentColor[0];
	    color[1] = currentColor[1];
	    color[2] = currentColor[2];
	}
	void drawCircle();
private:
	int cX, cY;
	float rad;
	bool full;
	float color[3];
};

void Circle::drawCircle()
{
   float t = 0; // Angle parameter

   glColor3f(color[0], color[1], color[2]);
   if(!full) {glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);}
   else {glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);}
   glBegin(GL_POLYGON);
      for(int i = 0; i < 100; ++i)
	  {
         glVertex3f(cX + rad * cos(t), cY + rad * sin(t), 0.0);
         t += 2 * PI / 100;
	  }
   glEnd();
}

// Vector of circles.
vector<Circle> circles;

// Iterator to traverse a MultisegmentLine array.
vector<Circle>::iterator circleIterator;

// Function to draw all circles in the circle array
void drawCircles(void)
{
	// Loop through the circle array drawing each circle.
   circleIterator = circles.begin();
   while(circleIterator != circles.end() )
   {
      circleIterator->drawCircle();
	  circleIterator++;
   }
}

// Function to draw point selection box in left selection area.
void drawPointSelectionBox(void)
{  
   if (primitive == POINT) glColor3f(1.0, 1.0, 1.0); // Highlight.
   else glColor3f(0.8, 0.8, 0.8); // No highlight.
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glRectf(0.0, 0.9*height, 0.1*width, height);

   // Draw black boundary.
   glColor3f(0.0, 0.0, 0.0); 
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glRectf(0.0, 0.9*height, 0.1*width, height); 

   // Draw point icon.
   glPointSize(pointSize);
   glColor3f(currentColor[0], currentColor[1], currentColor[2]);
   glBegin(GL_POINTS);
      glVertex3f(0.05*width, 0.95*height, 0.0);
   glEnd();  
}

// Function to draw line selection box in left selection area.
void drawLineSelectionBox(void)
{  
   if (primitive == LINE) glColor3f(1.0, 1.0, 1.0); // Highlight.
   else glColor3f(0.8, 0.8, 0.8); // No highlight.
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glRectf(0.0, 0.8*height, 0.1*width, 0.9*height);

   // Draw black boundary.
   glColor3f(0.0, 0.0, 0.0);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glRectf(0.0, 0.8*height, 0.1*width, 0.9*height);

   // Draw line icon.
   glColor3f(currentColor[0], currentColor[1], currentColor[2]);
   glBegin(GL_LINES);
      glVertex3f(0.025*width, 0.825*height, 0.0);
      glVertex3f(0.075*width, 0.875*height, 0.0);
   glEnd();  
}

// Function to draw rectangle selection box in left selection area.
void drawRectangleSelectionBox(void)
{  
   if (primitive == RECTANGLE) glColor3f(1.0, 1.0, 1.0); // Highlight.
   else glColor3f(0.8, 0.8, 0.8); // No highlight.
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glRectf(0.0, 0.7*height, 0.1*width, 0.8*height);

   // Draw black boundary.
   glColor3f(0.0, 0.0, 0.0);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glRectf(0.0, 0.7*height, 0.1*width, 0.8*height);

   // Draw rectangle icon.
   glColor3f(currentColor[0], currentColor[1], currentColor[2]);
   if(!fillShape) {glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);}
   else {glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);}
   glRectf(0.025*width, 0.735*height, 0.075*width, 0.765*height);
   glEnd();  
}

void drawMultilineSelectionBox(void)
{
	if (primitive == MULTILINE) glColor3f(1.0, 1.0, 1.0); // Highlight.
   else glColor3f(0.8, 0.8, 0.8); // No highlight.
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glRectf(0.0, 0.6*height, 0.1*width, 0.7*height);

   // Draw black boundary.
   glColor3f(0.0, 0.0, 0.0);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glRectf(0.0, 0.6*height, 0.1*width, 0.7*height);

   // Draw multisegment line icon.
   glColor3f(currentColor[0], currentColor[1], currentColor[2]);
   glBegin(GL_LINES);
      glVertex3f(0.025*width, 0.625*height, 0.0);
      glVertex3f(0.045*width, 0.675*height, 0.0);
	  glVertex3f(0.045*width, 0.675*height, 0.0);
      glVertex3f(0.065*width, 0.645*height, 0.0);
	  glVertex3f(0.065*width, 0.645*height, 0.0);
	  glVertex3f(0.075*width, 0.685*height, 0.0);
   glEnd();
}

void drawCircleSelectionBox()
{
   float t = 0; // Angle parameter

   if (primitive == CIRCLE) glColor3f(1.0, 1.0, 1.0); // Highlight.
   else glColor3f(0.8, 0.8, 0.8); // No highlight.
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glRectf(0.0, 0.5*height, 0.1*width, 0.6*height);

   // Draw black boundary.
   glColor3f(0.0, 0.0, 0.0);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glRectf(0.0, 0.5*height, 0.1*width, 0.6*height);

   // Draw circle icon
   glColor3f(currentColor[0], currentColor[1], currentColor[2]);
   if(!fillShape) {glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);}
   else {glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);}
   glBegin(GL_POLYGON);
      for(int i = 0; i < 100; ++i)
	  {
         glVertex3f(0.05*width + 15 * cos(t), 0.55*height + 15 * sin(t), 0.0);
         t += 2 * PI / 100;
	  }
   glEnd();
}

// Function to draw unused part of left selection area.
void drawInactiveArea(void)
{
   glColor3f(0.6, 0.6, 0.6);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glRectf(0.0, 0.0, 0.1*width, (1 - NUMBERPRIMITIVES*0.1)*height);

   glColor3f(0.0, 0.0, 0.0);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glRectf(0.0, 0.0, 0.1*width, (1 - NUMBERPRIMITIVES*0.1)*height);
}

// Function to draw temporary point.
void drawTempPoint(void)
{
   glColor3f(1.0, 0.0, 0.0);
   glPointSize(pointSize);
   glBegin(GL_POINTS);
      glVertex3f(tempX, tempY, 0.0);
   glEnd(); 
}

// Function to draw a grid.
void drawGrid(void)
{
   int i;
   
   glEnable(GL_LINE_STIPPLE);
   glLineStipple(1, 0x5555); 
   glColor3f(0.75, 0.75, 0.75);

   glBegin(GL_LINES);
	  for (i=2; i <=9; i++)
	  {
         glVertex3f(i*0.1*width, 0.0, 0.0);
         glVertex3f(i*0.1*width, height, 0.0);
	  }
	  for (i=1; i <=9; i++)
	  {
         glVertex3f(0.1*width, i*0.1*height, 0.0);
         glVertex3f(width, i*0.1*height, 0.0);
	  }
   glEnd();
   glDisable(GL_LINE_STIPPLE);
}

// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT);
   glColor3f(currentColor[0], currentColor[1], currentColor[2]); 

   drawPointSelectionBox();
   drawLineSelectionBox();
   drawRectangleSelectionBox();
   drawMultilineSelectionBox();
   drawCircleSelectionBox();
   drawInactiveArea();

   drawPoints();
   drawLines();
   drawRectangles();
   drawMultilines();
   drawCircles();
   if ( ((primitive == LINE) || (primitive == RECTANGLE) || (primitive == CIRCLE) || (primitive == MULTILINE)) &&
	   (pointCount == 1) ) drawTempPoint();
   if (isGrid) drawGrid();

   glutSwapBuffers();
}

// Function to pick primitive if click is in left selection area.
void pickPrimitive(int y)
{
   if ( y < (1- NUMBERPRIMITIVES*0.1)*height ) primitive = INACTIVE;
   else if ( y < (1 - 4*0.1)*height) primitive = CIRCLE;
   else if ( y < (1 - 3*0.1)*height) primitive = MULTILINE;
   else if ( y < (1 - 2*0.1)*height ) primitive = RECTANGLE;
   else if ( y < (1 - 1*0.1)*height ) primitive = LINE;
   else primitive = POINT;
}

// The mouse callback routine.
void mouseControl(int button, int state, int x, int y)
{
   if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
   {
      y = height - y; // Correct from mouse to OpenGL co-ordinates.

	  // Click outside canvas - do nothing.
      if ( x < 0 || x > width || y < 0 || y > height ) ;

	  // Click in left selection area.
      else if ( x < 0.1*width ) 
	  {
	     pickPrimitive(y);
		 pointCount = 0;
	  }

	  // Click in canvas.
      else
	  {
	     if (primitive == POINT) points.push_back( Point(x,y) );
         else if (primitive == LINE) 
		 { 
	        if (pointCount == 0)
			{
               tempX = x; tempY = y;
		       pointCount++;
			}
	        else 
			{
               lines.push_back( Line(tempX, tempY, x, y) );
		       pointCount = 0;
			}
		 }
         else if (primitive == RECTANGLE) 
		 { 
	        if (pointCount == 0)
			{
               tempX = x; tempY = y;
		       pointCount++;
			}
	        else 
			{
               rectangles.push_back( Rectangle(tempX, tempY, x, y) );
		       pointCount = 0;
			}
		 }
		 else if (primitive == MULTILINE)
		 {
			if (pointCount == 0)
			{
				tempX = x; tempY = y;
				pointCount++;
			}
			else if (pointCount == 1)
			{
				multisegmentLines.push_back( MultisegmentLine(tempX, tempY, x, y) );
			}
			else 
			{
				multisegmentLinesIterator=multisegmentLines.end(); 
				multisegmentLinesIterator->addPoint(x, y);
			}
		 }
		 else if (primitive == CIRCLE)
		 {
			 if (pointCount == 0)
			 {
				 tempX = x; tempY = y;
				 pointCount++;
			 }
			 else
			 {
				 circles.push_back( Circle(tempX, tempY, x, y));
				 pointCount = 0;
			 }
		 }
	  }
   }
   glutPostRedisplay();
}

// Initialization routine.
void setup(void) 
{
   glClearColor(1.0, 1.0, 1.0, 0.0);
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, (GLsizei)w, (GLsizei)h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   // Set viewing box dimensions equal to window dimensions.
   glOrtho(0.0, (float)w, 0.0, (float)h, -1.0, 1.0);
   
   // Pass the size of the OpenGL window to globals.
   width = w; 
   height = h;  

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch (key) 
   {
      case 27:
         exit(0);
         break;
	  case 69:
		  if (primitive==MULTILINE) 
		  {
			  stillDraw = false;
			  pointCount = 0;
		  }
		  break;
      default:
         break;
   }
}

// Clear the canvas and reset for fresh drawing.
void clearAll(void)
{
   points.clear();
   lines.clear();
   rectangles.clear();
   multisegmentLines.clear();
   circles.clear();
   primitive = INACTIVE;
   pointCount = 0;
}

// The right button menu callback function.
void rightMenu(int id)
{
   if (id==1) 
   {
	  clearAll();
	  glutPostRedisplay();
   }
   if (id==2) exit(0);
}

// The sub-menu callback function.
void grid_menu(int id)
{
   if (id==3) isGrid = 1;
   if (id==4) isGrid = 0;
   glutPostRedisplay();
}

// Color menu
void color_menu(int id)
{
	if (id==5) {currentColor[0]=1.0; currentColor[1]=0.0; currentColor[2]=0.0;} //Red
	if (id==6) {currentColor[0]=0.0; currentColor[1]=0.0; currentColor[2]=1.0;} //Blue
	if (id==7) {currentColor[0]=0.0; currentColor[1]=1.0; currentColor[2]=0.0;} //Green
	if (id==8) {currentColor[0]=1.0; currentColor[1]=1.0; currentColor[2]=0.2;}; //Yellow
	if (id==9) {currentColor[0]=1.0; currentColor[1]=0.49; currentColor[2]=0.0;}; //Orange
	if (id==10){currentColor[0]=1.0; currentColor[1]=0.49; currentColor[2]=1.0;}; //Purple
	glutPostRedisplay();
}

// Choose fill or wire
void fill_menu(int id)
{
	if (id==11)	fillShape = true;
	if (id==12) fillShape = false;
	glutPostRedisplay();
}

// Function to create menu.
void makeMenu(void)
{
   int sub_menu;
   sub_menu = glutCreateMenu(grid_menu);
   glutAddMenuEntry("On", 3);
   glutAddMenuEntry("Off",4);

   int sub2;
   sub2 = glutCreateMenu(color_menu);
   glutAddMenuEntry("Red", 5);
   glutAddMenuEntry("Blue", 6);
   glutAddMenuEntry("Green", 7);
   glutAddMenuEntry("Yellow", 8);
   glutAddMenuEntry("Orange", 9);
   glutAddMenuEntry("Purple", 10);

   int sub3;
   sub3 = glutCreateMenu(fill_menu);
   glutAddMenuEntry("Fill", 11);
   glutAddMenuEntry("Outline", 12);

   glutCreateMenu(rightMenu);
   glutAddSubMenu("Grid", sub_menu);
   glutAddSubMenu("Color", sub2);
   glutAddSubMenu("Fill/Wire", sub3);
   glutAddMenuEntry("Clear",1);
   glutAddMenuEntry("Quit",99);
   glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Left click on a box on the left to select a primitive." << endl
        << "Then left click on the drawing area: once for point, twice for line, rectangle, or circle." << endl
		<< "For multi-segment line, left click to add segments, 'e' to stop." << endl
		<< "For a circle, the second click determines the radius of the circle." << endl
        << "Right click for menu options." <<  endl; 
}

// Main routine.
int main(int argc, char **argv) 
{
   printInteraction();
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_DOUBLE); 
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow("DrawingCanvas.cpp");
   setup(); 
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutMouseFunc(mouseControl); 

   makeMenu(); // Create menu.

   glutMainLoop(); 

   return 0;  
}