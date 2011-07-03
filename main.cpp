#if 0
#include <iostream>

#include "display.hpp"

using namespace gtc;

struct ViewOnly
{
    static void init(void)
    {
        glClearColor(0.0, 0.0, 0.0, 1.0);
    }

    static void compute(void)
    {
    }
    
    static void display(void)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glFlush();
    }
};


int main(int argc, char* argv[])
{
    int width;
    int height;
             
    if (1<argc)
    {
        for (char** ptr = &argv[1]; NULL != *ptr; ++ptr)
        {
            if (0 == strcmp(*ptr, "-geometry"))
            {
                sscanf(*(ptr+1), "%dx%d", &width, &height);
                break;
            }
        }
    }
       
    Display<DrawImage> display(argc, argv, width, height);
    display.loop();

    return 0;
}
#endif

#include <GLUT/glut.h>

#define TEXSIZE 64

GLubyte bits[TEXSIZE][TEXSIZE][3];
GLuint texName;

void disp( void ) {
        glClear(GL_COLOR_BUFFER_BIT);
            glBindTexture(GL_TEXTURE_2D , texName);

                glBegin(GL_POLYGON);
                        glTexCoord2f(0 , 0); glVertex2f(-0.9 , -0.9);
                                glTexCoord2f(0 , 1); glVertex2f(-0.9 , 0.9);
                                        glTexCoord2f(1 , 1); glVertex2f(0.9 , 0.9);
                                                glTexCoord2f(1 , 0); glVertex2f(0.9 , -0.9);
                                                    glEnd();
                                                        glFlush();
}

void timer(int value) {
        glRotatef(1 , 0.5 , 1 , 0.25);
            glutPostRedisplay();
                glutTimerFunc(50 , timer , 0);
}

int main(int argc , char ** argv) {
    unsigned int i , j;

    for (i = 0 ; i < TEXSIZE ; i++) {
        int r = (i * 0xFF) / TEXSIZE;
        for (j = 0 ; j < TEXSIZE ; j++) {
            bits[i][j][0] = (GLubyte)r;
            bits[i][j][1] = (GLubyte)(( j * 0xFF ) / TEXSIZE);
            bits[i][j][2] = (GLubyte)~r;
        }
    }

    glutInit(&argc , argv);
    glutInitWindowSize(400 , 300);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);

    glutCreateWindow("Kitty on your lap");
    glutDisplayFunc(disp);
    glutTimerFunc(100 , timer , 0);

    glEnable(GL_TEXTURE_2D);
    glGenTextures(1 , &texName);
    glBindTexture(GL_TEXTURE_2D , texName);

    glTexImage2D(
            GL_TEXTURE_2D , 0 , 3 , TEXSIZE , TEXSIZE ,
            0 , GL_RGB , GL_UNSIGNED_BYTE , bits
            );

    glutMainLoop();
    return 0;
}
