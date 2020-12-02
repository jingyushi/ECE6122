#include <GL/glut.h>

static int year = 0, day = 0;
GLUquadricObj* quadratic{ nullptr };

void init(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
    if (!quadratic)
        quadratic = gluNewQuadric();

}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);


    glColor3f(1.0, 0.0, 0.0);

    glPushMatrix();
        /* Draw sun at origin */
        glutWireSphere(1.0, 20, 16);   /* draw sun */

        /* Remember reverse order */
        glRotatef((GLfloat)year, 0.0, 1.0, 0.0);
        glTranslatef(3.0, 0.0, 0.0);
        glRotatef((GLfloat)day, 0.0, 1.0, 0.0);
        glColor3f(0.0, 0.0, 1.0);
        glutWireSphere(0.2, 10, 8);    /* draw smaller planet */

        //The gluCylinder function draws a cylinder oriented along the z-axis.
        // The base of the cylinder is placed at z = 0, and the top at z = height.

        // gluCylinder(GLUquadric * qobj, GLdouble baseRadius, GLdouble topRadius, GLdouble height, GLint slices, GLint stacks);

        glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
        gluCylinder(quadratic, 0.1f, 0.1f, 3.0f, 32, 32);
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
        glColor3f(0.0, 1.0, 1.0);
        gluCylinder(quadratic, 0.05f, 0.1f, 10.0f, 32, 32);

    glPopMatrix();

    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 10.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 'd':
        day = (day + 10) % 360;
        glutPostRedisplay();
        break;
    case 'D':
        day = (day - 10) % 360;
        glutPostRedisplay();
        break;
    case 'y':
        year = (year + 5) % 360;
        glutPostRedisplay();
        break;
    case 'Y':
        year = (year - 5) % 360;
        glutPostRedisplay();
        break;
    default:
        break;
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();

    gluDeleteQuadric(quadratic);
    return 0;
}