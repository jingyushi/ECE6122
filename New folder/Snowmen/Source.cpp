
#include <iostream>
//#include <GL/glew.h> // Include the GLEW header file
#include <GL/glut.h> // Include the GLUT header file
#include <conio.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h> // standard definitions

using namespace std;

#define ESC 27

GLUquadricObj* quadratic{ nullptr };

//ECE_PacMan Pacman;
//ECE_Ghosts Ghosts[4];

//----------------------------------------------------------------------
// Global variables
//
// The coordinate system is set up so that the (x,y)-coordinate plane
// is the ground, and the z-axis is directed upwards. The y-axis points
// to the north and the x-axis points to the east.
//
// The values (x,y) are the current camera position. The values (lx, ly)
// point in the direction the camera is looking. The variables angle and
// deltaAngle control the camera's angle. The variable deltaMove
// indicates the amount of incremental motion for the camera with each
// redraw cycle. The variables isDragging and xDragStart are used to
// monitor the mouse when it drags (with the left button down).
//----------------------------------------------------------------------

// Camera position
float x = 0.0, y = -5.0, z = 1.0; // initially 5 units south of origin
float deltaMove = 0.0; // initially camera doesn't move

// Camera direction
float lx = 0.0, ly = 1.0; // camera points initially along y-axis
float angle = 0.0; // angle of rotation for the camera direction
float deltaAngle = 0.0; // additional angle change when dragging

// Mouse drag control
int isDragging = 0; // true when dragging
int xDragStart = 0; // records the x-coordinate when dragging starts


float colorDarkWhite[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
float colorOffWhite[4] = { 1.0f, 248.0f/255.0f, 220.0f/255.0f, 1.0f };
float colorWhite[4] = { .60, 0.60, 0.60, 1.0f };
float colorBlack[4] = { 0.01, 0.01, 0.01, 1.0f };
float colorOrange[4] = { 1.0, 0.5, 0.5, 1.0f };
float colorGreen[4] = { 0.0, 1.0, 0.0, 1.0f };
float colorDarkGreen[4] = { 0.0, 0.5, 0.0, 1.0f };
float colorBlue[4] = { 0.0, 0.0, 1.0, 1.0f };
GLfloat shininess[] = { 5 };

bool bIsLightingOn = false;
int sceneRotationAngle = 0;
float dis = 80.0f;
GLfloat position[] = { 80.0f, 80.0f, 80.0f, 1.0f };

//----------------------------------------------------------------------
// Reshape callback
//
// Window size has been set/changed to w by h pixels. Set the camera
// perspective to 45 degree vertical field of view, a window aspect
// ratio of w/h, a near clipping plane at depth 1, and a far clipping
// plane at depth 100. The viewport is the entire window.
//
//----------------------------------------------------------------------
void changeSize(int w, int h)
{
    float ratio = ((float)w) / ((float)h); // window aspect ratio
    glMatrixMode(GL_PROJECTION); // projection matrix is active
    glLoadIdentity(); // reset the projection
    gluPerspective(20.0, ratio, 0.1, 100000.0); // perspective transformation
    glMatrixMode(GL_MODELVIEW); // return to modelview mode
    glViewport(0, 0, w, h); // set viewport (drawing area) to entire window
}

//----------------------------------------------------------------------
// Draw one snowmen (at the origin)
//
// A snowman consists of a large body sphere and a smaller head sphere.
// The head sphere has two black eyes and an orange conical nose. To
// better create the impression they are sitting on the ground, we draw
// a fake shadow, consisting of a dark circle under each.
//
// We make extensive use of nested transformations. Everything is drawn
// relative to the origin. The snowman's eyes and nose are positioned
// relative to a head sphere centered at the origin. Then the head is
// translated into its final position. The body is drawn and translated
// into its final position.
//----------------------------------------------------------------------
void drawSnowman()
{
    // Draw body (a 20x20 spherical mesh of radius 0.75 at height 0.75)
    if (bIsLightingOn)
    {
        glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, colorOffWhite);
        glMaterialfv(GL_FRONT, GL_AMBIENT, colorDarkWhite);
        glMaterialfv(GL_FRONT, GL_SPECULAR, colorWhite);
    }
    else
    {
        glColor3f(1.0, 1.0, 1.0); // set drawing color to white
    }

    glPushMatrix();
        glTranslatef(0.0, 0.0, 0.75);
        glutSolidSphere(0.75, 100, 100);
    glPopMatrix();

    // Draw the head (a sphere of radius 0.25 at height 1.75)
    glPushMatrix();
        glTranslatef(0.0, 0.0, 1.75); // position head
        glutSolidSphere(0.25, 100, 100); // head sphere

        // Draw Eyes (two small black spheres)
        if (bIsLightingOn)
        {
            glMaterialfv(GL_FRONT, GL_DIFFUSE, colorBlack);
            glMaterialfv(GL_FRONT, GL_AMBIENT, colorBlack);
            glMaterialfv(GL_FRONT, GL_SPECULAR, colorBlack);
        }
        else
        {
            glColor3f(0.0, 0.0, 0.0); // eyes are black
        }

        glPushMatrix();
            glTranslatef(0.0, -0.18, 0.10); // lift eyes to final position
            glPushMatrix();
                glTranslatef(-0.05, 0.0, 0.0);
                glutSolidSphere(0.05, 100, 100); // right eye
            glPopMatrix();
            glPushMatrix();
                glTranslatef(+0.05, 0.0, 0.0);
                glutSolidSphere(0.05, 100, 100); // left eye
            glPopMatrix();
        glPopMatrix();

        // Draw Nose (the nose is an orange cone)
        if (bIsLightingOn)
        {
            glMaterialfv(GL_FRONT, GL_DIFFUSE, colorOrange);
            glMaterialfv(GL_FRONT, GL_AMBIENT, colorOrange);
        }
        else
        {
            glColor3f(1.0, 0.5, 0.5); // nose is orange
        }

        glPushMatrix();
            glRotatef(90.0, 1.0, 0.0, 0.0); // rotate to point along -y
            glutSolidCone(0.08, 0.5, 10, 2); // draw cone
        glPopMatrix();
    glPopMatrix();

    // Draw a faux shadow beneath snow man (dark green circle)
    glColor3f(0.0, 0.5, 0.0);
    if (bIsLightingOn)
    {
        glMaterialfv(GL_FRONT, GL_DIFFUSE, colorDarkGreen);
        glMaterialfv(GL_FRONT, GL_AMBIENT, colorDarkGreen);
    }
    else
    {
        glColor3f(0.0, 0.5, 0.0);
    }
    glPushMatrix();
        glTranslatef(0.2, 0.2, 0.001);    // translate to just above ground
        glScalef(1.0, 1.0, 0.0); // scale sphere into a flat pancake
        glutSolidSphere(0.75, 20, 20); // shadow same size as body
    glPopMatrix();
}

