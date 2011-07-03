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
                glutInitWindowSize(width, height);
                
                glutCreateWindow(argv[0]);
                glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
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
        static RGBA8* image_;

        static void init(int width, int height)
        {

            GLubyte bits[512][512][3];
            for (int i = 0 ; i < 512 ; i++) {
                int r = (i * 0xFF) / 512;
                for (int j = 0 ; j < 512 ; j++) {
                    bits[i][j][0] = (GLubyte)r;
                    bits[i][j][1] = (GLubyte)(( j * 0xFF ) / 512);
                    bits[i][j][2] = (GLubyte)~r;
                }
            }

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
                         GL_UNSIGNED_BYTE, bits);

            //glClearColor(0.0, 0.0, 0.0, 1.0);
        }

        static void compute(void)
        {
        }

        static void display(void)
        {
            glClear(GL_COLOR_BUFFER_BIT);

            glBindTexture(GL_TEXTURE_2D, texture_);

            glBegin(GL_POLYGON);
            glTexCoord2f(0, 0); 
            glVertex2f(-0.9 , -0.9);
            glTexCoord2f(0, 1); 
            glVertex2f(-0.9 , 0.9);
            glTexCoord2f(1, 1); 
            glVertex2f(0.9 , 0.9);
            glTexCoord2f(1, 0); 
            glVertex2f(0.9 , -0.9);
            glEnd();
            glFlush();
            glFlush();
        }
    };

} /* namespace gtc */

#endif /* DISPLAY_HPP */
