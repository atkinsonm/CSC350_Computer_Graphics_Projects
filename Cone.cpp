////////////////////////////////////////////////////
// Michael Meluso
//
// CSC 350 - Assignment 2
//
// Cone.cpp
//
// Displays a cone made with triangles
// and an ellipse.
//
////////////////////////////////////////////////////

#include <iostream>
#include "stdafx.h"
#include <cstdlib>
#include <cmath>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#define PI 3.14159265

using namespace std;

// Globals variables
static float xR = 10.0; // Width of circle.
static float yR = 2.0; // Height of circle.
static float X = 50.0; // X-coordinate of center of circle.
static float Y = 65.8; // Y-coordinate of center of circle.
static int numVertices = 5; // Number of vertices on circle.

// Drawing (display) routine.
void drawScene(void)
{
	float t = 0; // Angle parameter

	// Clear screen to background color.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(0,0,0);
    // Draw triangulated portion of the shape.
    glBegin(GL_TRIANGLES);
		glVertex3f(50.0, 25.0, 0.0);
		glVertex3f(40.0, 65.2, 0.0);
		glVertex3f(43.5, 64.2, 0.0);
		glVertex3f(50.0, 25.0, 0.0);
		glVertex3f(43.5, 64.2, 0.0);
		glVertex3f(50.0, 63.8, 0.0);
		glVertex3f(50.0, 25.0, 0.0);
		glVertex3f(50.0, 63.8, 0.0);
		glVertex3f(57.0, 64.2, 0.0);
		glVertex3f(50.0, 25.0, 0.0);
		glVertex3f(57.0, 64.2, 0.0);
		glVertex3f(60.0, 65.2, 0.0);
    glEnd();

	glBegin(GL_LINE_LOOP);
		for(int i = 0; i < 50; ++i)
		{
			glVertex3f(X + xR * cos(t), Y + yR * sin(t), -0.5);
			 t += 2 * PI / 50;
		}
	glEnd();

	glFlush();

	// Create OpenGL window with title.
    glutCreateWindow("Assignment2.cpp");
}

// Initialization routine.
void setup(void)
{
    // Set background (or clearing) color.
    glClearColor(1.0, 1.0, 1.0, 0.0);
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   // Set viewport size to be entire OpenGL window.
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    
    // Set matrix mode to projection.
    glMatrixMode(GL_PROJECTION);
    
    // Clear current projection matrix to identity.
    glLoadIdentity();
    
    // Specify the orthographic (or perpendicular) projection,
    // i.e., define the viewing box.
    glOrtho(0.0, 100.0, 0.0, 100.0, -1.0, 1.0);
    
    // Set matrix mode to modelview.
    glMatrixMode(GL_MODELVIEW);
    
    // Clear current modelview matrix to identity.
    glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
    switch(key)
    {
            // Press escape to exit.
        case 27:
            exit(0);
            break;
        default:
            break;
    }
}

// Main routine: defines window properties, creates window,
// registers callback routines and begins processing.
int main(int argc, char **argv)
{
	// Initialize GLUT.
    glutInit(&argc, argv);
    
    // Set display mode as single-buffered and RGB color.
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    
    // Set OpenGL window size.
    glutInitWindowSize(500, 500);
    
    // Set position of OpenGL window upper-left corner.
    glutInitWindowPosition(100, 100);
    
    // Create OpenGL window with title.
    glutCreateWindow("Assignment2.cpp");
    
    // Initialize.
    setup();

	// Register display routine.
    glutDisplayFunc(drawScene);
    
    // Register reshape routine.
    glutReshapeFunc(resize);
    
    // Register keyboard routine.
    glutKeyboardFunc(keyInput);
    
    // Begin processing.
    glutMainLoop();

	glDisable(GL_DEPTH_TEST);

	return 0;
}