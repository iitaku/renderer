#include "helper.hpp"
namespace gtc
{
    namespace helper
    {
        float make_inf(void)
        {
            union {
                float f;
                unsigned int i;
            } u;
            u.i = 0x7f800000;
            return u.f;
        }

    } /* namespace helper */

} /* namespace gtc */
