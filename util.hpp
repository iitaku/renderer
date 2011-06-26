#ifndef UTIL_HPP
#define UTIL_HPP

template<typename T>
class ThreeElement
{
private:
    T x_;
    T y_;
    T z_;

public:
    ThreeElement()
        : x_(0), y_(0), z_(0)
    {}

    ThreeElement(T r, T g, T b)
        : x_(r), y_(g), z_(b)
    {}

    ThreeElement<T>& operator=(const ThreeElement<T>& rhs)
    {
        this->x_ = rhs.x_;
        this->y_ = rhs.y_;
        this->z_ = rhs.z_;
        return *this;
    }

    /* add */
    ThreeElement<T> operator+(const ThreeElement<T>& rhs)
    {
        return ThreeElement<T>(this->x_+rhs.x_, this->y_+rhs.y_, this->z_+rhs.z_);
    }

    template<typename S>
    ThreeElement<T> operator+(S s)
    {
        return ThreeElement<T>(this->x_+s, this->y_+s, this->z_+s);
    }

    /* sub */
    ThreeElement<T> operator-(const ThreeElement<T>& rhs)
    {
        return ThreeElement<T>(this->x_-rhs.x_, this->y_-rhs.y_, this->z_-rhs.z_);
    }

    template<typename S>
    ThreeElement<T> operator-(S s)
    {
        return ThreeElement<T>(this->x_-s, this->y_-s, this->z_-s);
    }

    /* mul */
    ThreeElement<T> operator*(const ThreeElement<T>& rhs)
    {
        return ThreeElement<T>(this->x_*rhs.x_, this->y_*rhs.y_, this->z_*rhs.z_);
    }

    template<typename S>
    ThreeElement<T> operator*(S s)
    {
        return ThreeElement<T>(this->x_*s, this->y_*s, this->z_*s);
    }

    /* inner product */
    float innex_product(const ThreeElement<T>& rhs)
    {
        return (this->x_*rhs->x_+this->y_*rhs->y_+this->z_*rhs->z_);
    }

    /* norm */
    ThreeElement<T> norm()
    {
        float d = sqrt(Sq(this->x_)+Sq(this->y_)+Sq(this->z_));
        return ThreeElement<T>(this->x_/d, this->y_/d, this->z_/d);
    }
};

#endif /* UTIL_HPP */
