#ifndef HELPER_HPP
#define HELPER_HPP

#define DEF_TYPE(_sign_type, _value_type) \
    const static sign_type_class_t sign_type = _sign_type; \
    const static value_type_class_t value_type = _value_type; \

#include "porting.hpp"

namespace gtc
{
    namespace helper
    {
        enum sign_type_class_t
        {
            Unsigned,
            Signed,
        };

        enum value_type_class_t
        {
            Integer,
            Floating,
        };
        
        template<typename T>
        struct TypeClass
        {
            DEF_TYPE(Signed, Integer)
        };
   
        template<>
        struct TypeClass<unsigned char>
        {
            DEF_TYPE(Unsigned, Integer)
        };

        template<>
        struct TypeClass<unsigned short>
        {
            DEF_TYPE(Unsigned, Integer)
        };

        template<>
        struct TypeClass<unsigned int>
        {
            DEF_TYPE(Unsigned, Integer)
        };

        template<>
        struct TypeClass<unsigned long long>
        {
            DEF_TYPE(Unsigned, Integer)
        };
        
        template<>
        struct TypeClass<float>
        {
            DEF_TYPE(Signed, Floating)
        };

        template<>
        struct TypeClass<double>
        {
            DEF_TYPE(Signed, Floating)
        };

        FUNC_DECL
        float make_inf(void)
        {
            union {
                float f;
                unsigned int i;
            } u;
            u.i = 0x7f800000;
            return u.f;
        }

        template<typename T>
        FUNC_DECL
        T make_max(void)
        {
            if (Unsigned == TypeClass<T>::sign_type)
            {
                T max = static_cast<T>(-1);
                return max;
            }
            else if (Signed == TypeClass<T>::sign_type)
            {
                T max = static_cast<T>(~(1<<(sizeof(T)*8-1)));
                return max;
            }
            else
            {
                /* unreachable */
                return static_cast<T>(0);
            }
        }
        
        template<>
        FUNC_DECL
        float make_max<float>(void)
        {
            union {
                float f;
                unsigned int i;
            } u;
            u.i = 0x7f7fffff;
            return u.f;
        }
 
        template<>
        FUNC_DECL
        double make_max<double>(void)
        {
            union {
                double f;
                unsigned long long i;
            } u;
            u.i = 0x7fefffffffffffff;
            return u.f;
        }

        template<typename T>
        FUNC_DECL
        T make_min(void)
        {
            return ~make_max<T>();
        }
        
        template<>
        FUNC_DECL
        float make_min<float>(void)
        {
            union {
                float f;
                unsigned int i;
            } u;
            u.i = 0x8f7fffff;
            return u.f;
        }
 
        template<>
        FUNC_DECL
        double make_min<double>(void)
        {
            union {
                double f;
                unsigned long long i;
            } u;
            u.i = 0x8fefffffffffffff;
            return u.f;
        }

        template<typename T>
        FUNC_DECL
        T add_sat(T val1, T val2)
        {
            if (Unsigned == TypeClass<T>::sign_type)
            {
                T result = val1 + val2;
                if (result < val1)
                {
                    return make_max<T>();
                }
                else
                {
                    return result;
                }
            }
            else if (Signed == TypeClass<T>::sign_type)
            {
                T result = val1 + val2;
                 
                if (static_cast<T>(0) < val1 && static_cast<T>(0) < val2)
                {
                    return (result < val1) ? make_max<T>() : result;
                }
                else if (val1 < static_cast<T>(0) && val2 < static_cast<T>(0))
                {
                    return (val1 < result) ? make_min<T>() : result;
                }
                else
                {
                    return result;
                }
            }
            else
            {
                /* unreachable */
                return static_cast<T>(0);
            }
        }

    } /* namespace helper */

} /* namespace gtc */

#endif /* HELPER_HPP */
