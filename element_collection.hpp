#ifndef ELEMENT_COLLECTION_HPP
#define ELEMENT_COLLECTION_HPP

#include <cmath>

#define SQ(val) (val*val)
#define MAX(val1, val2) (val1<val2 ? val2 : val1)
#define MIN(val1, val2) (val1<val2 ? val1 : val2)

namespace gtc
{

    template<typename T>
    class ThreeElement
    {
        private:
            T v1_;
            T v2_;
            T v3_;

        public:
            ThreeElement(void)
                : v1_(0), v2_(0), v3_(0)
            {}

            ThreeElement(T v1, T v2, T v3)
                : v1_(v1), v2_(v2), v3_(v3)
            {}

            ThreeElement(const ThreeElement<T>& other)
                : v1_(other.v1_), v2_(other.v2_), v3_(other.v3_)
            {} 

            ThreeElement<T>& operator=(const ThreeElement<T>& rhs) 
            {
                this->v1_ = rhs.v1_;
                this->v2_ = rhs.v2_;
                this->v3_ = rhs.v3_;
                return *this;
            }

            /* add */
            ThreeElement<T> operator+(const ThreeElement<T>& rhs) const
            {
                return ThreeElement<T>(this->v1_+rhs.v1_,
                                       this->v2_+rhs.v2_,
                                       this->v3_+rhs.v3_);
            }

            template<typename S>
            ThreeElement<T> operator+(S s) const
            {
                return ThreeElement<T>(this->v1_+s,
                                       this->v2_+s,
                                       this->v3_+s);
            }

            /* sub */
            ThreeElement<T> operator-(const ThreeElement<T>& rhs) const
            {
                return ThreeElement<T>(this->v1_-rhs.v1_,
                                       this->v2_-rhs.v2_,
                                       this->v3_-rhs.v3_);
            }

            template<typename S>
            ThreeElement<T> operator-(S s) const
            {
                return ThreeElement<T>(this->v1_-s, 
                                       this->v2_-s, 
                                       this->v3_-s);
            }

            /* mul */
            ThreeElement<T> operator*(const ThreeElement<T>& rhs) const
            {
                return ThreeElement<T>(this->v1_*rhs.v1_, 
                                       this->v2_*rhs.v2_, 
                                       this->v3_*rhs.v3_);
            }

            template<typename S>
            ThreeElement<T> operator*(S s) const
            {
                return ThreeElement<T>(this->v1_*s, 
                                       this->v2_*s, 
                                       this->v3_*s);
            }

            /* dot product */
            float dot(const ThreeElement<T>& rhs) const
            {
                return (this->v1_*rhs.v1_+
                        this->v2_*rhs.v2_+
                        this->v3_*rhs.v3_);
            }

            float self_dot(void) const
            {
                return (this->v1_*this->v1_+
                        this->v2_*this->v2_+
                        this->v3_*this->v3_);
            }

            /* exterior product */
            ThreeElement<T> cross(const ThreeElement<T>& rhs) const
            {
                return ThreeElement<T>(this->v2_*rhs.v3_-this->v3_*rhs.v2_,
                                       this->v3_*rhs.v1_-this->v1_*rhs.v3_,
                                       this->v1_*rhs.v2_-this->v2_*rhs.v1_);
            }


            /* normalize  */
            ThreeElement<T> normalize(void) const
            {
                float d = sqrt(SQ(this->v1_)+
                               SQ(this->v2_)+
                               SQ(this->v3_));

                return ThreeElement<T>(this->v1_/d, 
                                       this->v2_/d, 
                                       this->v3_/d);
            }
     };


    template<typename T>
    class FourElement
    {
        private:
            T v1_;
            T v2_;
            T v3_;
            T v4_;

        public:
            FourElement(void)
                : v1_(0), v2_(0), v3_(0), v4_(0)
            {}

            FourElement(T v1, T v2, T v3)
                : v1_(v1), v2_(v2), v3_(v3), v4_(0)
            {}

            FourElement(T v1, T v2, T v3, T v4)
                : v1_(v1), v2_(v2), v3_(v3), v4_(v4)
            {}

            FourElement(const FourElement<T>& other)
                : v1_(other.v1_), v2_(other.v2_), v3_(other.v3_), v4_(other.v4_)
            {} 

            FourElement<T>& operator=(const FourElement<T>& rhs) 
            {
                this->v1_ = rhs.v1_;
                this->v2_ = rhs.v2_;
                this->v3_ = rhs.v3_;
                this->v4_ = rhs.v4_;
                return *this;
            }

            /* add */
            FourElement<T> operator+(const FourElement<T>& rhs) const
            {
                return FourElement<T>(this->v1_+rhs.v1_, 
                                      this->v2_+rhs.v2_, 
                                      this->v3_+rhs.v3_,
                                      this->v4_+rhs.v4_);
            }

            template<typename S>
            FourElement<T> operator+(S s) const
            {
                return FourElement<T>(this->v1_+s, 
                                      this->v2_+s, 
                                      this->v3_+s, 
                                      this->v4_+s);
            }

            /* sub */
            FourElement<T> operator-(const FourElement<T>& rhs) const
            {
                return FourElement<T>(this->v1_-rhs.v1_, 
                                      this->v2_-rhs.v2_, 
                                      this->v3_-rhs.v3_, 
                                      this->v4_-rhs.v4_);
            }

            template<typename S>
            FourElement<T> operator-(S s) const
            {
                return FourElement<T>(this->v1_-s, 
                                      this->v2_-s, 
                                      this->v3_-s, 
                                      this->v4_-s);
            }

            /* mul */
            FourElement<T> operator*(const FourElement<T>& rhs) const
            {
                return FourElement<T>(this->v1_*rhs.v1_, 
                                      this->v2_*rhs.v2_, 
                                      this->v3_*rhs.v3_,
                                      this->v4_*rhs.v4_);
            }

            template<typename S>
            FourElement<T> operator*(S s) const
            {
                return FourElement<T>(this->v1_*s, 
                                      this->v2_*s, 
                                      this->v3_*s,
                                      this->v4_*s);
            }

            /* dot product */
            float dot(const FourElement<T>& rhs) const
            {
                return (this->v1_*rhs.v1_+
                        this->v2_*rhs.v2_+
                        this->v3_*rhs.v3_+
                        this->v4_*rhs.v4_);
            }

            float self_dot(void) const
            {
                return (this->v1_*this->v1_+
                        this->v2_*this->v2_+
                        this->v3_*this->v3_+
                        this->v4_*this->v4_);
            }

            /* normalize */
            FourElement<T> normalize (void) const
            {
                float d = sqrt(SQ(this->v1_)+
                               SQ(this->v2_)+
                               SQ(this->v3_)+
                               SQ(this->v4_));

                return FourElement<T>(this->v1_/d, 
                                      this->v2_/d, 
                                      this->v3_/d, 
                                      this->v4_/d);
            }
     };

    typedef ThreeElement<float> Coord;
    typedef ThreeElement<float> Vector;
    typedef FourElement<unsigned char> RGBA8;

} /* namespace gtc */

#endif /* ELEMENT_COLLECTION_HPP */
