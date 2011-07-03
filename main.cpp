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
