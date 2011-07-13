#ifndef HELPER_HPP
#define HELPER_HPP

namespace gtc
{
    namespace helper
    {
        FUNC_DECL
        static float make_inf(void)
        {
            union {
                float f;
                unsigned int i;
            } u;
            u.i = 0x7f800000;
            return u.f;
        }

        FUNC_DECL
        static float make_max(void)
        {
            return 0.0;
        }

    } /* namespace helper */

} /* namespace gtc */

#endif /* HELPER_HPP */
