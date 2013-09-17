////////////////////////////////////////////////////
// Michael Meluso
//
// CSC 350 - Assignment 2
//
// Ushape.cpp
//
// Displays the partially triangulated U shape
// in figure 2.17b of the textbook.
//
////////////////////////////////////////////////////

#include <iostream>
#include "stdafx.h"

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

using namespace std;

// Vertex coordinates for the triangles.
static float vertices[] =  
{	
	10.0, 80.0, 0.0,
    10.0, 85.0, 0.0,
	17.0, 80.0, 0.0,
    24.0, 85.0, 0.0,
	17.0, 50.0, 0.0,
	24.0, 58.0, 0.0,
	26.0, 50.0, 0.0,
	28.0, 58.0, 0.0,
	35.0, 50.0, 0.0,
	28.0, 85.0, 0.0,
	35.0, 80.0, 0.0,
	42.0, 85.0, 0.0,
	42.0, 80.0, 0.0
};

// Drawing (display) routine.
void drawScene(void)
{
	int i;

    // Clear screen to background color.
    glClear(GL_COLOR_BUFFER_BIT);
    
    // The vertex pointers point to the data for the figure.
	glVertexPointer(3, GL_FLOAT, 0, vertices);

	// Draw the shape
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3f(0,1,1);
    glBegin(GL_TRIANGLE_STRIP);
		for(i = 0; i < 13; ++i) glArrayElement(i);
    glEnd();

	// Draw black borders to show triangles of the shape
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(0,0,0);
    // Draw triangulated portion of the shape.
    glBegin(GL_TRIANGLE_STRIP);
		for(i = 0; i < 13; ++i) glArrayElement(i);
    glEnd();

    // Flush created objects to the screen, i.e., force rendering.
    glFlush();
}

// Initialization routine.
void setup(void)
{
    // Set background (or clearing) color.
    glClearColor(1.0, 1.0, 1.0, 0.0);

	// Allow creation of vertex arrays
	glEnableClientState(GL_VERTEX_ARRAY);
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
    glOrtho(0.0, 90.0, 0.0, 90.0, -1.0, 1.0);
    
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
    glutInitWindowPosition(90, 90);
    
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

	glDisableClientState(GL_VERTEX_ARRAY);
	return 0;
}