//----------------------------------------------------------------------
// Update with each idle event
//
// This incrementally moves the camera and requests that the scene be
// redrawn.
//----------------------------------------------------------------------
void update(void)
{
    if (deltaMove) { // update camera position
        x += deltaMove * lx * 0.1;
        y += deltaMove * ly * 0.1;
    }
    glutPostRedisplay(); // redisplay everything
}
//ECE_Pacman
// x, y, z
// size
// color
// render()
//----------------------------------------------------------------------
// Draw the entire scene
//
// We first update the camera location based on its distance from the
// origin and its direction.
//----------------------------------------------------------------------
void renderScene(void)
{
    int i, j;

    // Clear color and depth buffers
    glClearColor(0.0, 0.0, 0.0, 1.0); // black background
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Reset transformations
    glLoadIdentity();

    // Set the camera centered at (x,y,1) and looking along directional
    // vector (lx, ly, 0), with the z-axis pointing up
    gluLookAt(
        dis, -dis, dis, // camera location
        0.0, 0.0, 0.0,    // where camera is pointed
        0.0, 0.0, 1.0);      // landscape or portrait

    // Rotate the wholescene by an angle around the z-axis
    glRotatef(sceneRotationAngle, 0.0f, 0.0f, 1.0f);

    // Move the light with the scene
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    // Draw ground - 200x200 square colored green
    if (bIsLightingOn)
    {
        glMaterialfv(GL_FRONT, GL_DIFFUSE, colorGreen);
        glMaterialfv(GL_FRONT, GL_AMBIENT, colorGreen);
        glMaterialfv(GL_FRONT, GL_SPECULAR, colorGreen);
    }
    else
    {
        glColor3f(0.0, 0.7, 0.0);

    }
    glBegin(GL_QUADS);
        glVertex3f(-30.0, -30.0, 0.0);
        glVertex3f(-30.0, 30.0, 0.0);
        glVertex3f(30.0, 30.0, 0.0);
        glVertex3f(30.0, -30.0, 0.0);
    glEnd();

    // Draw 49 snow men
    for (i = -3; i <= 3; i++)
    {
        for (j = -3; j <= 3; j++)
        {
            glPushMatrix();
                glTranslatef(i*8, j*8, 0);
                drawSnowman();
            glPopMatrix();
        }
    }

    glutSwapBuffers(); // Make it all visible
}

