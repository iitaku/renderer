#ifndef FOUR_ELEMENT_HPP
#define FOUR_ELEMENT_HPP

namespace gtc
{

    template<typename T>
    class FourElement
    {
        private:
            T v1_;
            T v2_;
            T v3_;
            T v4_;

        public:
            FourElement()
                : v1_(0), v2_(0), v3_(0), v4_(0)
            {}

            FourElement(T v1, T v2, T v3, T v4)
                : v1_(v1), v2_(v2), v3_(v3), v4_(v4)
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
            FourElement<T> operator+(const FourElement<T>& rhs)
            {
                return FourElement<T>(this->v1_+rhs.v1_, 
                                      this->v2_+rhs.v2_, 
                                      this->v3_+rhs.v3_,
                                      this->v4_+rhs.v4_);
            }

            template<typename S>
                FourElement<T> operator+(S s)
                {
                    return FourElement<T>(this->v1_+s, 
                                          this->v2_+s, 
                                          this->v3_+s, 
                                          this->v4_+s);
                }

            /* sub */
            FourElement<T> operator-(const FourElement<T>& rhs)
            {
                return FourElement<T>(this->v1_-rhs.v1_, 
                                      this->v2_-rhs.v2_, 
                                      this->v3_-rhs.v3_, 
                                      this->v4_-rhs.v4_);
            }

            template<typename S>
                FourElement<T> operator-(S s)
                {
                    return FourElement<T>(this->v1_-s, 
                                          this->v2_-s, 
                                          this->v3_-s, 
                                          this->v4_-s);
                }

            /* mul */
            FourElement<T> operator*(const FourElement<T>& rhs)
            {
                return FourElement<T>(this->v1_*rhs.v1_, 
                                      this->v2_*rhs.v2_, 
                                      this->v3_*rhs.v3_,
                                      this->v4_*rhs.v4_);
            }

            template<typename S>
                FourElement<T> operator*(S s)
                {
                    return FourElement<T>(this->v1_*s, 
                                          this->v2_*s, 
                                          this->v3_*s,
                                          this->v4_*s);
                }

            /* inner product */
            float inner_product(const FourElement<T>& rhs)
            {
                return (this->v1_*rhs->v1_+
                        this->v2_*rhs->v2_+
                        this->v3_*rhs->v3_+
                        this->v4_*rhs->v4_);
            }

            /* norm */
            FourElement<T> norm()
            {
                float d = sqrt(Sq(this->v1_)+
                               Sq(this->v2_)+
                               Sq(this->v3_)+
                               Sq(this->v4_));

                return FourElement<T>(this->v1_/d, 
                                      this->v2_/d, 
                                      this->v3_/d, 
                                      this->v4_/d);
            }
     };

} /* namespace gtc */

#endif /* FOUR_ELEMENT_HPP */
