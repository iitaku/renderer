#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "four_element.hpp"

namespace gtc
{
    typedef FourElement<unsigned char> RGBA;

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
                glutCreateWindow(argv[0]);
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
        static GLuint texture_;
        static RGBA* image_;

        static void init(int width, int height)
        {
            width_ = width;
            height_ = height;

            glEnable(GL_TEXTURE_2D);
            glGenTextures(1, &texture_);
            glBindTexture(GL_TEXTURE_2D, texture_);

            //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 
            //width, height, );

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

} /* namespace gtc */

#endif /* DISPLAY_HPP */