void toggleLighting()
{
    bIsLightingOn = !bIsLightingOn;

    if (bIsLightingOn)
    {
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glLightfv(GL_LIGHT0, GL_AMBIENT, colorOffWhite);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, colorOffWhite);
        glLightfv(GL_LIGHT0, GL_SPECULAR, colorDarkWhite);

    }
    else
    {
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
    }
    glutPostRedisplay();
}
//----------------------------------------------------------------------
// User-input callbacks
//
// processNormalKeys: ESC, q, and Q cause program to exit
// pressSpecialKey: Up arrow = forward motion, down arrow = backwards
// releaseSpecialKey: Set incremental motion to zero
//----------------------------------------------------------------------
void processNormalKeys(unsigned char key, int xx, int yy)
{
    if (key == ESC || key == 'q' || key == 'Q')
    {
        exit(0);
    }
    else if (key == 'd' || key == 'D')
    {
        dis -= 1.0;;
    }
    else if (key == 'u' || key == 'U')
    {
        dis += 2.0;;
    }
    else if (key == 'r' || key == 'R')
    {
        sceneRotationAngle = (sceneRotationAngle+5)%360;
    }
    else if (key == 'l' || key == 'L')
    {
        toggleLighting();
    }

}

void pressSpecialKey(int key, int xx, int yy)
{
    switch (key) 
    {
    case GLUT_KEY_UP: deltaMove = 1.0; break;
    case GLUT_KEY_DOWN: deltaMove = -1.0; break;
    }
}

void releaseSpecialKey(int key, int x, int y)
{

    switch (key) 
    {
    case GLUT_KEY_UP: deltaMove = 0.0; break;
    case GLUT_KEY_DOWN: deltaMove = 0.0; break;
    }
}

//----------------------------------------------------------------------
// Process mouse drag events
//
// This is called when dragging motion occurs. The variable
// angle stores the camera angle at the instance when dragging
// started, and deltaAngle is a additional angle based on the
// mouse movement since dragging started.
//----------------------------------------------------------------------
void mouseMove(int x, int y)
{
    if (isDragging) 
    { // only when dragging
        // update the change in angle
        deltaAngle = (x - xDragStart) * 0.005;

        // camera's direction is set to angle + deltaAngle
        lx = -sin(angle + deltaAngle);
        ly = cos(angle + deltaAngle);
    }
}

void mouseButton(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON) 
    {
        if (state == GLUT_DOWN) 
        { // left mouse button pressed
            isDragging = 1; // start dragging
            xDragStart = x; // save x where button first pressed
        }
        else 
        { /* (state = GLUT_UP) */
            angle += deltaAngle; // update camera turning angle
            isDragging = 0; // no longer dragging
        }
    }
}
void init() 
{
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
}
//----------------------------------------------------------------------
// Main program  - standard GLUT initializations and callbacks
//----------------------------------------------------------------------
int main(int argc, char **argv)
{
    printf("\n\
-----------------------------------------------------------------------\n\
  OpenGL Sample Program:\n\
  - Drag mouse left-right to rotate camera\n\
  - Hold up-arrow/down-arrow to move camera forward/backward\n\
  - q or ESC to quit\n\
-----------------------------------------------------------------------\n");

    // general initializations
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 400);
    glutCreateWindow("Winter Wonderland");
    init();
    // register callbacks
    glutReshapeFunc(changeSize); // window reshape callback
    glutDisplayFunc(renderScene); // (re)display callback
    glutIdleFunc(update); // incremental update
//    glutIgnoreKeyRepeat(1); // ignore key repeat when holding key down
    glutMouseFunc(mouseButton); // process mouse button push/release
    glutMotionFunc(mouseMove); // process mouse dragging motion
    glutKeyboardFunc(processNormalKeys); // process standard key clicks
    glutSpecialFunc(pressSpecialKey); // process special key pressed
                        // Warning: Nonstandard function! Delete if desired.
    glutSpecialUpFunc(releaseSpecialKey); // process special key release

    // Initial Pacman
    // Initial Ghosts
    if (!quadratic)
        quadratic = gluNewQuadric();

    // enter GLUT event processing cycle
    glutMainLoop();

    return 0; // this is just to keep the compiler happy
}