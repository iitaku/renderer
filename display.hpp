#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <iostream>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "four_element.hpp"

namespace gtc
{
    typedef FourElement<unsigned char> RGBA8;

    template<typename F>
    class Display
    {
        private:
            
            static void callback(void)
            {
                F::compute();
                F::display();
            }

        public:
            Display(int& argc, char* argv[], int width, int height)
            {
                glutInit(&argc, argv);
                glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
                
                glutInitWindowSize(width, height);
                
                glutCreateWindow("Renderer");
                glutDisplayFunc(callback);

                F::init(width, height);
            }

            void loop()
            {
                glutMainLoop();
            }
    };
    
    struct DrawImage
    {
        static int width_;
        static int height_;
        static int counter_;
        static GLuint texture_;
        static RGBA8* image_;

        static void init(int width, int height)
        {
                       
            width_ = width;
            height_ = height;

            image_ = new RGBA8[width_*height_];

            for (int i=0; i<height_; ++i)
            {
                for (int j=0; j<width_; ++j)
                {
                    image_[i*width_+j] = RGBA8(i%255, j%255, 0, 0.0);
                }
            }
            
            glEnable(GL_TEXTURE_2D);
 
            glGenTextures(1, &texture_);

            glBindTexture(GL_TEXTURE_2D, texture_);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 
                         width_, height_, 0, GL_RGBA,
                         GL_UNSIGNED_BYTE, image_);
            
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        }

        static void compute(void)
        {

            for (int i=0; i<height_; ++i)
            {
                for (int j=0; j<width_; ++j)
                {
                    image_[i*width_+j] = RGBA8((counter_+i)%255, (counter_+j)%255, 0, 0.0);
                }
            }
                      
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 
                         width_, height_, 0, GL_RGBA,
                         GL_UNSIGNED_BYTE, image_);
            
            counter_++;

        }

        static void display(void)
        {
            glClear(GL_COLOR_BUFFER_BIT);

            glBegin(GL_POLYGON);
            glTexCoord2f(0, 0); glVertex2f(-0.9 , -0.9);
            glTexCoord2f(0, 1); glVertex2f(-0.9 , 0.9);
            glTexCoord2f(1, 1); glVertex2f(0.9 , 0.9);
            glTexCoord2f(1, 0); glVertex2f(0.9 , -0.9);
            glEnd();

            glutSwapBuffers();
            glutPostRedisplay();
        }
    };

} /* namespace gtc */

#endif /* DISPLAY_HPP */
