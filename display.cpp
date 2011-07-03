#include <stdlib.h>

#include "display.hpp"


namespace gtc
{

/* DrawImage */
int DrawImage::width_ = 0;
int DrawImage::height_ = 0;
GLuint DrawImage::texture_ = 0;
RGBA* DrawImage::image_ = NULL;

} /* namespace gtc */
